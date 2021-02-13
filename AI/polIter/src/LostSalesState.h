#pragma once
#include "FIFOQueue.h"
class LostSalesState
{
public:
	enum class Status : size_t { AwaitOrder =0, AwaitDemand =1 };


	/// <summary>
	/// We follow notation in Zipkin (2008) Old and new methods for lost-sales inventory systems. This
    /// is a FIFOqueue because it makes updates more efficient. 
	/// </summary>
	FIFOQueue<size_t> StateVector;

	size_t TotalInv;

	void UpdateTotalInv();
	
	/// <summary>
	/// The process status
	/// </summary>
	Status processStatus;


	//Explicitly request the assignment operator and copy constructor. 
	LostSalesState& operator=(const LostSalesState&) = default;
	LostSalesState(const LostSalesState&) = default;

	//Assigns number to each state. If states are same, same number must be assigned. 
	std::size_t Hash() const;
	friend bool operator==(const LostSalesState & lhs, const LostSalesState & rhs);
	friend bool operator!=(const LostSalesState & lhs, const LostSalesState & rhs);



	LostSalesState(FIFOQueue<size_t> StateVector, Status processStatus);
	
	LostSalesState();


	static constexpr bool CanSerialize = true;
	friend std::ostream& operator<<(std::ostream& os, const LostSalesState& state);
	friend std::istream& operator>>(std::istream& is, LostSalesState& state);


};

