#pragma once
#include "NetworkMDP.h"
#include "NetworkState.h"
	

class NetworkAgent
{
	const NetworkMDP* mdp_ptr;
public:


	NetworkAgent(const NetworkMDP* mdp_ptr);
	
	size_t GetAction(const NetworkState& state) const;

private:
	/// <summary>
	/// Cheapest ship-from warehouses for each of the customers. 
	/// </summary>
	std::vector<std::vector<size_t>> ClosestWarehouses;

	/// <summary>
	/// Order in which warehouses are replenished for heuristic. 
	/// </summary>
	std::vector<size_t> transferPriorities;


};

