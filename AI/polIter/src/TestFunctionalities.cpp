#include "TestFunctionalities.h"
#include "RNG.h"
#include <vector>
#include <iostream>
#include "ExactMDPSolver.h"
#include "NetworkMDP.h"
#include "NetworkAgent.h"
#include "PoultryMDP.h"
#include "PoultryAgent.h"
#include "SampleGenerator.h"
#include "Demonstrator.h"
#include "TabularAgent.h"
#include "ImprovingAgent.h"
#include "AgentComparer.h"
#include "SimpleAgent.h"
#include "SimpleMDP.h"
#include "CartesianProduct.h"
#include "MDPAdapter.h"
#include "PoultryMDP.h"
#include "HardwareUtilities.h"
#include "ApproximatePolicyIterationSolver.h"
#include "FIFOQueue.h"
#include <sstream>
#include "InfoAcqMDP.h"
#include "TravelMDP.h"
#include "SCGameMDP.h"
#include "SCGameState.h"
#include "DiscreteDist.h"
#include "LostSalesMDP.h"
#include "LostSalesBaseStockAgent.h"
#include "Settings.h"
#include "SLAInvAgent.h"
#include "SLAInvMDP.h"
#include "SLAInvState.h"

void TestFunctionalities::TestDemonstrator()
{
	//Demonstrator
	auto mdp = NetworkMDP::GetComplexInstance(3);
	auto agent = NetworkAgent(&mdp);
	Demonstrator demonstrator{ &mdp };
	auto startstate = mdp.InitialState();
	
	auto trace = demonstrator.GetTrace(startstate, agent, 2);
	for (auto& comb : trace.StateSequence)
	{
		std::cout << comb.state;
	}
	ExactMDPSolver solver{&mdp};
	solver.ComputeOptimalCosts(agent, 1e-4, { 0 });
	TabularAgent<NetworkMDP> tabAgent{&mdp,&solver};
	{
		auto trace = demonstrator.GetTrace(startstate, tabAgent, 20);
		for (auto& comb : trace.StateSequence)
		{
			std::cout << comb.state;
		}
	}
	ImprovingAgent impAgent{ &mdp,&agent,{} };
	{
		auto trace = demonstrator.GetTrace(startstate, impAgent, 20);
	}
}

void TestFunctionalities::TestImprovingAgent()
{

	auto mdp = NetworkMDP::GetComplexInstance(3);
	mdp = NetworkMDP::GetSemiconInstance(false);
	auto agent = NetworkAgent(&mdp);
	auto toImprove = NetworkAgent(&mdp);
	auto improvingAgent = ImprovingAgent(&mdp, &toImprove, { });

	AgentComparer comparer(&mdp);
	comparer.CompareAgents(agent,improvingAgent,2);


}

void TestFunctionalities::TestOptimalAlgorithm()
{
	{
		auto mdp = LostSalesMDP::GetSimpleInstance();
		double dynamicCosts = 0.0;
		double heuristicCosts = 0.0;
		{
			double tolerance{ 1e-6 };
			ExactMDPSolver exactSolver{ &mdp };

			GreedyAgent agent(&mdp);
			dynamicCosts = 2*exactSolver.ComputeOptimalCosts(agent, tolerance);

			//heuristicCosts = 2*exactSolver.ComputePolicyCosts(agent, tolerance);
		}
		std::cout << dynamicCosts << "  " << heuristicCosts << "  " << (heuristicCosts - dynamicCosts) / dynamicCosts << std::endl;
	}
	return;
	{
		auto mdp = PoultryMDP::GetSimpleInstance();
	
		double dynamicCosts=0.0;
		double heuristicCosts = 0.0;
		{
			double tolerance{ 1e-6 };
			ExactMDPSolver exactSolver{ &mdp };

			//PoultryAgent agent(&mdp);
			GreedyAgent agent(&mdp);
			dynamicCosts = exactSolver.ComputeOptimalCosts(agent, tolerance);

			heuristicCosts = exactSolver.ComputePolicyCosts(agent, tolerance);
		}
		std::cout << dynamicCosts << "  " << heuristicCosts << "  " << (heuristicCosts - dynamicCosts) / dynamicCosts << std::endl;
	}
	return;
	{
		//auto mdp = NetworkMDP::GetSemiconInstance(false);
		auto mdp = NetworkMDP::GetSemiconInstance(false);
		//auto mdp = NetworkMDP::GetComplexInstance(4);

		double dynamicCosts;
		double heuristicCosts = 0.0;
		{
			double tolerance{ 1e-6 };
			ExactMDPSolver exactSolver{ &mdp };

			NetworkAgent agent(&mdp);
	
			dynamicCosts = exactSolver.ComputeOptimalCosts(agent, tolerance, {});

			heuristicCosts = exactSolver.ComputePolicyCosts(agent, tolerance);
		}
		std::cout << dynamicCosts << "  " << heuristicCosts << "  " << (heuristicCosts - dynamicCosts) / dynamicCosts << std::endl;
	}

}

