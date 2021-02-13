#pragma once
#include "LostSalesMDP.h"
class LostSalesBaseStockAgent
{

	const LostSalesMDP* mdp_ptr;
public:

	LostSalesBaseStockAgent(const LostSalesMDP* mdp_ptr);

	size_t GetAction(const LostSalesState& state) const;



};

