#include "NetworkMDP.h"
#include <iostream>
#include <algorithm>
#include <tuple>
#include <assert.h>
#include <string>
#include <numeric>
#include "EventStream.h"
#include "CartesianProduct.h"

void NetworkMDP::ToTikzFigure(std::ostream & os, const State & State, size_t action, bool explicitAction, bool first)
{
	//Should only be called for action states. 
	if (!AwaitsAction(State))
		throw;

	auto ProcessStatus = StatusList[State.processStatus];
	
	std::cout << State;
	std::cout << action << " (action)" << unsigned(ProcessStatus.statusType) << std::endl;

	os << "{" << std::endl;
	os << "\\setbeamertemplate{navigation symbols}{}" << std::endl;
	os << "\\usebackgroundtemplate{ \\includegraphics[trim = {2cm 1.8cm 2cm 3.13cm},height = \\paperheight,width = \\paperwidth]{worldmap} }" << std::endl;
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
	//Draw grid:dir

	/*os << "\\draw[gray, step = 0.1](0, -4.8) grid(15.6, 4.8);" << std::endl;
	os << "\\draw[black, step = 1](0, -4) grid(15, 4);" << std::endl;*/

	//Based on remember picture:
	/*os << "\\begin{plainframe}" << std::endl;
	os << "\\begin{tikzpicture}[remember picture, overlay]" << std::endl;
	os << "\\node[at = (current page.center)]{" << std::endl;
	os << "\\includegraphics[trim = {2cm 1.8cm 2cm 3.13cm},clip,keepaspectratio,width = \\paperwidth, height = \\paperheight]{worldmap} };" << std::endl;*/


	//switch (ProcessStatus.statusType)
	//{

	//}

	
	//convert state to tikz figure. 
	for (auto& wh : Warehouses)
	{
		if (!wh.regionalHub)
		{
			os << "\\warehouse";
		}
		else
		{
			os << "\\twarehouse";
		}
		int inv = State.Inventories[wh.number];
		if (ProcessStatus.statusType == StatusType::ReplenishmentEvent)
		{
			if (wh.number == SourceWarehouse)
			{
				inv++;				
			}
		}
		os << ToNumber(inv) << "{" << wh.loc.x << "," << wh.loc.y << "} {W" << wh.number << "} {" << wh.LabelPlacement << "}" << std::endl;
	}

	for (auto& cust : Customers)
	{
		if (State.CustStatus[cust.number] == 0)
		{
			os << "\\custGreen";
		}
		else
		{
			os << "\\custRed";
		}
		//Demand
		std::string dem = "";
		if (ProcessStatus.statusType == StatusType::DemandArrival)
		{
			if (ProcessStatus.eventModifier == cust.number)
			{
				dem = "x";
			}
		}
		os << "{" << cust.loc.x << "," << cust.loc.y << "}{c" << cust.number << "} {" << cust.LabelPlacementTikz << "} {" << dem << "}" << std::endl;
	}

	if (explicitAction)
	{
		//Add arrow if needed:


		if (ProcessStatus.statusType == StatusType::DemandArrival)
		{
			if (action < Warehouses.size())
			{
				auto wh = Warehouses[action];
				auto customer = Customers[ProcessStatus.eventModifier];
				auto data = WHCustCostMatrix[customer.number][wh.number];

				os << "\\emergency" << data.type << "{NW" << wh.number << "}{Nc" << customer.number << "}"<<"{"<< std::round( data.Cost*10)/10.0 << "}" << std::endl;
			}
		}

		if (ProcessStatus.statusType == StatusType::ReplenishmentEvent)
		{
			if (action != Warehouses.size())
			{
				if (action != SourceWarehouse)
				{
					os << "\\transshipment" << "{NW" << SourceWarehouse << "}{NW" << action << "}" << std::endl;
				}
			}
		}

		if (ProcessStatus.statusType == StatusType::EvalMoves)
		{
			if (action != TransferActions.size())
			{
				auto ts = TransferActions[action];
				os << "\\transshipment" << "{NW" << ts.NumberFrom << "}{NW" << ts.NumberTo << "}" << std::endl;
			}
		}

		for (auto& wh : Warehouses)
		{
			//auto cust = Customers[4];
			for (auto& cust : Customers)
			{
				if (wh.number ==whsourceTikz)
				{
					auto data = WHCustCostMatrix[cust.number][wh.number];
					if (data.Cost < 36)
					{
				//		os << "\\emergency" << data.type << "{NW" << wh.number << "}{Nc" << cust.number << "}" << "{" << std::round(data.HoursOfflight) << "/" << std::round(data.HoursOfTruck) << "}" << std::endl;
					}
				}
			}
		}

	}


	
	std::string message;
	switch (ProcessStatus.statusType)
	{
	case StatusType::AlertRaised:
		message = "Alert raised";
		break;
	case StatusType::AlertsExpired:
		message = "Alert expired";
		break;
	case StatusType::EvalMoves:
		if (action == TransferActions.size()&& explicitAction)
		{
			message = "No further actions";
		}
		else
		{
			message = "Evaluating transshipments";
		}
		break;
	case StatusType::DemandArrival:
		message = "Handle part failure";
		break;
	case StatusType::ReplenishmentEvent:
		if (explicitAction&& action==SourceWarehouse)
		{
			message = "Keep in main"; 
		}
		else
		{
			message = "Component repair completion";
		}
		break;
	default:
		throw;
		break;
	}
	   
	os << "\\writemessage{" << message << "}" << std::endl;


	//os << "\\end{tikzpicture}" << std::endl;
	//os << "\\end{plainframe}" << std::endl;

	os << "\\end{tikzpicture}" << std::endl;
	os << "\\end{columns}" << std::endl;
	os << "\\end{frame}" << std::endl;
	os << "}" << std::endl;

}

