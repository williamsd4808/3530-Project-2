#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <limits>
#include <math.h>

#include <cstring>
#include <stdio.h>

using namespace std;

//I pretty much exclusively use camel case, sorry snake case users.

int trans(string, string);
int cost(int*, int);
int trans(string, string);
// Basic structures for me to create a dummy graph to work off of. 
// The graph will be undirected and weighted. Weights seem like they would have to be calculated
// based off of transformations needed from source string to target string.
// Need a queue for Dijkstra's. Min priority.
// Probably will want a stack to manage shortest path printing.

class Node
{
public:
	//Some of the following are just public for convenience
	string name;
	//Not sure if this is where I should put this. TBD
	int distance;
	int numAdj;
	int index;
	//Again, not sure if this is where I want this functionality.
	bool visited;
	//This probably shouldn't be Node**. It should probably be an Edge* which
	//would have a member variable of Node*
	Node** arrayOfNodePtrs;
	int* magi;
	//This seems to be a necessity for Dijkstra's. No one else should have to 
	//use or worry about this pointer, so just leave this one for me.
	Node* parent;
	Node();
	Node(string, int, int);
	void setMagi();
	bool operator>(Node);
};

//Would probably never use this constructor
Node::Node()
{
	name = "";
	arrayOfNodePtrs = new Node*[8];
	magi = new int[8];
	visited = false;
	distance = (2^31);
	parent = NULL;
	index = 0;
}

Node::Node(string n, int worldSize, int numMagi)
{
	name = n;
	arrayOfNodePtrs = new Node*[worldSize];
	visited = false;
	distance = (2^31);
	magi = new int[numMagi];
	index = 0;
}

//This overloaded operator would be useful if we keep track of distance within the node.
//Not a bad approach I think since we will have to revisit every node again
//anyway in order to set visited = false. While doing so, we can just set
// distance = static_cast< int >(numeric_limits< char >::max());.

//The main issue with this is the case where destination is unreachable. In this
//case, our code has a distance to destination implying that there is a path.
//This means that we wil have to base path validation off of parent node.
//Not an issue, just logically strange to have a numeric distance for an
//inexistant path.
bool Node::operator>(Node otherNode)
{
	return (this->distance > otherNode.distance) ? true : false;
}

//End Of Node

//May or may not use this class. This is essentially the custom pointer class.
//This may be where we need to have 
class Edge
{
public:
	//Only public ATM for convenience
	Node* ptr;
};

//End of Edge

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
	void push(T*);
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
void Stack<T>::push(T* element)
{
	array[currentSize] = *element;
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
	//cout << "Initialize before heapify finished" << endl;

	//heapify(PQ, numElements);

	//TEST
	//cout << "Initialize after heapify finished" << endl;
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
	cout << "Testing array after heapify: " << endl;
	for(int j = 1; j <= numElementsH; j++)
	{
		cout << PQH[j].distance << " " << endl;
	}

	return;
}

void MinHeapPQ::resizePQ()
{
	//TEST
	//cout << "Entering resize function" << endl;


	if(numElements == PQCapacity)
	{
		//TESTING
		//cout << "Increasing array capacity" << endl;


		Node** temp = PQ;
		PQ = new Node*[PQCapacity * 2];
		for(int i = 1; i <= numElements; i++)
		{
			PQ[i] = temp[i];
		}
		PQCapacity = PQCapacity * 2;
		delete temp;


		//TESTING
		//cout << "Our new array capacity is: " << PQCapacity << endl;
	}
	
	else if(numElements <= PQCapacity/4 && PQCapacity < 10)
	{
		//TESTING
		//cout << "Reducing array capacity from current capacity: " << PQCapacity << endl;


		Node** temp = PQ;
		PQ = new Node*[(PQCapacity/2)+1];
		for(int i = 1; i <= numElements; i++)
		{
			PQ[i] = temp[i];
		}
		PQCapacity = (PQCapacity / 2) + 1;
		delete temp;

		//TESTING
		//cout << "Our new array capacity: " << PQCapacity << endl;
	}
	//TEST
	//cout << "Exiting resize function" << endl;


	return;
}

int MinHeapPQ::size()
{
	return numElements;
}

