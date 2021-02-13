#pragma once
#include "ExactMDPSolver.h"

template <class MDP>
class TabularAgent
{
	MDPAdapter<MDP> adapter;
public:



	TabularAgent(const MDP* mdp_ptr, const ExactMDPSolver<MDP>* solver) :adapter{ mdp_ptr },solver { solver }
	{

	}

	using State = typename MDP::State;

	size_t GetAction(const State& state) const
	{		
		auto action = solver->CurrentAction(state);
		if (adapter.IsAllowedAction(state, action))
		{
			return action;
		}
		else
		{			
			throw "Illegal action returned for state by ExactMDPSolver. Did you call an optimization routine on the ExactMDPSolver that is the basis of the TabularAgent?";
		}
	}

private:
	const ExactMDPSolver<MDP>* const solver;
	
};

