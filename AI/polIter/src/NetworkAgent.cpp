#include "NetworkAgent.h"
#include <assert.h>
#include <tuple>

NetworkAgent::NetworkAgent(const NetworkMDP* mdp_ptr): mdp_ptr{mdp_ptr}
{
	//Computes closest warehouses for each customer. 
	ClosestWarehouses.reserve(mdp_ptr->Customers.size());

	for (size_t cust = 0; cust < mdp_ptr->Customers.size(); cust++)
	{
		std::vector<size_t> ClosestToCust{};
		ClosestToCust.reserve(mdp_ptr->Warehouses.size());
		using pair = std::pair<size_t, double>;
		std::vector<pair> WarehouseOrder{};
		WarehouseOrder.reserve(mdp_ptr->Warehouses.size());
		for (size_t wh = 0; wh < mdp_ptr->Warehouses.size(); wh++)
		{
			WarehouseOrder.emplace_back(wh, mdp_ptr->WHCustCostMatrix[cust][wh].Cost);
		}
		std::sort(WarehouseOrder.begin(), WarehouseOrder.end(), [](pair& a, pair& b) {return a.second < b.second;});
		for (pair p : WarehouseOrder)
		{
			//std::cout << p.first << "  " << p.second << std::endl;
			ClosestToCust.push_back(p.first);
		}
		ClosestWarehouses.push_back(ClosestToCust);
	}

	

	//Creates list of warehouses according to warehouse centrality
	std::vector<std::tuple<size_t, double>> warehouseimportance{};
	for (size_t warehouse = 0; warehouse < mdp_ptr->Warehouses.size(); warehouse++)
	{
		warehouseimportance.emplace_back(warehouse, 0);
	}
	for (size_t i=0;i<mdp_ptr->Customers.size();i++)
	{
		auto cust = mdp_ptr->Customers[i];
		auto wh = ClosestWarehouses[i][0];//Warehouse closest to this customer
		auto& tuple = warehouseimportance[wh];
		std::get<1>(tuple) += cust.importance * cust.relativeDemandFrequency ;
		if (mdp_ptr->Warehouses.size() > 1)
		{
			auto wh = ClosestWarehouses[i][1];//Second closest warehouse
			auto& tuple = warehouseimportance[wh];
			std::get<1>(tuple) += cust.importance * cust.relativeDemandFrequency / 2;
		}
	}
	//Sort to see which warehouses have the most "weight", i.e. they are closest or second-closest to many important customers. 

	std::sort(warehouseimportance.begin(), warehouseimportance.end(), [](const auto& lhs, const auto& rhs) {
		return std::get<1>(lhs) > std::get<1>(rhs);
	});

	transferPriorities.reserve(mdp_ptr->Warehouses.size());
	for (auto& tup : warehouseimportance)
	{
		auto wh = std::get<0>(tup);
		{
			for (size_t i = 0 ; i< mdp_ptr->TransferActions.size();i++)
			{
				auto transfer = mdp_ptr->TransferActions[i];
				if (transfer.NumberFrom == 0 && transfer.NumberTo == wh)
				{
					transferPriorities.push_back(i);
				}
			}
		}
	}

	

}

size_t NetworkAgent::GetAction(const NetworkState & state) const
{
	//Logic assumes that this will only be called if the state allows actions to be taken.
	assert(mdp_ptr->AwaitsAction(state));
	auto& StatusContext = mdp_ptr->StatusList[state.processStatus];
	switch (StatusContext.statusType)
	{
	case NetworkMDP::StatusType::EvalMoves:
	{
		//Evaluate possible transfer moves;

		for (auto transfer : transferPriorities)
		{//Find non-full warehouse that is highest on priority list
			//warehouse = action;
			if (mdp_ptr->IsAllowedAction(state, transfer))//warehouse not full yet
			{
				return transfer;
			}
		}
		return mdp_ptr->TransferActions.size();//No move is the default action;
	}
	case NetworkMDP::StatusType::DemandArrival:
	{//Event is a demand arrival. 
		//Iterate over the closest warehouses to this warehouse. 
		for (auto action : ClosestWarehouses[StatusContext.eventModifier])
			//Get the closest warehouse that is nonempty. 
		{
			if (mdp_ptr->IsAllowedAction(state, action))
			{
				return action;
			}
		}//If all warehouses empty:
		return mdp_ptr->Warehouses.size();//Indicates emergency procedure
	}
	default:
	{
		throw "unhandled event";
	}
	}
}
