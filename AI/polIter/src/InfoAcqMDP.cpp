#include "InfoAcqMDP.h"
#include "InfoAcqState.h"
#include "EventStream.h"
#include "CartesianProduct.h"
#include "assert.h"
#include <numeric>
#include <cmath>
#include <iomanip>

InfoAcqMDP::RandomSource InfoAcqMDP::GetRandomSource(const State& state, RNG& rng) const
{
	std::vector<size_t> events;
	events.reserve(NumAttributes());
	for (size_t i = 0; i < NumAttributes(); i++)
	{
		if (state.ValueOfAttributes[i] == -1)
		{
			events.push_back((size_t)(rng.GenEvent(EventProbsPerAttribute[i])));
		}
		else
		{
			events.push_back((size_t)(state.ValueOfAttributes[i]));
		}
	}
	return events;
}



bool InfoAcqMDP::HasAnotherEvent(const RandomSource& randomSource, const size_t& eventTime) const
{
	return true;
}

size_t InfoAcqMDP::GetEvent(const State& state, const RandomSource& randomSource, size_t& randomSourceStatus) const
{
	assert(!IsFinal(state));
	size_t event;
	if (state.status == InfoAcqMDP::State::Status::AwaitAtrributeRetrieval)
	{
		event = randomSource[state.StatusModifier];
	}
	else
	{
		event = randomSource[NextUnknownAttribute(state)];
	}
	return event;
}

std::string InfoAcqMDP::Identifier() const
{
	return "InfoAcquisition";
} 
bool InfoAcqMDP::AwaitsAction(const State& state) const
{
	return state.status==State::Status::AwaitAction;
}

size_t InfoAcqMDP::NumValidActions() const
{  
	return NumAttributes() + NumFinalDecisions();
}

