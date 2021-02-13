#pragma once
#include "SLAInvState.h"
#include "SLAInvMDP.h"

class SLAInvAgent
{

	const SLAInvMDP* mdp_ptr;
public:

	SLAInvAgent(const SLAInvMDP* mdp_ptr);

	size_t GetAction(const SLAInvState& state) const;



};