void TestFunctionalities::TestCartProduct()
{
	{
		std::vector<std::vector<float>> BaseSets;

	//	BaseSets.push_back({ });
		BaseSets.push_back({ 2.0f,2.2f,2.4f });
		BaseSets.push_back({ 2.0f,2.2f,2.4f });

		CartesianProduct CP(BaseSets);
		std::cout << "Testing Cartesian Product " << std::endl;

		while (CP.HasNext())
		{
			auto& next = CP.GetNext();
			for (auto i : next)
			{
				std::cout << i << "\t";
			}
			std::cout << " ;" << std::endl;
		}
		CP.Reset();
		std::cout << "reset" << std::endl;
		while (CP.HasNext())
		{
			auto& next = CP.GetNext();
			for (auto i : next)
			{
				std::cout << i << "\t";
			}
			std::cout << " ;" << std::endl;
		}
	}

	{
		
		std::vector<size_t> MaxLevels = { 2 };
		CartesianProduct<double> CP(MaxLevels);
		std::cout << "Testing Cartesian Product " << std::endl;

		while (CP.HasNext())
		{
			auto& next = CP.GetNext();
			for (auto i : next)
			{
				std::cout << i << "\t";
			}
			std::cout << " ;" << std::endl;
		}
		CP.Reset();
		std::cout << "reset" << std::endl;
		while (CP.HasNext())
		{
			auto& next = CP.GetNext();
			for (auto i : next)
			{
				std::cout << i << "\t";
			}
			std::cout << " ;" << std::endl;
		}

	}

	//CartesianProduct<Test> t;
}

void TestFunctionalities::TestAdapter()
{
	std::cout << "testing adapter" << std::endl;
	{
		using MDP = NetworkMDP;
		auto mdp = MDP::GetComplexInstance(4);
		RNG rng{ 26071983 };
		auto adapter = MDPAdapter<MDP>{ &mdp };
		auto tracker = adapter.GetNewTracker();
		auto randomSource = adapter.GetRandomSource(tracker,rng);
		auto randomSourceStatus = adapter.GetRandomSourceStatus(tracker, randomSource);
		
		if (adapter.IncorporateEventsUntilNextNonTrivialAction(randomSource, randomSourceStatus, tracker))
		{
			adapter.Test(tracker.state);
		}
		else
		{
			std::cout << "no action state reached" << std::endl;
		}

		
		
		
	}

	{
		using MDP = PoultryMDP;
		auto mdp = MDP::GetSimpleInstance();
		RNG rng{ 26071983 };
		auto adapter = MDPAdapter<MDP>{ &mdp };
		auto tracker = adapter.GetNewTracker();
		auto randomSource = adapter.GetRandomSource(tracker, rng);
		auto randomSourceStatus = adapter.GetRandomSourceStatus(tracker, randomSource);

		if (adapter.IncorporateEventsUntilNextNonTrivialAction(randomSource, randomSourceStatus, tracker))
		{
			adapter.Test(tracker.state);
		}
		else
		{
			std::cout << "no action state reached" << std::endl;
		}
	}
	

	{
		using MDP = InfoAcqMDP;
		auto mdp = MDP::GetSimpleInstance();
		RNG rng{ 26071983 };
		auto adapter = MDPAdapter<MDP>{ &mdp };
		auto tracker = adapter.GetNewTracker();
		//auto randomSource = adapter.GetRandomSource(tracker, rng);
		//auto randomSourceStatus = adapter.GetRandomSourceStatus(tracker, randomSource);

		//if (adapter.IncorporateEventsUntilNextNonTrivialAction(randomSource, randomSourceStatus, tracker))
		{
			adapter.Test(tracker.state);
		}
		//else
		{
			//std::cout << "no action state reached" << std::endl;
		}
	}


	return;
}