std::vector<std::vector<NetworkState::CustStatusOption>> NetworkMDP::LegalCustStatusOptions() const
{
	std::vector<std::vector<NetworkMDP::State::CustStatusOption>> CustStatusList;
	std::vector<size_t> vec{};
	for (size_t i = 0; i < Customers.size(); i++)
	{
		//Options for this customer status are 0,1,...,MaxOption
		vec.push_back(Customers[i].MaxOption );
	}	
	CartesianProduct<State::CustStatusOption> CP(vec);
	while (CP.HasNext())
	{
		auto& CustStatus = CP.GetNext();
		auto total = std::accumulate(CustStatus.begin(), CustStatus.end(), unsigned{});
		if (total <= maxAlertedCustomers)
		{
			CustStatusList.push_back(CustStatus);
		}
	}
	return CustStatusList;
}

NetworkMDP::NetworkMDP()
{


}
/// <summary>
/// Initiates a matrix that gives the costs of satisfying a customer from each warehouse. 
/// </summary>
void NetworkMDP::InitiateWHCustCostMatrix()
{
	WHCustCostMatrix.reserve(Customers.size());
	//populate cost matrix

	for (auto &cust: Customers)
	{
	
		std::vector<WHCustData> CostForCust{};
		CostForCust.reserve(Warehouses.size());
		for (auto &wh : Warehouses)
		{			
			double regionpenalty = wh.loc.RegionPenaltyTo(cust.loc);//in hours
			
			
			double dist = wh.loc.DistTo(cust.loc);//In distance on map, multiply by 3 to gain distance in hours

			double hoursFlight = (dist * 1.8) + regionpenalty;
			if (wh.regionalHub)
			{
				hoursFlight += 8;
			}
			else
			{
				hoursFlight += 24;
			}
			double hoursTruck = (dist * 14.0)+2.0;
			if (!wh.regionalHub&& dist>0.3)
			{
				hoursTruck += 8.0;
			}
			if (dist > 0.3 && wh.loc.region==1)
			{//Crossing borders is inconvenient in far east
				hoursTruck += 100;
			}
			//Cost is hours of flight times importance of customer. 
			std::string desc;
			double distCost = hoursFlight;
			if (hoursTruck < hoursFlight)
			{
				distCost = hoursTruck;
			}


			if (distCost < 10)
			{
				desc = "I";

			}
			else
			{
				if (distCost < 24)
				{
					desc = "II";
				}
				else
				{
					desc = "III";
				}
			}
			double cost{ distCost*cust.importance };


			WHCustData data{ cost,hoursFlight,hoursTruck, desc };
			CostForCust.push_back(data);
		}
		WHCustCostMatrix.push_back(CostForCust);
	}
}

