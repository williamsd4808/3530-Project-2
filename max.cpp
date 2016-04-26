#include <iostream>

int maxTrans(int* magi, int size)
{
	// If there are no possible trasnformations for a magi
	if (size == 0)
	{
		return 0;
	}
	// trans is the maximum number of transformations a magi can make
	// streak is the number of transformations in a streak
	int trans = 1, streak;
	for (int i = 0; i < size-1; ++i)
	{
		// the minimum number of transformations 
		streak = 1;
		// Check the rest of the numbers in the array if it's the first element
		// or if the element before was greater than the current element
		if (i == 0 || magi[i-1] > magi[i])
		{
			// Iterate through the rest of the elements in the array after the current one
			for (int j = i; j < size-1; ++j)
			{
				// If the next element is greater than the current element, add one to the streak
				if (magi[j] < magi[j+1])
				{
					++streak;
				}
			}
		}
		// If the value of streak is greater than the current greatest number of transformations,
		// overwrite the maximum transformations variable
		if (streak > trans)
			trans = streak;
	}
	return trans;
}

int costTrans(int* magi, int size, int num)
{
	// Streak is the number of transformations in the current streak
	// Cost is the total cost of the current streak
	int streak, cost;
	for (int i = 0; i < size-1; ++i)
	{
		// Streak starts at the first element being observed and its cost is that element's cost
		streak = 1;
		cost = magi[i];
		// If we're only looking for a streak size of 1 return cost
		if (num == 1)
			return cost;
		// Check the rest of the array if we're starting on the first element or
		// if the element before is greater than the current element
		if (i == 0 || magi[i-1] > magi[i])
		{
			for (int j = i; j < size-1; ++j)
			{
				// If the next element is greater than the current element, increase the streak count and cost
				if (magi[j] < magi[j+1])
				{
					++streak;
					cost += magi[j+1];
					// If we found the desired streak length, then return the streak's current cost
					if (streak == num)
						return cost;
				}
			}
		}
	}
	return cost;
}

int main()
{
	int *max;
	int size;
	int desiredLength;
	std::cin >> size;
	max = new int[size];
	for (int i = 0; i < size; ++i)
	{
		std::cin >> max[i];
	}
	std::cin >> desiredLength;
	std::cout << maxTrans(max, size) << std::endl;
	std::cout << costTrans(max, size, desiredLength) << std::endl;
	return 0;
}