void TestFunctionalities::TestApproximatePolicyImprovement()
{
	{
		std::string SubDir = "InfoAcqAPI_test";
		if (HardwareUtilities::world_rank == 0)
		{
			std::cout << SubDir << std::endl;
		}
		auto mdp = InfoAcqMDP::GetSimpleInstance();

		HardwareUtilities::SetSubDir(SubDir);
		//auto mdp = PoultryMDP::GetSimpleInstance();
		//auto exactSolver = ExactMDPSolver(&mdp);
		auto agent = RandomAgent(&mdp);
		//auto agentCosts = exactSolver.ComputePolicyCosts(agent, 1e-8);
		//auto optimalcosts = exactSolver.ComputeOptimalCosts(1e-8); 
		//double optimalcosts{};
		//double agentCosts{};

		ApproximatePolicyIterationSolver solver{ &mdp };

		solver.Solve(agent,/*resume_generation*/ 0, 0);
		//solver.SummarizeResults(agent, 3, optimalcosts);

	}
	return;
	//{
	//	std::string SubDir = "PoultryTestsAPI_test";
	//	if (HardwareUtilities::world_rank == 0)
	//	{
	//		std::cout << SubDir << std::endl;
	//	}
	//	auto mdp = PoultryMDP::GetComplexInstance();

	//	HardwareUtilities::SetSubDir(SubDir);
	//	//auto mdp = PoultryMDP::GetSimpleInstance();
	//	auto exactSolver = ExactMDPSolver(&mdp);
	//	auto agent = PoultryAgent(&mdp);
	//	//auto agentCosts = exactSolver.ComputePolicyCosts(agent, 1e-8);
	//	//auto optimalcosts = exactSolver.ComputeOptimalCosts(1e-8);
	//	//For the complex instance. 
	//	//3.9 million states. 
	//	double optimalcosts{ 1.65318 };
	//	//double agentCosts{ 24.6555 };
	//	
	//	ApproximatePolicyIterationSolver solver{ &mdp };
	//	
	//	solver.Solve(agent,/*resume_generation*/ 1, optimalcosts);
	//	//solver.SummarizeResults(agent, 3, optimalcosts);
	//
	//}
	return;
	{


		using MDP = NetworkMDP;
		auto mdp = MDP::GetSemiconInstance(false);
		auto agent = NetworkAgent(&mdp);

		auto exactSolver = ExactMDPSolver(&mdp);

		//auto optimalcosts= exactSolver.ComputeOptimalCosts(1e-8);
		//auto agentCosts= exactSolver.ComputePolicyCosts(agent, 1e-8);
		double agentCosts{ 414.826 };
		double optimalcosts{ 338.83 };
		if (HardwareUtilities::world_rank == 0)
		{
			std::cout << "Exact: " << optimalcosts << " network agent" << agentCosts << std::endl;
		}
		ApproximatePolicyIterationSolver solver{ &mdp };
		solver.Solve(agent, 0, optimalcosts);
	}


}

void TestFunctionalities::TestFIFOQueue()
{
	FIFOQueue<unsigned short> queue{ 3 };
	queue.push_back(3);
	queue.push_back(2);
	queue.push_back(1);
	queue.pop_front();

	std::cout << queue << std::endl;
	{
		auto os = HardwareUtilities::GetOutputStreamTo("test_TestFIFOQueue.txt");

		os << queue << std::endl;
	}

	auto is = HardwareUtilities::GetInputStreamFrom("test_TestFIFOQueue.txt");
	FIFOQueue<unsigned short> copy{};
	is >> copy;


	std::cout << copy << std::endl;

	std::cout << (copy == queue) << std::endl;
	copy.pop_front();
	std::cout << (copy == queue) << std::endl;


}

void TestFunctionalities::TestSampleEfficiency(bool netIsReady=true)
{
//	using MDP = NetworkMDP;
//	auto mdp = MDP::GetSemiconInstance(false);
//	auto agent = NetworkAgent(&mdp);

	HardwareUtilities::SetSubDir("TestSampleEfficiency");

	using MDP = PoultryMDP;
	auto mdp = MDP::GetComplexInstance();
	auto agent = PoultryAgent(&mdp);

	int Generation=0;
	{
		if (!netIsReady)
		{

			SampleGenerator generator(&mdp, &agent);
			size_t numSamples = 5 * HardwareUtilities::hardware_threads;
			auto trainingData = generator.GenerateSamples(/*samples=*/numSamples);
			trainingData.GatherAndSave(mdp.Identifier(), Generation);
			return;
			Generation++;

			NNTrainer trainer(&mdp, Generation);
			trainer.TrainEnsemble();
			return;
		}
		else
		{
			Generation = 1;
		}
	}
	//Create the agent
	NNAgent nnagent(&mdp, Generation);
	AgentComparer comparer(&mdp);
	//comparer.CompareAgents(agent, nnagent, 1000);

	SampleGenerator generator(&mdp, &nnagent);

	for (size_t numsimultTraj : std::vector<size_t>{ 100, 200,350,500,700 })
	{
		std::cout << "-----------------------" << std::endl;
		std::cout << numsimultTraj << std::endl;
		StateTree<NetworkMDP, NNAgent<MDP>>::SimultaneousTrajectories = numsimultTraj;

		auto time = std::chrono::steady_clock::time_point();
		size_t numSamples = 100 * HardwareUtilities::hardware_threads;
		auto trainingData = generator.GenerateSamples(/*samples=*/numSamples);

		//Gathers data from all nodes in the MPI world, and saves it at location given by identifier. 
		trainingData.GatherAndSave(mdp.Identifier(), Generation);
		auto time2 = std::chrono::steady_clock::time_point();
		std::cout << "Completed collection of samples, timing: " << ((time2 - time).count() / 1000000000) << std::endl;

	}

}

void TestFunctionalities::TestInputOutput()
{
	{
		using MDP = NetworkMDP;
		auto mdp = MDP::GetSimpleInstance();
		MDPAdapter adapter{ &mdp };
		Sample<MDP>::TestInputOutput(adapter, "TestSampleInputOutput2.txt");
	}
	{
		using MDP = PoultryMDP;
		auto mdp = MDP::GetSimpleInstance();
		MDPAdapter adapter{ &mdp };
		Sample<MDP>::TestInputOutput(adapter, "TestSampleInputOutput2.txt");
	}
}

