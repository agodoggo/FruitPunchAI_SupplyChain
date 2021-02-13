#pragma once
#include <vector>
#include <stdlib.h>
#include <string>
#include <iostream>

class DiscreteDist
{
	std::vector<double> TranslatedPMF{};
	long min{ 0 };
	long max{ 0 };
	inline static const double epsilon{ 1e-16 };
    bool IsProbMassFunction(std::vector<double> PMF);

	
public:
	long Max()
	{
		return max;
	}
	long Min()
	{
		return min;
	}

	std::vector<double> ToPMF();


	long GetFractile(double alpha);

	//DiscreteDist& operator= (const DiscreteDist&) = default;

	//DiscreteDist(const DiscreteDist&) = default;


	static DiscreteDist GetConstantDistribution(long constant);
	static DiscreteDist GetZeroDistribution();


	static std::vector<double>  GetPoissonPMF(double rate);
	static std::vector<double>  GetGeometricPMF(double mean);
	

	DiscreteDist Add(const DiscreteDist& other);

	DiscreteDist TakeMaximumWith(long value);

	DiscreteDist Invert();

	void ToConsole();

    double ProbabilityAt(long value);

	double Expectation();

	DiscreteDist(const std::vector<double>& TranslatedPMF, long offSet=0l);

};

