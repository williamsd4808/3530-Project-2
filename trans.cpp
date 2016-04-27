#include<iostream>
#include<string>
#include<algorithm>
using namespace std;

int trans(string charm1, string charm2){
  //initialize minimum editing distance array
  //strlen +1 to allow for the zero space in beginning of array
  int arr[charm1.length()+1][charm2.length()+1];

  //Initialize array to correct values
  for(int i = 0; i <= charm1.length(); i++)
    arr[i][0] = i;
  for(int j = 0; j <= charm2.length(); j++)
    arr[0][j] = j;

  //Begin finding correct editing distance for substrings
  //Minimum editing distance is found at arr[charm1.len()][charm2.len()]
  for(int i = 1; i <= charm1.length(); i++){
    for(int j = 1; j <= charm2.length(); j++){
      int c1, c2, c3;
      c1 = arr[i-1][j] +1;
      c2 = arr[i][j-1] +1;
      c3 = arr[i-1][j-1];
      if( //Allows for either character to be either upper or lowercase
	 ((((int)charm1.at(i-1) % 65) % 32) + 65) !=
	 ((((int)charm2.at(j-1) % 65) % 32) + 65)
	 )
	c3++;

      //Find minimum value
      arr[i][j] = min(min(c1, c2), c3);
    }
  }

  //Find minimum editing distance for the two strings
  //Return that value

  return arr[charm1.length()][charm2.length()];
}

int main(){
  int res = trans("Baseball", "Basketball");

  cout << res << endl;
  return 0;
}
      