/// <summary>
/// Initiates a list of probabilities of the various events, based on the statuscontexts;
/// </summary>
void NetworkMDP::NormalizeEventProbs()
{
	double TotalProb{ 0.0 };
	for (double& unscaledProb : EventProbsStatic)
	{
		TotalProb += unscaledProb;
	}
	for (size_t i = 0; i < EventProbsStatic.size(); i++)
	{
		EventProbsStatic[i] /= TotalProb;
	}

}

//Note that initiatestatusinfo and initiateeventprobs must be aligned to arrive at proper alignment. 
void NetworkMDP::InitiateStatusInfo()
{
	StatusList.reserve(Customers.size() + 1/*ReplenishmentEvent */ + 1 /*awaitEvent*/);
	EventProbsStatic.reserve(Customers.size() + 1);
	double totalRelativeDemandFrequency{ 0.0 };

	for (auto& c : Customers)
	{//Demand Arrival
		Status status{};
		status.eventModifier = c.number;
		status.numLegalActions = Warehouses.size() + 1;//One for each warehouse + emergency procedure.
		status.statusType = StatusType::DemandArrival;
		status.isOfEventType = true;
		status.canBeTakenOnByState = true;
		EventProbsStatic.push_back(c.relativeDemandFrequency);
		StatusList.push_back(status);
		totalRelativeDemandFrequency += c.relativeDemandFrequency;
	}
	double replenishmentProb { totalRelativeDemandFrequency / rho };

	{//Replenishment
		Status status{};
		status.eventModifier = SourceWarehouse;
		//Setting numlegalactions here to 0 is also possible. 
		//Then we would only add the part on stock if it does not exceed capacity, and 
		//modify the State status to EvaluateMove
		status.numLegalActions = Warehouses.size() + 1;//(One for each destination warehouse + stay put (1)) + ignore
		status.statusType = StatusType::ReplenishmentEvent;
		status.isOfEventType = true;
		//Setting canBeTakenOnByState=false would also be possible. See above. 	
		status.canBeTakenOnByState = false;
		StatusList.push_back(status);
		EventProbsStatic.push_back(replenishmentProb);
	}

	double alertProb = totalRelativeDemandFrequency * alertsPerDemand;
	{//Alerts raised
		for (auto& c : Customers)
		{
			Status status{};
			status.eventModifier = c.number;
			status.numLegalActions = 0;
			status.statusType = StatusType::AlertRaised;
			status.isOfEventType = true;
			status.canBeTakenOnByState = false;
			StatusList.push_back(status);
			EventProbsStatic.push_back(alertProb / Customers.size());
		}
	}

	{//Alerts expired
		Status status{};
		status.eventModifier = 0;//not meaningfull
		status.numLegalActions = 0;
		status.statusType = StatusType::AlertsExpired;
		status.isOfEventType = true;
		status.canBeTakenOnByState = false;
		StatusList.push_back(status);
		EventProbsStatic.push_back(alertProb * resetsPerAlert);
	}

	//NO PROBS BELOW THIS POINT, and only situations with eventType=false! Strict separation between the two is needed for program logic. 
	{//Evaluate Moves
		Status status{};
		status.eventModifier = 0;//not meaningfull
		status.numLegalActions = TransferActions.size() + 1;//One for each possible transfer + stay put;
		status.statusType = StatusType::EvalMoves;
		status.isOfEventType = false;
		status.canBeTakenOnByState = true;
		StatusList.push_back(status);
		EvaluateMoveStatus = StatusList.size() - 1;
		//Is not an event, so no prob attached!
	}

	{//Await Event
		Status status{};
		status.eventModifier = 0;//Not meaningfull;
		status.numLegalActions = 0;
		status.statusType = StatusType::AwaitEvent;
		status.isOfEventType = false;
		status.canBeTakenOnByState = true;
		StatusList.push_back(status);
		AwaitEventStatus = StatusList.size() - 1;
	}

	//Prints statuses
	
	//size_t i{ 0 };
	/*for (auto& status : StatusList)
	{
		std::cout << StatusTypeToString(status.statusType) << "  " << status.eventModifier << " : " <<( i++) << std::endl;
	}*/
}


