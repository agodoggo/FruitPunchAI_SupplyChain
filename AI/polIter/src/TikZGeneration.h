#pragma once
#include "NetworkMDP.h"
#include "StateCombination.h"
#include <vector>
namespace TikZGeneration
{
	void outputState(NetworkMDP& mdp, NetworkMDP::State& state, int number);
	void OutputTrace(std::vector<StateCombination<NetworkMDP>>& trace, NetworkMDP mdp, std::string filename);
};

