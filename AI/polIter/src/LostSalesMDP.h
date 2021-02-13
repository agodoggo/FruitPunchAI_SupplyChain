#pragma once
#include "RNG.h"
#include "LostSalesState.h"
#include "DiscreteDist.h"

class LostSalesMDP
{

public:
	std::string Identifier() const;

	using State = LostSalesState;

	enum class DemandType : size_t { Poisson = 1, Geometric = 2 };

	DemandType demandType;
	double ExpectedDemand;

	//Discount factor (may be overridden while initializing)
	double alpha{ 1.0 };

	friend class LostSalesBaseStockAgent;

private:

	std::vector<double> DemandProbs;

	size_t Leadtime{ 0 };
	size_t MaxOrderSize{ 0 };
	size_t MaxInvInSystem{ 0 };

	double h{ 0.0 }, p{ 0.0 };

	//Mark this as private
	LostSalesMDP() {}

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
	size_t NumValidActions() const;



	bool IsAllowedAction(const State& state, size_t action) const;
	double ModifyStateWithAction(State& state, size_t action) const;


	const std::vector<double>& EventProbs() const;
	double ModifyStateWithEvent(State& state, size_t event) const;
		
	size_t NumFeatures() const;
	void ExtractFeatures(const State& state, float* out_feat_array) const;


	//These static functions return specific instances
	//of the network MDP for testing. 
	static LostSalesMDP GetSimpleInstance();
	static LostSalesMDP GetComplicatedInstance();
	static size_t NumInstances();
	static LostSalesMDP GetInstance(size_t instanceID);

	

	LostSalesMDP::State InitialState() const;
	std::vector<State> AllStates() const;

private:
	static LostSalesMDP GetParametrizedInstance(size_t Leadtime, double p, double h, LostSalesMDP::DemandType demandType, double ExpectedDemand);





};

