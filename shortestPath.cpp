#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

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
	bool operator>(Node&);
};

//Would probably never use this constructor
Node::Node()
{
	name = "";
	arrayOfNodePtrs = new Node*[8];
	magi = new int[8];
	visited = false;
	distance = static_cast< int >(numeric_limits< char >::max());
	parent = NULL;
}

Node::Node(string n, int worldSize, int numMagi)
{
	name = n;
	arrayOfNodePtrs = new Node*[worldSize];
	visited = false;
	distance = static_cast< int >(numeric_limits< char >::max());
	magi = new int[numMagi];
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
bool Node::operator>(Node &otherNode)
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
	void shortestPath(T*, T*);
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

//First, we take in the undirected graph. That, or this will be a function of the graph which 
//therefore would have access to its member variables.


//Actual point of this file
//May just do the prints from here so I don't have to store this anywhere
// and return an array or the stack
// We may just choose to do all of the 
template<typename T>
void Graph<T>::shortestPath(T* source, T* destination)
{
	//MPQ to manage Dijkstra's. Priority based on distance variable.
	priority_queue<T, vector<T>, greater<T> > minHeap;
	Stack<Node> pathStorage(size);
	//Fill the queue with every node. All except source has priority MAX
	for (int i = 0; i < size; ++i)
	{
		if(source->name == arrayOfNodes[i].name)
		{
			arrayOfNodes[i].distance = 0;
		}
		minHeap.push(arrayOfNodes[i]);
	}
	//run until all nodes checked
	while(!minHeap.empty())
	{
		Node* temp = &minHeap.pop();
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
				temp->arrayOfNodePtrs[i]->parent = temp;
			}
		}
		temp->visited = true;
	}
}

template<typename T>
void Graph<T>::resetGraph()
{
	for (int i = 0; i < size; ++i)
	{
		arrayOfNodes[i].distance = static_cast< int >(numeric_limits< char >::max());
		arrayOfNodes[i].visited = false;
		arrayOfNodes[i].parent = NULL;
	}
}

//End of Graph

int trans(string charm1, string charm2){

	//Placeholder for functionality
	int result = 0;
	//input two charm strings

	//output the int value of the number of transformations required

	return result; 
}

int main()
{
	return 0;
}