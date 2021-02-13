#pragma once
#include<vector>
#include<cmath>
#include<cstddef>
#include<iostream>
#include "SimpleState.h"


class SimpleMDP
{
public:

	using State = SimpleState;

	std::string Identifier() const;

	/// <summary>
	/// Indicates whether the state is awaiting an event. States that await an event do not allow for actions: the next
	/// step must be modifying the state with an event. 
	/// </summary>
	/// <param name="state">The state</param>
	/// <returns>A bool indicating whether the state awaits an event</returns>
	bool AwaitsAction(const State& state) const;

	size_t NumValidActions() const;
	bool IsAllowedAction(const State& state, size_t action) const;
	double ModifyStateWithAction(State& state, size_t action) const;



	size_t NumFeatures() const;
	void ExtractFeatures(const State& state, float* out_feat_array) const;


	SimpleMDP::State InitialState() const;



	static SimpleMDP GetSimpleInstance();

	/// <summary>
	/// Below is what is needed for exact optimization:
	/// </summary>
	//discount factor used for model;
	double alpha{ 0.9 };

	const std::vector<double>& EventProbs() const;
	double ModifyStateWithEvent(State& state, size_t event) const;
	std::vector<State> AllStates() const;
	/// <summary>
	/// End of what is needed for exact optimization
	/// </summary>
private:
	std::vector<double> eventprobs;
};

