#pragma once
#include <vector>
#include <type_traits>
#include <stdexcept>


template <typename T>
class CartesianProduct
{
private:
	std::vector<std::vector<T>> Sets;
	std::vector<size_t> CurIter;
	std::vector<T> CurValues;
	bool hasNext;
	
public:
	
	size_t NumStates()
	{
		size_t numStates{ 1 };
		for (size_t i = 0; i < Sets.size(); i++)
		{
			numStates *= Sets[i].size();
		}
		return numStates;
	}

	static constexpr bool IsValid()
	{
		if (std::is_same<T, size_t>::value
			|| std::is_same<T, int>::value
			|| std::is_same<T, unsigned>::value			
			|| std::is_same<T, short>::value
			|| std::is_same<T, unsigned short>::value
			|| std::is_same<T, long>::value
			|| std::is_same<T, unsigned long>::value
			|| std::is_same<T, unsigned long long>::value
			|| std::is_same<T, long long>::value
			|| std::is_same<T, char>::value
			|| std::is_same<T, unsigned char>::value
			|| std::is_same<T, double>::value
			|| std::is_same<T, float>::value
			)
		{
			return true;
		}
		return false;
	}
	
	

	CartesianProduct(std::vector<std::vector<T>> Sets) : Sets{ Sets }, CurIter(Sets.size(), 0), CurValues(Sets.size())
	{
		hasNext = NumStates() > 0;
	}

	CartesianProduct(std::vector<size_t> SetMaxLevels) : Sets(SetMaxLevels.size(), std::vector<T>()), CurIter(SetMaxLevels.size(), 0), CurValues(SetMaxLevels.size())
	{
		static_assert(IsValid(), "Constructor with MaxLevels can only be used with T a numeric type, e.g. int, size_t, unsigned, double, float, long");
		size_t iter{ 0 };
		for (size_t max : SetMaxLevels)
		{			
			Sets[iter].reserve(max+1);
			for (size_t i = 0; i <= max; i++)
			{
				//Here, we convert from size_t. 
				Sets[iter].push_back((T)(i));
			}
			iter++;
		}
		hasNext = NumStates() > 0;

	}

	bool HasNext()
	{
		return hasNext;
	}

	std::vector<T>& GetNext()
	{
		//Create element based on current status of CurVals
		for (size_t i = 0; i < CurIter.size(); i++)
		{
			CurValues[i] = Sets[i][CurIter[i]];
		}
		//update status of CurVals;
		size_t digit = 0;//zero unsigned int
		while (digit < CurIter.size() &&
			++CurIter[digit] == Sets[digit].size())
		{
			CurIter[digit++] = 0;
		}
		if (digit == CurIter.size())
		{
			hasNext = false;
		}
		return CurValues;
	}

	void Reset()
	{
		hasNext = NumStates() > 0;
		for (size_t i = 0; i < CurIter.size(); i++)
		{
			CurIter[i] = 0;
		}
	}

};

