#pragma once
#include "Sample.h"
#include <vector>
#include "RNG.h"
#include "ExactMDPSolver.h"
#include "math.h"
#include <ostream>
#include "MDPAdapter.h"
#include "AgentAdapter.h"
#include "Experiment.h"
#include "BanditController.h"
#include "PoultryMDP.h"
#include "PoultryAgent.h"

template <class MDP, class Agent, bool IncurTestOverhead = false>
class StateTree
{

	using State = typename MDP::State;
	using RandomSource = typename MDPAdapter<MDP>::RandomSource;
	using TrajectoryTracker = typename MDPAdapter<MDP>::TrajectoryTracker;
	using RandomSourceStatus = typename MDPAdapter<MDP>::RandomSourceStatus;
public:

	static size_t SimultaneousTrajectories;



	StateTree(const MDP* mdp_ptr, Agent* agent_ptr) : adapter{ mdp_ptr }, agent_adapter{ mdp_ptr,agent_ptr }
	{

	}

	/// <summary>
	/// Only for testing purposes.
	/// </summary>
	static ExactMDPSolver<MDP, IncurTestOverhead>* exactMDPSolver;
private:

	MDPAdapter<MDP> adapter;
	AgentAdapter<MDP, Agent> agent_adapter;



	using TrajectoryInfo = typename BanditController<MDP, IncurTestOverhead>::TrajectoryInfo;

