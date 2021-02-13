#pragma once
#include<vector>
#include<array>
#include<iostream>


template <typename T>
class FIFOQueue
{
	using index = size_t;
	index first_item;
	index num_items;
	std::vector<T> items;




public:
	size_t Capacity() const
	{
		return items.size();
	}

	class const_iterator {
		index current;
		const FIFOQueue* queue_ptr;
	public:
		const_iterator(index current, const FIFOQueue* queue_ptr) : current(current), queue_ptr{ queue_ptr } {}

		bool operator!=(const_iterator& rhs) { return current != rhs.current; }
		const T& operator*() {
			if (current >= queue_ptr->Capacity())
			{
				return queue_ptr->items[current - queue_ptr->Capacity()];
			}
			return queue_ptr->items[current];
		}
		void operator++() {
			++current;
		}
	};

	const_iterator begin() const
	{
		return const_iterator(first_item, this);
	}

	const_iterator end() const
	{
		return const_iterator(first_item + num_items, this);
	}

	FIFOQueue() : FIFOQueue(0)
	{}



	FIFOQueue(size_t capacity) : first_item{ 0 }, num_items{ 0 }, items( capacity )
	{
		if (capacity * 2 > std::numeric_limits<index>::max())
		{//Note that because of the wraparound logic, we need that index can fit twice the size of the maximum capacity..
			std::string message = "cannot fit this capacity";
			std::cout << message << std::endl;
			throw message;
		}
	}

	void push_back(T item)
	{
		if (num_items == items.size())
		{
			std::string message = "Attempting to add an element to a FIFOQueue that is allready full. ";
			std::cout << message << std::endl;
			throw message;
		}
		index end = first_item + num_items;
		if (end >= items.size())
		{
			end -= items.size();
		}
		items[end] = item;
		num_items++;
	}

	T& back()
	{
		if (IsEmpty())
		{
			throw "Queue is empty";
		}
		index back = first_item + num_items -1;
		if (back >= items.size())
		{
			back -= items.size();
		}
		return items[back];

	}

	bool IsEmpty() const
	{
		return num_items == 0;
	}
	

	void pop_front()
	{
		items[first_item++] = 0;
		num_items--;
		if (first_item == items.size())
		{
			first_item = 0;
		}
	}
	T& front()
	{
		if (IsEmpty())
		{
			throw "Queue is empty";
		}
		return items[first_item];
	}
	const T& front() const
	{
		if (IsEmpty())
		{
			throw "Queue is empty";
		}
		return items[first_item];
	}



	friend bool operator==(const FIFOQueue& lhs, const FIFOQueue& rhs)
	{
		if (lhs.num_items != rhs.num_items)
		{
			return false;
		}
		auto lhsIter = lhs.begin();
		auto rhsIter = rhs.begin();
		auto lhsEnd = lhs.end();
		while (lhsIter != lhsEnd)
		{
			if (*(lhsIter) != *(rhsIter))
			{
				return false;
			}
			++lhsIter;
			++rhsIter;
		}
		return true;

	}

	friend bool operator!=(const FIFOQueue& lhs, const FIFOQueue& rhs)
	{
		return !(lhs == rhs);
	}

	friend std::ostream& operator<<(std::ostream& os, const FIFOQueue& queue)
	{
		os << queue.num_items << "\t";
		os << queue.Capacity() << "\t";
		for (index item : queue)
		{
			os << item << "\t";
		}
		return os;
	}

	friend std::istream& operator>>(std::istream& is, FIFOQueue& queue)
	{
		index num_items;
		is >> num_items;

		size_t capacity{ 0 };
		is >> capacity;
		queue.items = std::vector<T>(capacity);

		queue.num_items = 0;
		queue.first_item = 0;
		//Now add the items one by one. 
		for (index i = 0; i < num_items; i++)
		{
			T item;
			is >> item;
			queue.push_back(item);
		}
		return is;
	}




};

