#include "Binary_Search_Tree.h"
#include <iostream>
#include <windows.h>
using namespace std;


void SetCursorPosition(int x, int y)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	::SetConsoleCursorPosition(output, pos);
}

void BinarySearchTree::Print(Node* node, int x, int y)
{	if (node == nullptr)
		return;

	SetCursorPosition(x, y);
	cout << node->key;
	Print(node->left, x - (5 / (y + 1)), y + 1); // 트리구조 프린트
	Print(node->right, x + (5 / (y + 1)), y + 1); // 트리구조 프린트
}

void BinarySearchTree::Print_Inorder(Node* node)
{
	// 전위 순회 (Preorder Traversal)
	// 중위 순회 (Inorder Traversal)
	// 후위 순회 (Postorder Traversal)

	//    [중]
	// [좌]  [우]

	if (node == nullptr)
		return;

	// 전위 : [중]이 앞에 온다 (중좌우)
	// 중위 : [중]이 중간에 온다 (좌중우)
	// 후위 : [중]이 마지막에 온다 (좌우중)

	cout << node->key << endl;	// 이 코드가 여기있으면 전위 순회
	Print_Inorder(node->left);
	// cout << node->key << endl;	// 이 코드가 여기있으면 중위 순회
	Print_Inorder(node->right);
	// cout << node->key << endl;	// 이 코드가 여기있으면 후위 순회
}

// 재귀함수를 사용하는 버전
// 가독성이 좋다
Node* BinarySearchTree::Search(Node* node, int key)
{
	if (node == nullptr || key == node->key)
		return node;

	if (key < node->key)
		return Search(node->left, key);
	else
		return Search(node->right, key);

	// 트리의 재귀적 속성을 이용하면 재귀함수를 통해 노드를 탐색하는 기능을 쉽게 구현할 수 있지만 
	// 경우에 따라 하나의 반복문을 통해 찾는 것도 방법
	// 아무래도 트리의 크기가 커지면 하나의 while문을 통해 탐색하는 것이 재귀함수를 이용하는 것보다 빠를 것이다.
}

// 재귀 함수를 사용하지 않는 버전
// 성능적으로는 더 이득
Node* BinarySearchTree::Search2(Node* node, int key)
{
	while (node && key != node->key)
	{
		if (key < node->key)
			node = node->left;
		else
			node = node->right;
	}

	return node;
}

Node* BinarySearchTree::Min(Node* node)
{
	while (node->left)
		node = node->left;
	return node;
}

Node* BinarySearchTree::Max(Node* node)
{
	while (node->right)
		node = node->right;
	return node;
}
// 이진 탐색 트리에서 가장 왼쪽 끝에 위치한 노드가 가장 작은 수, 
// 가장 오른쪽 끝에 위치한 노드가 가장 큰 수를 나타낸다.

Node* BinarySearchTree::Next(Node* node)
{
	if (node->right)
		return Min(node->right);

	Node* parent = node->parent;

	while (parent && node == parent->right)
	{
		node = parent;
		parent = parent->parent;
	}

	return parent;
}
// 일단 우측 노드가 존재한다는 것은 현재 노드보다 큰 수들만 모여있기 때문에, 
// 우측 노드의 서브트리에서 가장 작은 수를 찾으면 다음으로 큰 수일 것이다.

// 우측 노드가 없다면 부모 노드로 올라가게 되고, 
// 우측 노드를 가지고 있지 않은 노드가 그 중에서 가장 작은 수를 가지는 노드가 될 것이다.

void BinarySearchTree::Insert(int key)
{
	// 새로운 노드 할당
	Node* newNode = new Node();
	// 새로운 노드의 키값 저장
	newNode->key = key;

	// 루트 노드가 없으면 새로운 노드가 루트
	if (_root == nullptr)
	{
		_root = newNode;
		return;
	}

	// node를 만들어 준후 루트 노트 저장
	Node* node = _root;
	// parent 노드 널 초기화
	Node* parent = nullptr;

	// 루트 노드로 부터 시작해서 대소 비교를 통해 아래로 내려감
	while (node)
	{
		parent = node;
 		if (key < node->key)
			node = node->left;
		else
			node = node->right;
	}

	// 새로운 노드의 부모를 저장
	newNode->parent = parent;

	// 새로운 노드와 직속부모와의 키값 대소 비교에 따른 새로운 노드의 위치 결정
	if (key < parent->key)
		parent->left = newNode;
	else
		parent->right = newNode;

	// 루트 노드부터 대소 비교를 시작하여 삽입할 위치를 찾는다. 
	// 키값이 비교하는 노드의 키값보다 작으면 왼쪽, 크면 오른쪽 서브트리를 계속 탐색하게 될 것이다.

	// 그러다 다음 노드가 없는 리프 노드에 도달하게 되는데, 
	// 그 때 새로 만든 노드를 추가하면 삽입 기능은 완성이다.
	
	// 추가한다는 것은 노드 간의 연결을 의미한다.

}


void BinarySearchTree::Delete(int key)
{
	Node* deleteNode = Search(_root, key);
	Delete(deleteNode);
}

void BinarySearchTree::Delete(Node* node)
{
	if (node == nullptr)
		return;

	if (node->left == nullptr)
		Replace(node, node->right);
	else if (node->right == nullptr)
		Replace(node, node->left);
	else
	{
		// 자식 노드가 2개 모두 있을 때
		// 다음 데이터 찾기
		Node* next = Next(node);
		node->key = next->key;
		Delete(next);
	}
}
// 노드 삭제는 자식이 0~1개일 때와 2개일 때로 나뉜다.

// 0~1개일 때는 자식 노드와 해당 노드의 위치를 바꿔주고 연결 정보만 수정해주면 쉽게 완료된다.
// 만약 25번을 삭제한다면, 26번과 자리를 체인지 하고 연결 정보만 수정하면됨.

// 만약, 자식 노드가 없어 바꾸려는 노드가 null일 때에는 부모 노드의 연결 정보를 null로 바꿔주기 때문에 예외처리 없이 사용 가능하다.

// 자식 노드가 2개 모두 존재한다면, 삭제하려는 노드보다 다음으로 큰 수의 노드를 찾아 위치를 바꿔주고 
// 해당 노드를 삭제시키는 작업을 반복하면 트리가 알아서 정리될 것이다.

// 그러니까 자식 노드가 없는 경우에 
//   [20]
// [10]  [30]
//[9]  [25]  [40]
//        [26]  [50]
// 현재 이 상태에서 26을 삭제한다고 한다면 Replace함수에서 
// 부모 노드의 연결 정보를 null로 바꿔주고 26이 삭제가 된다
//   [20]
// [10]  [30]
//[9]  [25]  [40]
//             [50]

// u 서브트리를 v 서브트리로 교체, 그리고 delete u
void BinarySearchTree::Replace(Node* u, Node* v)
{
	if (u->parent == nullptr)
		_root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;

	if (v)
		v->parent = u->parent;

	delete u;
}