void TestFunctionalities::TestInfoAcq()
{
	auto mdp = InfoAcqMDP::GetComplexInstance();
	auto State = mdp.InitialState();

	std::cout << "test" << std::endl;

	//RandomAgent agent{ &mdp };
	//GreedyAgent agent2{ &mdp };
	//ImprovingAgent impAgent{ &mdp, &agent2};

	//AgentComparer comparer{ &mdp };

	//comparer.CompareAgents(agent2, impAgent, 10);

	ExactMDPSolver solver{ &mdp };
	solver.ComputeOptimalCosts(1e-8);
}

void TestFunctionalities::TestTravelWoman()
{
	auto mdp = TravelMDP::GetSimpleInstance();
	auto State = mdp.InitialState();

	GreedyAgent agent{ &mdp };
	//RandomAgent agent2{ &mdp };

	MDPAdapter adapter{ &mdp };
	AgentAdapter agentAdapter{ &mdp,&agent };
	
	auto tracker = adapter.GetNewTracker();

	RNG rng{123123};

	for (size_t i = 0; i < 100; i++)
	{
		if (adapter.AwaitsAction(tracker))
		{
			agentAdapter.TakeAction(tracker, rng);
			std::cout << tracker.state << std::endl;
		}
		else
		{
			auto probs = mdp.EventProbs(tracker.state);
			auto nextEvent = rng.GenEvent(probs);
			tracker.accumulatedCosts += mdp.ModifyStateWithEvent(tracker.state, nextEvent);
			std::cout << tracker.state << std::endl;
			std::cout << tracker.accumulatedCosts << std::endl;
		}
	}

	
	
	ImprovingAgent impAgent{ &mdp, &agent };
	ExactMDPSolver solver{ &mdp };
	auto agentCosts = solver.ComputePolicyCosts(agent, 1e-4);
	std::cout << agentCosts << std::endl;
	//AgentComparer comparer{ &mdp };

	//comparer.CompareAgents(agent2, agent, 10);

	{
		solver.ComputeOptimalCosts(1e-8);
		TabularAgent agent(&mdp, &solver);
		AgentAdapter agentAdapter{ &mdp,&agent };
		auto tracker = adapter.GetNewTracker();

		for (size_t i = 0; i < 100; i++)
		{
			if (adapter.AwaitsAction(tracker))
			{
				agentAdapter.TakeAction(tracker, rng);
				std::cout << tracker.state << std::endl;
			}
			else
			{
				auto probs = mdp.EventProbs(tracker.state);
				auto nextEvent = rng.GenEvent(probs);
				tracker.accumulatedCosts += mdp.ModifyStateWithEvent(tracker.state, nextEvent);
				std::cout << tracker.state << std::endl;
				std::cout << tracker.accumulatedCosts << std::endl;
			}
		}
	}
}

void TestFunctionalities::TestPoultryAgent()
{
	auto mdp = PoultryMDP::GetComplexInstance();
	auto agent = PoultryAgent(&mdp);
//	auto randAgent = RandomAgent(&mdp);
	auto improvingAgent = ImprovingAgent(&mdp, &agent);
	AgentComparer comparer(&mdp);

	comparer.CompareAgents(improvingAgent, agent, 10);


	std::cout << "test" << std::endl;
}

void TestFunctionalities::TestSCGame()
{
	HardwareUtilities::ToConsole("Testing SC Game");

	bool AdapProbs = false;
	auto mdp = SCGameMDP::GetSimpleInstance(AdapProbs);

	std::string subdir = "SCGame";
	if (AdapProbs)
	{
		subdir = "SCGameAdap";
	}

	HardwareUtilities::ToConsole(subdir);
	HardwareUtilities::SetSubDir(subdir);
	ApproximatePolicyIterationSolver solver{ &mdp };
	auto agent = RandomAgent(&mdp);
	solver.Solve(agent,/*resume_generation*/0, 7.184);
	//solver.SummarizeResults(agent, 4, 7.184);



}

void TestFunctionalities::SCGameAIExample()
{

	size_t round = 1; //Options : 1,2,3,4,5,6,7,8,9,10
	std::vector<size_t> BoardState;
	//This is the initial boardstate, but anything goes:
	BoardState = {2,2,2,2,3,3,0,3,0 };//This uses the encoding in the PDF file that I send. 
	GetRecommendation(BoardState, round);
}


