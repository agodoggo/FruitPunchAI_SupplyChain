#include "LostSalesMDP.h"
#include "CartesianProduct.h"
#include<sstream>
std::string LostSalesMDP::Identifier() const
{
    std::ostringstream MDPName{};
    MDPName << "LostSales_L" << Leadtime << "_P" << p << "_H" << h << "_Dem";
    switch (demandType)
    {
    case DemandType::Poisson:
        MDPName << "POIS" << ExpectedDemand;
        break;
    case DemandType::Geometric:
        MDPName << "GEOM" << ExpectedDemand;
        break;
    default:
        throw "not implemented";
    }
    return MDPName.str();
}

bool LostSalesMDP::AwaitsAction(const State& state) const
{
    return state.processStatus == LostSalesState::Status::AwaitOrder;
}

size_t LostSalesMDP::NumValidActions() const
{
    return MaxOrderSize + 1;
}

bool LostSalesMDP::IsAllowedAction(const State& state, size_t action) const
{
    if (state.TotalInv + action <= MaxInvInSystem)
    {
        return true;
    }
    return false;
}

double LostSalesMDP::ModifyStateWithAction(State& state, size_t action) const
{
    //Now length becomes L+1:
    state.StateVector.push_back(action);
    state.TotalInv += action;
    state.processStatus = LostSalesState::Status::AwaitDemand;
    return 0.0;
}

const std::vector<double>& LostSalesMDP::EventProbs() const
{
    return DemandProbs;
}

double LostSalesMDP::ModifyStateWithEvent(State& state, size_t event) const
{
    state.processStatus = LostSalesState::Status::AwaitOrder;
    auto onHand = state.StateVector.front();
    state.StateVector.pop_front();//Length is L again. 
    if (onHand > event)
    {//There is sufficient inventory. Satisfy order and incur holding costs
        onHand -= event;
        state.TotalInv -= event;
        state.StateVector.front() += onHand;
        return onHand * h;
    }
    else
    {
        state.TotalInv -= onHand;
        return (event-onHand) * p;
    }
}

size_t LostSalesMDP::NumFeatures() const
{
    return Leadtime;
}

void LostSalesMDP::ExtractFeatures(const State& state, float* out_feat_array) const
{
    size_t NumFeats = NumFeatures();
    size_t iter{ 0 };
    for (auto inv : state.StateVector)
    {
        out_feat_array[iter++] = (float)(inv);
    }   
    if (iter != NumFeats)
    {
        throw "program logic error";
    }
    return;

}

LostSalesMDP LostSalesMDP::GetSimpleInstance()
{
    return GetParametrizedInstance(2, 19.0, 1.0, LostSalesMDP::DemandType::Poisson, 5.0);
}

LostSalesMDP LostSalesMDP::GetComplicatedInstance()
{//Idea: Demand Poisson - 2.5 or 3. P 9 or 19.  LT 8. 
    return  GetParametrizedInstance(8, 19.0, 1.0, LostSalesMDP::DemandType::Poisson, 3.0);
}



size_t LostSalesMDP::NumInstances()
{
    return 24;
}
LostSalesMDP LostSalesMDP::GetInstance(size_t instanceID)
{
	size_t currentID{ 0 };
	for (double P : {4.0, 9.0, 19.0, 39.0})
	{
		for (auto demType : std::vector<LostSalesMDP::DemandType>{ LostSalesMDP::DemandType::Poisson, LostSalesMDP::DemandType::Geometric })
		{
			for (size_t L : {2, 3, 4})
			{
				if (currentID == instanceID)
				{
					return GetParametrizedInstance(L, P, 1.0, demType, 5.0);
				}
				currentID++;
			}
		}
	}
    std::cout << "INVALID ID";
    throw "invalid ID";
}



LostSalesMDP::State LostSalesMDP::InitialState() const
{
    auto queue = FIFOQueue<size_t>(Leadtime+1);
    queue.push_back(MaxInvInSystem);//<- initial on-hand

    for (size_t i = 0; i < Leadtime-1; i++)
    {
        queue.push_back(0);
    }
    return LostSalesState(queue, LostSalesState::Status::AwaitOrder);
}

std::vector<LostSalesState> LostSalesMDP::AllStates() const
{
    std::vector<size_t> MaxLevels;
    MaxLevels.push_back(MaxInvInSystem);

    for (size_t i = 0; i < Leadtime-1; i++)
    {
        MaxLevels.push_back(MaxOrderSize);
    }
    CartesianProduct<size_t> CartProdAwaitAction(MaxLevels);
    MaxLevels.push_back(MaxOrderSize);
    CartesianProduct<size_t> CartProdAwaitEvent(MaxLevels);

    std::vector<LostSalesState> AllStates;

    while (CartProdAwaitAction.HasNext())
    {
        auto& InvLevels = CartProdAwaitAction.GetNext();
        size_t total{ 0 };
        for (auto inv : InvLevels)
        {
            total += inv;
        }
        if (total <= MaxInvInSystem)
        {
            FIFOQueue<size_t> queue(Leadtime+1);
            for (auto inv : InvLevels)
            {
                queue.push_back(inv);
            }
            AllStates.emplace_back(std::move(queue),LostSalesState::Status::AwaitOrder);
        }
    }
    while (CartProdAwaitEvent.HasNext())
    {
        auto& InvLevels = CartProdAwaitEvent.GetNext();
        size_t total{ 0 };
        for (auto inv : InvLevels)
        {
            total += inv;
        }
        if (total <= MaxInvInSystem)
        {
            FIFOQueue<size_t> queue(Leadtime + 1);
            for (auto inv : InvLevels)
            {
                queue.push_back(inv);
            }
            AllStates.emplace_back(std::move(queue),LostSalesState::Status::AwaitDemand);
        }
    }


    return AllStates;
    
}

LostSalesMDP LostSalesMDP::GetParametrizedInstance(size_t Leadtime, double p, double h, LostSalesMDP::DemandType demandType, double ExpectedDemand)
{
    LostSalesMDP mdp;
    mdp.h = h;
    mdp.p = p;
    mdp.Leadtime = Leadtime;
    mdp.demandType = demandType;
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
    for (size_t i = 0; i <= mdp.Leadtime; i++)
    {
        DemOverLeadtime = DemOverLeadtime.Add(PerPeriodDistribution);
    }
    mdp.MaxOrderSize = PerPeriodDistribution.GetFractile(p / (p + h));
    mdp.MaxInvInSystem = DemOverLeadtime.GetFractile(p / (p + h));

    return mdp;
}
