#include "SimpleMDP.h"
#include "SimpleState.h"
#include "EventStream.h"
std::string SimpleMDP::Identifier() const
{
	return "SimpleMDP";
}
bool SimpleMDP::AwaitsAction(const State & state) const
{
	return state.status!=SimpleState::Status::AwaitEvent;
}

size_t SimpleMDP::NumValidActions() const
{
	return 2;
}

bool SimpleMDP::IsAllowedAction(const State & state, size_t action) const
{
	if (action == 0||action==1 )
		return true;
	return false;
}

double SimpleMDP::ModifyStateWithAction(State & state, size_t action) const
{
	if (state.status == SimpleState::Status::Actionable)
	{
		state.status = SimpleState::Status::Actionable2;
	}
	else
	{
		state.status = SimpleState::Status::AwaitEvent;
	}
	return 1.0;
}

size_t SimpleMDP::NumFeatures() const
{
	return 1;
}

void SimpleMDP::ExtractFeatures(const State& state, float* out_feat_array) const
{
	out_feat_array[0] = 1.0f;
	return;
}

const std::vector<double>& SimpleMDP::EventProbs() const
{	
	return eventprobs;
}

double SimpleMDP::ModifyStateWithEvent(State & state, size_t event) const
{
	if (event == 0)
	{
		state.status = SimpleState::Status::Actionable;
	}
	else
	{
		//remain in same state.
	}
	return 0.0;
}

SimpleMDP SimpleMDP::GetSimpleInstance()
{
	SimpleMDP mdp{};
	mdp.alpha = 1.0;// 0.5;
	mdp.eventprobs = std::vector<double>();
	mdp.eventprobs.push_back(1.0);
	mdp.eventprobs.push_back(0.0);
	return mdp;
}


std::vector<SimpleMDP::State> SimpleMDP::AllStates() const
{
	std::vector<SimpleMDP::State> vec{};
	vec.push_back(State(State::Status::Actionable));
	vec.push_back(State(State::Status::Actionable2));
	vec.push_back(State(State::Status::AwaitEvent));
	return vec;
}

SimpleMDP::State SimpleMDP::InitialState() const
{
	return State(State::Status::AwaitEvent);
}
