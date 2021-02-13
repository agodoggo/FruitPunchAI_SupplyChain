#include "TravelState.h"


std::size_t TravelState::Hash() const
{//For exact optimization a decent hash is needed. 
	std::size_t seed{ 123 };
	seed ^= seed + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	return seed;
}


TravelState::TravelState(std::vector <size_t> componentstates, size_t locFSE, size_t remFSE, size_t status) :
	ComponentStates{ componentstates }, LocationsFSE{ locFSE }, RemainingTimeFSE{ remFSE }, Status{status}
{
}

TravelState::TravelState() : ComponentStates{}, LocationsFSE{ 0 }, RemainingTimeFSE{ 0 }, Status{ 0 }
{
}

bool operator==(const TravelState& lhs, const TravelState& rhs)
{
	for (size_t i = 0; i < lhs.ComponentStates.size(); i++)
	{
		if (lhs.ComponentStates[i] != rhs.ComponentStates[i])
		{
			return false;
		}
	}
	if (lhs.RemainingTimeFSE != rhs.RemainingTimeFSE)
	{
		return false;
	}
	if (lhs.LocationsFSE != rhs.LocationsFSE)
	{
		return false;
	}
	return (lhs.Status == rhs.Status);
}


bool operator!=(const TravelState& lhs, const TravelState& rhs)
{
	return !(lhs == rhs);
}



std::ostream& operator<<(std::ostream& os, const TravelState& state)
{
	for (auto attr : state.ComponentStates)
	{
		os << attr << "\t";
	}
	os << static_cast<int>(state.LocationsFSE) << "\t";
	os << int(state.RemainingTimeFSE) << "\t";
	os << int(state.Status) << "\t";
	return os;
}

std::istream& operator>>(std::istream& is, TravelState& state)
{
	int i{};
	is >> i;
	//state.status = static_cast<TravelState::Status>(i);
	return is;
}
