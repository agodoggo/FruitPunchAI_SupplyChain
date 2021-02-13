#pragma once
#include "SimpleMDP.h"
#include "SimpleState.h"

class SimpleAgent
{
public:



	SimpleAgent();

	size_t GetAction(const SimpleState& state) const;

};

