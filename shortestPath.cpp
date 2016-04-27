#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <cmath>

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
	int index;

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
		index = 0;
		distance = (2^31);
		// *** distance = static_cast<int>(numeric_limits<char>::max());
	}
	// *** bool operator>(Node& otherNode)
	// *** {
	// *** 	return (this->distance > otherNode.distance) ? true : false;
	// *** }
};

//Start of MinHeapPQ

class MinHeapPQ
{
private:
	int numElements;
	int PQCapacity;

public:
	Node** PQ;
	MinHeapPQ();
	MinHeapPQ(int);
	//~MinHeapPQ();
	//Don't use this
	void heapify(Node*, int);
	void resizePQ();
	int size();
	void testPrint();
	void push(Node*);
	void pop();
	void testEmptying();
	Node* top();
	bool empty();

};

//MinHeapPQ functions/methods
MinHeapPQ::MinHeapPQ()
{
	numElements = 0;
	PQ = new Node*[11];
	PQCapacity = 10;
}

MinHeapPQ::MinHeapPQ(int arraySize)
{
	numElements = 0;
	PQCapacity = arraySize;
	PQ = new Node*[arraySize+1];
	//TEST
	//cout << "Initialize before heapify finished" << std::endl;

	//heapify(PQ, numElements);

	//TEST
	//cout << "Initialize after heapify finished" << std::endl;
}

//probably won't use this function
void MinHeapPQ::heapify(Node* PQH, int numElementsH)
{
	for(int i = numElementsH/2; i >= 1; i--)
	{
		Node rootElement = PQH[i];
		int child = 2 * i;

		while(child <= numElementsH)
		{
			if(child < numElementsH && PQH[child].distance < PQH[child+1].distance)
			{
				child++;
			}

			if(rootElement.distance >= PQH[child].distance)
			{
				break;
			}

			PQH[child/2] = PQH[child];
			child *= 2;
		}
		PQH[child/2] = rootElement;
	}
	//TEST
	std::cout << "Testing array after heapify: " << std::endl;
	for(int j = 1; j <= numElementsH; j++)
	{
		std::cout << PQH[j].distance << " " << std::endl;
	}

	return;
}

void MinHeapPQ::resizePQ()
{
	//TEST
	//cout << "Entering resize function" << std::endl;


	if(numElements == PQCapacity)
	{
		//TESTING
		//cout << "Increasing array capacity" << std::endl;


		Node** temp = PQ;
		PQ = new Node*[PQCapacity * 2];
		for(int i = 1; i <= numElements; i++)
		{
			PQ[i] = temp[i];
		}
		PQCapacity = PQCapacity * 2;
		delete temp;


		//TESTING
		//cout << "Our new array capacity is: " << PQCapacity << std::endl;
	}
	
	else if(numElements <= PQCapacity/4 && PQCapacity < 10)
	{
		//TESTING
		//cout << "Reducing array capacity from current capacity: " << PQCapacity << std::endl;


		Node** temp = PQ;
		PQ = new Node*[(PQCapacity/2)+1];
		for(int i = 1; i <= numElements; i++)
		{
			PQ[i] = temp[i];
		}
		PQCapacity = (PQCapacity / 2) + 1;
		delete temp;

		//TESTING
		//cout << "Our new array capacity: " << PQCapacity << std::endl;
	}
	//TEST
	//cout << "Exiting resize function" << std::endl;


	return;
}

int MinHeapPQ::size()
{
	return numElements;
}

void MinHeapPQ::push(Node* element)
{
	//TEST
	std::cout << "Entering Push function" << std::endl;


	PQ[numElements+1] = element;
	element->index = numElements + 1;
	numElements++;
	//resizePQ();
	int temp = numElements;
	Node* tempEle;
	//TEST
	//cout << "numElements is currently: " << numElements << std::endl;



	for (int i = 1; (pow(2, i)-1) < numElements; i++)
	{
		//TEST
		//cout << "i is currently: " << i << std::endl;
		//TEST
		std::cout << "Current iteration value: <" << (pow(2, i)-1) << ">." << std::endl;
		std::cout << "Current numElements: <" << numElements << ">." << std::endl;

		if(PQ[temp]->distance < PQ[temp/2]->distance)
		{
			PQ[temp/2]->index = PQ[temp]->index;
			PQ[temp]->index = PQ[temp]->index/2;
			PQ[temp] = PQ[temp/2];
			PQ[temp/2] = element;
		}
		temp = temp/2;
	}
	//TEST
	std::cout << "Exiting Push function" << std::endl;

	//TEST
	

	return;
}

