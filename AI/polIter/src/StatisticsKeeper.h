#pragma once
#include <math.h>
#include <string>
#include<iostream>
class StatisticsKeeper
{
public:
	StatisticsKeeper();


	void AddStatistic(double value);

	size_t numStatisticsGathered() const
	{
		return count;
	}

	double MuHat() const
	{
		if (count == 0)
		{
			std::string message = "Error in statisticsKeeper. count is zero: no mean available.";
			std::cout << message << std::endl;
			throw message; 

		}
		return sumOfVal / count;
	}
	double SigmaOfMean() const;
	 
	double VarianceOfMean() const;

private:
	size_t count;
	double sumOfVal;
	double sumOfVal2;
};

