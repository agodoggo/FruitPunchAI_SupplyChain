#pragma once
#include<vector>
#include<cmath>
#include<cstddef>
#include<iostream>
#include "TravelState.h"
#include "RNG.h"


class TravelMDP
{
public:
	//Flag to indigate that this MDP is driven by exogenous random events. 
	static constexpr bool ExogenousEventDriven = false;
	using State = TravelState;

	using RandomSource = std::vector<size_t>;
	using RandomSourceStatus = size_t;




	std::vector<std::vector<size_t>> DistanceMatrix;
	std::vector<std::vector<std::vector<double>>> DegradationMatrix;
	std::vector<size_t> ComponentsPerMachine;
	std::vector<size_t> DowntimeCost;
	std::vector<size_t> PrevCost;
	std::vector<size_t> CorrCost;
	std::vector<size_t> Repairtimepercomponent;

	std::vector<size_t> MachineComponents() const;
	size_t Locations() const;
	size_t Components() const;
	std::string Identifier() const;

	/// <summary>
	/// Indicates whether the state is awaiting an event. States that await an event do not allow for actions: the next
	/// step must be modifying the state with an event. 
	/// </summary>
	/// <param name="state">The state</param>
	/// <returns>A bool indicating whether the state awaits an event</returns>
	bool AwaitsAction(const State& state) const;

	size_t NumValidActions() const;
	bool FSEavailable(const State& state) const;
	bool MachineDown(const State& state, size_t component) const;
	bool IsAllowedAction(const State& state, size_t action) const;
	TravelMDP::RandomSource GetRandomSource(const State& state, RNG& rng) const;
	size_t GetEvent(const State& state, const RandomSource& randomSource, size_t& randomSourceStatus) const;
	double ModifyStateWithAction(State& state, size_t action) const;



	size_t NumFeatures() const;
	void ExtractFeatures(const State& state, float* out_feat_array) const;


	TravelMDP::State InitialState() const;

	static TravelMDP GetComplexinstance();

	static TravelMDP GetSimpleInstance();

	/// <summary>
	/// Below is what is needed for exact optimization:
	/// </summary>
	//discount factor used for model;
	double alpha{ 0.9 };

	const std::vector<double>& EventProbs(const State& state) const;
	double ModifyStateWithEvent(State& state, size_t event) const;
	std::vector<State> AllStates() const;
	size_t Maxvalue(size_t Location) const;
	std::vector<std::vector<size_t>> TotalCompStates() const;
	/// <summary>
	/// End of what is needed for exact optimization
	/// </summary>
private:
	std::vector<double> eventprobs;
};

