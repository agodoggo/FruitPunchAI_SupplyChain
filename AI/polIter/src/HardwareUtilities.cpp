#include "HardwareUtilities.h"
#include <iostream>
#include <string>
#include <sstream>
#include "RNG.h" 
#include <thread>
#include <chrono>
#include <fstream>

#ifdef __linux__

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>



//#define MPI_Available true

#ifdef MPI_Available
#include <mpi.h>
#endif

#define TorchAvailable true

#include <torch/torch.h>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;

#ifdef _WIN32

#include <shlobj.h> 
#include <Windows.h>
#include <wchar.h>
#include <KnownFolders.h>

#include <direct.h>
#endif

#endif

namespace HardwareUtilities
{
	size_t world_rank{ 0 };
	size_t world_size{ 1 };
	size_t hardware_threads{ 1 };

	std::string ProcessorName{ "N/A" };

	std::chrono::time_point<std::chrono::steady_clock> start_time;

	fs::path IORootPath;
	fs::path CurrentIOPath;


	bool PytorchAvailable()
	{
#ifdef TorchAvailable
		return true;
#else
		return false;
#endif 
	}

	bool NodeHasGPUSupport()
	{
#ifdef TorchAvailable
		return torch::cuda::is_available();
#else
		return false;
#endif 
	}

	bool MPIAvailable()
	{
#ifdef MPI_Available
		return true;
#else
		return false;
#endif
	}


	void AddBarrier()
	{
#ifdef MPI_Available
		MPI_Barrier(MPI_COMM_WORLD);
#endif
	}

	void Finalize()
	{
		AddBarrier();
		if (world_rank == 0)
		{
			std::chrono::time_point<std::chrono::steady_clock> end_time = std::chrono::steady_clock::now();
			std::cout << "Total timing for program: " << ((end_time - start_time).count() / 1000000) << " ms" << std::endl;
		}
#ifdef MPI_Available
		MPI_Finalize();
#endif
	}




	void SetSubDir(std::string subdir)
	{
		CurrentIOPath = IORootPath;
		CurrentIOPath /= subdir;
		fs::create_directory(CurrentIOPath);
	}

	void SetSubDir(std::string context, std::string subcontext)
	{
		CurrentIOPath = IORootPath;
		CurrentIOPath /= context;
		fs::create_directory(CurrentIOPath);
		CurrentIOPath /= subcontext;
		fs::create_directory(CurrentIOPath);

	}

	void Initialize(int argc, char* argv[])
	{
#ifdef MPI_Available
		// Initialize the MPI environment
		MPI_Init(NULL, NULL);
#endif
		start_time = std::chrono::steady_clock::now();

		if (argc > 1)
		{//first argument is expected to contain the directory where output goes;
		//This is how we set things up in batch processing (linux) and windows. 
			//So the output dir is the first argument given passed to the executable.
			// In visual studio, set under project->properties->debugging.
			IORootPath = fs::path(argv[1]);
		}
		else
		{//While developing on linux, use this output to be able to call without argument. 
#ifdef __linux__  
			const char* homedir;
			if ((homedir = getenv("HOME")) == NULL) {
				homedir = getpwuid(getuid())->pw_dir;
			}
			IORootPath = fs::path(homedir);
			IORootPath.append("polIter");
			fs::create_directory(IORootPath);
			
#else
#ifdef _WIN32
			//Get location of desktop.
			TCHAR    filePath[MAX_PATH];             // To store the path of the file.
			if (FAILED(SHGetFolderPath(NULL,
				CSIDL_DESKTOPDIRECTORY | CSIDL_FLAG_CREATE,
				NULL,
				SHGFP_TYPE_CURRENT,
				filePath))) // Store the path of the desktop in filePath.
			{
				//return false;
			}
			IORootPath = fs::path(filePath);
#elif
			std::string Message = "To develop on platforms other than linux or windows, please call polIter with a parameter";
			std::cout << Message;
			throw Message;
#endif
#endif
		}
		IORootPath /= "inputoutput";
		fs::create_directory(IORootPath);

		
		SetSubDir("default");
	
		hardware_threads =(size_t)( std::thread::hardware_concurrency());

#ifdef MPI_Available
		// Get the number of processes
		int size;
		MPI_Comm_size(MPI_COMM_WORLD, &size);
		world_size=(size_t)(size);
		int rank;
		// Get the rank of the process
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		world_rank = (size_t)(rank);
		// Get the name of the processor
		char processor_name[MPI_MAX_PROCESSOR_NAME];
		int name_len;
		MPI_Get_processor_name(processor_name, &name_len);

		ProcessorName =std::string(processor_name);
#endif


	}

