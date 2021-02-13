#pragma once
#include "MDPAdapter.h"
#include "RNG.h"

//Has only static members
template <typename MDP,typename Agent>
class AgentAdapter
{
private:
	using State = typename MDP::State;
	using RandomSourceStatus = typename MDPAdapter<MDP>::RandomSourceStatus;
	using RandomSource = typename MDPAdapter<MDP>::RandomSource;
	using TrajectoryTracker = typename MDPAdapter<MDP>::TrajectoryTracker;

	template <typename T, typename = void>
	struct providesTakesRandomActionsFlag : std::false_type {};
	template <typename T>
	struct providesTakesRandomActionsFlag<T, decltype((void)T::TakesRandomActions, void())> : std::true_type {};
	
	
public:
	static constexpr bool TakesRandomActions()
	{
		if constexpr (providesTakesRandomActionsFlag<Agent>::value)
		{
			return Agent::TakesRandomActions;
		}
		else
		{
			return false;
		}
	}
private:
	template <typename T, typename = void>
	struct providesOptimizedMultiactionFlag : std::false_type {};
	template <typename T>
	struct providesOptimizedMultiactionFlag<T, decltype((void)T::OptimizedMultiaction, void())> : std::true_type {};


	MDPAdapter<MDP> adapter;

public:
	static constexpr bool OptimizedMultiaction()
	{
		if constexpr (providesOptimizedMultiactionFlag<Agent>::value)
		{
			return Agent::OptimizedMultiaction;
		}
		else
		{
			return false;
		}
	}



	Agent* agent_ptr;


	AgentAdapter(const MDP* mdp_ptr,Agent* agent_ptr):
	adapter{mdp_ptr},agent_ptr{agent_ptr}
	{
		
	}

	size_t GetAction(const State& state,RNG& rng)
	{
		if constexpr(TakesRandomActions())
		{
			return agent_ptr->GetAction(state, rng);
		}
		else
		{
			return agent_ptr->GetAction(state);
		}
	}

	void TakeAction(TrajectoryTracker& tracker, RNG& rng)
	{		
		adapter.IncorporateAction(tracker, GetAction(tracker.state, rng));
	}

	void TakeActions(std::vector<TrajectoryTracker>& trajectories, RNG& rng, bool AllTrajectoriesAwaitAction)
	{
		if constexpr (OptimizedMultiaction())
		{
			if constexpr (TakesRandomActions())
			{
				if (AllTrajectoriesAwaitAction)
				{
					agent_ptr->template TakeAction<true>(trajectories, rng);
				}
				else
				{
					agent_ptr->template TakeAction<false>(trajectories, rng);
				}
			}
			else
			{
				if (AllTrajectoriesAwaitAction)
				{
					agent_ptr->template TakeAction<true>(trajectories);
				}
				else
				{
					agent_ptr->template TakeAction<false>(trajectories);
				}
			}

		}
		else
		{
			if (AllTrajectoriesAwaitAction)
			{
				for (size_t i = 0; i < trajectories.size(); i++)
				{
					TakeAction(trajectories[i], rng);
				}
			}
			else
			{
				for (size_t i = 0; i < trajectories.size(); i++)
				{
					if (adapter.AwaitsAction(trajectories[i]))
					{
						TakeAction(trajectories[i], rng);
					}
				}
			}
		}
	}




};