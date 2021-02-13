#pragma once
#include "SCGameState.h"
#include "RNG.h"
#include "StateCombination.h"
#include "Trace.h"
class SCGameMDP
{
public:
	std::string Identifier() const;

	using State = SCGameState;
	static constexpr size_t NumTaskTypes = 4;



private:
	bool adapprobs{ false };
	std::vector<std::string> DemandLocs;
	struct DemandInstance
	{
		size_t size;
		size_t location;
		DemandInstance(size_t size, size_t location)
			:size{ size }, location{location}
		{

		}
	};

	std::vector<DemandInstance> DemandInstances;
	std::vector<double> DemandProbs;

	std::vector<std::vector<size_t>> ExternalOrderActions;

	std::vector<std::vector<size_t>> ProductionActions;

	std::vector<size_t> TransshipmentActions;
	std::vector<std::vector<size_t>> InitialQDist;
	size_t NumInvLocations{ 0 };

	//Mark this is private
	SCGameMDP() {}
public:
	/// <summary>
/// Indicates the tasktype	of a state. Tasktype only exists for states that await an action. 
/// </summary>
/// <param name="state">The state</param>
/// <returns>A size_t that acts as a label for the task type</returns>
	size_t TaskType(const State& state) const;

	bool AwaitsAction(const State& state) const;


	void ToTikzFigure(std::ostream& os, Trace<SCGameMDP> trace, bool skipall = true);

	/// <summary>
	/// Number of actions that could potentially be valid for a state with a certain TaskType. 
	/// </summary>
	/// <returns>Integer. Note that return value I may only depend on the task-type of the state. Potential valid actions are then 0,1,...,I-1. To test validity of each action for a state, call IsAllowedAction</returns>
	size_t NumValidActions(size_t taskType) const;



	bool IsAllowedAction(const State& state, size_t action) const;


	double ModifyStateWithAction(State& state, size_t action) const;


	const std::vector<double>& EventProbs() const;

	double ModifyStateWithEvent(State& state, size_t event) const;


	size_t NumEventsRemaining(const State& state) const;

	size_t NumFeatures() const;
	void ExtractFeatures(const State& state, float* out_feat_array) const;



	static SCGameMDP GetSimpleInstance(bool force=false);


	SCGameMDP::State BlankState() const;

	SCGameMDP::State InitialState() const;


	SCGameMDP::State InitialState(RNG& rng) const;

	/// <summary>
/// Returns a bool indicating whether this state is the final state. 
/// </summary>
	bool IsFinal(const State& state) const;



};

