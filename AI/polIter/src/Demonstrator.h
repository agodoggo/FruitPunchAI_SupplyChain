#pragma once
#include <vector>
#include "RNG.h"
#include "MDPAdapter.h"
#include "AgentAdapter.h"
#include "Trace.h"
template<class MDP>
class Demonstrator
{
	MDPAdapter<MDP> adapter;
	RNG randGen;
	using State = typename MDP::State;


	using TrajectoryTracker = typename MDPAdapter<MDP>::TrajectoryTracker;
	using RandomSource = typename MDPAdapter<MDP>::RandomSource;
public:

	Demonstrator(const MDP* mdp):adapter{mdp},randGen{24031984}
	{
	
	}



	template <class Agent>
	Trace<MDP> GetTrace(State& state, Agent& agent, size_t MaxSteps)
	{
		return GetTrace(state, agent, std::move(adapter.GetRandomSource(adapter.GetTracker(state),randGen)), MaxSteps);
	}



	template <class Agent>
	Trace<MDP> GetTrace(State& state, Agent& agent, RandomSource randomSource, size_t MaxSteps)
	{
		auto agent_adapter = AgentAdapter<MDP, Agent>(adapter.mdp_ptr, &agent);

		auto tracker = adapter.GetTracker(state);
		std::vector<StateCombination<MDP>> StateCombinations{};
	
		auto randomSourceStatus = adapter.GetRandomSourceStatus(tracker,randomSource);

		Trace<MDP> trace;

		size_t eventTime{};

		if (!MDPAdapter<MDP>::StateCanSerialize())
		{
			std::cout << "Note: State cannot serialize. To see a trace, provide overload for << and >> operator, and set StateCanSerialize flag to true if you want to see a trace." << std::endl;
		}

		while (eventTime++ < MaxSteps)
		{
			if constexpr (MDPAdapter<MDP>::StateCanSerialize())
			{
				std::cout << tracker.state << std::endl;
			}
			StateCombination<MDP> combination{};
			combination.state = tracker.state;
			combination.accumulatedCosts = tracker.accumulatedCosts;
			combination.status = randomSourceStatus;
			StateCombinations.push_back(combination);

			if (adapter.TrajectoryHasEnded(tracker, randomSource, randomSourceStatus))
			{
				break;
			}
			else
			{
				if (!adapter.AwaitsAction(tracker))
				{
					adapter.IncorporateEvent(tracker, randomSource, randomSourceStatus);
				}
				else
				{
					auto action = agent_adapter.GetAction(tracker.state, randGen);
					StateCombinations.back().action = action;
					adapter.IncorporateAction(tracker, action);
				}
			}
		}




		trace.StateSequence = StateCombinations;
		trace.randomSource = randomSource;
	
		return trace;
	}
	
	
};

