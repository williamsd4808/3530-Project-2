#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>

// I've marked tentative code with a comment followed by three stars (// *** 'code')
int trans(std::string charm1, std::string charm2){
	int j = charm1.size();
	int k = charm2.size();
	char c1[j];
	char c2[k];
	strcpy(c1,charm1.c_str());	//copy strings to corresponding char arrays
	strcpy(c2,charm2.c_str());


	int dist[j+1][k+1];
	for(int i=0; i<j+1; i++){
		dist[i][0]=i;
	}
	for(int i=0; i<k+1; i++){
		dist[0][i]=i;
	}

	for(int i=1;i<j+1;i++){
		for(int h=1;h<k+1;h++){
			if(c1[i-1] == c2[h-1]){
				dist[i][h] = dist[i-1][h-1];
			}
			else{	
				int x = dist[i-1][h]+1;
				int y = dist[i][h-1]+1;
				int z = dist[i-1][h-1]+1;
				if(x<=y&&x<=z){dist[i][h] = x;}
				else if(y<=x&&y<=z){dist[i][h] = y;}
				else{ dist[i][h] = z;}
			}
		}
	}
	
	return dist[j][k];
}

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

class Node
{
public:
	// Realm charm and boolean value for whether the realm has been visited in the path
	std::string charm;
	bool visited;
	// Pointer to the magi array that needs to be initialized elsewhere, magi array size, number of max transformations
	int* magi;
	int magiSize;
	int magiTrans;
	// Array of pointers to the connected realms, the size of the array, the array corresponding to the adjacent realms that contains
	// the number of incantations required to get there, and the array the holds the cost required to get there
	Node** arrayOfNodePtrs; // May or may not the final form of this pointer (pointer to an array of pointers)
	int numAdj;
	int* incantRequired;
	int* costRequired;
	// For Daniel
	Node* parent;
	int distance;

	// Constructor
	Node(std::string n, int worldSize, int numMagi)
	{
		charm = n;
		magi = new int[numMagi];
		arrayOfNodePtrs = new Node*[worldSize];
		incantRequired = new int [worldSize];
		numAdj = 0;
		visited = false;
		magiSize = numMagi;
		// *** distance = static_cast<int>(numeric_limits<char>::max());
	}
	// *** bool operator>(Node& otherNode)
	// *** {
	// *** 	return (this->distance > otherNode.distance) ? true : false;
	// *** }
};

template<typename T>
class Graph
{
	T** arrayOfNodes;
	int size;
public:
	Graph(int s, T** ourNodes)
	{
		size = s;
		arrayOfNodes = ourNodes;
		// Assigns the maximum number of transformations that each node can accomplish
		for (int i = 0; i < s; ++i)
		{
			arrayOfNodes[i]->magiTrans = maxTrans(arrayOfNodes[i]->magi, arrayOfNodes[i]->magiSize);
		}
		// Links each node to each other if they can be reached
		for (int i = 0; i < s; ++i)
		{
			for (int j = 0; j < s; ++j)
			{
				if (i != j)
				{
					std::cout << "i: " << i << ", j: " << j << std::endl;
					int temp = trans(arrayOfNodes[i]->charm, arrayOfNodes[j]->charm);
					std::cout << "Transformations required to get from " << arrayOfNodes[i]->charm <<" to " << arrayOfNodes[j]->charm << " is " << temp << std::endl;
					if (temp <= arrayOfNodes[i]->magiTrans)
					{
						std::cout << temp << " is <= than " << arrayOfNodes[i]->magiTrans << std::endl;
						arrayOfNodes[i]->arrayOfNodePtrs[arrayOfNodes[i]->numAdj] = arrayOfNodes[j];
						arrayOfNodes[i]->incantRequired[arrayOfNodes[i]->numAdj] = temp;
						++arrayOfNodes[i]->numAdj;
					}
				}
			}
		}
	}
};

int main()
{
	// Initializing the node space
	int worldSize;
	std::cin >> worldSize;
	Node **nodePtrArray = new Node*[worldSize];

	// Creating individual nodes and putting them into the node space
	std::string charm;
	int magiSize;
	for (int i = 0; i < worldSize; ++i)
	{
		std::cin >> charm;
		std::cin >> magiSize;
		nodePtrArray[i] = new Node(charm, worldSize, magiSize);
		for (int j = 0; j < magiSize; ++j)
		{
			std::cin >> nodePtrArray[i]->magi[j];
		}
		std::cout << nodePtrArray[i]->magiSize << std::endl;
	}

	// Beginning and ending charm
	std::string begin, end;
	std::cin >> begin, end;

	Graph<Node> mainGraph(worldSize, nodePtrArray);
	return 0;
}