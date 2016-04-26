#include <iostream>
#include <cstring>
#include <stdio.h>
using namespace std;

int trans(string charm1, string charm2){
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

int main(){

	string a;
	string b;
	cin>>a>>b;
	int c = trans(a,b);
	cout<<c<<endl;

}
