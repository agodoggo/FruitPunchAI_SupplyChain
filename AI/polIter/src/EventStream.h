#pragma once
#include<vector>
#include"RNG.h"

template <typename T>
class EventStream
{
public:
	static std::vector<T> GetFixedLengthStream(size_t length, RNG& rng, const std::vector<double>& EventProbs)
	{
		std::vector<T> events;
		events.reserve(length);
		for (size_t i = 0; i < length; i++)
		{
			events.push_back((T)(rng.GenEvent(EventProbs)));
		}
		return events;
	}

	


};


