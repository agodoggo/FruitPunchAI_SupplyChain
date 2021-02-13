#include <vector>
#include <iostream>
#include <fstream>

#pragma once
class TravelState
{
public:

	//Explicitly request the assignment operator and copy constructor. 
	TravelState& operator=(const TravelState&) = default;
	TravelState(const TravelState&) = default;

	//Assigns number to each state. If states are same, same number must be assigned. 
	std::size_t Hash() const;
	friend bool operator==(const TravelState& lhs, const TravelState& rhs);
	friend bool operator!=(const TravelState& lhs, const TravelState& rhs);


	static constexpr bool CanSerialize = true;
	friend std::ostream& operator<<(std::ostream& os, const TravelState& state);
	friend std::istream& operator>>(std::istream& is, TravelState& state);


	std::vector <size_t> ComponentStates;

	size_t LocationsFSE;

	size_t RemainingTimeFSE;

	size_t Status;
	
	TravelState(std::vector <size_t> componentstates, size_t locFSE, size_t remFSE, size_t status);

	TravelState();
};
