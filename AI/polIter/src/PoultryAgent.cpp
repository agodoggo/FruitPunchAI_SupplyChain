#include "PoultryAgent.h"
#include <assert.h>
#include <tuple>
#include "ExactMDPSolver.h"

// Code Action heuristic


	PoultryAgent::PoultryAgent(const PoultryMDP* mdp_ptr):mdp_ptr{mdp_ptr}
	{
		PoultryMDP mdp;
		//So because every bin is visited less often because their are multiple bins,
		//the discount factor must be adapted. 
		EmbeddedDiscountFactor = mdp_ptr->alpha / (mdp_ptr->nrOfBins - (mdp_ptr->nrOfBins - 1) * mdp_ptr->alpha);

		mdp.alpha = EmbeddedDiscountFactor;


		mdp.EventProbsStatic = mdp_ptr->EventProbsStatic;
		mdp.MaxWeightBins = mdp_ptr->MaxWeightBins;
		mdp.nrOfBins = 1;
		mdp.FutureHorizonLength = 1;

		ExactMDPSolver solver{ &mdp ,/*silent (no reporting of optimization results):*/true};
		solver.ComputeOptimalCosts(1e-9);
		auto allStates = mdp.AllStates();


		ValuesPerWeightEmbeddedForRandomPolicy = std::vector<double>( mdp.MaxWeightBins + 1 , 0.0);
		for (auto& state: allStates)
		{
			if (!mdp.AwaitsAction(state))
			{
				auto wrapper = solver.find(state);
				size_t weight = wrapper->state.BinWeights[0];
				double cost = wrapper->newVal;
				ValuesPerWeightEmbeddedForRandomPolicy[weight] = cost;
			}
		}
		for (size_t i = 0; i < ValuesPerWeightEmbeddedForRandomPolicy.size(); i++)
		{
		//	std::cout << i << "  " << ValuesPerWeightEmbeddedForRandomPolicy[i] << std::endl;
		}
	}


	size_t PoultryAgent::GetAction(const PoultryState& state) const
	{
		size_t BestOption = 0;
		double BestEffectiveCosts=std::numeric_limits<double>::infinity();

		auto arrivingWeight = state.ArrivingWeights.front();

		for (size_t action = 0; action < mdp_ptr->NumValidActions(); action++)
		{
			if (mdp_ptr->IsAllowedAction(state, action))
			{
				auto binWeight = state.BinWeights[action];
				auto costs = -ValuesPerWeightEmbeddedForRandomPolicy[binWeight];
				auto ResultingWeight = binWeight + arrivingWeight;
				if (ResultingWeight > mdp_ptr->MaxWeightBins)
				{
					costs += ResultingWeight - mdp_ptr->MaxWeightBins - 1;
					ResultingWeight = 0;
				}
				costs += ValuesPerWeightEmbeddedForRandomPolicy[ResultingWeight];
				if (costs < BestEffectiveCosts)
				{
					BestEffectiveCosts = costs;
					BestOption = action;
				}
			}
		}
		return BestOption;
	}