void MinHeapPQ::push(Node* element)
{
	//TEST
	cout << "Entering Push function" << endl;


	PQ[numElements+1] = element;
	element->index = numElements + 1;
	numElements++;
	//resizePQ();
	int temp = numElements;
	Node* tempEle;
	//TEST
	//cout << "numElements is currently: " << numElements << endl;



	for (int i = 1; (pow(2, i)-1) < numElements; i++)
	{
		//TEST
		//cout << "i is currently: " << i << endl;
		//TEST
		cout << "Current iteration value: <" << (pow(2, i)-1) << ">." << endl;
		cout << "Current numElements: <" << numElements << ">." << endl;

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
	cout << "Exiting Push function" << endl;

	//TEST
	

	return;
}

void MinHeapPQ::pop()
{
	//TEST
	//cout << "Entered Pop function" << endl;



	if(numElements == 0)
	{
		cerr << "ERROR: Attempted pop from an empty queue" << endl;
		return;
	}
	//T poppedElement = PQ[1];
	//TEST
	//cout << "Deletion of PQ[1] successful. Num elements before decrement: " << numElements << ". Priority of popped element: " << poppedElement.distance << endl;


	Node* lastElement = PQ[numElements--];
	//TEST
	//cout << "NumElements after decrement: " << numElements << endl;
	int currentNode = 1;
	int child = 2;
	while(child <= numElements)
	{
		//TEST
		//cout << "entered while loop" << endl;


		if(child < numElements && PQ[child]->distance > PQ[child+1]->distance)
		{
			child++;
			//TEST
			//cout << "Incremented child" << endl;
		}
		if(lastElement->distance <= PQ[child]->distance)
		{
			break;
		}
		//TEST
		//cout << "Attempting assignment to node in while loop" << endl;

		PQ[currentNode] = PQ[child];
		PQ[child]->index = PQ[child]->index/2;
		currentNode = child;
		child *= 2;

		//TEST
		//cout << "Exiting while loop" << endl;
	}
	//TEST
	//cout << "currentNode is: " << currentNode << " and PQ[2] is " << PQ[2].distance << endl;
	//cout << "last element priority is: " << lastElement.distance << endl;


	PQ[currentNode] = lastElement;
	PQ[currentNode]->index = currentNode;
	//TEST
	//cout << "CurrentNodeValue: " << currentNode << endl;

	//TEST
	//cout << "Testing what's in CurrentNode: " << PQ[currentNode]->distance << endl;
	//cout << "Testing index of CurrentNode: " << PQ[currentNode]->index << endl;
	//resizePQ();

	//TEST
	//cout << "Exiting pop function" << endl;


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
		cerr << "Error: Queue empty" << endl;
		Node* element = new Node("Error", 1, 1);
		return element;
	}
	//cout << PQ[1].distance << endl;
	return PQ[1];
}

void MinHeapPQ::testPrint()
{
	for (int i = 1; i <= numElements; ++i)
	{
		cout << PQ[i]->name << ", " << PQ[i]->distance << endl;
	}
	cout << endl;
}

void MinHeapPQ::testEmptying()
{
	int size = numElements;
	for (int i = 0; i < size; ++i)
	{
		cout << top()->name << ", " << top()->distance << endl;
		pop();
	}
	return;
}

//End of minHeap


//T must have a valid > comparator to use shortestPath
template<typename T>
class Graph
{
private:
	T* arrayOfNodes;
	int size;
public:
	Graph();
	Graph(int, T*);
	void addNode(T);
	void shortestPath(T*, T*, int);
	void changeIndex(T*, MinHeapPQ &);
	void resetGraph();
};

//Definately don't want to use this constructor
template<typename T>
Graph<T>::Graph()
{
	arrayOfNodes = NULL;
	size = 0;
}

//Not exactly sure how I want to initialize the graph yet.
template<typename T>
Graph<T>::Graph(int sz, T* ourNodes)
{
	size = sz;
	arrayOfNodes = ourNodes;
}

//This will be what keeps the integrity of the min heap
template<typename T>
void Graph<T>::changeIndex(T* nodeCh, MinHeapPQ &myHeap)
{
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
}
//First, we take in the undirected graph. That, or this will be a function of the graph which 
//therefore would have access to its member variables.


