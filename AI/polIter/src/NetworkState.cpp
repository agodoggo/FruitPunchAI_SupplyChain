#include "NetworkState.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "SimpleState.h"

NetworkState::NetworkState(std::vector<InvCount> Inventories, std::vector<CustStatusOption> CustStatus, ProcessStatus processStatus) : Inventories{ Inventories }, CustStatus{ CustStatus }, processStatus{ processStatus }
{
}

NetworkState::NetworkState() : Inventories{}, CustStatus{}
{
}
bool operator==(const NetworkState & lhs, const NetworkState & rhs)
{//Only needed for exact optimization. 
	for (size_t i = 0; i < lhs.Inventories.size(); i++)
	{
		if (lhs.Inventories[i] != rhs.Inventories[i])
		{
			return false;
		}
	}	
	for (size_t i = 0; i < lhs.CustStatus.size();i++)
	{
		if (lhs.CustStatus[i] != rhs.CustStatus[i])
		{
			return false;
		}
	}
	return lhs.processStatus == rhs.processStatus;
}
bool operator!=(const NetworkState & lhs, const NetworkState & rhs)
{
	return !(lhs == rhs);
}
void NetworkState::TestInputOutput(std::string filename)
{
	std::cout << "testing input output labelledstatesample" << std::endl;

	NetworkState state({ 1,2,3 }, { 0,1,0 }, 4);
	std::cout << state;
	{
		std::cout << "writing to file \n";
		std::ofstream outputfile;
		outputfile.open(filename);
		outputfile << state;
	}
	{
		NetworkState instate{};
		std::cout << "reading from file \n";
		std::ifstream inputfile;
		inputfile.open(filename);
		inputfile >> instate;
		std::cout << instate;
	}
	std::cout << "testing complete" << std::endl;
}

std::size_t NetworkState::Hash() const
{//This is (only) needed for exact optimization. 
	std::size_t seed{ processStatus };
	for (size_t i = 0; i < Inventories.size(); i++)
	{
		seed ^= Inventories[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
	for (size_t i = 0;i < CustStatus.size();i++)
	{
		seed ^= CustStatus[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
	return seed;
}



std::ostream & operator<<(std::ostream & os, const NetworkState & state)
{
	os << state.Inventories.size() << "\t";
	for (size_t i = 0; i < state.Inventories.size(); i++)
	{
		os << state.Inventories[i] << "\t";
	}
	os << state.CustStatus.size() << "\t";
	for (size_t i = 0; i < state.CustStatus.size(); i++)
	{
		os << state.CustStatus[i] << "\t";
	}
	os << state.processStatus << "\t";
	os << std::endl;
	return os;
}

std::istream & operator>>(std::istream & is, NetworkState & state)
{
	{//Load Inventories
		size_t size;
		is >> size;
		std::vector<NetworkState::InvCount> inventories{};
		inventories.reserve(size);
		for (size_t i = 0; i < size; i++)
		{
			NetworkState::InvCount read;
			is >> read;
			inventories.push_back(read);
		}
		state.Inventories = inventories;
	}
	{//Load CustStatus
		size_t size;
		is >> size;
		std::vector<NetworkState::CustStatusOption> custStatuses{};
		custStatuses.reserve(size);
		for (size_t i = 0; i < size; i++)
		{
			NetworkState::CustStatusOption read;
			is >> read;
			custStatuses.push_back(read);
		}
		state.CustStatus = custStatuses;
	}	
	is >> state.processStatus;
	return is;
}

