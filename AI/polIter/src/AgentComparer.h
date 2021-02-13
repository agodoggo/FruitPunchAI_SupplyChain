#pragma once
#include "RNG.h"
#include "StatisticsKeeper.h"
#include "AgentAdapter.h"
#ifdef __linux__
//In my case, Torch is only available on the linux server. 
//There is a torch for visual studio c++, but it is not stable
//so not recommended to try to get that one to work..:q
#define TorchAvailable true
#endif
#if TorchAvailable
#include <torch/torch.h>
#endif
template <class MDP>
class AgentComparer
{
	MDPAdapter<MDP> adapter;

public:
	AgentComparer(const MDP* mdp_ptr) :
		adapter{ mdp_ptr }, rng{ 24031984 }
	{

	}

	using RandomSource = typename MDPAdapter<MDP>::RandomSource;
	using TrajectoryTracker = typename MDPAdapter<MDP>::TrajectoryTracker;
	
	template <typename Agent1, typename Agent2>
	void CompareAgents(Agent1& agent1, Agent2& agent2, size_t NumTrajectories, double baseline =std::numeric_limits<double>::max())
	{

		StatisticsKeeper statKeeper1;
		StatisticsKeeper statKeeper2;
		StatisticsKeeper statKeeperDelta;

		auto time1 = std::chrono::steady_clock::now();

#if TorchAvailable
		torch::NoGradGuard guard;
		//at::AutoNonVariableTypeMode non_var_type_mode(true);
#endif     
		
		for (size_t i = 0; i < NumTrajectories; i++)
		{
			auto tracker1 = adapter.GetNewTracker();
			auto tracker2 = adapter.GetNewTracker();

			auto randomSource = adapter.GetRandomSource( tracker1, rng);

			ProcessTrajectory(agent1, tracker1, randomSource, rng);
			ProcessTrajectory(agent2, tracker2, randomSource, rng);


			if (i % 2500 == 0)
			{
				std::cout << "Completed " << i << " out of " << NumTrajectories << std::endl;
			}
			statKeeper1.AddStatistic(tracker1.accumulatedCosts);
			statKeeper2.AddStatistic(tracker2.accumulatedCosts);
			statKeeperDelta.AddStatistic(tracker1.accumulatedCosts - tracker2.accumulatedCosts);
		}
		std::cout << statKeeper1.MuHat() << " +-  " << statKeeper1.SigmaOfMean() << std::endl;
		std::cout << statKeeper2.MuHat() << " +-  " << statKeeper2.SigmaOfMean() << std::endl;
		std::cout << "delta:" << statKeeperDelta.MuHat() << "   " << statKeeperDelta.SigmaOfMean() << std::endl;
		std::cout << "Positive means second agent is better" << std::endl;
		if (baseline != std::numeric_limits<double>::max())
		{
			std::cout << "Baseline: " << baseline << "; deviation second agent compared to baseline (in %) : " << 100 * (statKeeper2.MuHat() / baseline - 1.0) << " pm " << 100 * (statKeeper2.SigmaOfMean() / baseline) << std::endl;
		}
		auto time2 = std::chrono::steady_clock::now();
		std::cout << "Timing: " << ((time2 - time1).count() / 1000000) << std::endl;
		std::cout << "-------------------------------------------------" << std::endl;


	}
private:
	template <class Agent>
	void ProcessTrajectory(Agent& agent, TrajectoryTracker& tracker,  RandomSource& randomSource, RNG& rng)
	{
		auto randomSourceStatus = adapter.GetRandomSourceStatus(tracker,randomSource);
		AgentAdapter<MDP, Agent> agent_adapter{ adapter.mdp_ptr,&agent };

		while (adapter.IncorporateEventsUntilNextNonTrivialAction(randomSource, randomSourceStatus, tracker))
		{
			agent_adapter.TakeAction(tracker, rng);
		}
	}
	RNG rng;
};

