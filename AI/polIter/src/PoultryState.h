#pragma once
#include <vector>
#include <iostream>
#include "RNG.h"
#include "FIFOQueue.h"

class PoultryState
{
public:
	using ProcessStatus = size_t;

	using Weight = size_t;


	FIFOQueue<Weight> ArrivingWeights;
	std::vector<Weight> BinWeights;
	/// <summary>
	/// The process status
	/// </summary>
	ProcessStatus processStatus;


	// F: In case one wants to check input. Not implemented...
	//static void TestInputOutput(std::string filename);


	//Explicitly request the assignment operator and copy constructor. 
	PoultryState& operator=(const PoultryState&) = default;
	PoultryState(const PoultryState&) = default;

	//Assigns number to each state. If states are same, same number must be assigned. 
	std::size_t Hash() const;
	friend bool operator==(const PoultryState& lhs, const PoultryState& rhs);
	friend bool operator!=(const PoultryState& lhs, const PoultryState& rhs);



	static constexpr bool CanSerialize = true;
	friend std::ostream& operator<<(std::ostream& os, const PoultryState & state);
	friend std::istream& operator>>(std::istream& is, PoultryState & state);
	

	PoultryState(FIFOQueue<Weight> ArrivingWeights, std::vector<Weight> BinWeights, ProcessStatus processStatus);
	PoultryState(std::vector<Weight> ArrivingWeights, std::vector<Weight> BinWeights, ProcessStatus processStatus);

	PoultryState();

	
	


	   	  
};


