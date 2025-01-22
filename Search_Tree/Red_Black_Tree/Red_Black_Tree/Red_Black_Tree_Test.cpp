#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <thread>
#include "Red_Black_Tree.h"
using namespace std;

int main()
{
	BinarySearchTree bst;

	bst.Insert(30);
	bst.Print();
	this_thread::sleep_for(1s);

	bst.Insert(10);
	bst.Print();
	this_thread::sleep_for(1s);

	bst.Insert(20);
	bst.Print();
	this_thread::sleep_for(1s);

	bst.Insert(25);
	bst.Print();
	this_thread::sleep_for(1s);
	//return 0; // 삽입까지 실행

	// 삽입까지 했을때 트리 상황:
	//     [20]
	//  [10]   [30]
	//        [25]
	bst.Delete(20);
	bst.Print();
	this_thread::sleep_for(1s);
	//     [25]
	//  [10]   [30]


	bst.Delete(10);
	bst.Print();
	this_thread::sleep_for(1s);
}