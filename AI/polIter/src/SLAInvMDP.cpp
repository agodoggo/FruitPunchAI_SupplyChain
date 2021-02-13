#include <iostream>
#include <algorithm>
#include <tuple>
#include <assert.h>
#include <string>
#include <numeric>
#include "EventStream.h"
#include "CartesianProduct.h"
#include <sstream>
#include "SLAInvMDP.h"

std::string SLAInvMDP::Identifier() const
{
	return "SLAInvMDP";
}

bool SLAInvMDP::AwaitsAction(const State& state) const
{
	return state.processStatus == SLAInvState::Status::AwaitBasestock;
}

size_t SLAInvMDP::NumValidActions() const
{							 //copied from LostSales
	return MaxOrderSize + 1; // Optimal basestock can range from 0 to F^(-1) >= (p/(p+h)
}

double SLAInvMDP::ModifyStateWithAction(State& state, size_t action) const			//Determine basestock level and place order
{
	//Now length becomes L+1:
	if (state.IP < action) 
	{
		state.InventoryVector.push_back(action - state.IP);
		state.IP += (action - state.IP);
	}
	else 
	{
		state.InventoryVector.push_back(0);
		state.IP += 0;
	}
	state.processStatus = State::Status::AwaitDemand;
	return 0.0;
}

const std::vector<double>& SLAInvMDP::EventProbs() const
{
	return DemandProbs;
}




double SLAInvMDP::ModifyStateWithEvent(State& state, size_t event) const			//Demand is satisfied
{
	state.processStatus = SLAInvState::Status::AwaitBasestock;
	state.DaysRemaining -= 1;
	int MaxCumBO = floor(HorizonLenght * EBOTarget);
	int IOH = state.InventoryVector.front();
	state.InventoryVector.pop_front();			//Length is L again (inlcuding warehouse). 

	if (IOH < 0) {
		IOH -= event;
		state.IP -= event;
		state.InventoryVector.front() += IOH;
		if (state.CumBackorders >= MaxCumBO)	// state.CumBackorders is already above target
		{
			state.CumBackorders -= IOH;//Note that IOH is negative. 
			return -IOH * p;
		}
		else									// state.CumBackorders is lower than target
		{
			state.CumBackorders -= IOH;
			if (state.CumBackorders <= MaxCumBO)	//No penalty cost
			{
				return 0.0;
			}
			else									//Penalty cost from MaxCumBO 
			{
				return (state.CumBackorders - MaxCumBO) * p;
			}
		}
	}
	else //IOH > 0 
	{
		if (IOH >= event)
		{//There is sufficient inventory. Satisfy demand and incur holding costs
			IOH -= event;
			state.IP -= event;
			state.InventoryVector.front() += IOH;
			return IOH * h;
		}
		else {		//Not enough inventory to satisfy demand
			IOH -= event;
			state.IP -= event;
			state.InventoryVector.front() += IOH;
			if (state.CumBackorders >= MaxCumBO)	// state.CumBackorders is already above target
			{
				state.CumBackorders -= IOH;
				return -IOH * p;
			}
			else									// state.CumBackorders is lower than target
			{
				state.CumBackorders -= IOH;
				if (state.CumBackorders <= MaxCumBO)	//No penalty cost
				{
					return 0.0;
				}
				else									//Penalty cost from MaxCumBO 
				{
					return (state.CumBackorders - MaxCumBO) * p;
				}
			}
		}
	}
}


SLAInvMDP::State SLAInvMDP::InitialState() const				
{
	size_t DaysRemaining = HorizonLenght;
	auto queue = FIFOQueue<int>(Leadtime + 1);
	queue.push_back(MaxOrderSize);		// /*std::ceil(*/MaxOrderSize/*)*/);//<- initial on-hand = round up exp demand
	
	for (size_t i = 0; i < Leadtime - 1; i++) //Leadtime -1
	{
		queue.push_back(ExpectedDemand); //(MaxOrderSize);
	}
	return SLAInvState(DaysRemaining, queue, SLAInvState::Status::AwaitBasestock);
}

SLAInvMDP::State SLAInvMDP::InitialState_Vis() const
{
	size_t DaysRemaining = HorizonLenght;
	auto queue = FIFOQueue<int>(Leadtime + 1);
	queue.push_back(0);		// /*std::ceil(*/MaxOrderSize/*)*/);//<- initial on-hand = round up exp demand

	for (size_t i = 0; i < Leadtime - 1; i++) //Leadtime -1
	{
		queue.push_back(0); //(MaxOrderSize);
	}
	return SLAInvState(DaysRemaining, queue, SLAInvState::Status::AwaitBasestock);
}

size_t SLAInvMDP::NumEventsRemaining(const State& state) const
{
	return state.DaysRemaining;
}

bool SLAInvMDP::IsFinal(const State& state) const
{
	return state.DaysRemaining == 0;
}

size_t SLAInvMDP::NumFeatures() const
{
	return Leadtime + 2; //DaysRemaining and (CumBackorders - HorizonLength*EBOTarget)
}