//Actual point of this file
//May just do the prints from here so I don't have to store this anywhere
// and return an array or the stack
// We will want to call this function in main, followed by a call to reset, followed by
// another call to this function with reversed source and destination.
template<typename T>
void Graph<T>::shortestPath(T* source, T* destination, int startPos)
{
	//MPQ to manage Dijkstra's. Priority based on distance variable.
	MinHeapPQ minHeap;
	Stack<Node> pathStorage(size);
	arrayOfNodes[startPos].distance = 0;
	int startFound = 0;
	//Fill the queue with every node. All except source has priority MAX
	for (int i = 0; i < size; ++i)
	{
		if(source->name == arrayOfNodes[i].name)
		{
			startFound = 1;
		}
		minHeap.push(arrayOfNodes[i]);
		//This is necessary because source is stored in index 1 initially and i starts
		//at 0. When we hit the source node, we continue into the array, but we haven't
		//added a node for that iteration since source is already in the heap
		arrayOfNodes[i].index = i + 2 - startFound;
	}
	//run until all nodes checked
	Node* target = NULL;
	while(!minHeap.empty())
	{
		Node* temp = minHeap.top();
		if(temp == destination)
		{
			temp->visited = true;
			target = temp;
			break;
		}
		for(int i = 0; i < temp->numAdj; i++)
		{
			//Don't need to consider previously visited nodes
			if(temp->arrayOfNodePtrs[i]->visited)
			{
				continue;
			}
			//If the current distance to observed node is larger than current node distance + # of trans, replace
			if(temp->arrayOfNodePtrs[i]->distance > (temp->distance + trans(temp->name, temp->arrayOfNodePtrs[i]->name)))
			{
				//Need to make sure this keeps the integrity of the queue so that the algorithm
				//doesn't break. Probably need to make my own Queue.
				temp->arrayOfNodePtrs[i]->distance = temp->distance + trans(temp->name, temp->arrayOfNodePtrs[i]->name);
				changeIndex(temp->arrayOfNodePtrs[i], minHeap);
				temp->arrayOfNodePtrs[i]->parent = temp;
			}
		}
		temp->visited = true;
	}
	if(target == NULL)
	{
		cout << "IMPOSSIBLE" << endl;
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
				numChanges += trans(temp2->name, pathStorage.top().name);
				numGems += cost(temp2->magi, trans(temp2->name, pathStorage.top().name));
			}
		}
		cout << numChanges << " " << numGems << endl;
	}
}

template<typename T>
void Graph<T>::resetGraph()
{
	for (int i = 0; i < size; ++i)
	{
		arrayOfNodes[i].distance = (2^31);
		arrayOfNodes[i].visited = false;
		arrayOfNodes[i].parent = NULL;
		arrayOfNodes[i].index = 0;
	}
}

//End of Graph

int trans(string charm1, string charm2)
{
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

int cost(int *magi, int transNum)
{
	//Placeholder for functionality
	int returnCost = 0;
	//This exists for testing purposes only
	cout << "The number of transformations is: <" << transNum << ">." << endl;
	cout << "Assign a cost: " << endl;
	cin >> returnCost;
	//input array of magi power levels and number of transformations required

	//output the sum of the costs of the magi used (total cost of transformation)

	return returnCost;
  
}

int main()
{
	cout << "Input # of nodes to create" << endl;
	int numNode;
	cin >> numNode;
	Node** nodeArray = new Node*[numNode];
	//MinHeapPQ myPQ(numNode);
	string nodename = "";
	int distanceVal = 0;
	int numMagi;
	for (int i = 0; i < numNode; ++i)
	{
		cout << "Input a name for node <" << i <<">." << endl;
		cin >> nodename;
		cout << "Input number of magi:" << endl;
		cin >> numMagi;
		cout << "Testing distance." << endl;
		Node* n = new Node(nodename, numNode, numMagi);
		cout << n->distance << endl;
		cout << "Input magi power levels" << endl;
		for(int j = 0; j < numMagi; ++j){
			cin >> n->magi[j];
		}
		nodeArray[i] = n;
		//myPQ.push(n);
	}
	
	string charm1;
	string charm2;
	cout << "Input name of starting node:" << endl;
	cin >> charm1;
	cout << "Input name of ending node:" << endl;
	cin >> charm2;
	Node* startNode;
	Node* endNode;
	for(int i = 0; i < numNode; ++i)
	{
		if(nodeArray[i]->name==charm1){
			startNode = nodeArray[i];
		}
		else if(nodeArray[i]->name==charm2){
			endNode = nodeArray[i];
		}
	}

	return 0;
}

/*
int main()
{
	cout << "Input # of nodes to create" << endl;
	int numNode;
	cin >> numNode;
	Node** nodeArray = new Node*[numNode];
	MinHeapPQ myPQ(numNode);
	string nodename = "";
	int distanceVal = 0;
	for (int i = 0; i < numNode; ++i)
	{
		cout << "Input a name for node <" << i <<">." << endl;
		cin >> nodename;
		cout << "Input distance value" << endl;
		//cout << "Testing distance." << endl;
		cin >> distanceVal;
		Node* n = new Node(nodename, numNode, 5);
		n->distance = distanceVal;
		cout << n->distance << endl;
		nodeArray[i] = n;
		myPQ.push(n);
	}
	//myPQ.testPrint();
	myPQ.testEmptying();
	return 0;
}
*/