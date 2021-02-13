#include "PoultryMDP.h"
#include <iostream>
#include <algorithm>
#include <tuple>
#include <assert.h>
#include <string>
#include <numeric>
#include "EventStream.h"
#include "CartesianProduct.h"
#include <sstream>

std::string PoultryMDP::Identifier() const
{
	std::ostringstream MDPName{};

	MDPName << "Poultry" << nrOfBins << "_" << MaxWeightBins << "_" << EventProbsStatic.size() << "_" << FutureHorizonLength;

	return MDPName.str();
}

bool PoultryMDP::AwaitsAction(const State& state) const
{
	return state.processStatus == 1;
}

size_t PoultryMDP::NumValidActions() const
{
	return nrOfBins;
}

bool PoultryMDP::IsAllowedAction(const State& state, size_t action) const
{
	if constexpr (KeepBinsSorted)
	{
		if (action == 0)
		{
			return true;
		}
		else
		{//Action is only allowed if it does not have the same weight as the weight of the bin above it
		//This is to avoid multiple actions that lead to the same resulting state, this improves speed.
			return state.BinWeights[action] != state.BinWeights[action - 1];
		}
	}
	return true;
}


const std::vector<double>& PoultryMDP::EventProbs() const
{
	return EventProbsStatic;
}

double PoultryMDP::ModifyStateWithEvent(State& state, Event event) const
{
	//Check whether we are allowed to modify state with event
	if (!AwaitsAction(state)) {
		// Add newly generated weight at end of vector. Change processStatus to "Actionable"
		state.ArrivingWeights.back() = event;
		state.processStatus = 1;		
	}
	else {
		std::cout << "error in code: cannot modify state with event" << std::endl;
		throw "Error in code: cannot modify state with event";
	}
	return 0.0;
}


double PoultryMDP::ModifyStateWithAction(State& state, size_t action) const
{
	// Add first-in-line weight to indicated (action) bin. 		
	auto arrivingValue = state.ArrivingWeights.front();
	//Remove the arrivingweight from the front. 
	state.ArrivingWeights.pop_front();
	//Append 0 at the end:
	state.ArrivingWeights.push_back(0);
	//Set the processstatus to awaiting event
	state.processStatus = 0;
	//Add the weight to the selected bins. 
	state.BinWeights[action] += arrivingValue;



	if (state.BinWeights[action] > MaxWeightBins)
	{//We reached or exceeded the desired weight
		//Amount of exceedence:
		double GiveAway = (double)(state.BinWeights[action] - MaxWeightBins - 1);
		//Empty the bin:
		state.BinWeights[action] = 0;
		if constexpr (KeepBinsSorted)
		{
			std::sort(state.BinWeights.begin(), state.BinWeights.end());
		}
		return GiveAway;
	}
	else
	{
		if constexpr (KeepBinsSorted)
		{
			std::sort(state.BinWeights.begin(), state.BinWeights.end());
		}
		return 0.0;
	}
}


//size_t PoultryMDP::NumEventsRemaining(const State& state) const
//{
//	return 100;
//}

size_t PoultryMDP::NumFeatures() const
{
	if constexpr (BinaryFeatures)
	{
		return FutureHorizonLength * (EventProbsStatic.size() - 1) + nrOfBins * MaxWeightBins;
	}
	else
	{//Add both the weight of the bin, and the weight if the bin would become when the first-in-line is added.
		return FutureHorizonLength + nrOfBins*2;
	}
}

void PoultryMDP::ExtractFeatures(const State& state, float* out_feat_array) const
{
	if constexpr (BinaryFeatures)
	{
		size_t NumFeats = NumFeatures();
		size_t iter{ 0 };
		size_t MaxWeightArrivingItem = (EventProbsStatic.size() - 1);
		for (auto arrivingWeight : state.ArrivingWeights)
		{
			for (size_t i = 0; i < arrivingWeight; i++)
			{
				out_feat_array[iter++] = 1.0;
			}
			for (size_t i = 0; i < MaxWeightArrivingItem-arrivingWeight; i++)
			{
				out_feat_array[iter++] = 0.0;
			}
		}
		for (auto binWeight : state.BinWeights)
		{
			for (size_t i = 0; i < binWeight; i++)
			{
				out_feat_array[iter++] = 1.0;
			}
			for (size_t i = 0; i < MaxWeightBins - binWeight; i++)
			{
				out_feat_array[iter++] = 0.0;
			}
		}
		if (iter != NumFeats)
		{
			throw "program logic error";
		}
		return;
	}
	else
	{
		size_t NumFeats = NumFeatures();
		size_t iter{ 0 };
		auto firstArrivingWeight = state.ArrivingWeights.front();

		for (auto arrivingWeight : state.ArrivingWeights)
		{
			out_feat_array[iter++] = (float)(arrivingWeight);
		}
		for (auto binWeight : state.BinWeights)
		{
			out_feat_array[iter++] = (float)(binWeight);
			out_feat_array[iter++] = (float)(binWeight+firstArrivingWeight);
		}
		if (iter != NumFeats)
		{
			throw "program logic error";
		}
		return;
	}
}



// Function to calculate the number of ways to choose r out of n objects (for the binomial distribution in initial value)
int PoultryMDP::nCr(int n, int r)
{
	// Since nCr is same as nC(n-r) 
	// To decrease number of iterations 
	if (r > n / 2)
		r = n - r;

	int answer = 1;
	for (int i = 1; i <= r; i++) {
		answer *= (n - r + i);
		answer /= i;
	}

	return answer;
}

