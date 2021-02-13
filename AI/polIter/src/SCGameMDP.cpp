#include "SCGameMDP.h"
#include "CartesianProduct.h"

std::string SCGameMDP::Identifier() const
{
    return "SupplyChainGame";
}

size_t SCGameMDP::TaskType(const State& state) const
{
    auto converted = static_cast<size_t>(state.ProcessStatus);

    if (converted <= 3)
    {
        return converted;
    }
    //This is not an actions state.
    std::cout << "error" << std::endl;
    std::cout << state;
    throw;
}

bool SCGameMDP::AwaitsAction(const State& state) const
{
    return state.ProcessStatus != SCGameState::Status::AwaitDemand;
}

void SCGameMDP::ToTikzFigure(std::ostream& os, Trace<SCGameMDP> trace, bool skipall)
{

    std::vector<std::string> StatusNames = { "Place order", "Production", "Logistics", "Evolve" ,  "Demand" };
    std::vector<std::string> DemTypes = { "SP","SP","GC" };
    std::vector<std::string> DemLocs = { "GE", "NL", "NL" };


    bool first = true;


    size_t stateCombIter{ 0 };
    size_t repeats{ 0 };

    

    if (skipall)
    {
        first=false;
    }
    while(stateCombIter< trace.StateSequence.size())
    {
        auto& stComb = trace.StateSequence[stateCombIter];
        if (stComb.state.ProcessStatus == SCGameState::Status::AwaitProductionOrder ||
            stComb.state.ProcessStatus == SCGameState::Status::AwaitTransshipment
            )
        {
            if (!first)
            {
                stateCombIter++;
                continue;
            }
        }

       


        os << "{" << std::endl;
        os << "\\setbeamertemplate{navigation symbols}{}" << std::endl;
        os << "\\usebackgroundtemplate{ \\includegraphics[trim = {0cm 0cm 0.05cm 0cm},height = \\paperheight,width = \\paperwidth]{game} }" << std::endl;
        if (first)
        {
            os << "\\begin{frame}[plain]" << std::endl;
        }
        else
        {
            os << "\\begin{frame}[plain,noframenumbering]" << std::endl;
        }
        os << "\\begin{columns}" << std::endl;
        os << "\\column{\\dimexpr\\paperwidth - 0pt}" << std::endl;
        os << "\\begin{tikzpicture}[show background rectangle]" << std::endl;
        os << "\\path(0, -4.8) -- (15.6, 4.8);" << std::endl;

        //Here goes content. 
        //% \invs{ 0 }{1} {2} {3} {4} {5} {6} {7} {8}
        //% \invss{ 9 }{10} {11} {12} {13} {14} {15} {16}
        os << "\\invs";
        for (size_t i = 0; i <= 8; i++)
        {
            auto inv = stComb.state.InventoryPerLocation[i];
            if ( (first && stComb.state.ProcessStatus==SCGameState::Status::AwaitProductionOrder && i<=4)
                || (first && stComb.state.ProcessStatus == SCGameState::Status::AwaitTransshipment &&  i <=6) 
                ||  (first && i <= 7 && stComb.state.ProcessStatus == SCGameState::Status::Evolve)|| inv > 0 || i==8)
            {
                os << "{" << inv << "}";
            }
            else
            {
                os << "{" << "}";
            }
        }
        os << std::endl;
        os << "\\invss";
        for (size_t i = 9; i <= 16; i++)
        {
            
            os << "{" << stComb.state.InventoryPerLocation[i] << "}";
        }
        os << std::endl;
        


        std::string locstring = "";
        std::string dem = "";
        if (stComb.state.ProcessStatus == SCGameState::Status::AwaitDemand)
        {
            first = false;
            if (repeats == 0)
            {
                repeats++;
            }
            else
            {
                repeats = 0;
                stateCombIter++;
                dem = "Demand:";
                size_t event = trace.randomSource[stComb.status];
                auto& demand = DemandInstances[event];
                auto type = DemTypes[demand.location - 14];
                auto location = DemLocs[demand.location - 14];
                locstring = std::to_string(demand.size) + " " + type + " in " + location;
                os << "\\dem" << type << location << "{};";
            }
        }
        else
        {
            stateCombIter++;
        }



        size_t status = static_cast<size_t>(stComb.state.ProcessStatus);
        auto statusname = StatusNames[status];

        os << "\\score{" << dem << "}{" << stComb.state.score << "}{" << stComb.accumulatedCosts << "}{";
        if (stComb.state.DemandsRemaining >0 )
        {

               os << "Round: " << (11 - stComb.state.DemandsRemaining) << "}{" << locstring << "}";
        }
        else
        {
            os << "Completed" << "}{" << locstring << "}";
        }
        os << "\\end{tikzpicture}" << std::endl;
        os << "\\end{columns}" << std::endl;
        os << "\\end{frame}" << std::endl;
        os << "}" << std::endl;
    }
}

