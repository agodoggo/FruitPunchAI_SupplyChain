#include "HardwareUtilities.h"
#include "TestFunctionalities.h"
#include "InfoAcqMDP.h"
#include "GreedyAgent.h"
#include "Demonstrator.h"

void test()
{
//	TestFunctionalities::TestImprovingAgent();
//	TestFunctionalities::TestDemonstrator();
//	TestFunctionalities::TestCartProduct();
//	TestFunctionalities::TestInputOutput();
//	TestFunctionalities::TestAlgorithms();
//	TestFunctionalities::TestGeom();
//	TestFunctionalities::TestOptimalAlgorithm();
//	TestFunctionalities::TestSLAInv();
//	TestFunctionalities::TestSLAInvExample();
//	TestFunctionalities::LostSalesPaperTraining(false);
//	TestFunctionalities::LostSalesPaperTraining(true);
//	TestFunctionalities::LostSalesPaperTrainingSensitivity();
//	TestFunctionalities::LostSalesPaperAnalyze();
//	TestFunctionalities::LostSalesPaperVariance();
//	TestFunctionalities::TestOptimalAlgorithm();
//	TestFunctionalities::TestDiscreteDist();
//	TestFunctionalities::TestAdapter();
//	TestFunctionalities::TestSCGame();
//	TestFunctionalities::VisualizeSCGame();
//	TestFunctionalities::VisualizeSCGame();
	TestFunctionalities::SCGameAIExample();
//	TestFunctionalities::TestInfoAcq();
//	TestFunctionalities::TestTravelWoman();

//	TestFunctionalities::TestApproximatePolicyImprovement();
//	TestFunctionalities::TestFIFOQueue();
//	TestFunctionalities::TestSampleEfficiency(false);
	//TestFunctionalities::TestPoultryAgent();
}
	
int main(int argc, char *argv[])
{
	//This initializes MPI, if available, and facilitates seamless IO accross windows and linux
	//and accross multiple nodes if MPI
	HardwareUtilities::Initialize(argc, argv);
	HardwareUtilities::ReportHardwareInfo();
	test();
	
	//Finalizes MPI and times the whole execution. 
	HardwareUtilities::Finalize();
	return 0;

 }
