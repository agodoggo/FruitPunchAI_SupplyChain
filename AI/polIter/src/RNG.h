#pragma once
#include <random>
#include <vector>
#include <algorithm>
#include "HardwareUtilities.h"
class RNG
{
	RNG(std::seed_seq& seq) : gen_(seq), dist_(0.0, 1.0) {}

	//RNG() : RNG(std::random_device()())
	//{
	//} // Seeds the mt19937.

public:
	
	RNG() = delete;

	RNG(unsigned int i) : gen_(i+(unsigned)(HardwareUtilities::world_rank*1234)), dist_(0.0,1.0)  {}



    void Shuffle(std::vector<size_t>& vector)
	{
		std::shuffle(vector.begin(),vector.end(),gen_);
	}



	double NextUniform()
	{
		return dist_(gen_);
	}

	unsigned int NextSeed()
	{
		return distrUniform_(gen_);
	}

	RNG NextGen()
	{
		auto seq = std::seed_seq{ NextSeed(),NextSeed(),NextSeed(),NextSeed()};
		return RNG(seq);
	}

	void GeomParam(double alpha)
	{
		distGeom_ = std::geometric_distribution<size_t>(1-alpha);
	}

	size_t NextGeom()
	{
		return distGeom_(gen_);
	}
	//Below is equivalent to the above, after setting alpha right.

	//unsigned int NextGeom(double alpha)
	//{
	//	unsigned int EventCount{ 0 };
	//	double probLim = alpha;
	//	double AlphaSeed = NextUniform();
	//	while (AlphaSeed < probLim)
	//	{
	//		probLim *= alpha;
	//		EventCount++;
	//	}
	//	return EventCount;
	//}

	size_t GenEvent(const std::vector<double>& ProbsPerEvent);

	std::mt19937 gen_;
private:
	std::uniform_real_distribution<double> dist_;
	std::geometric_distribution<size_t> distGeom_;
	std::uniform_int_distribution<unsigned int> distrUniform_;
};