void TestFunctionalities::VisualizeSCGame()
{


	return;
	bool AdapProbs = false;
	HardwareUtilities::ToConsole("Visualising SC Game");
	
	std::string subdir = "SCGame";
	if (AdapProbs)
	{
		subdir = "SCGameAdap";
	}

	HardwareUtilities::SetSubDir(subdir);

	using MDP = SCGameMDP;
	auto mdp = MDP::GetSimpleInstance(AdapProbs);
	Demonstrator<MDP> demonstrator(&mdp);
	auto startState = mdp.InitialState();
	std::vector<size_t> EventList = { 1, 8/*, 2, 3, 13, 3, 10, 8, 4, 9*/ };

	//{
	//	RandomAgent<MDP> randAgent(&mdp);
	//	auto trace = demonstrator.GetTrace(startState, randAgent, EventList, 800);
	//	auto os = HardwareUtilities::GetOutputStreamTo("RandAgent.tex");
	//	mdp.ToTikzFigure(os, trace, false);
	//}
	//return;
	for (size_t generation = 4; generation <= 4; generation++)
	{
		NNAgent<MDP> nnAgent(&mdp,generation);
		std::cout << "neural network loaded" << std::endl;
		auto trace = demonstrator.GetTrace(startState, nnAgent, EventList, 800);
		std::cout << HardwareUtilities::IORootPath;
		std::string filename = "XXX";
		filename +=  ".tex";
		auto os = HardwareUtilities::GetOutputStreamTo(filename);
		os << "test"; 
		return;
		//std::string filename = "NN";
		//filename += std::to_string(generation) + ".tex";
		//auto os = HardwareUtilities::GetOutputStreamTo(filename);
		//mdp.ToTikzFigure(os, trace, true);
	}
	//return;
	
	//for (size_t generation = 4; generation <= 4; generation++)
	//{
	//	NNAgent<MDP> nnAgent(&mdp, generation);
	//	ImprovingAgent imp(&mdp, &nnAgent, {});
	//	auto trace = demonstrator.GetTrace(startState, imp, EventList, 800);
	//	std::string filename = "IMP";
	//	filename += std::to_string(generation) + ".tex";
	//	auto os = HardwareUtilities::GetOutputStreamTo(filename);
	//	mdp.ToTikzFigure(os, trace, true);
	//}

	return;

}

//Get's the recommended action for a certain boardgamestate and round.
//BoardGameState is a vector with 9 numbers representing the amount of inventory
//at 9 locations. 
void TestFunctionalities::GetRecommendation(std::vector<size_t> BoardGameState, size_t round)
{
	RNG rng{26071983};
	bool AdapProbs = false;

	std::string subdir = "SCGame";
	HardwareUtilities::SetSubDir(subdir);

	using MDP = SCGameMDP;
	auto mdp = MDP::GetSimpleInstance(AdapProbs);
	auto state = mdp.BlankState();
	
	state.DemandsRemaining = 11 - round;//So in the first round, there are 10 rounds remaining. 
	            							 //In the 10th round, there is one (last) demand remaining.
	for (size_t i = 0; i < 9; i++)
	{//With this we fill the state with the current board state:
		
		state.InventoryPerLocation[i + 8] = BoardGameState[i];
	}
	//I trained four generations of agents, the last one is the best one:
	size_t agentGeneration = 4;

	if (false)
	{//To work, the program loads checkpoints of neural net and metadata from these locations:
		//For debugging, you could turn this on:
		auto loc = HardwareUtilities::PathToModelFile(mdp.Identifier(), agentGeneration, 0, false);
		auto locMeta = HardwareUtilities::PathToModelFile(mdp.Identifier(), agentGeneration, 0, true);
		std::cout << "loc: " << loc << std::endl;
		std::cout << "locMeta: " << locMeta << std::endl;
	}
	NNAgent<MDP> agent(&mdp, agentGeneration);//This agent basically encapsulates the neural net and
	                            //enables it to take actions in the MDP/game
	                           //it loads files from the loc and locMeta locations above. 

	AgentAdapter adapter(&mdp, &agent);
	
	{//Here we place the external orders:
		auto action = adapter.GetAction(state, rng);
		mdp.ModifyStateWithAction(state, action);
	}	
	{//Here we do the assembly decision:
		auto action = adapter.GetAction(state, rng);
		mdp.ModifyStateWithAction(state, action);
	}	
	{//Here we do the transshipment decision:
		auto action = adapter.GetAction(state, rng);
		mdp.ModifyStateWithAction(state, action);
	}
	for (size_t i = 0; i < 8; i++)
	{//Now we print the decisions that were taken. 
		std::cout << state.InventoryPerLocation[i] << "\t";
	}
	std::cout << std::endl;



}

void TestFunctionalities::TestDiscreteDist()
{
	auto geom = DiscreteDist::GetPoissonPMF(5.0);
	DiscreteDist dist(geom);
	
	auto leadtimeDemand = DiscreteDist::GetZeroDistribution();
	
	for (size_t i = 0; i < 3+1; i++)
	{
		leadtimeDemand = leadtimeDemand.Add(dist);
	}
	dist.ToConsole();
	leadtimeDemand.ToConsole();
	std::cout << leadtimeDemand.Expectation() << std::endl;
}

void TestFunctionalities::LostSalesPaperVariance()
{
	using MDP = LostSalesMDP;
	double tolerance{ 1e-9 };
	{
		auto mdp = MDP::GetComplicatedInstance();
		std::cout << mdp.Identifier();
		ExactMDPSolver exactSolverBig{ &mdp };
		exactSolverBig.ComputeOptimalCosts(tolerance);
	}
	return;


	auto mdp = MDP::GetInstance(MDP::NumInstances() - 1);
	HardwareUtilities::SetSubDir(mdp.Identifier());
	//Silent MDP solver:
	ExactMDPSolver AverageCostSolver{ &mdp, true };
	TabularAgent agent{ &mdp,&AverageCostSolver };
	SampleGenerator<MDP, TabularAgent<MDP>, true> sampleGen(&mdp,&agent);
	sampleGen.GenerateSamples(1);

}