void NetworkMDP::InitiateTransferActions()
{
	TransferActions.reserve(Warehouses.size()* (Warehouses.size() - 1));
	MaxSystemInventory = 0;

	//size_t WHNumber = 0;
	for (size_t WHNumber = 0;WHNumber < Warehouses.size(); WHNumber++)
	{
		MaxSystemInventory += Warehouses[WHNumber].MaxInv;
		if ((!ForbidShipmentsFromLocal) || Warehouses[WHNumber].regionalHub)
		{
			for (size_t WHToNumber = 0;WHToNumber < Warehouses.size(); WHToNumber++)
			{
				if (WHNumber == WHToNumber)
				{//We are only interested in transfers from WH to different WH;
					continue;
				}
				auto dist = Warehouses[WHNumber].loc.DistTo(Warehouses[WHToNumber].loc);
				double distCost = distCosts(dist);
				auto TransferAction = NetworkMDP::TransferAction(WHNumber, WHToNumber, distCost);
				TransferActions.push_back(TransferAction);
			}
		}
	}
	
}


bool NetworkMDP::IsAllowedAction(const State & state, size_t action) const
{
	//Logic assumes that this will only be called if the state allows actions to be taken.
	assert(AwaitsAction(state));

	auto& StatusContext = StatusList[state.processStatus];
	switch (StatusContext.statusType)
	{
	case StatusType::EvalMoves:
	{
		//Can always choose not to make any moves:
		if (action == TransferActions.size())
		{
			return true;
		}
		//Otherwise, a transfer is legal if source has>0 inventory and dest is not full:
		auto TransferAction = TransferActions[action];
		return state.Inventories[TransferAction.NumberFrom] > 0 &&
			state.Inventories[TransferAction.NumberTo] < Warehouses[TransferAction.NumberTo].MaxInv;
	}
	case StatusType::DemandArrival:
	{
		//Emergency shipment action is always valid.
		if (action == Warehouses.size())
		{
			return true;
		}
		//Other actions encode that part is delivered from a certain warehouse, only legal if that warehouse contains stock.
		return state.Inventories[action] > 0;
	}
	default:
		throw "StatusType is uncovered";
	}
}



//Could make adaptions to allow for context-dependent event, but would be a disadvantage
//considering options for variance reduction
const std::vector<double>& NetworkMDP::EventProbs() const
{
	return EventProbsStatic;
}

/// <summary>
/// Initiates an MDP instance with dim*dim customers in grid form, and (dim-1)*(dim-1) warehouses in grid form. 
/// </summary>
/// <param name="dim">determines the dimensions of the grid of customers</param>
/// <returns>the MDP</returns>
NetworkMDP NetworkMDP::GetComplexInstance(size_t dim)
{
	if (dim <= 1)
	{
		throw "size must be strictly bigger than 1.";
	}
	NetworkMDP mdp{};
	mdp.name = "NetworkMDPComplexInstance";
	mdp.name += std::to_string(dim);

	//Initiate the list of customers with locations
	mdp.Customers.reserve(dim*dim);
	size_t custCount{ 0 };
	double prob = 1.0 / dim / dim;
	for (size_t x = 0; x < dim; x++)
	{
		for (size_t y = 0; y < dim; y++)
		{
			mdp.Customers.emplace_back(Location{ x + 0.0,y + 0.0,0 }, 10.0 + x, prob, custCount++);
		}
	}
	size_t whCount{ 0 };
	//Initiate the list of warehouses with locations
	for (size_t x = 0; x < dim - 1; x++)
	{
		for (size_t y = 0; y < dim - 1; y++)
		{
			mdp.Warehouses.emplace_back(Location{ x + 0.45,y + 0.4,0 }, 1, whCount++);
		}
	}

	//Some other parameters:
	mdp.SourceWarehouse = 0;
	mdp.alpha = 0.99;
	mdp.rho = 0.9;
	mdp.emergencyProcecureCosts = 50;
	mdp.maxAlertedCustomers = 0;
	mdp.alertsPerDemand = 0.3;
	mdp.resetsPerAlert = 2.0;
	mdp.MultiplicativeCostEffectOfAlertedCustomer = 4.0;
	mdp.Initiate();
	return mdp;
}

