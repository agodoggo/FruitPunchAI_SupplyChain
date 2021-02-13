#pragma once
#include "PoultryMDP.h"
#include "PoultryState.h"

class PoultryAgent
{
	std::vector<double> ValuesPerWeightEmbeddedForRandomPolicy;
	double EmbeddedDiscountFactor{ 0.0 };
	const PoultryMDP* mdp_ptr;
public:

	PoultryAgent(const PoultryMDP* mdp_ptr);

	size_t GetAction(const PoultryState& state) const;

	

};

