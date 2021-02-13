#pragma once
#include "StatisticsKeeper.h"
#include <vector>
#include "Experiment.h"
#include "RNG.h"
#include "MDPAdapter.h"
#include <deque>
#include "Settings.h"

template<typename MDP, bool IncurTestOverhead>
class BanditController
{
	MDPAdapter<MDP> adapter;
	using RandomSourceStatus = typename MDPAdapter<MDP>::RandomSourceStatus;
	using TrajectoryTracker = typename MDPAdapter<MDP>::TrajectoryTracker;

	using State = typename MDP::State;

	const State rootState;

	//The actions that are allowed for this state
	std::vector<size_t> allowedActions;

	//A deque containing any initiated experiments. Each experiment is a set of rollouts corresponding to a certain
	//eventsource; each rollout corresponding to a certain startingaction
	//Note that deque is crucial here: Items need to be pinned in memory.. 
	std::deque<Experiment<MDP>> ExperimentDoubleEndedQueue;
public:
	//A bool per action number indicating whether the action is still being considered as candidate for the best action. Note that ongoing experiments may
//also be for actions not in this set. 
	std::vector<bool> IsActivePerActionNumber;
private:



	std::vector<size_t> ActionNumbersCurrentExperiment;


	//Total number of experiments that were initiated thusfar. 
	size_t NumExperimentsInitiated;

	size_t StartTrajectories{ 10 }; //This is the minimum trajectories for each action.
	size_t MaxTrajectories{ 100};//This is the maximum trajectories per action. 

	double kLevel = 2;//This is the cutoff zvalue above which we will exclude actions. I.e. if an action is more than kLevel standard deviations. 
public:

	std::vector<std::vector<StatisticsKeeper>> statistics;
	size_t numExperimentsCompleted;

	size_t GetBestActiveActionNumber()
	{
		double BestMu = std::numeric_limits<double>::infinity();
		size_t numberForBestAction = 0;
		for (size_t i = 0; i < IsActivePerActionNumber.size(); i++)
		{
			if (IsActivePerActionNumber[i])
			{
				if (statistics[i][i].MuHat() < BestMu)
				{
					numberForBestAction = i;
					BestMu = statistics[i][i].MuHat();
				}
			}
		}
		return numberForBestAction;
	}
	
	double GetZValueForBestAction(bool IncludeInActive = false)
	{
		size_t BestActiveActionNumber = GetBestActiveActionNumber();


		double zValueForBestAlternative = 100.0;
		for (size_t i = 0; i < statistics.size(); i++)
		{
			if ((IncludeInActive||IsActivePerActionNumber[i]) && i != BestActiveActionNumber)
			{
				auto& statisticsForDelta = statistics[BestActiveActionNumber][i];
				//	if (statisticsForDelta.MuHat() < 0.0)
				//		throw;
				double sigma = statisticsForDelta.SigmaOfMean();
				double mu = statisticsForDelta.MuHat();
				double zValue = mu/ sigma;
				if (sigma == 0.0)
				{
					zValue = 100.0;
				}
				zValueForBestAlternative = std::min(zValue, zValueForBestAlternative);
			}
		}
		return zValueForBestAlternative;

	}
private:

	bool CanTerminate()
	{
		
		if (numExperimentsCompleted < StartTrajectories)
		{//Must continue
			return false;
		}
		if (numExperimentsCompleted >= MaxTrajectories)
		{//Must terminate
			return true;
		}
		//Terminate if zvalue of best solution compared to best alternative is bigger than kLevel
		return GetZValueForBestAction() > kLevel;
	}

	
	void UpdateActiveActionNumbers()
	{//Determines for all active action numbers whether they are within k sigma of the currently best performing action.
	//Those that are, remain active, others are not kept. 


		if (numExperimentsCompleted < StartTrajectories)
		{
			return;
		}
		size_t BestActiveActionNumber = GetBestActiveActionNumber();
		for (size_t i = 0; i < IsActivePerActionNumber.size(); i++)
		{
			if (i != BestActiveActionNumber && IsActivePerActionNumber[i])
			{
				auto& statisticsForDelta = statistics[BestActiveActionNumber][i];
				if (statisticsForDelta.MuHat() > kLevel * statisticsForDelta.SigmaOfMean() )//So the estimate of the difference of the mean is 
					//bigger than k timers the standard deviation. This is no longer considered as potentially optimal. 
				{
					IsActivePerActionNumber[i] = false;
				}
			}
		}
	}

public:

