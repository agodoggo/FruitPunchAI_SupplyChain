#pragma once
#include <fstream>
#include <string>
#include <chrono>
#ifdef __linux__
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif
namespace HardwareUtilities
{
	extern fs::path IORootPath, CurrentIOPath;

	extern size_t world_rank, world_size;
	extern size_t hardware_threads;
	extern std::string ProcessorName;
	extern std::chrono::time_point<std::chrono::steady_clock> start_time;
	constexpr size_t defaultRank=1000000000;


	void Initialize(int argc, char *argv[]);

	bool PytorchAvailable();
	bool NodeHasGPUSupport();
	bool MPIAvailable();
	void AddBarrier();
	void Finalize();
	void ReportHardwareInfo();
	void TestMPI();
	void SetSubDir(std::string context);
	void SetSubDir(std::string context,std::string subcontext);

	void Remove(std::string filename);
	void ToConsole(std::string message);
	
	std::ifstream GetInputStreamFrom(std::string filename);
	std::ofstream GetOutputStreamTo(std::string filename);

	std::string FileNameForData(std::string ProblemName, size_t AgentGeneration, size_t rank = defaultRank);

	std::string PathToModelFile(std::string ProblemName, size_t AgentGeneration, size_t version, bool MetaInfo);
	/// <summary>
/// To test whether your input/output directory is properly set up, 
/// this loads a file input.txt from 
/// inputoutput directory, reads a string from it, and displays the string.
/// </summary>
	void TestInput();

};

