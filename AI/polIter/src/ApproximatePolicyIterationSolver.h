#pragma once
#include "NNAgent.h"
#include "NNTrainer.h"
#include "SampleGenerator.h"
#include "HardwareUtilities.h"
#include "AgentComparer.h"
#include "GreedyAgent.h"
#include "MDPAdapter.h"
#include "SampleData.h"
#include "RNG.h"
#include "Settings.h"

template<typename MDP>
class ApproximatePolicyIterationSolver
{
	MDPAdapter<MDP> adapter;

	template <typename Agent>
	void GenerateSamples(Agent& agent, size_t Generation, long long TimeOut, bool append = false)
	{
		auto time1 = std::chrono::steady_clock::now();
		if (HardwareUtilities::world_rank == 0)
		{
			std::cout << "Collecting samples for training generation: " << Generation << std::endl;
		}

		SampleGenerator<MDP,Agent> generator(adapter.mdp_ptr, &agent);


		size_t numSamples = Settings::Current.MaxSamples;
		double FractionOnGPUNode = 0.2;
		
		if (HardwareUtilities::world_size > 1)
		{
			double TotalWorkAbility = HardwareUtilities::world_size-1 + FractionOnGPUNode;
			numSamples /= TotalWorkAbility;
		}
		
		long long timeOut = TimeOut;
		if (HardwareUtilities::world_rank == 0&& HardwareUtilities::world_size>1)
		{//If there are multiple jobs, then the first job is a GPU job
			//that first runs compareagents, while the other jobs are allready simulating. 
			//We will adapt time and work for that job accordingly. 
			numSamples *= FractionOnGPUNode;
			long long reserve{ 10};//For the compareagents that is run on this node. 
			
			if (timeOut > reserve+2)
			{
				timeOut -= reserve;
			}
			else
			{
				timeOut = 2;
			}
		}

		auto trainingData = generator.GenerateSamples(/*samples=*/numSamples,timeOut);
		
		
		trainingData.GatherAndSave(adapter.Identifier(), Generation,append);
		auto time2 = std::chrono::steady_clock::now();
		if (HardwareUtilities::world_rank == 0)
		{
			std::cout << "Completed collection of samples, timing: " << ((time2 - time1).count() / 1000000000) << std::endl;
			trainingData.PrintStatistics();
		}
		HardwareUtilities::AddBarrier();
	
	}

	void TrainAgent(size_t AgentGeneration)
	{
		if (HardwareUtilities::world_rank == 0)
		{//This is the GPU node
			NNTrainer<MDP> trainer(adapter.mdp_ptr, AgentGeneration);
		
			trainer.TrainEnsemble();
		}
		HardwareUtilities::AddBarrier();

	}


public:
	ApproximatePolicyIterationSolver(MDP* mdp_ptr) :adapter{ mdp_ptr }
	{


	}

	template<typename BaselineAgent>
	void SummarizeResults(BaselineAgent& baseline_agent, size_t generations,double optimal= 0.0)
	{
		AgentComparer comparer(adapter.mdp_ptr);
		if (generations == 0)
		{
			return;
		}
		NNAgent<MDP> nnagent(adapter.mdp_ptr, 1);

		if (HardwareUtilities::world_rank == 0)
		{
			std::cout << "Gen 1 vs baseline:" << std::endl;
			comparer.CompareAgents(baseline_agent, nnagent, 100000, optimal);
			for (size_t generation = 2; generation <= generations; generation++)
			{
                NNAgent<MDP> nnagent(adapter.mdp_ptr, generation);
				NNAgent<MDP> nnagent2(adapter.mdp_ptr, generation - 1);
				std::cout << "Gen " << generation << " vs gen "<< (generation-1)  << std::endl;

				comparer.CompareAgents(nnagent2, nnagent, 100000, optimal);

			}
		}
	}

	template<typename BaselineAgent>
	void Solve(BaselineAgent& baseline_agent,size_t resumeGeneration=0, double optimal=0.0)
	{
		auto time1 = std::chrono::steady_clock::now();


		size_t gen=resumeGeneration;
		AgentComparer comparer(adapter.mdp_ptr);

		if (gen == 0)
		{
			GenerateSamples(baseline_agent, gen,/*TimeOut*/Settings::Current.TimeOut);
			gen++;
		}
		size_t generation = gen;
		size_t numGens = Settings::Current.NumGensPolicyOpt;
		for (generation = gen; generation <= numGens; generation++)
		{
			HardwareUtilities::ToConsole("-----------------------");
			HardwareUtilities::ToConsole("Starting training cyclus for agent");

			////Train the agent neural net
			TrainAgent(generation);
			//return;
			////Create the agent
			NNAgent<MDP> nnagent(adapter.mdp_ptr, generation);

			if (HardwareUtilities::world_rank == 0)
			{
				//comparer.CompareAgents(baseline_agent, nnagent, 5000, optimal);
				if (generation > 1)
				{
					NNAgent<MDP> lastAgent(adapter.mdp_ptr, generation - 1);
					//		comparer.CompareAgents(lastAgent, nnagent, 20000, optimal);
				}
			}
			//return;
			if (generation < numGens)
			{
				GenerateSamples(nnagent, generation,/*TimeOut*/Settings::Current.TimeOut, false);
			}
		}
		auto time2 = std::chrono::steady_clock::now();
		if (HardwareUtilities::world_rank == 0)
		{
			std::cout << "Finished approximate policy iteration. Timing (in second): " << ((time2 - time1).count() / 1000000000) << std::endl;
			std::cout <<"Settings: " << Settings::Current.AsString() << std::endl;
			std::cout <<"Problem: " << adapter.Identifier() << std::endl;
		}
	}

};