NetworkMDP NetworkMDP::GetSimpleInstance()
{

	NetworkMDP mdp{};
	size_t custs = 3;

	mdp.name = "NetworkMDPSimpleInstance";


	//Initiate the list of customers with locations
	mdp.Customers.reserve(custs);

	size_t custCount{ 0 };
	double prob = 1.0 / custs;

	for (double crit = 0.001; crit < 6.0; crit += 2.0)
	{
		mdp.Customers.emplace_back(Location{ 0.0,0.0,0 }, crit, prob, custCount++);
	}
	size_t whCount{ 0 };
	//Initiate the list of warehouses with locations
	mdp.Warehouses.emplace_back(Location{ 0.0,0.0,0 }, 2, whCount++);


	//Some other parameters:

	mdp.SourceWarehouse = 0;
	mdp.alpha = 0.99;
	mdp.rho = 0.95;
	mdp.emergencyProcecureCosts = 1;
	mdp.maxAlertedCustomers = 1;
	mdp.alertsPerDemand = 0.02;
	mdp.resetsPerAlert = 2.0;
	mdp.MultiplicativeCostEffectOfAlertedCustomer = 4.0;
	mdp.Initiate();
	return mdp;
}

NetworkMDP NetworkMDP::GetSemiconInstance(bool SmallerInstance)
{
	
	NetworkMDP mdp{};
	if (SmallerInstance)
	{
		mdp.name="NetworkSemiconSmall";
	}
	else
	{
		mdp.name = "NetworkSemiconBig";
	}


	
	mdp.ForbidShipmentsFromLocal = true;
	mdp.Customers.reserve(13);
	mdp.Warehouses.reserve(12);
	double prob = 1.0;//Note that probabilities are not normalized anyhow.
	double crit = 1.0;//basic criticality of all customers
	size_t custCount{ 0 };
	size_t WHCount{ 0 };

	//NL (EMEA: region 0)

	mdp.Warehouses.emplace_back(Location{ 0.51,1.2,0 }, 2, WHCount++, "below right = 2 mm",true);
	if (!SmallerInstance)
	{
		mdp.Customers.emplace_back(Location{ 0.52,1.22,0 }, crit, prob, custCount++, "above right=1.6 mm");
	}
	
	//Israel
	if (!SmallerInstance)
	{
		mdp.Customers.emplace_back(Location{ 2.07,-0.35,0 }, crit, prob, custCount++, "above right=1.6 mm");
		mdp.Warehouses.emplace_back(Location{ 2.07,-0.35,0 }, 1, WHCount++, "above left = 1.6 mm",false);
	}
	//Asia: region 1
	//Singapore:
	mdp.Warehouses.emplace_back(Location{ 5.65,-2.23,1 }, 1, WHCount++, "above = 1 mm",true);


	//Northern china
	if (false&&!SmallerInstance)
	{
		mdp.Customers.emplace_back(Location{ 6.6,0.35,1 }, crit, prob, custCount++, "above left =3.2 mm");
		mdp.Warehouses.emplace_back(Location{ 6.6,0.35,1 }, 1, WHCount++, "above = 2 mm", false);
	}
	//China
	mdp.Customers.emplace_back(Location{ 6.3,-0.2,1 }, crit, prob, custCount++, "above =1.6 mm");
	mdp.Customers.emplace_back(Location{ 6.6,-0.38,1 }, crit, prob, custCount++, "above =1.6 mm");
	mdp.Warehouses.emplace_back(Location{ 6.4,-0.3,1 }, 1, WHCount++, "left = 2.2 mm",false);

	//Taiwan
	mdp.Customers.emplace_back(Location{ 6.59,-0.86,1 }, crit, prob, custCount++, "below = 2.1 mm");
	mdp.Customers.emplace_back(Location{ 6.61,-0.83,1 }, crit, prob, custCount++, "right = 2.5 mm");
	mdp.Warehouses.emplace_back(Location{ 6.61,-0.83,1 }, 1, WHCount++, "below right = 4.1 mm",false);

	//Shenzhen
	mdp.Customers.emplace_back(Location{ 6.2,-0.9,1 }, crit, prob, custCount++, "left = 2.5 mm");
	mdp.Warehouses.emplace_back(Location{ 6.2,-0.9,1 }, 1, WHCount++, "below = 1.5 mm", false);

	//%korea
	mdp.Customers.emplace_back(Location{ 6.9,0.02,1 }, crit, prob, custCount++, "above = 1.6 mm");
	mdp.Customers.emplace_back(Location{ 6.93,-0.06 ,1 }, crit, prob, custCount++, "above right = 1.6 mm");
	mdp.Warehouses.emplace_back(Location{ 6.93,-0.06,1 }, 1, WHCount++, "below = 1.55 mm",false);


	//%japan
	mdp.Customers.emplace_back(Location{ 7.65,0.18 ,1 }, crit, prob, custCount++, "above = 1.6 mm");

	mdp.Warehouses.emplace_back(Location{ 7.65,0.18 ,1 }, 1, WHCount++, "above right = 3.5 mm",false);
	mdp.Customers.emplace_back(Location{ 7.59,0.05,1 }, crit, prob, custCount++, "below right = 4.3 mm");
	mdp.Warehouses.emplace_back(Location{ 7.59,0.05,1 }, 1, WHCount++, "below = 3.2 mm",false);

	//Amerikas: Region 2.

	if (!SmallerInstance)
	{

		//%US
		mdp.Customers.emplace_back(Location{ 12.68,0.65,2 }, crit, prob, custCount++, "above right = 1.6 mm");
		mdp.Warehouses.emplace_back(Location{ 12.68,0.65,2 }, 1, WHCount++, "below right = 1.6 mm",false);

		mdp.Customers.emplace_back(Location{ 13.3,-0.15,2 }, crit, prob, custCount++, "above right = 1.6 mm");
		mdp.Warehouses.emplace_back(Location{ 13.3,-0.15,2 }, 1, WHCount++, "below right = 1.6 mm",false);

		//mdp.Customers.emplace_back(Location{ 14.05,-0.45 ,2}, crit, prob, custCount++, "above right = 1.6 mm");
		mdp.Warehouses.emplace_back(Location{ 12.95,-0.2,2 }, 1, WHCount++, "below left = 1.6 mm",true);
	}
	//Some other parameters:
	mdp.SourceWarehouse = 0;
	mdp.alpha = 0.99;
	mdp.rho = 0.9;
	mdp.maxAlertedCustomers = 1;

	mdp.alertsPerDemand = 0.05;
	mdp.resetsPerAlert = 2.0;
	mdp.MultiplicativeCostEffectOfAlertedCustomer = 4.0;
	mdp.emergencyProcecureCosts = 120;
	mdp.Initiate();
	return mdp;

}