//Assuming that this method checks if any of the total set of NumValidActions is not allowed before performing action on state!!!!?
bool InfoAcqMDP::IsAllowedAction(const State& state, size_t action) const
{
	if (action >= NumAttributes() && state.status!=State::Status::FinalDecisionMade)
	{
		return true;
	}
	else
	{
		if (state.ValueOfAttributes[action] == -1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

double InfoAcqMDP::ModifyStateWithAction(State& state, size_t action) const
{
	double reward = 0;
	if (action < NumAttributes())
	{
		state.status = State::Status::AwaitAtrributeRetrieval;
		state.StatusModifier = action;
		reward = CostofRetrieval[action];
	}
	else 
	{
		state.status = State::Status::FinalDecisionMade;
		
		state.StatusModifier = action - NumAttributes();
		if (IsFinal(state))
		{
			 reward = CostofDecisions(state);
		}
	}
	return reward;
}

size_t InfoAcqMDP::NumFeatures() const
{
	//Attribute values, 
	return 2*NumAttributes();
}

void InfoAcqMDP::ExtractFeatures(const State& state, float* out_feat_array) const
{
	size_t NumFeats = NumFeatures();
	size_t iter{ 0 };
	{

		for (size_t i = 0; i < NumAttributes(); i++)
		{
			if (state.ValueOfAttributes[i] == -1) 
			{
				out_feat_array[iter++] = (float)(0);
			}
			else
			{
				out_feat_array[iter++] = (float)(state.ValueOfAttributes[i]);
			}
		}
		for (size_t i = 0; i < NumAttributes(); i++)
		{
			if (state.ValueOfAttributes[i] == -1)
			{
				out_feat_array[iter++] = 0.0;
			}
			else
			{
				out_feat_array[iter++] = 1.0;
			}
		}

		if (iter != NumFeats)
		{
			throw "program logic error";
		}
		return;
	}
}

size_t InfoAcqMDP::NumFinalDecisions() const
{
	return ValueforFinalDecision.size();
}

size_t InfoAcqMDP::NumAttributes() const
{
	return EventProbsPerAttribute.size();
}



bool InfoAcqMDP::IsFinal(const State& state) const
{
	if (NextUnknownAttribute(state) == NumAttributes())
	{
		return state.status == InfoAcqState::Status::FinalDecisionMade;
	}
	else
	{
		return false;
	}
}


double InfoAcqMDP::ModifyStateWithEvent(State & state, size_t event) const
{
	double reward = 0;
	if (state.status == InfoAcqMDP::State::Status::AwaitAtrributeRetrieval)
	{
		state.status = InfoAcqState::Status::AwaitAction;
		state.ValueOfAttributes[state.StatusModifier] = (int)(event);
		state.StatusModifier = 0;
	}
	else
	{
		state.ValueOfAttributes[NextUnknownAttribute(state)] = (int)(event);
	}
	if (IsFinal(state))
	{
		reward = CostofDecisions(state);
	}
	return reward;
}

double normalCDF(double x)
{
	return erfc(-x / sqrt(2.0)) / 2.0;
}

double InfoAcqMDP::CostofDecisions(State& state) const
{
	assert(IsFinal(state));
	double reward;
	//reward = std::accumulate(state.ValueOfAttributes.begin(), state.ValueOfAttributes.end(), 0)+ValueforFinalDecision[state.StatusModifier];
	//reward = -(((double)(ValueforFinalDecision[state.StatusModifier]) * state.ValueOfAttributes[2]) - (state.ValueOfAttributes[1] * state.ValueOfAttributes[0])+((double)(ValueforFinalDecision[state.StatusModifier]) / state.ValueOfAttributes[3]) - (state.ValueOfAttributes[4] * (pow((double)(ValueforFinalDecision[state.StatusModifier]),2)))-(state.ValueOfAttributes[5] * state.ValueOfAttributes[6] * -state.ValueOfAttributes[7] * (double)(ValueforFinalDecision[state.StatusModifier])) + (state.ValueOfAttributes[8] + state.ValueOfAttributes[9]));
	if (ValueforFinalDecision[state.StatusModifier] == 0)
	{
		reward = 0;
	}
	else
	{
		reward = -((normalCDF((ValueforFinalDecision[state.StatusModifier] - (1600+ state.ValueOfAttributes[6]*200)) / 100) * -1500) + 
		(5 * (state.ValueOfAttributes[4] + 1) * (state.ValueOfAttributes[5] + 1) * (1 - normalCDF((ValueforFinalDecision[state.StatusModifier] - (1600 + state.ValueOfAttributes[6] * 200)) / 100)))* (
			ValueforFinalDecision[state.StatusModifier] - ((1200 + state.ValueOfAttributes[2] * 300) * (1 - (0.1 * state.ValueOfAttributes[3])) - 200 * state.ValueOfAttributes[1]) - ((1000 * state.ValueOfAttributes[0]) / (5 * (1 + state.ValueOfAttributes[4])))));

	}

	return reward;
}

size_t InfoAcqMDP::NextUnknownAttribute(const State& state) const
{	
	for (size_t i = 0; i < NumAttributes(); i++)
	{
		if (state.ValueOfAttributes[i] == -1)
		{
			return i;
		}
	}
	return NumAttributes();
}
InfoAcqMDP InfoAcqMDP::GetRealSimpleInstance()
{
	InfoAcqMDP mdp{};
	mdp.alpha = 1.000;
	mdp.CostofRetrieval = { 5,20};
	mdp.ValueforFinalDecision = {0, 30 };

	size_t m = 4;
	std::vector<double> p = { 1.0 / 6, 1.0 / 6};

	assert(mdp.CostofRetrieval.size() == m);
	assert(p.size() == m);

	for (size_t i = 0; i < m; i++)
	{
		if (i == 3)
		{
			std::vector <double> temp;
			temp = { (1 - p[i]),(p[i]),0,0 };
			mdp.EventProbsPerAttribute.push_back(temp);

		}
		else
		{
			std::vector <double> temp;
			temp = { (p[i]) / 2,(1 - p[i]) / 2, (1 - p[i]) / 2, (p[i]) / 2 };
			mdp.EventProbsPerAttribute.push_back(temp);
		}
	}

	

	return mdp;
}

InfoAcqMDP InfoAcqMDP::GetSimpleInstance()
{
	InfoAcqMDP mdp{};
	mdp.alpha = 1.000;
	mdp.CostofRetrieval = {1,2,1,2,1,1,2,1,2,1};
	mdp.ValueforFinalDecision = {1,2,3,4,5,6,7,8,9,10};

	size_t m = 10;
	std::vector<double> p = { 10.0 / 12, 10.0 / 12,9.0 / 12, 10.0 / 12, 2.0 / 12, 3.0 / 12, 6.0 / 12, 8.0 / 12, 5.0 / 12, 7.0 / 12, };

	assert(mdp.CostofRetrieval.size() == m);
	assert(p.size() == m);

	for (size_t i = 0; i < m; i++)
	{
		int n = 3;
		std::vector <double> temp;
		for (int k = 0; k <= n; k++)
		{
			temp.push_back(mdp.binomialProbability(n, k, p[i]));
		}
		mdp.EventProbsPerAttribute.push_back(temp);
	}

	return mdp;
}

InfoAcqMDP InfoAcqMDP::GetComplexInstance()
{
	InfoAcqMDP mdp{};
	mdp.alpha = 1.000;
	mdp.CostofRetrieval = {100,500,700,450,100,50,800};
	mdp.ValueforFinalDecision = { 0,1600,1800,2000,2200 };

	size_t m = 7;
	std::vector<double> p = { 1.0 / 2, 1.0 / 3, 1.0 / 4, 1.0 / 2, 1.0 / 2, 1.0 / 3, 1.0 / 4};

	assert(mdp.CostofRetrieval.size() == m);
	assert(p.size() == m);

	for (size_t i = 0; i < m; i++)
	{
		if (i == 3)
		{
			std::vector <double> temp;
			temp = { (1-p[i]),(p[i]),0,0};
			mdp.EventProbsPerAttribute.push_back(temp);

		}
		else
		{
			std::vector <double> temp;
			temp = { (p[i]) / 2,(1 - p[i]) / 2, (1 - p[i]) / 2, (p[i]) / 2 };
			mdp.EventProbsPerAttribute.push_back(temp);
		}
	}

	return mdp;
}
// Function to calculate the number of ways to choose r out of n objects (for the binomial distribution in initial value)
int InfoAcqMDP::nCr(int n, int r)
{
	// Since nCr is same as nC(n-r) 
	// To decrease number of iterations 
	if (r > n / 2)
		r = n - r;

	int answer = 1;
	for (int i = 1; i <= r; i++) {
		answer *= (n - r + i);
		answer /= i;
	}

	return answer;
}

// Function to calculate binomial random variable probability 
double InfoAcqMDP::binomialProbability(int n, int k, double p)
{
	return InfoAcqMDP::nCr(n, k) * pow(p, k) * pow(1 - p, n - k);
}

const std::vector<double>& InfoAcqMDP::EventProbs(const State& state) const
{
	if (state.status == State::Status::AwaitAtrributeRetrieval)
	{
		return EventProbsPerAttribute[state.StatusModifier];
	}
	else
	{
		return EventProbsPerAttribute[NextUnknownAttribute(state)];
	}
}


std::vector<std::vector<int>> InfoAcqMDP::MaxValueperAtt() const
{
	std::vector<std::vector<int>> val1;
	for (size_t k = 0; k < NumAttributes(); k++)
	{
		std::vector<int> val;
		for (int i = 0; i <= (int)(EventProbsPerAttribute[k].size()); i++)
		{
			val.push_back(i-1);
		}
		val1.push_back(val);
	}
	return val1;
}

std::vector<InfoAcqMDP::State> InfoAcqMDP::AllStates() const
{
	
	std::vector<InfoAcqMDP::State> AllStates;

	CartesianProduct<int> cartAttValues{MaxValueperAtt()};


	while (cartAttValues.HasNext())
	{
		auto attValuesVec = cartAttValues.GetNext();
		AllStates.push_back(State(State::Status::AwaitAction, attValuesVec, 0));
		//std::cout << State(State::Status::AwaitAction, attValuesVec, 0) << std::endl;
		for (size_t i = 0; i < NumAttributes(); i++)
		{
			if (attValuesVec[i] == -1)
			{
				AllStates.push_back(State(State::Status::AwaitAtrributeRetrieval, attValuesVec, i));
				//std::cout << State(State::Status::AwaitAtrributeRetrieval, attValuesVec, i) << std::endl;
			}
		}
		for (size_t i = 0; i < NumFinalDecisions(); i++)
		{
			AllStates.push_back(State(State::Status::FinalDecisionMade, attValuesVec, i));
			//std::cout << State(State::Status::FinalDecisionMade, attValuesVec, i) << std::endl;
		}

	}
	return AllStates;
}

InfoAcqMDP::State InfoAcqMDP::InitialState() const
{
	std::vector<int> initAttr(EventProbsPerAttribute.size(), -1);
	return InfoAcqState(InfoAcqState::Status::AwaitAction, initAttr, size_t { 0 });
}
