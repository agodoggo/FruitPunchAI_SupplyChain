#include "Settings.h"
#include<sstream>

void Settings::InitSettings()
{
	//15 minutes;
	TimeOut = 9000;
	MaxSamples = 10000;
	FractionRandomActions = 0.05;
	InitialRollouts = 100;
	MaxRollouts = 1000;
	kValueBanditOptimizer = 2.0;
	NumGensPolicyOpt = 4;
	MiniBatchSize = 64;
	HiddenLayers = { 64,64,64 };
}

std::string Settings::AsString()
{
    std::ostringstream SettingsName{};
	SettingsName << "TimeOut" << TimeOut << "MSamp" << MaxSamples << "N" << InitialRollouts <<
		"N" << MaxRollouts << "K" << kValueBanditOptimizer << "Gens" << NumGensPolicyOpt << "MiniB" << MiniBatchSize<< "FracRand"<< FractionRandomActions << "Hidden";
	for (auto hidden : HiddenLayers)
	{
		SettingsName << hidden << "_";
	}
    return SettingsName.str();
}

Settings Settings::Current;