void MinHeapPQ::pop()
{
	//TEST
	//cout << "Entered Pop function" << std::endl;



	if(numElements == 0)
	{
		std::cerr << "ERROR: Attempted pop from an empty queue" << std::endl;
		return;
	}
	//T poppedElement = PQ[1];
	//TEST
	//cout << "Deletion of PQ[1] successful. Num elements before decrement: " << numElements << ". Priority of popped element: " << poppedElement.distance << std::endl;


	Node* lastElement = PQ[numElements--];
	//TEST
	//cout << "NumElements after decrement: " << numElements << std::endl;
	int currentNode = 1;
	int child = 2;
	while(child <= numElements)
	{
		//TEST
		//cout << "entered while loop" << std::endl;


		if(child < numElements && PQ[child]->distance > PQ[child+1]->distance)
		{
			child++;
			//TEST
			//cout << "Incremented child" << std::endl;
		}
		if(lastElement->distance <= PQ[child]->distance)
		{
			break;
		}
		//TEST
		//cout << "Attempting assignment to node in while loop" << std::endl;

		PQ[currentNode] = PQ[child];
		PQ[child]->index = PQ[child]->index/2;
		currentNode = child;
		child *= 2;

		//TEST
		//cout << "Exiting while loop" << std::endl;
	}
	//TEST
	//cout << "currentNode is: " << currentNode << " and PQ[2] is " << PQ[2].distance << std::endl;
	//cout << "last element priority is: " << lastElement.distance << std::endl;


	PQ[currentNode] = lastElement;
	PQ[currentNode]->index = currentNode;
	//TEST
	//cout << "CurrentNodeValue: " << currentNode << std::endl;

	//TEST
	//cout << "Testing what's in CurrentNode: " << PQ[currentNode]->distance << std::endl;
	//cout << "Testing index of CurrentNode: " << PQ[currentNode]->index << std::endl;
	//resizePQ();

	//TEST
	//cout << "Exiting pop function" << std::endl;


	return;
}

bool MinHeapPQ::empty()
{
	if(numElements == 0)
	{
		return true;
	}
	return false;
}

Node* MinHeapPQ::top()
{
	if(empty())
	{
		std::cerr << "Error: Queue empty" << std::endl;
		Node* element = new Node("Error", 1, 1);
		return element;
	}
	//cout << PQ[1].distance << std::endl;
	return PQ[1];
}

void MinHeapPQ::testPrint()
{
	for (int i = 1; i <= numElements; ++i)
	{
		std::cout << PQ[i]->charm << ", " << PQ[i]->distance << std::endl;
	}
	std::cout << std::endl;
}

void MinHeapPQ::testEmptying()
{
	int size = numElements;
	for (int i = 0; i < size; ++i)
	{
		std::cout << top()->charm << ", " << top()->distance << std::endl;
		pop();
	}
	return;
}

//End of minHeap

//Start of Stack

template<typename T>
class Stack
{
private:
	int currentSize;
	int capacity;
	T* array;
public:
	Stack();
	Stack(int);
	bool empty();
	T top();
	T pop();
	void push(T);
	int size();
	int getCapacity();
	void resize();
};

//Stack Methods

//Probably don't want to use this, opt for the other constructor.
template<typename T>
Stack<T>::Stack()
{
	currentSize = 0;
	capacity = 8;
	array = new T[8];
}

//This constructor has unneeded space overhead but it cuts down on any resizing.
//This is because of in the worst case (which is very much an edge case), the graph is a
//linked list with source being the head and destination being the tail. In this scenario,
//the shortest path is the path of traversing every node, which is the maximum amount of
//pushes that would ever have to happen. Thus, the array would never need to resize.
template<typename T>
Stack<T>::Stack(int numRealms)
{
	currentSize = 0;
	capacity = numRealms;
	array = new T[numRealms];
}

