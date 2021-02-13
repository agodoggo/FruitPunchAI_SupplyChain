#pragma once
#include<vector>
#include<tuple>
#include "ExactMDPSolver.h"
#include "StateTree.h"
#include "RNG.h"
#include <ostream>
#include "Sample.h"
#include "SampleData.h"
#include <future>
#include <algorithm>
#include "math.h" 
#include "MDPAdapter.h"
#include "AgentAdapter.h"
#include "HardwareUtilities.h"
#include "RandomAgent.h"
#include <sstream>
#ifdef __linux__
//In my case, Torch is only available on the linux server. 
//There is a torch for visual studio c++, but it is not stable
//so not recommended to try to get that one to work..:q
#define TorchAvailable true
#endif
#if TorchAvailable
#include <torch/torch.h>
#endif

template <class MDP, class Agent, bool IncurTestOverhead = false>
class SampleGenerator
{	

	MDPAdapter<MDP> adapter;
	AgentAdapter<MDP, Agent> agent_adapter;
public:

	using State = typename MDP::State;



	SampleGenerator(const MDP* mdp_ptr, Agent* agent_ptr) :adapter{ mdp_ptr }, agent_adapter{mdp_ptr,agent_ptr }, rng{ 26071983 }
	{
	}

	SampleGenerator(const MDP* mdp_ptr, Agent* agent_ptr, size_t randSeed) :adapter{ mdp_ptr }, agent_adapter{ mdp_ptr,agent_ptr }, rng{ randSeed }
	{
	}



public:

	SampleData<MDP> GenerateSamples(size_t SamplesToCollect,long long TimeOutInSeconds = 1000000000000 /*default: no real timeout*/)
	{//This function could probably be more readable if I would utilize the return value of 
		//the futures, but it works..	
		if (SamplesToCollect == 0)
		{
			throw "Cannot collect zero samples";
		}
		std::cout << "Node " << HardwareUtilities::world_rank << " started " << std::endl;


		//Determine number of batches (/jobs); each will be presented as a separate job.
		size_t batches = SamplesToCollect;
		auto time1 = std::chrono::steady_clock::now();

		if (batches > HardwareUtilities::hardware_threads)
		{
			batches = HardwareUtilities::hardware_threads;
		}
			
		bool parallelAct = true;
		ExactMDPSolver<MDP, IncurTestOverhead> solver{adapter.mdp_ptr,true };//<- note that this must remain outside of constexpr if; lifetime must be scope of this function!
		if constexpr (IncurTestOverhead)
		{//This is only for testing purposes, i.e. validation of simulation
			batches = 1;
			parallelAct = false;
			solver.ComputePolicyCosts(*(agent_adapter.agent_ptr), 1e-8);
			StateTree<MDP, Agent, IncurTestOverhead>::exactMDPSolver = &solver;
		}
	
		//divide up the samples that need to be collected over the batches.
		//Round up initially, and down later. 
		size_t RemainingSamples = SamplesToCollect;
		size_t RemainingBatches = batches;
		std::vector<Worker> Workers{};
		std::vector<std::future<void>> futures;
		Workers.reserve(batches);
		futures.reserve(batches);
		size_t totalCollected = 0;
	
		
		for (size_t i = 0; i < batches; i++)
		{
			size_t ToCollectInBatch = (size_t)(round(RemainingSamples * 1.0 / (RemainingBatches--)));
			Workers.push_back(Worker{ rng.NextGen(),&adapter,&agent_adapter,ToCollectInBatch,/*MaxSecond*/TimeOutInSeconds });

			RemainingSamples -= ToCollectInBatch;
			totalCollected += ToCollectInBatch;
			//std::this_thread::sleep_for(std::chrono::milliseconds(100));
			if (parallelAct)
			{
				futures.push_back(std::async([](Worker* worker, size_t number) {worker->Generate(number);}, &(Workers[i]), i));
			}
		}
		
		if (parallelAct)
		{
			//Execute the work in parallel:
			for (auto &e : futures)
			{//wait until all parallel computations are completed
				e.get();
			}
		}
		else
		{
			size_t number{ 0 };
			for (auto&worker : Workers)
			{
				worker.Generate(number++);
			}
		}
				
		SampleData<MDP> combinedData;
				
		combinedData.samples.reserve(SamplesToCollect);
		for (auto&worker : Workers)
		{
			auto&samples = worker.data.samples;
			for (size_t i = 0; i < samples.size(); i++)
			{
				combinedData.samples.push_back(std::move(samples[i]));
			
			}
		}

		auto time2 = std::chrono::steady_clock::now();

		std::ostringstream str{};
		str <<  "Node " << HardwareUtilities::world_rank << " collected " << combinedData.samples.size() <<"/" <<  SamplesToCollect << " samples over " << batches << " treads. Timing: " << ((time2 - time1).count() / 1000000000) << "s";
		if (combinedData.samples.size() < SamplesToCollect)
		{
			str << "(TIMEOUT)";
		//	for (auto& worker : Workers)
		//	{
		//		str << " " << worker.data.samples.size();
		//	}
		}
		std::cout << str.str() << std::endl;
		//Randomize so that the samples are in no particular order. 
		std::shuffle(std::begin(combinedData.samples), std::end(combinedData.samples), rng.gen_);
		return combinedData;
	}

private:

