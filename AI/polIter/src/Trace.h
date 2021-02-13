#pragma once
#include "StateCombination.h"
#include "MDPAdapter.h"
template<class MDP>
class Trace
{
	using RandomSource = typename MDPAdapter<MDP>::RandomSource;
public:

	Trace() {}
	std::vector<StateCombination<MDP>> StateSequence;

	RandomSource randomSource;
};

