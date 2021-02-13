#pragma once
#include <vector>
#include <iostream>
#include "RNG.h"
#include "FIFOQueue.h"

class SLAInvState 
{
public:
	enum class Status : size_t { AwaitBasestock = 0, AwaitDemand = 1 };

	FIFOQueue<int> InventoryVector;

	int IP;

	size_t DaysRemaining{ };		//attributes already initialized (from SCGame)

	size_t CumBackorders{ 0 };			//every day we check number of backorders and add this to the variable CumBackorders

	Status processStatus{ 0 };			//First await demand

	void UpdateTotalInv();

	SLAInvState(size_t DaysRemaining, FIFOQueue<int> InventoryVector, Status processStatus);

	SLAInvState(); //constructor
};