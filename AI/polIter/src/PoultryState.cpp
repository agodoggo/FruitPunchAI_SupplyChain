#include <vector>
#include <iostream>
#include <fstream>
#include "SimpleState.h"
#include "PoultryState.h"

PoultryState::PoultryState(FIFOQueue<Weight> ArrivingWeights, std::vector<Weight> BinWeights, ProcessStatus processStatus) :
	ArrivingWeights{ ArrivingWeights }, BinWeights{ BinWeights }, processStatus{ processStatus }
{
}

PoultryState::PoultryState(std::vector<Weight> arrivingweightsvec, std::vector<Weight> BinWeights, ProcessStatus processStatus):
	ArrivingWeights{arrivingweightsvec.size()}, BinWeights{ BinWeights }, processStatus{ processStatus }
{
	for (auto item : arrivingweightsvec)
	{
		ArrivingWeights.push_back(item);
	}
}

PoultryState::PoultryState() : ArrivingWeights{}, BinWeights{}, processStatus{ 0 }
{
}

bool operator==(const PoultryState& lhs, const PoultryState& rhs)
{
	if (lhs.ArrivingWeights != rhs.ArrivingWeights)
	{
		return false;
	}
	for (size_t i = 0; i < lhs.BinWeights.size(); i++)
	{
		if (lhs.BinWeights[i] != rhs.BinWeights[i])
		{
			return false;
		}
	}
	return lhs.processStatus == rhs.processStatus;
}
bool operator!=(const PoultryState& lhs, const PoultryState& rhs)
{
	return !(lhs == rhs);
}

std::size_t PoultryState::Hash() const
{//This is (only) needed for exact optimization. 
	std::size_t seed{ processStatus };
	for (auto item : ArrivingWeights)
	{
		seed ^= item + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
	for (size_t item: BinWeights)
	{
		seed ^= item + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
	return seed;
}


std::ostream& operator<<(std::ostream& os, const PoultryState& state)
{
	os << state.ArrivingWeights;
	os << state.BinWeights.size() << "\t";
	for (size_t i = 0; i < state.BinWeights.size(); i++)
	{
		os << state.BinWeights[i] << "\t";
	}
	os << state.processStatus << "\t";
	os << std::endl;
	return os;
}

std::istream& operator>>(std::istream& is, PoultryState& state)
{
	using Weight = typename PoultryState::Weight;
	{//Load Arriving Weights
		FIFOQueue<Weight> queue;
		is >> queue;
		state.ArrivingWeights = queue;
	}
	{//Load bin weights
		size_t size;
		is >> size;
		std::vector<Weight> binWeights{};
		binWeights.reserve(size);
		for (size_t i = 0; i < size; i++)
		{
			Weight read;
			is >> read;
			binWeights.push_back(read);
		}
		state.BinWeights = binWeights;
	}
	is >> state.processStatus;
	return is;
}