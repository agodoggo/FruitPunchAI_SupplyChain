#include "LostSalesBaseStockAgent.h"

LostSalesBaseStockAgent::LostSalesBaseStockAgent(const LostSalesMDP* mdp_ptr)
	:mdp_ptr{mdp_ptr}
{
}

size_t LostSalesBaseStockAgent::GetAction(const LostSalesState& state) const
{
	size_t action = mdp_ptr->MaxInvInSystem - state.TotalInv;
	//We maximize, so actually this is capped base-stock. 
	if (action > mdp_ptr->MaxOrderSize)
	{
		action = mdp_ptr->MaxOrderSize;
	}
	return action;
}
