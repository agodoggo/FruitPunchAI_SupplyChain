#include "TravelMDP.h"
#include "TravelState.h"
#include "CartesianProduct.h"
#include "EventStream.h"
#include <algorithm>
#include "numeric"
#include "assert.h"

std::string TravelMDP::Identifier() const
{
	return "TravelMDP";
}

bool TravelMDP::AwaitsAction(const State& state) const
{
	return (state.Status == 0);
}

size_t TravelMDP::Locations() const
{
	return ComponentsPerMachine.size();
}

size_t TravelMDP::Components() const
{
	return accumulate(ComponentsPerMachine.begin(), ComponentsPerMachine.end(), 0);
}

std::vector<size_t> TravelMDP::MachineComponents() const
{
	std::vector<size_t> temp;
	for (size_t i = 0; i < Locations(); i++)
	{
		for (int j = 0; j < ComponentsPerMachine[i]; j++)
		{
			temp.push_back(i);
		}
	}
	
	assert(Components() == temp.size());
		return temp;
}

size_t TravelMDP::NumValidActions() const
{
	return (Locations() + Components());
}

bool TravelMDP::FSEavailable(const State& state) const
{
	if (state.RemainingTimeFSE == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool TravelMDP::MachineDown(const State& state, size_t machine) const
{
	for (size_t i = 0; i < Components(); i++)
	{
		if (MachineComponents()[i] == machine)
		{
			if (state.ComponentStates[i] >= DegradationMatrix[i].size() - 1) 
			{
				return true;
			}
		}
	}
	return false;
}

bool TravelMDP::IsAllowedAction(const State& state, size_t action) const
{
	if (!FSEavailable(state))
	{
		return false;
	}
	else
	{
		if (action < Locations())
		{
			return true;
		}
		else
		{
			if (state.LocationsFSE == MachineComponents()[action-Locations()])
			{
				if (state.ComponentStates[action - Locations()] > 0)
				{
					return true;
				}
			}
		}
	}
	return false;
}
/*
size_t TravelMDP::GetEvent(const State& state, const RandomSource& randomSource, size_t& randomSourceStatus) const
{
	size_t event;
	if (state.ComponentStates[state.Status - 1] == DegradationMatrix[state.Status - 1].size())
	{
		event =  randomSource[DegradationMatrix[state.Status - 1][state.ComponentStates[state.Status - 1] - 1]];
	}
	else
	{
		event = randomSource[DegradationMatrix[state.Status - 1][state.ComponentStates[state.Status - 1]]];
	}
	return event;
}
*/


double TravelMDP::ModifyStateWithAction(State& state, size_t action) const
{
	assert(state.Status == 0);
	assert(action < Locations() + Components());
	state.Status = 1;
	if (action < Locations())
	{
		size_t currloc = state.LocationsFSE;
		state.LocationsFSE = action;
		state.RemainingTimeFSE = DistanceMatrix[currloc][action];
		return 0;
	}
	else
	{
		if (state.ComponentStates[action - Locations()] == DegradationMatrix[action - Locations()].size() - 1)
		{
			state.RemainingTimeFSE = Repairtimepercomponent[action - Locations()];
			state.ComponentStates[action - Locations()] ++;
			return CorrCost[action - Locations()];
		}
		else
		{
			state.RemainingTimeFSE = Repairtimepercomponent[action - Locations()];
			state.ComponentStates[action - Locations()] = DegradationMatrix[action - Locations()].size();
			return PrevCost[action - Locations()];
		}
		return 0;
	}
}

size_t TravelMDP::NumFeatures() const
{
	return 1;
}

void TravelMDP::ExtractFeatures(const State& state, float* out_feat_array) const
{
	out_feat_array[0] = 1.0f;
	return;
}


const std::vector<double>& TravelMDP::EventProbs(const State& state) const
{
	if (state.ComponentStates[state.Status - 1] == DegradationMatrix[state.Status - 1].size())
	{
		return DegradationMatrix[state.Status - 1][state.ComponentStates[state.Status - 1]-1];
	}
	else
	{
	return DegradationMatrix[state.Status - 1][state.ComponentStates[state.Status - 1]];
	}
}

double TravelMDP::ModifyStateWithEvent(State& state, size_t event) const
{
	assert(state.Status != 0);
	size_t Component = state.Status-1;
	size_t Machine = MachineComponents()[Component];
	if (state.ComponentStates[Component] < DegradationMatrix[Component].size()-1 && !MachineDown(state, MachineComponents()[Component]))
	{
		state.ComponentStates[Component] = state.ComponentStates[Component]+event;
	}
	else
	{
		if (state.ComponentStates[Component] == DegradationMatrix[Component].size() && state.RemainingTimeFSE == 0)
		{
			state.ComponentStates[Component] = 0;
		}
	}
	if (state.Status == Components())
	{
		if (state.RemainingTimeFSE == 0)
		{
			state.Status = 0;
		}
		else
		{
			state.Status = 1;
			state.RemainingTimeFSE--;
		}
		size_t temp = 0;
		for (size_t i = 0; i < ComponentsPerMachine.size(); i++)
		{
			if (MachineDown(state, i))
			{
				temp += DowntimeCost[i];
			}
		}
		return temp;
	}
	else
	{
		state.Status++;
	}
	return 0;
}

TravelMDP TravelMDP::GetSimpleInstance()
{
	TravelMDP mdp{};
	mdp.alpha = 0.99;

	mdp.ComponentsPerMachine = { 1,1,1 };
	mdp.CorrCost = { 3,3,3 };
	mdp.PrevCost = { 1,1,1 };
	mdp.DowntimeCost = { 100,100,100 };
	mdp.Repairtimepercomponent = { 1,3,1 };
	mdp.DistanceMatrix = { {0,3,3},{3,0,3},{3,3,0}};

	assert(mdp.Locations() == mdp.DistanceMatrix.size());
	assert(mdp.Components() == mdp.CorrCost.size());

	for (size_t i = 0; i < mdp.Components(); i++) //# of components
	{
		std::vector<std::vector<double>> temp;
		for (size_t j = 0; j < 4; j++) //# of failure states
		{
			temp.push_back({ 0.5,0.5 });
		}
		temp.push_back({1.0});
		mdp.DegradationMatrix.push_back(temp);
	}

	return mdp;
}


std::vector<TravelMDP::State> TravelMDP::AllStates() const
{
	std::vector<TravelMDP::State> AllStates;
	
	CartesianProduct<size_t> cartAttValues{ TotalCompStates() };
	
	while (cartAttValues.HasNext())
	{
		auto CompStateVec = cartAttValues.GetNext();
		for (size_t i = 0; i < Locations(); i++)
		{
			for (size_t j = 0; j <= Components(); j++)
			{
				//if (count(CompStateVec.begin(), CompStateVec.end(), 0) == CompStateVec.size() && j < Components())
				//{
				//	j++;
				//}
				for (size_t k = 0; k <= Maxvalue(i); k++)
				{
					AllStates.push_back(TravelState(CompStateVec, i, k, j));
					//std::cout << TravelState(CompStateVec, i, k, j) << std::endl;
				}
			}
		}
	}
	return AllStates;
}

size_t TravelMDP::Maxvalue(size_t Location) const
{
	size_t MaxD = 0;
	for (size_t j = 0; j < DistanceMatrix[Location].size(); j++)
	{
		if (DistanceMatrix[j][Location] > MaxD) {
			MaxD = DistanceMatrix[j][Location];
		}
	}
	for (size_t k = 0; k < Components(); k++)
	{
		if (MachineComponents()[k] == Location && Repairtimepercomponent[k] > MaxD)
		{
			MaxD = Repairtimepercomponent[k];
		}
	}
	return MaxD;
}


std::vector<std::vector<size_t>> TravelMDP::TotalCompStates() const
{
	std::vector<std::vector<size_t>> val1;
	for (size_t k = 0; k < Components(); k++)
	{
		std::vector<size_t> val;
		for (int i = 0; i <= DegradationMatrix[k].size(); i++)
		{
			val.push_back(i);
		}
		val1.push_back(val);
	}		
	return val1;
}

TravelMDP::State TravelMDP::InitialState() const
{
	return TravelState({ 0,0,0 }, 0, 0, 0);
}

TravelMDP TravelMDP::GetComplexinstance()
{
	return TravelMDP();
}