void SLAInvMDP::ExtractFeatures(const State& state, float* out_feat_array) const
{
	static_cast<double>(state.CumBackorders);
	static_cast<double>(HorizonLenght);
	size_t NumFeats = NumFeatures();
	size_t iter{ 0 };
	out_feat_array[iter++] = static_cast<float>(state.DaysRemaining);
	out_feat_array[iter++] = static_cast<float>(state.CumBackorders - HorizonLenght*EBOTarget); //Better when positive, negative if EBOtarget is not achieved
	//
	for (auto inv : state.InventoryVector)
	{
		out_feat_array[iter++] = (float)(inv);
	}
	if (iter != NumFeats)
	{
		throw "program logic error";
	}
	return;

}

SLAInvMDP SLAInvMDP::GetSimpleInstance1()
{
	return GetParametrizedInstance(4, 19.0, 1.0, SLAInvMDP::DemandType::Poisson, 5.0, 5, 0.2);
}

SLAInvMDP SLAInvMDP::GetSimpleInstance2()
{
	return GetParametrizedInstance(30, 99.0, 1.0, SLAInvMDP::DemandType::Poisson, 5.0, 100, 0.05);
}

SLAInvMDP SLAInvMDP::GetSimpleInstance3()
{
	return GetParametrizedInstance(30, 99.0, 1.0, SLAInvMDP::DemandType::Poisson, 5.0, 100, 0.05);
}


SLAInvMDP SLAInvMDP::GetParametrizedInstance(size_t Leadtime, double p, double h, SLAInvMDP::DemandType demandType, double ExpectedDemand, size_t HorizonLenght, double EBOTarget)
{
	SLAInvMDP mdp;
	mdp.h = h;
	mdp.p = p;
	mdp.Leadtime = Leadtime;
	mdp.HorizonLenght = HorizonLenght;
	mdp.EBOTarget = EBOTarget;
	mdp.ExpectedDemand = ExpectedDemand;
	switch (demandType)
	{
	case DemandType::Poisson:
		mdp.DemandProbs = DiscreteDist::GetPoissonPMF(ExpectedDemand);
		break;
	case DemandType::Geometric:
		mdp.DemandProbs = DiscreteDist::GetGeometricPMF(ExpectedDemand);
		break;
	default:
		throw "not implemented";
	}
	auto PerPeriodDistribution = DiscreteDist(mdp.DemandProbs);		
	auto DemOverLeadtime = DiscreteDist::GetZeroDistribution();
	for (size_t i = 0; i < mdp.Leadtime ; i++)
	{
		DemOverLeadtime = DemOverLeadtime.Add(PerPeriodDistribution);
	}

	mdp.DemOverLeadtimeProbs = DemOverLeadtime.ToPMF();

	mdp.MaxOrderSize = PerPeriodDistribution.GetFractile(p / (p + h)); 
	mdp.MaxInvInSystem = DemOverLeadtime.GetFractile(p / (p + h));  

	return mdp;
}




void SLAInvMDP::ToScreenInitial(State& state)
{
	if (state.processStatus == SLAInvState::Status::AwaitBasestock) {
		statuss = "Await Action";
	}
	else {
		statuss = "Await Event";
	}
	std::cout << "Initial state: " << statuss
		<< "\nDays remaing: " << state.DaysRemaining << "\t| CumBO: " << state.CumBackorders
		<< "\t| IP: " << state.IP << " \t | IOH: " << state.InventoryVector.front()
		<< "\t| FIFO vector: " << state.InventoryVector << "\n\n";
}


void SLAInvMDP::ToScreen(State& state)
{

	if (state.processStatus == SLAInvState::Status::AwaitDemand)							//Determine basestock and order inv
	{
		statuss = "Action";
		std::cout << "State: " << statuss << " | determine basestock level and place order: "<< state.InventoryVector.back() << " parts \n" 
			//<< "Total costs: " << SLAInvMDP::totalCost(state) << "\n"
			<< "Days remaing: " << state.DaysRemaining << "\t| CumBO: " << state.CumBackorders 
			<< "\t| IP: " << state.IP << " \t | IOH: " << state.InventoryVector.front()
			<< "\t| FIFO vector: " << state.InventoryVector << "\n\n";
	}
	else																					//Demand arrives
	{
		statuss = "Event";
		std::cout << "State: "<< statuss <<  " | satisfy arriving demand \n"
			//<< "Total costs: " << SLAInvMDP::totalCost(state, event) << "\n"
			<< "Max Allowed Backorders: " << floor(HorizonLenght * EBOTarget) << "\t| Total penalty cost: " << std::max(state.CumBackorders - floor(HorizonLenght * EBOTarget), 0.0) * p << std::endl
			<< "Days remaing: " << state.DaysRemaining << "\t| CumBO: " << state.CumBackorders
			<< "\t| IP: " << state.IP << " \t | IOH: " << state.InventoryVector.front()
			<< "\t| FIFO vector: " << state.InventoryVector << "\n\n";
			
	}

	
}

