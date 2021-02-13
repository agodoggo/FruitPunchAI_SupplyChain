#include "MDPAdapter.h"
#include "RNG.h"
template <class MDP>
class RandomAgent
{
	MDPAdapter<MDP> adapter;
public:


	static constexpr bool TakesRandomActions = true;


	RandomAgent(const MDP* mdp_ptr) :adapter{ mdp_ptr }
	{}



	using State = typename MDP::State;

	size_t GetAction(const State& state, RNG& rng) const
	{
		auto numActions = adapter.NumValidActions(state);
		size_t numAllowedActions{ 0 };
		for (size_t action = 0; action < numActions; action++)
		{
			if (adapter.IsAllowedAction(state, action))
			{
				numAllowedActions++;
			}
		}
		double budget = rng.NextUniform() * numAllowedActions;
		for (size_t action = 0; action < numActions; action++)
		{
			if (adapter.IsAllowedAction(state, action))
			{
				budget--;
				if (budget <= 0.0)
				{
					return action;
				}
			}
		}
		std::cout << "error in random agent";
		throw "error in random agent";
	}
};
