#pragma once
#include "MDPAdapter.h"
#include <vector>
#include "StatisticsKeeper.h"
#include <limits>
#include <cmath>

template<typename MDP>
class Experiment
{
private:
	using RandomSource = typename MDPAdapter<MDP>::RandomSource;

public:
	RandomSource randomSource;

	std::vector<double> CostsPerTrajectory;
	size_t NumTrajectoriesRemaining;

	Experiment(RandomSource randomSource,size_t NumActionsPossible, size_t NumTrajectoriesTested):
		randomSource{randomSource},CostsPerTrajectory(NumActionsPossible,std::numeric_limits<double>::quiet_NaN()), NumTrajectoriesRemaining{ NumTrajectoriesTested }
	{
		assert(std::numeric_limits<double>::has_quiet_NaN);
	}

	void StatisticCollectionAborted(size_t action)
	{
		--NumTrajectoriesRemaining;
	}

	void RecordStatistic(size_t action, double value)
	{
		CostsPerTrajectory[action] = value;
		--NumTrajectoriesRemaining;
	}

	bool IsFinished()
	{
		return NumTrajectoriesRemaining == 0;
	}

	void UpdateStatisticsKeeper(std::vector<std::vector<StatisticsKeeper>>& statisticsKeepers,std::vector<bool>& IsActivePerActionNumber)
	{
		assert(statisticsKeepers.size() == CostsPerTrajectory.size());

		for (size_t i = 0; i < IsActivePerActionNumber.size(); i++)
		{
			if (IsActivePerActionNumber[i])
			{
				if (!std::isfinite(CostsPerTrajectory[i])) {
					std::string message = "Logic error in Experiment; statistics are missing which should be there. Are new actions added to ActiveActions?";
					std::cout << message << std::endl;
					throw message;
				}
				
				statisticsKeepers[i][i].AddStatistic(CostsPerTrajectory.at(i));
				for (size_t j = 0; j < IsActivePerActionNumber.size(); j++)
				{
					if (IsActivePerActionNumber[j])
					{
						if (j != i)
						{
							double statistic = CostsPerTrajectory.at(j) - CostsPerTrajectory.at(i);
							if (!std::isfinite(statistic))
							{
								std::string message = "Invalid statistic in Experiment";
								std::cout << message << std::endl;
								throw message;
							}
							statisticsKeepers[i][j].AddStatistic(statistic);
						}
					}
				}

			}
		}
	}



};