	struct TrajectoryInfo
	{
		//The current status of the eventsource for this trajectory.
		RandomSourceStatus status;
		//The variance reduction device that this trajectory is controlled by. 
		Experiment<MDP>* experiment;
		//The number of the initial action that defines this trajectory
		size_t action_number;
	};

	void FillPartialExperiment(TrajectoryTracker& tracker, TrajectoryInfo& trajectoryInfo, RNG& randGen)
	{
		tracker = adapter.GetTracker(rootState);
		if (ActionNumbersCurrentExperiment.size() == 0)
		{
			//Update the actions that are still "participating" 
			UpdateActiveActionNumbers();
			//We will initiate a new experiment now, increase counter: 
			NumExperimentsInitiated++;
			//Fill vector with actions participating in current experiment
			for (size_t i = 0; i < IsActivePerActionNumber.size(); i++)
			{
				if (IsActivePerActionNumber[i])
				{
					ActionNumbersCurrentExperiment.push_back(i);
				}
			}
			ExperimentDoubleEndedQueue.emplace_back(adapter.GetRandomSource(tracker, randGen), allowedActions.size(), ActionNumbersCurrentExperiment.size());
		}

		
		//This action is active, it will be the next action to schedule for the current experiment:
		trajectoryInfo.action_number = ActionNumbersCurrentExperiment.back();
		ActionNumbersCurrentExperiment.pop_back();
		trajectoryInfo.status = adapter.GetRandomSourceStatus( tracker, ExperimentDoubleEndedQueue.back().randomSource);
		//Experiment for this setting is the last experiment that was initiated, at the back of the queue. 
		trajectoryInfo.experiment = &ExperimentDoubleEndedQueue.back();
		//So this experiment corresponds to the appropriate action number:
		adapter.IncorporateAction( tracker, allowedActions[trajectoryInfo.action_number]);
	}


	BanditController(const MDP* mdp_ptr, State rootState, std::vector<size_t> allowedActions) :
		adapter{ mdp_ptr }, rootState{ rootState }, allowedActions{ allowedActions }, ExperimentDoubleEndedQueue{}, IsActivePerActionNumber(allowedActions.size(), true), ActionNumbersCurrentExperiment{}, NumExperimentsInitiated{ 0 },
		statistics{ allowedActions.size(),std::vector<StatisticsKeeper>{allowedActions.size(), StatisticsKeeper{}} }, //A vector of vectors of statisticskeeper, one for every combination of action numbers. 
		numExperimentsCompleted{ 0 }
	{	

		StartTrajectories = Settings::Current.InitialRollouts;
		MaxTrajectories = Settings::Current.MaxRollouts;
		kLevel = Settings::Current.kValueBanditOptimizer;

		if (IncurTestOverhead)
		{
			StartTrajectories = 5000;
			MaxTrajectories = std::max(StartTrajectories, MaxTrajectories);
		}
	}

	bool MoreStatisticsNeeded()
	{
		while (!ExperimentDoubleEndedQueue.empty() && ExperimentDoubleEndedQueue.front().IsFinished())
		{
			ExperimentDoubleEndedQueue.front().UpdateStatisticsKeeper(statistics,IsActivePerActionNumber);
			UpdateActiveActionNumbers();
			numExperimentsCompleted++;
			ExperimentDoubleEndedQueue.pop_front();
			if (CanTerminate())
			{
				return false;
			}
		}
		return true;

	}
};