	RNG rng;

	class Worker
	{
	public:
		
		Worker(RNG randgen, MDPAdapter<MDP>* adapter_ptr, AgentAdapter<MDP,Agent>* agent_adapter_ptr, size_t SamplesToCollect, long long MaxTimeInSeconds = 10000000) :
			RandGen{ randgen }, adapter_ptr{ adapter_ptr }, agent_adapter_ptr{ agent_adapter_ptr }, SamplesToCollect{ SamplesToCollect }, MaxTimeInSeconds{ MaxTimeInSeconds }
		{
		}

		RNG RandGen;
		MDPAdapter<MDP>* adapter_ptr;
		AgentAdapter<MDP, Agent>* agent_adapter_ptr;
		
		size_t SamplesToCollect;
		long long MaxTimeInSeconds;
		bool TimeoutReached = false;
		SampleData<MDP> data;

		void Generate(size_t batchNumber)
		{
			auto time1 = std::chrono::steady_clock::now();
			data.samples.reserve(SamplesToCollect);			
			StateTree<MDP,Agent,IncurTestOverhead> stateTree(adapter_ptr->mdp_ptr, agent_adapter_ptr->agent_ptr);
			

#if TorchAvailable
			torch::NoGradGuard guard;
			//This won't compile at present. 
			//torch::set_num_threads(1);
			//at::AutoNonVariableTypeMode non_var_type_mode(true);
#endif     
			RandomAgent randagent(adapter_ptr->mdp_ptr);
			AgentAdapter randomAgentAdapter(adapter_ptr->mdp_ptr, &randagent);

			auto tracker = adapter_ptr->GetRandomTracker(RandGen);

			bool ResetStateToStartAfterTrajectory = true;

			if (!adapter_ptr->HasFinalState())
			{
				//Infinite horizon. 
				//So we want to start in a "random state", to avoid that all threads 
				//start in the same state and thus collect very similar samples early on. 
				ResetStateToStartAfterTrajectory = false;
				for (size_t i = 0; i < 2; i++)
				{
					auto randomSource = adapter_ptr->GetRandomSource(tracker, RandGen);
					auto randomSourceStatus = adapter_ptr->GetRandomSourceStatus(tracker, randomSource);
					while (!adapter_ptr->TrajectoryHasEnded(tracker, randomSource, randomSourceStatus))
					{
						if (adapter_ptr->AwaitsAction(tracker))
						{
							randomAgentAdapter.TakeAction(tracker, RandGen);
						}
						else
						{
							adapter_ptr->IncorporateEvent(tracker, randomSource, randomSourceStatus);
						}
					}
				}
			}



			do
			{
				//Intentional: In some sub-trajectories, up to 2 times the number of random actions, in some sub-trajectories,
				//no random actions at all.
				double FractionOfRandomActions = RandGen.NextUniform()* 2.0* Settings::Current.FractionRandomActions;
				auto randomSource = adapter_ptr->GetRandomSource( tracker, RandGen);
				auto randomSourceStatus = adapter_ptr->GetRandomSourceStatus( tracker,randomSource);
				

				while (!adapter_ptr->TrajectoryHasEnded(tracker,randomSource, randomSourceStatus )//While loop X
					&& (data.samples.size() < SamplesToCollect) && (!TimeoutReached))
				{
					if (adapter_ptr->AwaitsAction(tracker))
					{

						if constexpr(!IncurTestOverhead)
						{
							if (adapter_ptr->TakeActionIfTrivial(tracker))
							{//The action in this state is trivial (because only a single one is valid), 
								//and this action was incorporated by the adapter.
								continue;//The while loop X
							}
						}
						//So there are multiple actions. Get simulation statistics.
						//First, find the benchmark action:
						size_t action = agent_adapter_ptr->GetAction(tracker.state, RandGen);
						//Now, do the simulations:
						data.samples.push_back(stateTree.ComputeSample(tracker.state, RandGen, action));
						
						if (RandGen.NextUniform() > FractionOfRandomActions)
						{//We follow the roll-out actions
							size_t bestAction = data.samples.back().BestAction;
							adapter_ptr->IncorporateAction(tracker, bestAction);
						}
						else
						{//We take a random actions
							randomAgentAdapter.TakeAction(tracker,RandGen);
						}
					}
					else
					{
						adapter_ptr->IncorporateEvent(tracker, randomSource, randomSourceStatus);
					}
					auto time2 = std::chrono::steady_clock::now();
					long long delta = ((time2 - time1).count() / 1000000000);
					if (delta > MaxTimeInSeconds)
					{
						TimeoutReached = true;
					}

				}
				if (ResetStateToStartAfterTrajectory)
				{
					tracker = adapter_ptr->GetRandomTracker(RandGen);
				}
				else
				{//So we continue with the current state, to get more diverse samples. 
					tracker = adapter_ptr->GetTracker(tracker.state);
				}
		
			} while (data.samples.size() < SamplesToCollect && (!TimeoutReached));
			
			

		}

	};
};

