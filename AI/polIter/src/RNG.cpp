#include "RNG.h"

size_t RNG::GenEvent(const std::vector<double>& ProbsPerEvent)
{
	double d = NextUniform();
	size_t event{ 0 };
	while (d > ProbsPerEvent[event])
	{
		d -= ProbsPerEvent[event++];
	}
	return event;
}