template<typename T>
bool Stack<T>::empty()
{
	if(currentSize == 0)
	{
		return true;
	}
	return false;
}

template<typename T>
T Stack<T>::top()
{
	return array[currentSize - 1];
}

template<typename T>
T Stack<T>::pop()
{
	T element = array[currentSize - 1];
	if(currentSize < capacity/2 && capacity > 8)
	{
		resize();
	}
	currentSize--;
	return element;
}

template<typename T>
void Stack<T>::push(T element)
{
	array[currentSize] = element;
	currentSize++;
	if(currentSize == capacity)
	{
		resize();
	}
}

template<typename T>
int Stack<T>::size()
{
	return currentSize;
}

template<typename T>
int Stack<T>::getCapacity()
{
	return capacity;
}

//Resize using 2/1 ratio
template<typename T>
void Stack<T>::resize()
{
	if(currentSize == capacity)
	{
		T* temp = new T[capacity * 2];
		for (int i = 0; i < capacity; ++i)
		{
			temp[i] = array[i];
		}
		delete array;
		array = temp;
		capacity = capacity * 2;
	}
	else
	{
		T* temp = new T[capacity/2];
		for (int i = 0; i < currentSize; ++i)
		{
			temp[i] = array[i];
		}
		delete array;
		array = temp;
		capacity = capacity/2;
	}
}

//End of Stack methods

template<typename T>
class Graph
{
	T** arrayOfNodes;
	int size;
	T *startingNode, *destinationNode;
public:
	Graph(int s, T** ourNodes, std::string st, std::string en)
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
			if (arrayOfNodes[i]->charm == st)
			{
				startingNode = arrayOfNodes[i];
				startingNode->index = i;
			}
			if (arrayOfNodes[i]->charm == en)
			{
				destinationNode = arrayOfNodes[i];
				destinationNode->index = i;
			}
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

		//std::cout << startingNode->charm << std::endl;
		//std::cout << startingNode->arrayOfNodePtrs[0]->charm << std::endl;
		//std::cout << startingNode->arrayOfNodePtrs[1]->charm << std::endl;
		//std::cout << startingNode->arrayOfNodePtrs[1]->arrayOfNodePtrs[1]->charm << std::endl;

		//shortestPath(startingNode, destinationNode, startingNode->index);
		//resetGraph();
		//shortestPath(destinationNode,startingNode, destinationNode->index);
	}

	void shortestPath(T*, T*, int);
	void changeIndex(T*, MinHeapPQ &);
	void resetGraph();
};

template<typename T>
void Graph<T>::changeIndex(T* nodeCh, MinHeapPQ &myHeap)
{
	std::cout << "gets to changeIndex\n";
	while((nodeCh->index) > 0)
	{
		if(myHeap.PQ[nodeCh->index/2]->distance > nodeCh->distance)
		{
			Node* parent = myHeap.PQ[nodeCh->index/2];
			myHeap.PQ[nodeCh->index/2] = nodeCh;
			myHeap.PQ[nodeCh->index] = parent;
			parent->index = nodeCh->index;
			nodeCh->index = nodeCh->index/2;
		}
		else
		{
			break;
		}
	}
	std::cout << "leaves changeIndex\n";
}