size_t SCGameMDP::NumValidActions(size_t taskType) const
{
	switch (taskType)
	{
		case 0:
            return ExternalOrderActions.size();
        case 1:
            return ProductionActions.size();
        case 2:
            return TransshipmentActions.size();	
        case 3:
            return 1;
	default:
		throw;
	}
}

bool SCGameMDP::IsAllowedAction(const State& state, size_t action) const
{
    switch (state.ProcessStatus)
    {
    case SCGameState::Status::AwaitExternalOrder:
    {
        return true;
    }
    case SCGameState::Status::AwaitProductionOrder:
    {
        auto& prodQuantities = ProductionActions[action];
        return prodQuantities[0] <= state.InventoryPerLocation[10] &&
            prodQuantities[1] <= state.InventoryPerLocation[11] &&
            prodQuantities[0] + prodQuantities[1] <= state.InventoryPerLocation[9];
    }
    case SCGameState::Status::AwaitTransshipment:
    {
        auto& transQuantity = TransshipmentActions[action];
        return transQuantity <= state.InventoryPerLocation[15];
    }
    case SCGameState::Status::Evolve:
    {
        return true;
    }
    default:
        throw;
    }
}

double SCGameMDP::ModifyStateWithAction(State& state, size_t action) const
{
    switch (state.ProcessStatus)
    {
    case SCGameState::Status::AwaitExternalOrder:
    {
        auto& orderQuantities = ExternalOrderActions[action];
        for (size_t i = 0; i < 5; i++)
        {
            state.InventoryPerLocation[i] = orderQuantities[i];
        }
        state.ProcessStatus = SCGameState::Status::AwaitProductionOrder;
        return 0.0;
    }
    case SCGameState::Status::AwaitProductionOrder:
    {
        auto& prodQuantities = ProductionActions[action];
        state.InventoryPerLocation[5] += prodQuantities[0];
        state.InventoryPerLocation[6] += prodQuantities[1];
        state.InventoryPerLocation[9] -= prodQuantities[0] + prodQuantities[1];
        state.InventoryPerLocation[10] -= prodQuantities[0];
        state.InventoryPerLocation[11] -= prodQuantities[1];
        state.ProcessStatus = SCGameState::Status::AwaitTransshipment;
        return 0.0;
    }
    case SCGameState::Status::AwaitTransshipment:
    {
        auto transQuantity = TransshipmentActions[action];
        state.InventoryPerLocation[15] -= transQuantity;
        state.InventoryPerLocation[7] += transQuantity;
        //Now move the pawns according to the rules. 
        state.ProcessStatus = SCGameState::Status::Evolve;
        return 0.0;
    }
    case SCGameState::Status::Evolve:
    {
        auto& invs = state.InventoryPerLocation;

        invs[14] += invs[7];
        invs[7] = 0;
        invs[15] += invs[3] + invs[12];
        invs[3] = 0;
        invs[12] = invs[5];
        invs[5] = 0;
        invs[10] += invs[0];
        invs[0] = 0;
        invs[9] += invs[8];
        invs[8] = invs[1];
        invs[1] = 0;
        invs[16] += invs[4] + invs[13];
        invs[4] = 0;
        invs[13] = invs[6];
        invs[6] = 0;
        invs[11] += invs[2];
        invs[2] = 0;
        state.ProcessStatus = SCGameState::Status::AwaitDemand;
        return 0.0;
    }
    default:
        throw;
    }
}

const std::vector<double>& SCGameMDP::EventProbs() const
{
    return DemandProbs;
}