std::vector<NetworkMDP::State> NetworkMDP::AllStates() const
{
	std::vector<State> AllStates;
	//To ensure that resizes of AllStates are not needed, we compute in advance how many elements will be added. 
	//Count number of statuses that can be taken on by a state. 
	size_t capacity =
		std::count_if(StatusList.begin(), StatusList.end(), [](auto s) {return s.canBeTakenOnByState;});
	
	//Fill list with all possible legal custstatusoptions. 
	auto CustStatusList = LegalCustStatusOptions();
	//Final capacity should be multiplied by size of this list. 
	capacity *= CustStatusList.size();

	std::vector<size_t> MaxInvPerWarehouse;
	for (size_t i = 0; i < Warehouses.size(); i++)
	{
		//Inventory for this warehouse takes on values 0,1,...,MaxInv
		MaxInvPerWarehouse.push_back((size_t)(Warehouses[i].MaxInv));
	}
	//Create cartesian product, containing all possible combinations of inventory per warehouse. 
	CartesianProduct<NetworkMDP::State::InvCount> InvCartProd(MaxInvPerWarehouse);

	//multiply with all possible values
	capacity *= InvCartProd.NumStates();
	AllStates.reserve(capacity);
		
	while (InvCartProd.HasNext())
	{
		//Get next element in Cartesian product. 
		auto Inventories = InvCartProd.GetNext();
		for (size_t i = 0; i < StatusList.size(); i++)
		{
			if (StatusList[i].canBeTakenOnByState)
			{
				for (auto custStatus : CustStatusList)
				{
					AllStates.push_back(State(Inventories, custStatus,(NetworkState::ProcessStatus)( i)));
				}
			}
		}
	}	
	
	assert((capacity == AllStates.size()) && "Improper capacity prediction in AllStates");
	return AllStates;
}

