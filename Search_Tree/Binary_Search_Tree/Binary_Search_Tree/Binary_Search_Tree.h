#pragma once

struct Node
{
	Node* parent = nullptr;
	Node* left = nullptr;
	Node* right = nullptr;
	int		key = {};	// key == data
};
// 기본적으로 부모 노드를 가리키는 포인터와 왼쪽, 오른쪽 자식 노드를 가리키는 포인터, 
// 그리고 해당 노드의 키값을 가지는 노드로 구성되어있다.

class BinarySearchTree
{
public:
	void Print() { Print(_root, 10, 0); };
	void Print(Node* node, int x, int y);
	void Print_Inorder() { Print_Inorder(_root); };
	void Print_Inorder(Node* node);

	Node* Search(Node* node, int key);
	Node* Search2(Node* node, int key);

	Node* Min(Node* node);
	Node* Max(Node* node);
	Node* Next(Node* node);

	void Insert(int key);
	void Delete(int key);
	void Delete(Node* node);

	void Replace(Node* u, Node* v);

private:
	Node* _root = nullptr;
};