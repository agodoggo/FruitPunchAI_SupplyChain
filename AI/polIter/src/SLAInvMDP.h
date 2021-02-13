#pragma once
#include<vector>
#include<cmath>
#include<cstddef>
#include<iostream>
#include "DiscreteDist.h"
#include "SLAInvState.h"

#include "RNG.h"
#include "Trace.h"



class SLAInvMDP
{

public:

	std::string Identifier() const;

	using State = SLAInvState;

	enum class DemandType : size_t { Poisson = 1, Geometric = 2 };
	DemandType demandType;
	double ExpectedDemand;

	friend class SLAInvAgent;

	bool AwaitsAction(const State& state) const;

	size_t NumValidActions() const; //Number of possible basestocks until F(p/(p+h))

	double ModifyStateWithAction(State& state, size_t action) const;

	const std::vector<double>& EventProbs() const;
	double ModifyStateWithEvent(State& state, size_t event) const;

	size_t NumFeatures() const;
	void ExtractFeatures(const State& state, float* out_feat_array) const;

	SLAInvMDP::State InitialState() const;

	SLAInvMDP::State InitialState_Vis() const;
	//static SLAInvMDP GetSimpleInstance();
	static SLAInvMDP GetSimpleInstance1();
	static SLAInvMDP GetSimpleInstance2();
	static SLAInvMDP GetSimpleInstance3();



	size_t NumEventsRemaining(const State& state) const;
	bool IsFinal(const State& state) const;


	void ToScreenInitial(State& state);
	void ToScreen(State& state);
	std::string statuss;   //Not required

public:
	double h{ 0.0 };
	double p{ 0.0 };
	double EBOTarget{ 0.0 };
	size_t Leadtime{ 0 }; 
	size_t HorizonLenght{ 0 };

	std::vector<double> DemandProbs;
	std::vector<double> DemOverLeadtimeProbs;
	size_t MaxOrderSize{ 0 };
	size_t MaxInvInSystem{ 0 };

	static SLAInvMDP GetParametrizedInstance(size_t Leadtime, double p, double h, SLAInvMDP::DemandType demandType, double ExpectedDemand, size_t HorizonLenght, double EBOTarget);


	SLAInvMDP() {}
};

 

