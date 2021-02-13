cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(polIter)
find_package(Torch REQUIRED)

add_executable(polIter src/polIter.cpp src/SLAInvState.cpp src/SLAInvMDP.cpp src/SLAInvAgent.cpp src/LostSalesBaseStockAgent.cpp src/LostSalesMDP.cpp src/LostSalesState.cpp src/DiscreteDist.cpp src/Settings.cpp src/NetworkMDP.cpp src/RNG.cpp src/NetworkState.cpp src/SimpleMDP.cpp src/SimpleState.cpp src/HardwareUtilities.cpp src/SimpleAgent.cpp src/TestFunctionalities.cpp src/NetworkAgent.cpp src/TikZGeneration.cpp src/StatisticsKeeper.cpp src/PoultryAgent.cpp src/PoultryMDP.cpp src/PoultryState.cpp src/InfoAcqMDP.cpp src/InfoAcqState.cpp src/SCGameMDP.cpp src/SCGameState.cpp src/TravelMDP.cpp src/TravelState.cpp)
target_compile_options(polIter PRIVATE "-Wall") 
target_link_libraries(polIter "${TORCH_LIBRARIES}")
target_link_libraries(polIter stdc++fs)
#add_library(cuda SHARED IMPORTED)
#set_property(TARGET cuda PROPERTY IMPORTED_LOCATION "/src/libcuda.so.1")
#target_link_libraries(polIter cuda)

set_target_properties(polIter PROPERTIES
	CXX_STANDARD 17
)


#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=ivybridge -O3 -std=c++1z")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -march=native -O3")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -march=native -O3 -std=c++1z")
