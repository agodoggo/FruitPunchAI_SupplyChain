#pragma once
#include "Sample.h" 
#include <vector>
#include "RNG.h"
#include "HardwareUtilities.h"


template <typename MDP>
class SampleData
{
	void Save(std::string location)
	{
		auto os = HardwareUtilities::GetOutputStreamTo(location);
		os << samples.size() << std::endl;
		for (auto sample : samples)
		{
			os << sample;
		}
	}


	void Load(std::string location)
	{
		auto is = HardwareUtilities::GetInputStreamFrom(location);
		size_t num_samples;
		is >> num_samples;
		samples.reserve(samples.size() + num_samples);
		for (size_t i = 0; i < num_samples; i++)
		{
			Sample<MDP> sample;
			is >> sample;
			samples.push_back(sample);
		}
	}


	

public:
	SampleData() :samples{}
	{
	}

	void PrintStatistics()
	{
		std::vector<double> levels = { 0.5, 1.0, 1.5,2.0,2.5, 3.0 };
		std::vector<size_t> counts( levels.size(),0);
		for (auto sample : samples)
		{
			for (size_t i = 0; i < levels.size(); i++)
			{
				if (sample.zValue > levels[i])
				{
					counts[i]++;
				}
			}
		}

		std::cout << samples.size() << " samples; ";
		for (size_t i = 0; i < levels.size(); i++)
		{
			std::cout << (counts[i]*100)/samples.size() << "% at Z>" << levels[i] << "; ";
		}
		std::cout << std::endl;
	}
	

	/// <summary>
	/// Gathers data collected by all the nodes, and saves it at location identified by ProblemName and AgentGeneration. 
	/// </summary>
	/// <param name="ProblemName">First identifier used for determining the location of the file. </param>
	/// <param name="AgentGeneration">Second identifier used for determining the location of the file.</param>
	/// <param name="append">Optional identifier (default false). If set to true, then newly collected data is appended to any existing data in the location.</param>
	void GatherAndSave(std::string ProblemName, size_t AgentGeneration, bool append = false)
	{	//This should also work for a multinode environment.
//Each node with rank !=0 saves file in location with node rank appended:
		if (HardwareUtilities::world_rank != 0)
		{
			auto loc = HardwareUtilities::FileNameForData(ProblemName, AgentGeneration, HardwareUtilities::world_rank);
			HardwareUtilities::Remove(loc);
			Save(loc);
		}
		//Wait until all nodes have completed the save. 
		HardwareUtilities::AddBarrier();
		if (HardwareUtilities::world_rank == 0)
		{//Node 0 is responsible for collecting the data, and saving it in the final location
			//Note that this is rank 0, so data for rank 0 need not be loaded because allready in memory. 
			for (size_t rank = 1; rank < HardwareUtilities::world_size; rank++)
			{
				auto loc = HardwareUtilities::FileNameForData(ProblemName, AgentGeneration, rank);
				Load(loc);
				HardwareUtilities::Remove(loc);
			}
			auto loc = HardwareUtilities::FileNameForData(ProblemName, AgentGeneration);

			if (append)
			{//Get cdata in current location;
				Load(loc);

			}
			RNG rng(11112014);
			std::shuffle(samples.begin(), samples.end(), rng.gen_);
			HardwareUtilities::Remove(loc);
			Save(loc);
		}
	}


	//Loads data at the location identified by problemname and agentgeneration, in current context. 
	void Load(std::string ProblemName, size_t AgentGeneration)
	{
		auto loc = HardwareUtilities::FileNameForData(ProblemName, AgentGeneration);
		Load(loc);
	}



	
	std::vector<Sample<MDP>> samples;

};