void TestFunctionalities::LostSalesPaperAnalyze()
{
	double tolerance{ 1e-4 };
	using MDP = LostSalesMDP;
	if (HardwareUtilities::world_rank == 0)
	{
		for (size_t i = MDP::NumInstances()-1; i < MDP::NumInstances(); i++)
		{
			for (size_t gen = 4; gen <= 4; gen++)
			{
				auto mdp = MDP::GetInstance(i);
				HardwareUtilities::SetSubDir(mdp.Identifier());
				//Silent MDP solver:
				ExactMDPSolver AverageCostSolver{ &mdp, false };
				NNAgent<MDP> nnagent(&mdp, gen);
				double approxGain = AverageCostSolver.ComputePolicyCosts(nnagent, tolerance);
				double gain = AverageCostSolver.ComputeOptimalCosts(nnagent, tolerance);
				std::cout << mdp.Identifier() << " gen " << gen << "  costs:  " << gain << "  (approxGain-gain)/gain(%) " << (100 * (approxGain - gain) / gain) << std::endl;
			}
			std::cout << std::endl;
		}
	}
	return;
}


void TestFunctionalities::LostSalesPaperTraining(bool evaluate)
{
	using MDP = LostSalesMDP;
	
	Settings::Current.HiddenLayers = { 64,32,32 };//128,64,64
	Settings::Current.InitialRollouts = 250;//500
	Settings::Current.MaxRollouts = 4000;//4000
	Settings::Current.TimeOut = 9000;
	Settings::Current.MaxSamples = 2500;//4000
	Settings::Current.MiniBatchSize = 64;	
	Settings::Current.NumGensPolicyOpt = 4;
	Settings::Current.kValueBanditOptimizer = 2.0;
	Settings::Current.FractionRandomActions = 0.04;//0.05

	for (size_t i = 0; i < MDP::NumInstances(); i++)
	//for(size_t i : {0,1/*18,20,21,22,23*/})
	{
		auto mdp = MDP::GetInstance(i);
		HardwareUtilities::SetSubDir(mdp.Identifier(), Settings::Current.AsString());
			
		
		if (!evaluate)
		{
			//We train with alpha=0.975
			mdp.alpha = 0.975;
			ApproximatePolicyIterationSolver approxSolver{ &mdp };
			LostSalesBaseStockAgent agent{ &mdp };
			//RandomAgent agent{ &mdp };
			approxSolver.Solve(agent,0, 2 * 14.678 / (1 - mdp.alpha));
		}
		else
		{
			double tolerance{ 1e-8 };
			//Silent MDP solver:
			ExactMDPSolver AverageCostSolver{ &mdp, true };
			double bestApproxGain = std::numeric_limits<double>::infinity();
			size_t bestGen = 0;
			for (size_t gen = 1; gen <= Settings::Current.NumGensPolicyOpt; gen++)
			{
				NNAgent<MDP> nnagent(&mdp, gen);
			    double approxGain = AverageCostSolver.ComputePolicyCosts(nnagent, tolerance);
				if (approxGain < bestApproxGain)
				{
					bestApproxGain = approxGain;
					bestGen = gen;
				}

			}
			NNAgent<MDP> nnagent(&mdp, Settings::Current.NumGensPolicyOpt);

			double gain = AverageCostSolver.ComputeOptimalCosts(nnagent, tolerance);
			std::cout << mdp.Identifier() << " Best Gen: " << bestGen << "  costs:  " << gain << "  (approxGain-gain)/gain(%) " << (100 * (bestApproxGain - gain) / gain) << std::endl;

		}

	}
	return;
}

