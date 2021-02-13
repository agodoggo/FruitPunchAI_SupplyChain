#include <vector>
#include <iostream>
#include <fstream>
#include "SimpleState.h"
#include "SLAInvState.h"

SLAInvState::SLAInvState()
{
	IP = 0; //not sure
}

void SLAInvState::UpdateTotalInv()
{
	IP = 0;
	for (auto inv : InventoryVector)
	{
		IP += inv;
	}
}

SLAInvState::SLAInvState(size_t DaysRemaining, FIFOQueue<int> InventoryVector, Status processStatus)
	:DaysRemaining{DaysRemaining}, InventoryVector {InventoryVector}, processStatus{ processStatus }
{
	UpdateTotalInv();
}


// SLAInvState state{};
