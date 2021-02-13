#pragma once
#include <iostream>

template <class MDP>
class StateWrapper
{
public:
	using State = typename MDP::State;
	StateWrapper(State state, size_t placeholders) : state{ state }, newVal{ 0.0 }, oldVal{ 0.0 }, TransitsToStates{}, CostsInState{}, Action{}, CostsPerAction{}
	{
		TransitsToStates.reserve(placeholders);
		for (size_t i = 0; i < placeholders; i++)
		{
			TransitsToStates.push_back(nullptr);
		}
	}

	void PrintResults()
	{
		std::ostream& os = std::cout;
		os << state << "new val: " << newVal << "\t" << std::endl;
		os << "\t Action " << Action;
		os << "\t Cost: " << CostsInState << std::endl;


		os << std::endl;
	}

	/// <summary>
	/// Wrapped state
	/// </summary>
	State state;



	/// <summary>
	/// Newly computed value associated with state. 
	/// </summary>
	double newVal;
	/// <summary>
	/// Previously computed value associated with state. 
	/// </summary>
	double oldVal;


	/// <summary>
/// Pts to states that will be transitted to from this state. 
/// </summary>
	std::vector<StateWrapper<MDP>*> TransitsToStates;



	/// <summary>
/// Costs for being in this state for one period, including costs of 
/// -action, if state is actionstate
/// -expected costs, if state is eventstate.
/// </summary>
	double CostsInState;
	
	/// /// <summary>
	/// Action for this state, if applicable
	/// </summary>
	size_t Action;



	//Optional, only for debugging purposes. Holds the costs for all possible actions from this state. 
	std::vector<double> CostsPerAction;

	
};

