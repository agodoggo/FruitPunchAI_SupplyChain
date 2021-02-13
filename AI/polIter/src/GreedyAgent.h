#pragma once
#include <limits>
#include "MDPAdapter.h"

template <class MDP>
class GreedyAgent
{
	MDPAdapter<MDP> adapter;
public:


	GreedyAgent(const MDP* mdp_ptr) :adapter{ mdp_ptr }
	{}



	using State = typename MDP::State;

	size_t GetAction(const State& state) const
	{
		double lowestCost{ std::numeric_limits<double>::infinity() };
		size_t bestAction{};

		auto numActions = adapter.NumValidActions(state);
		for (size_t action = 0; action < numActions; action++)
		{
			if (adapter.IsAllowedAction(state, action))
			{
				auto next = adapter.GetTracker(state);
				adapter.IncorporateAction(next, action);
				auto costForAction = next.accumulatedCosts;
				if (costForAction < lowestCost)
				{
					lowestCost = costForAction;
					bestAction = action;
				}
			}
		}
		return bestAction;
	}
};
