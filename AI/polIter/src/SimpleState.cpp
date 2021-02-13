#include "SimpleState.h"


std::size_t SimpleState::Hash() const
{//For exact optimization a decent hash is needed. 
	std::size_t seed{ 123 };
	seed ^= seed + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	return seed;
}


SimpleState::SimpleState(SimpleState::Status status): status{status}
{
}

SimpleState::SimpleState() : status{ 0 }
{

}

bool operator==(const SimpleState & lhs, const SimpleState & rhs)
{
	return lhs.status==rhs.status;
}

bool operator!=(const SimpleState & lhs, const SimpleState & rhs)
{
	return !(lhs==rhs);
}



std::ostream & operator<<(std::ostream & os, const SimpleState & state)
{
	os << static_cast<int>(state.status)<< "\t";
	return os;
}

std::istream & operator>>(std::istream & is, SimpleState & state)
{
	int i{};
	is >> i;
	state.status = static_cast<SimpleState::Status>(i);
	return is;
}
