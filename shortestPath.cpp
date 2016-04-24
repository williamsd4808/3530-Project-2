#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

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
	//Again, not sure if this is where I want this functionality.
	bool visited;
	Node();
	Node(string, int);
	//This probably shouldn't be Node**. It should probably be an Edge* which
	//would have a member variable of Node*
	Node** arrayOfNodePtrs;
	int* magi;
};

//Would probably never use this constructor
Node::Node()
{
	name = "";
	arrayOfNodePtrs = new Node*[8];
	magi = new int[8];
	visited = false;
	distance = static_cast< int >(numeric_limits< char >::max());
}

Node::Node(string n, int worldSize, int numMagi)
{
	name = n;
	arrayOfNodePtrs = new Node*[worldSize];
	visited = false;
	distance = static_cast< int >(numeric_limits< char >::max());
	magi = new int[numMagi];
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
	capacity = 8;
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
template<typename T>
class Graph
{
private:
	T* arrayOfNodes;

};

//First, we take in the undirected graph. That, or this will be a function of the graph which 
//therefore would have access to its member variables.


int main()
{
	return 0;
}