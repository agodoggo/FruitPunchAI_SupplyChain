#include <vector>
#include <iostream>
#include <fstream>

#pragma once
class SimpleState
{
public:

	//Explicitly request the assignment operator and copy constructor. 
	SimpleState & operator=(const SimpleState&) = default;
	SimpleState(const SimpleState&) = default;

	//Assigns number to each state. If states are same, same number must be assigned. 
	std::size_t Hash() const;
	friend bool operator==(const SimpleState& lhs, const SimpleState& rhs);
	friend bool operator!=(const SimpleState& lhs, const SimpleState& rhs);


	static constexpr bool CanSerialize = true;
	friend std::ostream& operator<<(std::ostream& os, const SimpleState& state);
	friend std::istream& operator>>(std::istream& is, SimpleState& state);


	enum class Status : int { AwaitEvent = 0, Actionable = 1, Actionable2 =2 };


	SimpleState(Status status);

	SimpleState();

	Status status;
};

