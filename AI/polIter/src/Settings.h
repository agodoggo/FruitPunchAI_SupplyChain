#pragma once
#include <string>
#include <vector>

class Settings
{
public:
	long long TimeOut;
	size_t MaxSamples;
	size_t NumGensPolicyOpt;

	
	size_t InitialRollouts;
	size_t MaxRollouts;
	double kValueBanditOptimizer;
	
	
	double FractionRandomActions;
	
	std::vector<int64_t> HiddenLayers;
	size_t MiniBatchSize;

	void InitSettings();
	std::string AsString();


	static Settings Current;
private:
	Settings()
	{
		InitSettings();
	}

};

