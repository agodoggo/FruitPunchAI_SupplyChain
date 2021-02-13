#pragma once
#include "MDPAdapter.h"

template <class MDP>
class StateCombination
{
public:
	using RandomSourceStatus = typename MDPAdapter<MDP>::RandomSourceStatus;

	StateCombination() {}

	typename MDP::State state;
	size_t action{ 0 };
	double accumulatedCosts{ 0.0 };
	RandomSourceStatus status{};

};

