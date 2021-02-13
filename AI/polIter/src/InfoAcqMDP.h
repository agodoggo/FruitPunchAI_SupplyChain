#pragma once
#include<vector>
#include<cmath>
#include<cstddef>
#include<iostream>
#include "InfoAcqState.h"
#include "RNG.h"


class InfoAcqMDP
{

public:

	using State = InfoAcqState;
	
	std::vector <double> ValueforFinalDecision;
	std::vector <double> CostofRetrieval;
	std::vector<std::vector<double>> EventProbsPerAttribute;


	//Ignore this for now. 

	//Random source contains vector with values per attribute.
	using RandomSource = std::vector<size_t>;
	using RandomSourceStatus = size_t;
	// (end: ignore this)

	RandomSource GetRandomSource(const State& state, RNG& rng) const;
	bool HasAnotherEvent(const RandomSource& randomSource, const size_t& eventTime) const;
	size_t GetEvent(const State& state, const RandomSource& randomSource, size_t& randomSourceStatus) const;


	//Just an identifier for the problem. 
	std::string Identifier() const;
	 
	/// <summary>
	/// Indicates whether the state is awaiting an action. A state that awaits an action cannot be a final state. 
	/// </summary>
	/// <param name="state">The state</param>
	/// <returns>A bool indicating whether the state awaits an action</returns>
	bool AwaitsAction(const State& state) const;

	//The number of all valid actions. Not all actions need to be allowed in any state.
	size_t NumValidActions() const;
	//Indicates whether an action betweem 0 and NumValidActions() is actually allowed in this state. 
	bool IsAllowedAction(const State& state, size_t action) const;
	//Modifies the state such that the action is incorporated. 
	double ModifyStateWithAction(State& state, size_t action) const;


	//Features for machine learning. 
	//The number of features. Each state has the same number of features. 
	size_t NumFeatures() const;
	//Will explain later how to do this. Or look at other MDPs for examples. 
	void ExtractFeatures(const State& state, float* out_feat_array) const;

	size_t NumFinalDecisions() const;
	size_t NumAttributes() const;




	//The starting state for the problem.
	InfoAcqMDP::State InitialState() const;


	//Should return a simple example instance to test stuff with. 
	static InfoAcqMDP GetRealSimpleInstance();
	//Should return a simple example instance to test stuff with. 
	static InfoAcqMDP GetSimpleInstance();

	static InfoAcqMDP GetComplexInstance();

	/// <summary>
	/// Below is what is needed for exact optimization:
	/// </summary>
	//discount factor used for model;
	double alpha{ 1.0 };


	/// <summary>
	/// Returns a bool indicating whether this state is the final state. 
	/// </summary>
	bool IsFinal(const State& state) const;


	//So that states are event-dependent;
	static constexpr bool StateDependentEvents = true;
	//Events are integer-valued. This should return, for every possible state
	//which does not await an action,
	//the probabilities for events to happen. 
	//E.g. returning a vector containing elements {0.1,0.5,0.4} indicates that event 0 has probability 10 %
	//event 1 has probability 50%, and event 2 has probability 40%
	const std::vector<double>& EventProbs(const State& state) const;
	double binomialProbability(int n, int k, double p);
	int nCr(int n, int r);

	//Should modify the state to incorporate the event. 
	double ModifyStateWithEvent(State& state, size_t event) const;

	//Calculates, given a state, the final cost. Can only be called if all information is available.
	double CostofDecisions(State& state) const;

	//Gives the next attribute in the vector that has not yet been retrieved.
	size_t NextUnknownAttribute(const State& state) const;


	//Should return a list containing all possible states.
	//Note that I have a CartesianProduct class, that should help here. 
	std::vector<std::vector<int>> MaxValueperAtt() const;

	std::vector<State> AllStates() const;


private:
	//Probably you will need a 
	//std::vector<std::vector<double>> eventprobsPerAttribute
	//const std::vector<double>& GetEventProbs(State& state) const;
};