	void VerifyAsymptoticCorrectness(const State& root, std::vector<std::vector<StatisticsKeeper>>& statistics, std::vector<size_t> allowedActions, size_t best_active_action_number)
	{//For testing purposes. 

		[[maybe_unused]] StateWrapper<MDP>* sw;
		if constexpr (IncurTestOverhead)
		{
			if (exactMDPSolver == nullptr)
			{
				std::string error = "Please note that IncurTestOverhead=true for StateTree may only be set from SampleGenerator; otherwise this error occurs.";
				std::cout << error;
				throw error;
			}

			sw = exactMDPSolver->find(root);

			if (sw->CostsPerAction.size() != statistics.size())
			{
				throw;
			}
		}
		if constexpr (MDPAdapter<MDP>::StateCanSerialize())
		{
			std::cout << root << std::endl;
		}
		{
			auto& stat = statistics[best_active_action_number][best_active_action_number];
			double Actual = 0.0;
			if constexpr (IncurTestOverhead)
			{
				Actual = sw->CostsPerAction[best_active_action_number];
			}
			double sigma = stat.SigmaOfMean();
			double delta = stat.MuHat() - Actual;
			std::cout << "Best: " << stat.MuHat() << "  " << (Actual) << "  delta: " << delta << " sigma: " << sigma << "  delta/sigma: " << (delta / sigma) << "  mu/sigma " << stat.MuHat() / stat.SigmaOfMean() << "    " << stat.numStatisticsGathered() << std::endl;
		}

		for (size_t i = 0; i < statistics.size(); i++)
		{
			if (i != best_active_action_number)
			{
				double deltaWithBMActual = 0.0;
				if constexpr (IncurTestOverhead)
				{
					deltaWithBMActual = sw->CostsPerAction[i] - sw->CostsPerAction[best_active_action_number];
				}
				auto& stat = statistics[best_active_action_number][i];
				double delta = (deltaWithBMActual - stat.MuHat());
				double sigma = stat.SigmaOfMean();
				std::cout << "     " << stat.MuHat() << "  " << (deltaWithBMActual) << "  delta: " << delta << " sigma: " << sigma << "  delta/sigma: " << (delta / sigma) << " mu/sigma  " << stat.MuHat() / stat.SigmaOfMean() << "    " << stat.numStatisticsGathered() << std::endl;

			}

		}
		std::cout << "--------------" << std::endl;

	}

public:
	Sample<MDP> ComputeSample(const State& rootState, RNG& randGen, size_t BenchmarkAction)
	{
		assert(adapter.AwaitsAction(rootState));

		auto allowedActions = adapter.GetAllowedActions(rootState);
	
		auto banditController = BanditController<MDP, IncurTestOverhead>{ adapter.mdp_ptr,rootState, allowedActions };
		auto Trajectories = std::vector<TrajectoryTracker>(SimultaneousTrajectories, adapter.GetTracker(rootState));
		auto TrajectoryInfoList = std::vector<TrajectoryInfo>(SimultaneousTrajectories, TrajectoryInfo{});


		for (size_t trajectoryIter = 0; trajectoryIter < SimultaneousTrajectories; trajectoryIter++)
		{
			banditController.FillPartialExperiment(Trajectories[trajectoryIter], TrajectoryInfoList[trajectoryIter], randGen);
		}

		bool TakeActionsWhileSomeTrajectoriesDoNotNeedAction = false;
		size_t trajectoryIter = 0;
		size_t collectedForCurrentIter = 0;
		while (banditController.MoreStatisticsNeeded())
		{
			//Maybe test whether aborting results that are half-way and that 
			//will not be needed anymore can speed up. Preliminary tests were not promising. 
			//for (size_t i = 0; i < SimultaneousTrajectories; i++)
			//{
			//	if (!banditController.IsActivePerActionNumber[TrajectoryInfoList[i].action_number])
			//	{
			//		TrajectoryInfoList[i].experiment->StatisticCollectionAborted(TrajectoryInfoList[i].action_number);
			//		//Fill spot again:
			//		banditController.FillPartialExperiment(Trajectories[i], TrajectoryInfoList[i], randGen);
			//	}
			//}


			trajectoryIter = 0;
			while (trajectoryIter < SimultaneousTrajectories)
			{
				
				if (adapter.IncorporateEventsUntilNextNonTrivialAction(TrajectoryInfoList[trajectoryIter].experiment->randomSource, TrajectoryInfoList[trajectoryIter].status, Trajectories[trajectoryIter]))
				{//Next step is a nontrivial action, so this state is ready for next action step
					trajectoryIter++;
					collectedForCurrentIter = 0;
				}
				else
				{//Experiment is finished, so collect results and fill spot with new experiment
					//Collect results:
					TrajectoryInfoList[trajectoryIter].experiment->RecordStatistic(TrajectoryInfoList[trajectoryIter].action_number, Trajectories[trajectoryIter].accumulatedCosts);
					//Fill spot again:
					banditController.FillPartialExperiment(Trajectories[trajectoryIter], TrajectoryInfoList[trajectoryIter], randGen);

					++collectedForCurrentIter;
					if (collectedForCurrentIter == 1024 ||
						(TakeActionsWhileSomeTrajectoriesDoNotNeedAction && collectedForCurrentIter == 3))
					{
						TakeActionsWhileSomeTrajectoriesDoNotNeedAction = true;
						trajectoryIter++;
						collectedForCurrentIter = 0;
					}
				}
			}
			//Take an action for trajectories.  
			agent_adapter.TakeActions(Trajectories, randGen, !TakeActionsWhileSomeTrajectoriesDoNotNeedAction);
		}

		auto zValueForBestAlternative = banditController.GetZValueForBestAction(true);

		//Collect the key results. 
		std::vector<float> CostPerAllowedAction{};
		CostPerAllowedAction.reserve(allowedActions.size());
		

		//Determine which action in this list is the benchmark action. 
		size_t NumberCorrespondingToBenchmarkAction = std::distance(allowedActions.begin(), std::find(allowedActions.begin(), allowedActions.end(), BenchmarkAction));
		if (NumberCorrespondingToBenchmarkAction == allowedActions.size())
		{
			for (size_t i = 0; i < allowedActions.size(); i++)
			{
				std::cout << allowedActions[i] << "\t";
			}
			std::cout << "- cannot find " << BenchmarkAction << std::endl;
			std::string message = "Benchmark action not allowed: This may happen if the roll-out agent takes actions that are not allowed.";
			std::cout << message << std::endl;
			throw message;
		}
		size_t BestActiveActionNumber = banditController.GetBestActiveActionNumber();
		double base = banditController.statistics[BestActiveActionNumber][NumberCorrespondingToBenchmarkAction].MuHat();

		for (size_t i = 0; i < allowedActions.size(); i++)
		{		
			double delta = banditController.statistics[BestActiveActionNumber][i].MuHat()-base;
			CostPerAllowedAction.push_back((float)(delta));
		}

		if constexpr (IncurTestOverhead)
		{
			VerifyAsymptoticCorrectness(rootState, banditController.statistics, allowedActions, BestActiveActionNumber);
		}
		size_t bestAction = allowedActions[BestActiveActionNumber];

		auto sample = Sample<MDP>(adapter, rootState, bestAction, zValueForBestAlternative, CostPerAllowedAction);
		return  sample;
	}
};

template<typename MDP, typename Agent, bool IncurTestOverhead>
size_t StateTree<MDP, Agent, IncurTestOverhead>::SimultaneousTrajectories = 250;


/// <summary>
/// Only for testing purposes.
/// </summary>
template <class MDP, class Agent, bool IncurTestOverhead>
ExactMDPSolver<MDP, IncurTestOverhead>* StateTree<MDP, Agent, IncurTestOverhead>::exactMDPSolver;
