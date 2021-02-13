#include "SCGameState.h"



SCGameState::SCGameState(size_t DemandsRemaining, std::vector<size_t> InventoryPerLocation, Status ProcessStatus) :
	DemandsRemaining{ DemandsRemaining }, InventoryPerLocation{ InventoryPerLocation }, ProcessStatus{ ProcessStatus }, score{ 0.0 }
{

}

SCGameState::SCGameState()
{
	
}




std::ostream& operator<<(std::ostream& os, const SCGameState& state)
{
	os << state.score << "\t";
    os << state.DemandsRemaining << "\t";
	os << state.InventoryPerLocation.size() << "\t";
	for (size_t i = 0; i < state.InventoryPerLocation.size(); i++)
	{
		os << state.InventoryPerLocation[i] << "\t";
	}
	os << static_cast<size_t>( state.ProcessStatus) << "\t";
	return os;
}

std::istream& operator>>(std::istream& is, SCGameState& state)
{
	is >> state.score;
	is >> state.DemandsRemaining;
	size_t invlocs{ 0 };
	is >> invlocs;
	std::vector<size_t> InvPerLocation{};
	InvPerLocation.reserve(invlocs);
	for (size_t i = 0; i < invlocs; i++)
	{
		size_t inv;
		is >> inv;
		InvPerLocation.push_back(inv);
	}
	state.InventoryPerLocation = InvPerLocation;

	size_t status;
	is >> status;
	state.ProcessStatus = static_cast<SCGameState::Status>(status);
	return is;
}
