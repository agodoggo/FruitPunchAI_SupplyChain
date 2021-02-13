#pragma once
#include "StateTree.h"
#include "RNG.h"
#include "MDPAdapter.h"
#include "AgentAdapter.h"

template <class MDP, class Agent>
class ImprovingAgent
{
public:

	static constexpr bool TakesRandomActions = true;

	ImprovingAgent(const MDP* mdp_ptr, Agent* agent_ptr, std::initializer_list<size_t> IgnoreTasks = {}) :stateTree(mdp_ptr, agent_ptr), adapter{ mdp_ptr }, agent_adapter{ mdp_ptr, agent_ptr }, improveTasks(
		MDPAdapter<MDP>::NumTaskTypes(), true )
	{
		for (size_t task : IgnoreTasks)
		{
			improveTasks[task] = false;
		}
	}
	using State = typename MDP::State;

	size_t GetAction(const State& state, RNG& rng)
	{
		size_t action{ 0 };
		if (adapter.ReturnActionIfTrivial(state,action))
		{//If there is only one allowed action, return that;
			return action;
		}
		action = agent_adapter.GetAction(state, rng);
		if (improveTasks[adapter.TaskType(state)])
		{
			return stateTree.ComputeSample(state, rng, action).BestAction;
		}
		return action;		
	}

private:
	StateTree<MDP,Agent> stateTree;
	MDPAdapter<MDP> adapter;
	AgentAdapter<MDP, Agent> agent_adapter;
	std::vector<bool> improveTasks;
};