double SCGameMDP::ModifyStateWithEvent(State& state, size_t event) const
{
    auto& invs = state.InventoryPerLocation;
    auto& demand = DemandInstances[event];

    state.DemandsRemaining--;
    state.ProcessStatus = SCGameState::Status::AwaitExternalOrder;
   
    if (demand.size > invs[demand.location])
    {
        double shortage = static_cast<double>(demand.size - invs[demand.location]);        
        state.score += static_cast<double>(invs[demand.location]);
        invs[demand.location] = 0;
        return shortage;
    }
    else
    {
        state.score += static_cast<double>(demand.size);
        invs[demand.location] -= demand.size;
        return 0.0;
    }
}

size_t SCGameMDP::NumEventsRemaining(const State& state) const
{
    return state.DemandsRemaining;
}

size_t SCGameMDP::NumFeatures() const
{
    return NumInvLocations+1; 
}

void SCGameMDP::ExtractFeatures(const State& state, float* out_feat_array) const
{
    size_t NumFeats = NumFeatures();
    size_t iter{ 0 };
    out_feat_array[iter++] =static_cast<float>( state.DemandsRemaining);
    for (size_t i = 0; i < NumInvLocations; i++)
    {
        out_feat_array[iter++] = static_cast<float>(state.InventoryPerLocation[i]);
    }
    if (iter != NumFeats)
    {
        throw "error in ExtractFeatures";
    }
}

SCGameMDP SCGameMDP::GetSimpleInstance(bool adapprobs)
{

    SCGameMDP mdp{};
    mdp.adapprobs = adapprobs;
    mdp.NumInvLocations = 17;
    mdp.DemandInstances.reserve(18);
    mdp.DemandProbs.reserve(18);

	std::vector<double> ProbsPerLoc = { 0.3,0.5,0.2 };
	if (!adapprobs)
	{
		ProbsPerLoc = { 1.0 / 3.0,1.0 / 3.0,1.0 / 3.0 };
	}
	for (size_t demLoc = 14; demLoc < 17; demLoc++)
	{
		for (size_t demSize = 1; demSize < 7; demSize++)
        {
            mdp.DemandInstances.emplace_back(demSize, demLoc);
            mdp.DemandProbs.push_back(ProbsPerLoc[demLoc-14]/6.0 );
        }
    }

   
    {//External order actions
        CartesianProduct<size_t> prod({ 6,6,6,2,2 });

        while (prod.HasNext())
        {
            auto option = prod.GetNext();
            size_t costs = option[0] + option[1] + option[2] + 3 * (option[3] + option[4]);
            if (costs == 6)
            {
                mdp.ExternalOrderActions.push_back(option);   
            }
        }
    }
    {//Production actions
        CartesianProduct<size_t> prod({ 6,6 });
        while (prod.HasNext())
        {
            auto option = prod.GetNext();
            mdp.ProductionActions.push_back(option);
        }
    }
    {//Transshipment actions:
        for (size_t i = 0; i < 7; i++)
        {
            mdp.TransshipmentActions.push_back(i);
        }
    }

    {
        CartesianProduct<size_t> initial({ 4,4,4,4 });
        while (initial.HasNext())
        {
            mdp.InitialQDist.push_back(initial.GetNext());
        }
    }

    return mdp;
}

SCGameMDP::State SCGameMDP::BlankState() const
{
    std::vector<size_t> invPerLocation(17, 0);
    SCGameState state(10, invPerLocation, SCGameState::Status::AwaitExternalOrder);
    return state;
}

SCGameMDP::State SCGameMDP::InitialState() const
{
    std::vector<size_t> invPerLocation(17,0 );
    invPerLocation[8] = 2;
    invPerLocation[9] = 2;
    invPerLocation[10] = 2;
    invPerLocation[11] = 2;
    invPerLocation[12] = 3;
    invPerLocation[13] = 3;
    invPerLocation[15] = 3;
    size_t remainingDemands = 10;
    SCGameState state(remainingDemands, invPerLocation, SCGameState::Status::AwaitExternalOrder);
    return state;
}

SCGameMDP::State SCGameMDP::InitialState(RNG& rng) const
{
    auto state = InitialState();
    size_t test = static_cast<size_t>( rng.NextUniform() * InitialQDist.size());
    auto& invDist = InitialQDist[test];
    for (size_t i = 8; i < 12; i++)
    {
        state.InventoryPerLocation[i] = invDist[i - 8];
    }
    return state;
}

bool SCGameMDP::IsFinal(const State& state) const
{
    return state.DemandsRemaining == 0;
}