void TestFunctionalities::LostSalesPaperTrainingSensitivity()
{
	using MDP = LostSalesMDP;
	bool Evaluate = false;
	for (size_t i = 0; i < MDP::NumInstances(); i++)
	{
		auto mdp = MDP::GetInstance(i);
		Settings::Current.HiddenLayers = {128,64,64,64 };
		Settings::Current.InitialRollouts = 500;
		Settings::Current.MaxRollouts = 4000;
		Settings::Current.TimeOut = 9000;
		Settings::Current.MaxSamples = 4000;
		Settings::Current.MiniBatchSize = 64;
		Settings::Current.NumGensPolicyOpt = 4;
		Settings::Current.kValueBanditOptimizer = 2.0;
		Settings::Current.FractionRandomActions = 0.05;
		for (size_t i = 3; i < 4; i++)
		{
			
			if (i == 1)
			{
				Settings::Current.MaxSamples = 5000;
			}
			if (i == 2)
			{
				Settings::Current.MaxRollouts = 5000;
			}
			if (i == 3)
			{
				Settings::Current.HiddenLayers = { 128,64,64 };
			}
			HardwareUtilities::SetSubDir(mdp.Identifier(),Settings::Current.AsString());
			{
				//We train with alpha=0.975
				mdp.alpha = 0.975;
				ApproximatePolicyIterationSolver approxSolver{ &mdp };
				LostSalesBaseStockAgent agent{ &mdp };
				//RandomAgent agent{ &mdp };

				if (Evaluate)
				{
					ExactMDPSolver AverageCostSolver{ &mdp, false };

					double tolerance = 1e-8;
					mdp.alpha = 1.0;
					NNAgent<MDP> nnagent(&mdp, Settings::Current.NumGensPolicyOpt);
					double approxGain = AverageCostSolver.ComputePolicyCosts(nnagent, tolerance);
					double gain = AverageCostSolver.ComputeOptimalCosts(nnagent, tolerance);
					std::cout << Settings::Current.AsString() << std::endl;
					std::cout << mdp.Identifier() << "  costs:  " << gain << "  (approxGain-gain)/gain(%) " << (100 * (approxGain - gain) / gain) << std::endl;
					std::cout << std::endl;

				}
				else
				{
					approxSolver.Solve(agent, 0, 2 * 14.678 / (1 - mdp.alpha));
				}
			}
		}
	}
	return;
}

void TestFunctionalities::TestSLAInv()
{
	
	/*
   ////////////////////////////    VISUALIZATION   ////////////////////////////

  SLAInvMDP mdp = SLAInvMDP::GetSimpleInstance3();
  SLAInvAgent Agent(&mdp);

  std::ofstream myFile;
  myFile.open("output_test13.xls");
  auto state = mdp.InitialState_Vis();
  myFile << "Leadtime \t Exp demand \t EBOTarget \t Horizon \t SLA BO \t Holding \t Penalty" << std::endl;
  myFile << mdp.Leadtime << "\t" << mdp.ExpectedDemand << "\t" << mdp.EBOTarget << "\t" << mdp.HorizonLenght << "\t" << mdp.EBOTarget * mdp.HorizonLenght << "\t" << mdp.h << "\t" << mdp.p << std::endl << std::endl;
  myFile << "\t";
  for (int a = mdp.HorizonLenght; a > 0; a--) {
	  myFile << a << "\t";
  }
  myFile << std::endl;

  for (state.CumBackorders = 0; state.CumBackorders <= mdp.EBOTarget * mdp.HorizonLenght; state.CumBackorders++) {
	  myFile << state.CumBackorders;
	  for (state.DaysRemaining = mdp.HorizonLenght; state.DaysRemaining >= 1; state.DaysRemaining--) {
		  size_t action = Agent.GetAction(state);
		  myFile << "\t" << action;
	  }
	  myFile << std::endl;
  }



  std::cout << "SLA Max CumBO = " << mdp.EBOTarget * mdp.HorizonLenght << std::endl;
  mdp.ToScreenInitial(state);
  */

  ////////////////////////////    SIMULATION TEST   ////////////////////////////

	double cost{ 0.0 };
	std::ofstream myFile;
	myFile.open("output_test3.xls");
	myFile << "Period \t Action \t Event \t CumBO" << std::endl;

	SLAInvMDP mdp = SLAInvMDP::GetSimpleInstance2();


	SLAInvAgent Agent(&mdp);
	RNG rng{ 1231234 };

	auto state = mdp.InitialState();

	size_t HorizonLenght = mdp.NumEventsRemaining(state);
	//mdp.ToScreenInitial(state);
	size_t SLAFailed{ 0 };
	for (int i = 1; i <= HorizonLenght; i++) {
		size_t action = Agent.GetAction(state); //Action * Leadtime : to compromise for lacking DemOverLeadtimeProbs
		//std::cout << "ACTION" << i << " = " << action << std::endl;
		mdp.ModifyStateWithAction(state, action);
		//mdp.ToScreen(state);
		size_t event = rng.GenEvent(mdp.EventProbs());
		//std::cout << "DEMAND = " << event << std::endl;
		myFile << i << "\t" << action << "\t" << event << "\t" << state.CumBackorders << std::endl;
		//mdp.ModifyStateWithEvent(state, event);
		cost += mdp.ModifyStateWithEvent(state, event);
		//std::cout << "BASESTOCK = " << action << std::endl;

		//std::cout << "Cost = " << cost << std::endl;
		if (SLAFailed == 0) {
			if (state.CumBackorders > mdp.EBOTarget * mdp.HorizonLenght) {
				SLAFailed = state.DaysRemaining;
			}
			else {

			}
		}
		else {

		}
		//mdp.ToScreen(state);

	}

	std::cout << "\n\nExpDemand = " << mdp.ExpectedDemand << "\t\t EBOTarget = " << mdp.EBOTarget << "\t Leadtime = " << mdp.Leadtime << std::endl;
	std::cout << "Horizon: " << HorizonLenght << "\t\t SLA Failed: " << SLAFailed << " periods until end" << std::endl;
	std::cout << "CumBackorder - MaxAllowed CumBO = " << state.CumBackorders - (mdp.HorizonLenght * mdp.EBOTarget) << "\t Cost = " << cost << "\n\n\n";

}

