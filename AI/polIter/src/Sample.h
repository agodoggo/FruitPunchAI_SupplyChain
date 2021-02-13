#pragma once
#include<vector>
#include<fstream>
#include<iostream>
#include<cmath>
#include "MDPAdapter.h"

template<typename MDP>
class Sample
{
public:
	using State = typename MDP::State;

	static constexpr bool HasState = MDPAdapter<MDP>::StateCanSerialize();

	size_t BestAction{ 0 };
	double zValue{ 0.0 };
	std::vector<float> ValPerAllowedAction;
	
	State state;

	//These are needed only if the state cannot serialize:
	std::vector<float> features;
	std::vector<size_t> allowedActionsInState;
	size_t offSet{ 0 };
	
	Sample(MDPAdapter<MDP>& mdpAdapter, State state, size_t BestAction, double zValue, std::vector<float>& ValPerAllowedAction) :
		BestAction{ BestAction }, zValue{ zValue }, ValPerAllowedAction{ ValPerAllowedAction }, state{ state }, features{}, allowedActionsInState{}
	{
		if (std::isnan(zValue) || std::isinf(zValue))
		{
			std::string message = "Invalid sample created: zValue should be a finite double";
			std::cout << message << std::endl;
			throw message;
		}
		for (auto val : ValPerAllowedAction)
		{
			if (std::isnan(val) || std::isinf(val))
			{
				std::string message = "Invalid sample created: ValPerAllowedAction should contain only finite floats";
				std::cout << message << std::endl;
				throw message;
			}
		}
		if constexpr (!HasState)
		{
			features = mdpAdapter.GetFeatures(state);
			allowedActionsInState = mdpAdapter.GetAllowedActions(state);
			offSet = mdpAdapter.ActionOffset(state);
		}

	}

	Sample()
	{
	}

	static void TestInputOutput(MDPAdapter<MDP> adapter, std::string filename)
	{
		std::cout << "testing input output labelledstatesample" << std::endl;

		std::cout << adapter.Identifier() <<"   " << adapter.StateCanSerialize() << std::endl;

		auto tracker = adapter.GetNewTracker();
		RNG rng(26071983);
		auto RSource = adapter.GetRandomSource(tracker, rng);
		auto RSourceStatus = adapter.GetRandomSourceStatus(tracker, RSource);
		if (!adapter.IncorporateEventsUntilNextNonTrivialAction(RSource, RSourceStatus, tracker))
		{
			std::string message = "cannot find nontrivial action state";
			std::cout << message << std::endl;
			throw message;
		}

		auto allowedActions = adapter.GetAllowedActions(tracker);
		size_t BestAction = allowedActions[0];

		std::vector<float> AssociatedCosts{};
		for (size_t i = 0; i < allowedActions.size(); i++)
		{
			AssociatedCosts.push_back((float)(rng.NextUniform()));
		}
		
		Sample<MDP> sample(adapter, tracker.state, BestAction,2.1, AssociatedCosts);



		std::cout << sample;
		{
			std::cout << "writing to file \n";
			auto oStr = HardwareUtilities::GetOutputStreamTo(filename);
			oStr << sample;
		}
		{
			Sample insample;
			std::cout << "reading from file \n";
			auto iStr = HardwareUtilities::GetInputStreamFrom(filename);
			iStr >> insample;
			std::cout << insample;
		}
		std::cout << "test completed" << std::endl;


	}

	friend std::ostream& operator<<(std::ostream& os, const Sample& sample)
	{
		//For checking
		os << sample.HasState << "\t";

		//Always needed:	
		os << sample.BestAction << "\t";
		os << sample.zValue << "\t";
		os << sample.ValPerAllowedAction.size() << "\t";
		for (size_t i = 0; i < sample.ValPerAllowedAction.size(); i++)
		{
			os << sample.ValPerAllowedAction[i] << "\t";
		}

		if constexpr (sample.HasState)
		{//If state supports serialization, then other things can be computed on the fly. 
			os << sample.state;
		}
		else
		{//If state does not support serialization, then other things must be computed. 
			os << sample.offSet << "\t";
			os << sample.features.size() << "\t";
			for (size_t i = 0; i < sample.features.size(); i++)
			{
				os << sample.features[i] << "\t";
			}
			os << sample.allowedActionsInState.size() << "\t";
			for (size_t i = 0; i < sample.allowedActionsInState.size(); i++)
			{
				os << sample.allowedActionsInState[i] << "\t";
			}
		}
		os << std::endl;
		return os;
	}

	friend std::istream& operator>>(std::istream& is, Sample& sample)
	{
		bool serializedHasState;
		is >> serializedHasState;
		if (HasState != serializedHasState)
		{
			std::string message = "Serialization options were changed since this sampledata was saved. No longer possible to load this data!";
			std::cout << message << std::endl;
			throw message;
		}		
		is >> sample.BestAction;		
		is >> sample.zValue;
		{
			size_t size{ 0 };
			is >> size;
			std::vector<float> ValPerAction{};
			ValPerAction.reserve(size);
			for (size_t i = 0; i < size; i++)
			{
				float value;
				is >> value;
				ValPerAction.push_back(value);
			}
			sample.ValPerAllowedAction = ValPerAction;
		}

		if constexpr (HasState)
		{
			is >> sample.state;
		}
		else
		{
			is >> sample.offSet;
			{
				size_t size;
				is >> size;
				sample.features.reserve(size);
				for (size_t i = 0; i < size; i++)
				{
					float feature;
					is >> feature;
					sample.features.push_back(feature);
				}
			}
			{
				size_t size;
				is >> size;
				sample.allowedActionsInState.reserve(size);
				for (size_t i = 0; i < size; i++)
				{
					size_t action;
					is >> action;
					sample.allowedActionsInState.push_back(action);
				}
			}
		
		}
		
		return is;
	}

	
};