	void ReportHardwareInfo()
	{
		if (world_rank == 0)
		{
			std::cout << "IO directory: " << IORootPath << std::endl;
		}
		AddBarrier();
		if (MPIAvailable())
		{
			if (NodeHasGPUSupport())
			{
				std::cout << "rank " << world_rank << " out of " << world_size << " nodes (name " << ProcessorName << "); with " << hardware_threads << " hardware threads. with GPU and CUDA support" << std::endl;
			}
			else
			{
				std::cout << "rank " << world_rank << " out of " << world_size << " nodes (name " << ProcessorName << "); with " << hardware_threads << " hardware threads. without GPU support" << std::endl;
			}
		}
		else
		{
			if (NodeHasGPUSupport())
			{
				std::cout << "MPI not available; there are " << hardware_threads << " hardware threads on a single node with GPU and CUDA support" << std::endl;
			}
			else
			{
				std::cout << "MPI not available; there are " << hardware_threads << " hardware threads on a single node without GPU support" << std::endl;
			}
		
		}
		AddBarrier();
	}

	void TestMPI()
	{
		
		if (world_rank > 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}


		{
			std::stringstream str{ };
			str << "MPI_filecommunication_test";
			str << world_rank;
			str << ".txt";
			auto stream = HardwareUtilities::GetOutputStreamTo(str.str());
			stream << "Some_test_message" << world_rank << std::endl;
		}

		AddBarrier();


		if (world_rank == 0)
		{//This is the master
			for (size_t i = 0; i < world_size; i++)
			{
				std::stringstream str{ };
				str << "MPI_filecommunication_test";
				str << i;
				str << ".txt";
				std::ifstream inputfile = HardwareUtilities::GetInputStreamFrom(str.str());
				std::string string{};
				inputfile >> string;
				std::cout << string << "<- read from file in MPIcommunicationtest " << std::endl;
			}
		}
	}




	std::ifstream GetInputStreamFrom(std::string filename)
	{
		std::ifstream ifs;
		auto path = CurrentIOPath;
		path.append(filename);
		if (!fs::exists(path))
		{
			std::cout << "Trying to open " << path << " but file does not exist" << std::endl;
			throw;
		}
		ifs.open(path);
		return ifs;
	}

	std::ofstream GetOutputStreamTo(std::string filename)
	{
		std::ofstream of;
		auto path = CurrentIOPath;
		path.append(filename);
		of.open(path);
		return of;
	}


	void Remove(std::string filename)
	{
		auto path = CurrentIOPath;
		path.append(filename);
		fs::remove(path);
		
	}
	void ToConsole(std::string message)
	{
		if (world_rank == 0)
		{
			std::cout << message << std::endl;
		}
	}
	std::string PathToModelFile(std::string ProblemName, size_t AgentGeneration, size_t version, bool MetaInfo)
	{
		std::ostringstream stream;
		
		stream << "model_" << ProblemName << "_" << AgentGeneration << "_" << version;
		if (MetaInfo)
		{
			stream << ".meta";
		}
		else
		{
			stream << ".pt";
		}	
		auto path = CurrentIOPath;
		path.append(stream.str());
		return path.string();
	}


	std::string FileNameForData(std::string ProblemName, size_t AgentGeneration, size_t rank)
	{
		std::ostringstream stream;
		stream << "labelleddata_" << ProblemName << "_" << AgentGeneration; 
		if (rank != defaultRank)
		{
			stream << "_" << rank;
		}		
		return stream.str();
	}




	void TestInput()
	{
		if (world_rank == 0)
		{
			std::ifstream inputfile = HardwareUtilities::GetInputStreamFrom("input.txt");
			std::string s{};
			inputfile >> s;
			std::cout << s << "<- input " << std::endl;
		}
	}

}