NetworkMDP::State NetworkMDP::InitialState() const
{
	std::vector<NetworkMDP::State::InvCount> inventories{};
	inventories.reserve(Warehouses.size());
	for (size_t whIter = 0; whIter < Warehouses.size(); whIter++)
	{
		inventories.push_back(Warehouses[whIter].MaxInv);
	}
	std::vector<NetworkMDP::State::CustStatusOption> CustStatus{};
	CustStatus.reserve(CustStatus.size());
	for (size_t whIter = 0; whIter < Customers.size(); whIter++)
	{
		CustStatus.push_back(0);
	}

	/// Initial state is inventory maxed out everywhere, and 
	/// awaiting the first event. 
	return State(inventories, CustStatus,(State::ProcessStatus)( AwaitEventStatus));
}


std::string NetworkMDP::Identifier() const
{
	return name;
}

bool NetworkMDP::AwaitsAction(const State & state) const
{
	return state.processStatus != AwaitEventStatus;
}



double NetworkMDP::ModifyStateWithAction(State & state, size_t action) const
{
	auto& StatusContext = StatusList[state.processStatus];
	switch (StatusContext.statusType)
	{
	case StatusType::DemandArrival:
	{
		auto custNumber = StatusContext.eventModifier;
		double alertMultiplier = 1.0;
		if (state.CustStatus[custNumber] == 1)
		{
			alertMultiplier = MultiplicativeCostEffectOfAlertedCustomer;
		}
		if (action == Warehouses.size())//=Emergency shipment action
		{//Emergency shipment; no transition made;
			state.processStatus =(NetworkState::ProcessStatus)(EvaluateMoveStatus);
			return emergencyProcecureCosts * Customers[custNumber].importance*alertMultiplier;
		}
		else
		{
			//Inventories at location are reduced
			--state.Inventories[action];
			state.processStatus = (NetworkState::ProcessStatus)(EvaluateMoveStatus);
			return WHCustCostMatrix[custNumber][action].Cost *alertMultiplier;//Note that customer importance is allready in the CustCostMatrix
		}
	}
	case StatusType::EvalMoves:
	{
		//Move a part
		if (action == TransferActions.size())
		{
			//If we choose not to make any moves, then, 
			state.processStatus = (NetworkState::ProcessStatus)(AwaitEventStatus); 
			return 0.0;
		}
		else
		{
			auto& TransferAction = TransferActions[action];
			//Maybe more moves are needed. 
			state.processStatus = (NetworkState::ProcessStatus)( EvaluateMoveStatus);
			//Commenting out the next line means that we 
			//allow multiple transfers after a single "event":
			state.processStatus = (NetworkState::ProcessStatus)(AwaitEventStatus);

			--state.Inventories[TransferAction.NumberFrom];
			++state.Inventories[TransferAction.NumberTo];
			return TransferAction.Cost;
		}
	}
	case StatusType::AwaitEvent:
		throw "Cannot perform action in AwaitEvent state";
		break;
	default:
		throw "Unhandled case";
		break;
	}
}

double NetworkMDP::ModifyStateWithEvent(State & state, size_t event) const
{
	auto& status = StatusList[event];
	unsigned int alerts{ 0 };

	switch (status.statusType)
	{
	case StatusType::AlertRaised:
		for (auto custStatus : state.CustStatus)
			alerts += custStatus;
		if (alerts < maxAlertedCustomers && state.CustStatus[status.eventModifier] ==0)
		{//If there are not allready too many customers on alert, than add alert flag to this customer. 
			state.CustStatus[status.eventModifier] = 1;
			//Evaluation of possibly any moves are needed. 
			state.processStatus = (NetworkState::ProcessStatus)(EvaluateMoveStatus); 
		}
		else
		{
			//No change in state, so can revert to awaiteventstatus.
			state.processStatus = (NetworkState::ProcessStatus)(AwaitEventStatus); 
		}
		return 0.0;
	case StatusType::AlertsExpired:
		//Remove any alerts
	{
		bool changes = false;
		for (auto& custStatus : state.CustStatus)
		{
			if (custStatus > 0)
			{
				changes = true;
				custStatus = 0;
			}
		}
		if (changes)
		{
			state.processStatus = (NetworkState::ProcessStatus)(EvaluateMoveStatus);
			return 0;
		}
		else
		{//No changes were made to the state, so we can await the next event.
			state.processStatus = (NetworkState::ProcessStatus)(AwaitEventStatus);
			return 0;
		}
	}
	case StatusType::ReplenishmentEvent:
		if (Warehouses[0].MaxInv > state.Inventories[0])
		{
			state.Inventories[0]++;
			state.processStatus =(NetworkState::ProcessStatus)( EvaluateMoveStatus);
		}
		else
		{
			state.processStatus = (NetworkState::ProcessStatus)(AwaitEventStatus);
		}
		return 0.0;
	case StatusType::DemandArrival:
		state.processStatus = (NetworkState::ProcessStatus)(event); 
		return 0.0;

	default:
		throw "StatusType not handled in NetworkMDP::ModifyStateWithEvent";

	}
	//avoids warning under GNU compilers:
	return 0.0;
}


