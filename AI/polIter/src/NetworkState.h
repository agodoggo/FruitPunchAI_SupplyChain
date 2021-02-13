#pragma once
#include <vector>
#include <iostream>
#include "RNG.h"


class NetworkState
{
public:
	static void TestInputOutput(std::string filename);
	
	using ProcessStatus = unsigned int; 

	using InvCount = unsigned int;
	using CustStatusOption = unsigned short;
	//Explicitly request the assignment operator and copy constructor. 
	NetworkState & operator=(const NetworkState&) = default;
	NetworkState(const NetworkState&) = default;

    //Assigns number to each state. If states are same, same number must be assigned. 
	std::size_t Hash() const;
	friend bool operator==(const NetworkState& lhs, const NetworkState& rhs);
	friend bool operator!=(const NetworkState& lhs, const NetworkState& rhs);
	

	static constexpr bool CanSerialize = true;
	friend std::ostream& operator<<(std::ostream& os, const NetworkState& state);
	friend std::istream& operator>>(std::istream& is, NetworkState& state);


		

	
	//Constructor for use with AllStates
	NetworkState(std::vector<InvCount> Inventories,std::vector<CustStatusOption> CustStatus, ProcessStatus processStatus);
	
	NetworkState();

	std::vector<InvCount> Inventories;
	std::vector<CustStatusOption> CustStatus;
	/// <summary>
	/// The process status
	/// </summary>
	ProcessStatus processStatus{ 0 };



};	