template<typename T>
void Graph<T>::shortestPath(T* source, T* destination, int startPos)
{
	//MPQ to manage Dijkstra's. Priority based on distance variable.
	MinHeapPQ minHeap;
	Stack<Node*> pathStorage(size);
	arrayOfNodes[startPos]->distance = 0;
	arrayOfNodes[startPos]->index = 1;
	int startFound = 0;
	//Fill the queue with every node. All except source has priority MAX
	for (int i = 0; i < size; ++i)
	{
		if(source->charm == arrayOfNodes[i]->charm)
		{
			startFound = 1;
		}
		minHeap.push(arrayOfNodes[i]);
		//This is necessary because source is stored in index 1 initially and i starts
		//at 0. When we hit the source node, we continue into the array, but we haven't
		//added a node for that iteration since source is already in the heap
		arrayOfNodes[i]->index = i + 2 - startFound;
	}
	//run until all nodes checked
	Node* target = NULL;
	std::cout << "Starts main while loop in shortestPath\n";
	while(!minHeap.empty())
	{
		Node* temp = minHeap.top();
		if(temp == destination)
		{
			temp->visited = true;
			target = temp;
			break;
		}
		//std::cout << "Starts nested for loop\n";
		for(int i = 0; i < temp->numAdj; i++)
		{
			//Don't need to consider previously visited nodes
			if(temp->arrayOfNodePtrs[i]->visited)
			{
				continue;
			}
			//If the current distance to observed node is larger than current node distance + # of trans, replace
			if(temp->arrayOfNodePtrs[i]->distance > (temp->distance + trans(temp->charm, temp->arrayOfNodePtrs[i]->charm)))
			{
				//Need to make sure this keeps the integrity of the queue so that the algorithm
				//doesn't break. Probably need to make my own Queue.
				temp->arrayOfNodePtrs[i]->distance = temp->distance + trans(temp->charm, temp->arrayOfNodePtrs[i]->charm);
				changeIndex(temp->arrayOfNodePtrs[i], minHeap);
				temp->arrayOfNodePtrs[i]->parent = temp;
			}
		}
		//std::cout << "Ends nested for loop\n";
		temp->visited = true;
		minHeap.pop();
	}
	std::cout << "Exits main while loop in shortestPath\n";
	if(target == NULL)
	{
		std::cout << "IMPOSSIBLE" << std::endl;
		return;
	}
	else
	{
		Node* temp = target;
		int numChanges = 0;
		int numGems = 0;
		while(temp->parent != NULL)
		{
			pathStorage.push(temp);
			temp = temp->parent;
		}
		//need to do this since popping decreases size
		int iterateLimit = pathStorage.size();
		Node* temp2;
		for (int i = 0; i < iterateLimit; ++i)
		{
			temp2 = pathStorage.pop();
			if(!pathStorage.empty())
			{
				numChanges += trans(temp2->charm, pathStorage.top()->charm);
				numGems += costTrans(temp2->magi, temp2->magiSize, trans(temp2->charm, pathStorage.top()->charm));
			}
		}
		std::cout << numChanges << " " << numGems << std::endl;
	}
}

template<typename T>
void Graph<T>::resetGraph()
{
	for (int i = 0; i < size; ++i)
	{
		arrayOfNodes[i]->distance = (2^31);
		arrayOfNodes[i]->visited = false;
		arrayOfNodes[i]->parent = NULL;
		arrayOfNodes[i]->index = 0;
	}
}

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
		//nodePtrArray[i]->index = i;
	}

	// Beginning and ending charm
	std::string begin, end;
	std::cin >> begin, end;
	Node* source;
	Node* des;
	int sourceStarter = -1;
	int desStarter = -1;
	for (int i = 0; i < worldSize; ++i)
	{
		if(nodePtrArray[i]->charm == begin)
		{
			source = nodePtrArray[i];
			sourceStarter = i;
		}
		else if(nodePtrArray[i]->charm == end)
		{
			des = nodePtrArray[i];
			desStarter = i;
		}
	}
	Graph<Node> mainGraph(worldSize, nodePtrArray, begin, end);
	mainGraph.shortestPath(source, des, sourceStarter);
	mainGraph.resetGraph();
	mainGraph.shortestPath(des, source, desStarter);
	return 0;
}

/* 
Notable change I made to the Stack structure:
	changed the push function so that it takes a T element not T* element and initiated the stack as a Node*
	don't think it should have any major difference
Also there were a lot of instances where pointers were trying to access its variables with periods instead of
	arrows so I changed them. I don't think those would have caused a problem but it's a possibilty I suppose.
I couldn't tell what the indeces for the nodes should've been initalized as so I initialized them as their index
	in the node array in the graph structure. If you wanted them initialized to 0 you can just get rid of the
	code at line 779 (nodePtrArray[i]->index = i;). It doesn't seem to make a difference in execution completion
	though.
After a bit of bug hunting, I found that the main while loop in shortestPath is stuck in an infinite loop,
	although it's currently 4:20 am (not even joking) and I'm not quite sure how to fix it. If you point me
	in the right direction I can probably fix it if my code is too hard to read or you don't have the time to 
	fix it. I'm assuming however, the loop is not getting to the destination node.
*/