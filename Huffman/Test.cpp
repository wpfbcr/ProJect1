#include<iostream>
#include"heap.h"
#include"filecompare.h"
using namespace std;

int main()
{
	//  int arr[]={9,8,7,6,5,4,3,2,1,0};
	//  Heap<int> h(arr,sizeof(arr)/sizeof(arr[0]));
	//  h.Display();
	char * filename = "text";
	filecompare f;
	f.createCompress(filename);
	f.uncompress("text");
	return 0;
}