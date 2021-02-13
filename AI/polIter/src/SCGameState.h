#pragma once
#include <iostream>
#include <vector>
class SCGameState
{

public:
	size_t DemandsRemaining{ 0 };

	std::vector<size_t> InventoryPerLocation;
	

	enum class Status : size_t { AwaitExternalOrder = 0, AwaitProductionOrder = 1, AwaitTransshipment = 2, Evolve = 3, AwaitDemand = 4 };

	
	Status ProcessStatus{ 0 };
	double score{ 0.0 };

	//Explicitly request the assignment operator and copy constructor. 
	SCGameState& operator=(const SCGameState&) = default;
	SCGameState(const SCGameState&) = default;


	static constexpr bool CanSerialize = true;
	friend std::ostream& operator<<(std::ostream & os, const SCGameState& state);
	friend std::istream& operator>>(std::istream & is, SCGameState& state);

	SCGameState(size_t DemandsRemaining, std::vector<size_t> InventoryPerLocation, Status ProcessStatus);

	SCGameState();


};

