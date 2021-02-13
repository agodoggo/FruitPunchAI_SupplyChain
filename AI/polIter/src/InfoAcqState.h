#include <vector>
#include <iostream>
#include <fstream>

#pragma once
class InfoAcqState
{
public:
	//

	enum class Status : int { AwaitAction = 0, AwaitAtrributeRetrieval = 1, FinalDecisionMade = 2 };
	Status status;
	std::vector<int> ValueOfAttributes;

	//Explicitly request the assignment operator and copy constructor. 
	InfoAcqState& operator=(const InfoAcqState&) = default;
	InfoAcqState(const InfoAcqState&) = default;


	size_t StatusModifier;


	InfoAcqState(Status status, std::vector<int> ValueOfAttributes, size_t StatusModifier);

	InfoAcqState();


	static constexpr bool CanSerialize = true;
	friend std::ostream& operator<<(std::ostream& os, const InfoAcqState& state);
	friend std::istream& operator>>(std::istream& is, InfoAcqState& state);


	//Assigns number to each state. If states are same, same number must be assigned. 
	std::size_t Hash() const;
	friend bool operator==(const InfoAcqState& lhs, const InfoAcqState& rhs);
	friend bool operator!=(const InfoAcqState& lhs, const InfoAcqState& rhs);


};

