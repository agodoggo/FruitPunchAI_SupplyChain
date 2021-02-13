#include "InfoAcqState.h"
#include <vector>



InfoAcqState::InfoAcqState(Status status, std::vector<int> ValueOfAttributes, size_t StatusModifier):
	status{ status }, ValueOfAttributes{ ValueOfAttributes }, StatusModifier{ StatusModifier }
{
}

InfoAcqState::InfoAcqState() : status{ 0 }, ValueOfAttributes{}, StatusModifier{ 0 }
{
}


std::size_t InfoAcqState::Hash() const
{//For exact optimization a decent hash is needed. 
	int seed{ static_cast<int>(status) };
	for (auto item : ValueOfAttributes)
	{
		seed ^= item + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
	seed ^= StatusModifier + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	return seed;
}

bool operator==(const InfoAcqState& lhs, const InfoAcqState& rhs)
{
	if (lhs.status != rhs.status)
	{
		return false;
	}
	for (size_t i = 0; i < lhs.ValueOfAttributes.size(); i++)
	{
		if (lhs.ValueOfAttributes[i] != rhs.ValueOfAttributes[i])
		{
			return false;
		}
	}
	return lhs.StatusModifier == rhs.StatusModifier;
}

bool operator!=(const InfoAcqState& lhs, const InfoAcqState& rhs)
{//entire state should be equal.. 
	return !(lhs==rhs);
}


//Should output entire state. 
std::ostream& operator<<(std::ostream& os, const InfoAcqState& state)
{
	os << state.ValueOfAttributes.size() << "\t";
	for (auto attr : state.ValueOfAttributes)
	{
		os << attr << "\t";
	}
	os << static_cast<int>(state.status) << "\t";
	os << int (state.StatusModifier) << "\t";
	return os;
}

std::istream& operator>>(std::istream& is, InfoAcqState& state)
{
	{
		size_t size;
		is >> size;
		std::vector<int> attributes;
		for (size_t i = 0; i < size; i++)
		{
			int read;
			is >> read;
			attributes.push_back(read);
		}
		state.ValueOfAttributes = attributes;
	}
	size_t i,j;
	is >> i;
	state.status = static_cast<InfoAcqState::Status>(i);
	is >> j;
	state.StatusModifier = static_cast<size_t>(j);
	return is;
}