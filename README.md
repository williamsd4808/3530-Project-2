# 3530-Project-2


The methods to be designed are as follow




Transformations required between two charms:

int trans(string charm1, string charm2){

  //input two charm strings

  //output the int value of the number of transformations required

  return result; 

}

Max incantations availible in a realm:

int maxIncant(int[] magi){

  //input array of magi power levels
  
  //output max number of transormations in a realm
  
  return result;
  
}

Cost of a number of transforms:

int cost(int[] magi, int transNum){

  //input array of magi power levels and number of transformations required
  
  //output the sum of the costs of the magi used (total cost of transformation)
  
  return cost;
  
}

Each realm will be a node:

struct Node {

  string charm; //these two are guarenteed to be here

  int[] magi;
  
  Node*[] otherRealms;  //these are subject to change, and I'm not sure of the exact syntax, but this will be a basic setup.
	
	int[] incantRequired;
	
	int[] costRequired;
	
	bool visited;
	
	//other constructors can be added for convenience
	
};

I (Colin) will be making the trans method, and Nicolas will be on the cost and maxIncant methods (these are very closely linked, and are solved similarly). These three methods are largely independent and do not rely on the rest of the program. Daniel and Tyler will be programming the methods for shortest path and the graph of the problem.
