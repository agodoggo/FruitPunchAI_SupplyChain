#pragma once
#include<vector>
#include<cmath>
#include<cstddef>
#include<iostream>
#include "PoultryState.h"



class PoultryMDP
{
public:

    

	std::string Identifier() const;

	using State = PoultryState;
	using Weight = typename PoultryState::Weight;
	//For this problem, the events correspond to the weights arriving in the queue:
	using Event = Weight;


	//applicable discount factor
	double alpha{ 0.99 };//default, is mostly overridden in the creation
	


	static constexpr bool KeepBinsSorted = true;
	static constexpr bool BinaryFeatures = false;
	
	/// <summary>
	/// So the heuristic agent has access to private members of the MDP
	/// </summary>
	friend class PoultryAgent;


	// Specific parameters to the poulty problem
private:
	Weight MaxWeightBins{ 20 };
	size_t nrOfBins{ 2 };
	std::vector<double> EventProbsStatic;
	size_t FutureHorizonLength{ 1 };

	//Mark this as private
	PoultryMDP() {}
	
public:


	/// <summary>
	/// Indicates whether the state is awaiting an event. States that await an event do not allow for actions: the next
	/// step must be modifying the state with a (random) event. 
	/// </summary>
	/// <param name="state">The state</param>
	/// <returns>A bool indicating whether the state awaits an event</returns>
	bool AwaitsAction(const State& state) const;

	/// <summary>
	/// Number of actions that could potentially be valid for a state. Calls are only valid for states
	/// that do not await an event.
	/// </summary>
	/// <returns> Number of bins </returns>
	size_t NumValidActions() const;



	bool IsAllowedAction(const State& state, size_t action) const;	
	double ModifyStateWithAction(State& state, size_t action) const;


	const std::vector<double>& EventProbs() const;
	double ModifyStateWithEvent(State& state, Event event) const;


	//size_t NumEventsRemaining(const State& state) const;

	size_t NumFeatures() const;
	void ExtractFeatures(const State& state, float* out_feat_array) const;

		
	//These static functions return specific instances
	//of the network MDP for testing. 
	static PoultryMDP GetSimpleInstance();

	static PoultryMDP GetComplexInstance();


	PoultryMDP::State InitialState() const;
	std::vector<State> AllStates() const;

private:


	// F: For probability density of binomial distribution
	int nCr(int n, int r);
	double binomialProbability(int n, int k, double p);

	
	


	


};