// Function to calculate binomial random variable probability 
double PoultryMDP::binomialProbability(int n, int k, double p)
{
	return PoultryMDP::nCr(n, k) * pow(p, k) * pow(1 - p, n - k);
}


PoultryMDP PoultryMDP::GetSimpleInstance()
{
	//std::cout << "Memory:" << sizeof(Weight) << std::endl;
	PoultryMDP mdp{};
	mdp.alpha = 0.99;
	mdp.nrOfBins = 3;
	mdp.MaxWeightBins = 20;//30

	//allstates werkt alleen met horizon 1
	mdp.FutureHorizonLength = 1;

	
	// New weight is Binomially distributed with n=12 and p=10/12, variance = 5/3
	int n = 12;
	double p = 10.0 / 12;

	for (int k = 0; k <= n; k++) 
	{
		mdp.EventProbsStatic.push_back(mdp.binomialProbability(n, k, p));
	}
	
	return mdp;
}

PoultryMDP PoultryMDP::GetComplexInstance()
{
	//std::cout << "Memory:" << sizeof(Weight) << std::endl;
	PoultryMDP mdp{};
	mdp.alpha = 1.0;
	mdp.nrOfBins = 5;
	mdp.MaxWeightBins = 29;
	mdp.FutureHorizonLength = 1;


	// New weight is Binomially distributed with n=12 and p=8/12, variance = 5/3
	int n = 12;
	double p = 8.0 / 12;

	for (int k = 0; k <= n; k++)
	{
		mdp.EventProbsStatic.push_back(mdp.binomialProbability(n, k, p));
	//	std::cout << k << "   " << mdp.EventProbsStatic.back() << std::endl;
	}

	return mdp;
}


PoultryMDP::State PoultryMDP::InitialState() const
{
	//Initial state is awaitEventState
	FIFOQueue<Weight> futureArrivalsInitial(FutureHorizonLength);
	for (size_t i = 0; i < FutureHorizonLength; i++)
	{
		futureArrivalsInitial.push_back(Weight{ 0 });
	}

	std::vector<Weight> binWeightsInitial;
	for (size_t j = 0; j < nrOfBins; j++)
	{
		binWeightsInitial.push_back(Weight{ 0 });
	}


	/// Initial state is BinWeights zero everywhere, upcoming not known yet (assuming that no "actual" event occur for all times up to T-1, where T is future arrival time horizon)
	return State(futureArrivalsInitial, binWeightsInitial, 0);
}



std::vector<PoultryMDP::State> PoultryMDP::AllStates() const
{
	std::vector<PoultryMDP::State> AllStates;

	std::vector<size_t> MaxWeightPerBin(nrOfBins, MaxWeightBins);


	CartesianProduct<Weight> cartProdBinWeights{MaxWeightPerBin};
	int count = 0;
	while (cartProdBinWeights.HasNext())
	{
		auto binWeightsVec = cartProdBinWeights.GetNext();
		bool isSorted = true;
		if constexpr (KeepBinsSorted)
		{
			for (size_t i = 0; i < nrOfBins - 1; i++)
			{
				if (binWeightsVec[i] > binWeightsVec[i + 1])
				{
					isSorted = false;
				}
			}
		}
		if (isSorted)
		{
			count++;
		}
	}
	cartProdBinWeights.Reset();
	size_t MaxWeightOfItem = EventProbsStatic.size() - 1;

	std::vector<size_t> MaxIncomingWeightsActionState(FutureHorizonLength, MaxWeightOfItem);
	
	//Note that last weight of EventState is always zero. 
	std::vector<size_t> MaxIncomingWeightsAwaitEventState(FutureHorizonLength-1, MaxWeightOfItem);
	MaxIncomingWeightsAwaitEventState.push_back(0);

	
	CartesianProduct<Weight> cartProdIncomingAction(MaxIncomingWeightsActionState);
	CartesianProduct<Weight> cartProdIncomingEvent(MaxIncomingWeightsAwaitEventState);
	//Total number of states equals all combinations of a weight distribution over the bins, plus all combinations
	//of a weight distribution of incoming weights (and those are the await event configurations + the incoming actions configurations)
	AllStates.reserve(count*(cartProdIncomingAction.NumStates()+cartProdIncomingEvent.NumStates()));



	while (cartProdBinWeights.HasNext())
	{
		auto binWeightsVec = cartProdBinWeights.GetNext();
		bool isSorted = true;
		if constexpr (KeepBinsSorted)
		{
			for (size_t i = 0; i < nrOfBins-1; i++)
			{
				if (binWeightsVec[i] > binWeightsVec[i + 1])
				{
					isSorted=false;
				}
			}
		}
		if (isSorted)
		{

			while (cartProdIncomingAction.HasNext())
			{
				auto incomingVec = cartProdIncomingAction.GetNext();
				//1 because action states:
				AllStates.push_back(State(incomingVec, binWeightsVec, 1));
			}
			cartProdIncomingAction.Reset();
			while (cartProdIncomingEvent.HasNext())
			{
				auto incomingVec = cartProdIncomingEvent.GetNext();
				//0 becauses event states:
				AllStates.push_back(State(incomingVec, binWeightsVec, 0));
			}
			cartProdIncomingEvent.Reset();
		}
	}
	return AllStates;
}