void TestFunctionalities::TestSLAInvExample()
{
	SLAInvMDP mdp = SLAInvMDP::GetSimpleInstance2();
    SLAInvAgent agent(&mdp);
	RandomAgent altAgent(&mdp);
	//Max: You need to make sure that .Identifier returns a unique identifier, also if you change the settings
	//of your problem. See implementation for lost sales. 
	HardwareUtilities::SetSubDir(mdp.Identifier());
	auto initialstate = mdp.InitialState();
   
	if (false)//<-- just to skip what follows
	{
		//With this, you do 10 simulation steps, or actually 10 events:
		{
			Demonstrator demonstrator(&mdp);
			auto trace = demonstrator.GetTrace(initialstate, agent, 10);
			for (auto& tr : trace.StateSequence)
			{//Each element in the state sequence has the state, but sometimes also the action:
				std::cout << "action: " << tr.action << std::endl;
				mdp.ToScreen(tr.state);
			}
		}
	}
	//return;
	//Some settings for the deep reinforcement learning algorithm:
	Settings::Current.HiddenLayers = { 128,64,64,64 };
	Settings::Current.InitialRollouts = 100;
	Settings::Current.MaxRollouts = 2000;
	Settings::Current.TimeOut = 9000;
	Settings::Current.MaxSamples = 2000;
	Settings::Current.MiniBatchSize = 64;
	Settings::Current.NumGensPolicyOpt = 1;
	Settings::Current.kValueBanditOptimizer = 2.0;
	Settings::Current.FractionRandomActions = 0.05;

	//This trains a neural network agent. While training, it uses
	//the settings mentioned above. See paper for details. 
	ApproximatePolicyIterationSolver solver(&mdp);
	solver.Solve(agent, 0, 0.0);
	
	//return;

	//This loads the trained neural network agent. 
	NNAgent<SLAInvMDP> nnagent(&mdp, Settings::Current.NumGensPolicyOpt);


	
	//This does [replications] runs for both the agent and the altagent/nnAgent. It compares the average
	//costs over those two agents, and reports that as well as the difference. 
	{
		size_t replications = 10;
		RandomAgent altAgent(&mdp);
		AgentComparer comparer(&mdp);
		comparer.CompareAgents(agent, nnagent, replications);
		comparer.CompareAgents(agent, altAgent, replications);

	}
}

void TestFunctionalities::TestAlgorithms()
{

	//{
	//	auto mdp = NetworkMDP::GetComplexInstance(3);
	//	auto agent = NetworkAgent(&mdp);
	//	//auto agent = GreedyAgent<NetworkMDP>(&mdp);
	//	SampleGenerator<NetworkMDP, NetworkAgent, true> gen(&mdp, &agent);
	//	gen.GenerateSamples(/*samples=*/10);
	//}
	//{
	//	auto mdp = NetworkMDP::GetComplexInstance(3);
	//	auto agent = GreedyAgent<NetworkMDP>(&mdp);
	//	SampleGenerator gen(&mdp, &agent);
	//	gen.GenerateSamples(/*samples=*/10);
	//}
	//{
	//	auto mdp = InfoAcqMDP::GetRealSimpleInstance();
	//	auto agent = GreedyAgent(&mdp);
	//	SampleGenerator<InfoAcqMDP,GreedyAgent<InfoAcqMDP>,true> gen(&mdp, &agent);
	//	gen.GenerateSamples(10);
	//}

	{
		auto mdp = PoultryMDP::GetSimpleInstance();
		auto agent = PoultryAgent{ &mdp };
		SampleGenerator<PoultryMDP, PoultryAgent, true> gen(&mdp, &agent);
		gen.GenerateSamples(/*samples=*/10);
	}
	//{
	//	auto mdp = SimpleMDP::GetSimpleInstance();
	//	auto agent = SimpleAgent{};
	//	SampleGenerator<SimpleMDP,SimpleAgent,true> gen(&mdp, &agent);
	//	gen.GenerateSamples(/*samples=*/3);
	//}


}

void TestFunctionalities::TestGeom()
{
	RNG rng{26071983};
	double alpha = 0.95;
	rng.GeomParam(alpha);
	size_t max = 1000000;
	std::vector<size_t> tally1(max, 0);
	std::vector<size_t> tally2(max, 0);
	size_t repl = 100000;
	for (size_t i = 0; i < repl ; i++)
	{
		auto res = rng.NextGeom();
		tally1[res]++;
		/*	auto res2 = rng.NextGeom(alpha);
			tally2[res2]++;*/
	}

	size_t maxShown = 10;//max;
	for (size_t i = 0; i < maxShown; i++)
	{
		if (tally1[i] > 0)
		{
			std::cout << i << "  " << tally1[i] << std::endl;
		}
	}
	for (size_t i = 0; i < max; i++)
	{
		if (tally2[i] > 0)
		{
			std::cout << i << "  " << tally2[i] << std::endl;
		}
	}
}
