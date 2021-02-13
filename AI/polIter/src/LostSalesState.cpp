#include "LostSalesState.h"

void LostSalesState::UpdateTotalInv()
{
	TotalInv = 0;
	for(auto inv : StateVector)
	{
		TotalInv += inv;
	}
}

std::size_t LostSalesState::Hash() const
{
	std::size_t seed{ static_cast<size_t>( processStatus) };
	for (auto item : StateVector)
	{
		seed ^= item + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}	
	return seed;
}

LostSalesState::LostSalesState(FIFOQueue<size_t> StateVector, Status processStatus)
    :StateVector{ StateVector }, processStatus{processStatus}
{
	UpdateTotalInv();
}

LostSalesState::LostSalesState()
{
}

bool operator==(const LostSalesState& lhs, const LostSalesState& rhs)
{
	if (lhs.StateVector != rhs.StateVector)
	{
		return false;
	}
	
	return lhs.processStatus == rhs.processStatus;
}

bool operator!=(const LostSalesState& lhs, const LostSalesState& rhs)
{
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const LostSalesState& state)
{
	os << state.StateVector;
	os << static_cast<size_t>(state.processStatus) << "\t";
	return os;
}

std::istream& operator>>(std::istream& is, LostSalesState& state)
{
	{//Load Arriving Weights
		FIFOQueue<size_t> queue;
		is >> queue;
		state.StateVector = queue;
	}
	size_t status;
	is >> status;
	state.processStatus = static_cast<LostSalesState::Status>(status);
	state.UpdateTotalInv();
	return is;
}