size_t NetworkMDP::NumFeatures() const
{
	//To make the situation uniform, we always include 
	if (maxAlertedCustomers > 0)
	{//inventory per warehouse, vector indicating whether customers are alerted or not.., location of customer demand. 
		return Warehouses.size() + 2 * Customers.size();
	}
	else
	{
		return Warehouses.size() + Customers.size();
	}
}

void NetworkMDP::ExtractFeatures(const State& state, float* out_feat_array) const
{ //To ensure that each action state has the same number of features, we have a number of zeros for transshipment
	//states, i.e. states without a specific demand arrival. 
	size_t taskType = NetworkMDP::TaskType(state);
	size_t NumFeats = NumFeatures();
	size_t iter{ 0 };

	{
		//Do one-hot encoding for the location of the customer. 
		for (size_t i = 0; i < state.CustStatus.size(); i++)
		{
			out_feat_array[iter++] = 0.0;
		}
		if (taskType == 0)
		{//one-hot encoding for the arriving customer. 
			auto custNumber = StatusList[state.processStatus].eventModifier;
			out_feat_array[custNumber] = 1.0;
		}
		//Add other features as above. 
		for (size_t i = 0; i < state.Inventories.size(); i++)
		{
			out_feat_array[iter++] =(float)(state.Inventories[i]);

			/*for (size_t j = 0; j < state.Inventories[i]; j++)
			{
				out_feat_array[iter++] = (float)(1.0);
			}
			size_t Remainder = Warehouses[i].MaxInv - state.Inventories[i];

			for (size_t j = 0; j < Remainder; j++)
			{
				out_feat_array[iter++] = (float)(0.0);
			}*/
		}
		if (maxAlertedCustomers > 0)
		{
			for (size_t i = 0; i < state.CustStatus.size(); i++)
			{
				out_feat_array[iter++] = (float)(state.CustStatus[i]);
			}
		}
		if (iter != NumFeats)
		{
			throw "program logic error";
		}
		return;
	}	
}

void NetworkMDP::ToTikzFigure(std::ostream & os, const State & State, size_t action,bool first)
{
	ToTikzFigure(os, State, action,false,first);
	ToTikzFigure(os, State, action, true,false);
	return;
	for (size_t i = 0;i < Warehouses.size();i++)
	{
		whsourceTikz = i;
		ToTikzFigure(os, State, action, true, first);
	}

}


size_t NetworkMDP::TaskType(const State & state) const
{
	auto& StatusContext = StatusList[state.processStatus];
	switch (StatusContext.statusType)
	{
	case StatusType::DemandArrival:
	{
		return 0;
	}
	case StatusType::EvalMoves:
	{
		return 1;
	}
	default:
		throw "unhandled case in NetworkMDP::TaskType";
	}
}


size_t NetworkMDP::NumValidActions(size_t TaskType) const
{
	switch (TaskType)
	{
	case 0://Demand Arrival, must select from which warehouse to satisfy. 
	{
		return Warehouses.size() + 1;//One for each warehouse + emergency procedure.
	}
	case 1://Evaluate moves, must select a move. 
	{
		return TransferActions.size() + 1;//One for each possible transfer + stay put;
	}
	default:
		throw "unhandles case in NetworkMDP::NumValidActionsForTaskType";
	}
}

