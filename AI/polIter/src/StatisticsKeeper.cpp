#include "StatisticsKeeper.h"



StatisticsKeeper::StatisticsKeeper()
{

	count = 0;
	sumOfVal = 0.0;
	sumOfVal2 = 0.0;
}

void StatisticsKeeper::AddStatistic(double value)
{
	sumOfVal += value;
	sumOfVal2 += value * value;
	count++;
}

double StatisticsKeeper::SigmaOfMean() const
{
	if (count <= 1)
	{
		std::string message = "Error in statisticsKeeper. count is <=1: no sigma available.";
		std::cout << message << std::endl;
		throw message;

	}
	double SD2 = (sumOfVal2 - sumOfVal * sumOfVal / count) / (count - 1) / count;
	if (SD2 > 0.0)
	{
		return sqrt(SD2);
	}
	return 0.0;
}

double StatisticsKeeper::VarianceOfMean() const
{
	if (count <=1)
	{
		std::string message = "Error in statisticsKeeper. count is <=1: no variance available.";
		std::cout << message << std::endl;
		throw message;

	}
	double SD2 = (sumOfVal2 - sumOfVal * sumOfVal / count) / (count - 1) / count;
	if (SD2 > 0.0)
	{
		return SD2;
	}
	return 0.0;

}




