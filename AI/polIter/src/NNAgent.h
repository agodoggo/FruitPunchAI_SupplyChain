#pragma once
#include "SampleData.h"
#include "HardwareUtilities.h"
#include <string>
#include <fstream>
#include "MDPAdapter.h"
#include "AgentAdapter.h"
#include "RNG.h"
#ifdef __linux__
//In my case, Torch is only available on the linux server. 
//There is a torch for visual studio c++, but it is not stable (July 2019)
//so not recommended to try to get that one to work at present..:q
#define TorchAvailable true
#endif
#if TorchAvailable
#include <torch/torch.h>
#include "NNModels.h"
#endif

template <class MDP>
class NNAgent
{
private:
	using TrajectoryTracker = typename MDPAdapter<MDP>::TrajectoryTracker;
	using State = typename MDP::State;
	MDPAdapter<MDP> adapter;

public:

	static constexpr bool OptimizedMultiaction = true;

#if TorchAvailable
	LayerModel model;
#endif

	NNAgent(const MDP* mdp_ptr, size_t agentGeneration, size_t version = 0)
		:adapter{ mdp_ptr }
	{
		auto loc = HardwareUtilities::PathToModelFile(adapter.Identifier(), agentGeneration, version, false);
		auto locMeta = HardwareUtilities::PathToModelFile(adapter.Identifier(), agentGeneration, version, true);
		//std::cout << "Loading model at: " << loc << std::endl;
		//Load the neural network model that will do the inference. 
#if TorchAvailable
		model->LoadMetaInfo(locMeta);
		torch::load(model, loc);
#else
		std::cout << "Torch not available cannot load classifier" << std::endl;
		throw "Torch not available cannot load classifier";
#endif

	}
	
	template<bool AllTrajectoriesAwaitAction=true>
	void TakeAction(std::vector<TrajectoryTracker>& trajectories)
	{
		if (trajectories.size() == 0)
		{
			return;
		}
		

		auto legalActions = adapter.TotalValidActions();

		size_t numFeats = adapter.NumFeatures();
#if TorchAvailable
		size_t NumberOfTrajectories = (size_t)(trajectories.size());
		torch::Tensor features = torch::empty({ (long)(NumberOfTrajectories),(long)(numFeats) });
		float* features_ptr = features.data<float>();
		for (size_t stateIter = 0; stateIter < NumberOfTrajectories; stateIter++)
		{
			if constexpr (AllTrajectoriesAwaitAction)
			{
				adapter.ExtractFeatures(trajectories[stateIter].state, (features_ptr + stateIter * numFeats));
			}
			else
			{
				if (adapter.AwaitsAction(trajectories[stateIter]))
				{
					adapter.ExtractFeatures(trajectories[stateIter].state, (features_ptr + stateIter * numFeats));
				}
				else
				{
					for (size_t i = 0; i < numFeats; i++)
					{
						*(features_ptr + stateIter * numFeats + i) = 0.0;
					}
				}
			}
		}

		//Note that NoGradGuard was called elsewhere
		torch::Tensor prediction = model->forward(features);

		float* prediction_ptr = prediction.data<float>();

		for (size_t stateIter = 0; stateIter < NumberOfTrajectories; stateIter++)
		{
			if constexpr (!AllTrajectoriesAwaitAction)
			{
				if (!adapter.AwaitsAction(trajectories[stateIter]))
				{
					continue;
				}
			}
			size_t offset = adapter.ActionOffset(trajectories[stateIter].state);
			size_t numValidActionsForState = adapter.NumValidActions(trajectories[stateIter].state);

			float highest = -std::numeric_limits<float>::max();
			size_t prescribedAction = 0;
			for (size_t action = 0; action < numValidActionsForState; action++)
			{
				if (adapter.IsAllowedAction(trajectories[stateIter].state, action))
				{
					float outputVal = *(prediction_ptr + stateIter * legalActions +/*this is the transformed action:*/ (action + offset));
					if (outputVal > highest)
					{
						highest = outputVal;
						prescribedAction = action;
					}
				}
			}
			if (!adapter.IsAllowedAction(trajectories[stateIter].state, prescribedAction))
			{
				throw "illegal action prescribed by neural net logic";
			}
			adapter.IncorporateAction(trajectories[stateIter], prescribedAction);
		}
		



#else
		std::string Message = "Pytorch not supported, do not use NNagent!";
		std::cout << Message;
		throw Message;
#endif

	}

	size_t GetAction(const State& state)
	{
		auto TaskType = adapter.TaskType(state);
		size_t numFeats = adapter.NumFeatures(TaskType);

#if TorchAvailable
		torch::Tensor features = torch::empty({ 1,(long)(numFeats) });
		float* feats_ptr = features[0].data<float>();
		adapter.ExtractFeatures(state, feats_ptr);
		//Note that NoGradGuard was called elsewhere
		torch::Tensor prediction = model->forward(features);
		float* prediction_ptr = prediction.data<float>();
		//Instead of: prediction.data<float>()
		//std::vector<float> logprobs(prediction.data<float>(), prediction.data<float>() + prediction.numel());

		float highest = -std::numeric_limits<float>::max();
		size_t offset = adapter.ActionOffset(state);
		auto numValidActionsForState = adapter.NumValidActions(state);

		size_t prescribedAction = 0;
		for (size_t action = 0; action < numValidActionsForState; action++)
		{
			if (adapter.IsAllowedAction(state, action))
			{
				if (prediction_ptr[action + offset] > highest)
				{
					highest = prediction_ptr[action + offset];
					prescribedAction = action;
				}
			}
		}
		if (!adapter.IsAllowedAction(state, prescribedAction))
		{
			throw "illegal action prescribed by neural net logic";
		}
		return prescribedAction;
#else
		std::string Message = "Pytorch not supported, do not use NNagent!";
		std::cout << Message;
		throw Message;
		return 0;
#endif
	}

};


