#include "Red_Black_Tree.h"
#include <iostream>
#include <windows.h>

using namespace std;

// 레드 블랙 트리(Red-Black Tree)란 자가 균형 이진 탐색 트리이다. 이진 탐색 트리의 극단적인 경우 한쪽으로만 치우쳐진 트리가 완성될 가능성이 있다. 
// 그렇게 되면 시간 복잡도는 O(log N)을 보장해주지 못한다. 이러한 문제점을 방지하기 위해 고안해낸 것이 바로 레드 블랙 트리이다.

enum class ConsoleColor
{
	BLACK = 0,
	RED = FOREGROUND_RED,
	GREEN = FOREGROUND_GREEN,
	BLUE = FOREGROUND_BLUE,
	YELLOW = RED | GREEN,
	WHITE = RED | GREEN | BLUE
};

void SetCursorColor(ConsoleColor color)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	::SetConsoleTextAttribute(output, static_cast<SHORT>(color));
}

void SetCursorPosition(int x, int y)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	::SetConsoleCursorPosition(output, pos);
}

void ShowConsoleCursor(bool flag)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	::GetConsoleCursorInfo(output, &cursorInfo);
	cursorInfo.bVisible = flag;
	::GetConsoleCursorInfo(output, &cursorInfo);
}

BinarySearchTree::BinarySearchTree()
{
	_nil = new Node();	// Black
	_root = _nil;
}

BinarySearchTree::~BinarySearchTree()
{
	delete _nil;
}

void BinarySearchTree::Print(Node* node, int x, int y)
{
	if (node == _nil)
		return;

	SetCursorPosition(x, y);

	if (node->color == Color::Black)
		SetCursorColor(ConsoleColor::BLUE);
	else
		SetCursorColor(ConsoleColor::RED);

	cout << node->key;
	Print(node->left, x - (5 / (y + 1)), y + 1);
	Print(node->right, x + (5 / (y + 1)), y + 1);

	SetCursorColor(ConsoleColor::WHITE);
}

void BinarySearchTree::Print()
{
	::system("cls");
	ShowConsoleCursor(false);
	Print(_root, 10, 0);
}

Node* BinarySearchTree::Search(Node* node, int key)
{
	if (node == _nil || key == node->key)
		return node;

	if (key < node->key)
		return Search(node->left, key);
	else
		return Search(node->right, key);
}

Node* BinarySearchTree::Min(Node* node)
{
	while (node->left != _nil)
		node = node->left;
	return node;
}

Node* BinarySearchTree::Max(Node* node)
{
	while (node->right != _nil)
		node = node->right;
	return node;
}

Node* BinarySearchTree::Next(Node* node)
{
	if (node->right != _nil)
		return Min(node->right);

	Node* parent = node->parent;

	while (parent != _nil && node == parent->right)
	{
		node = parent;
		parent = parent->parent;
	}

	return parent;
}
// 트리의 재구성
// 레드 블랙 트리에서 삽입, 삭제 등의 이벤트가 발생하여 트리의 균형이 맞지 않을 때, 
// 즉 특성에 부합하지 않을 때 트리의 재구성이 필요하다.
// 회전(Rotation)과 색 변경(Color - Flipping)을 통해 트리의 균형을 맞춘다.
// 트리의 현재 상태에 따라 재구성 방법이 나뉜다.

void BinarySearchTree::Insert(int key)
{
	Node* newNode = new Node();
	newNode->key = key;

	Node* node = _root;
	Node* parent = _nil;

	while (node != _nil)
	{
		parent = node;
		if (key < node->key)
			node = node->left;
		else
			node = node->right;
	}

	newNode->parent = parent;

	if (parent == _nil)
		_root = newNode;
	if (key < parent->key)
		parent->left = newNode;
	else
		parent->right = newNode;

	// 검사
	newNode->left = _nil;
	newNode->right = _nil;
	newNode->color = Color::Red;

	InsertFixup(newNode);
}

void BinarySearchTree::InsertFixup(Node* node)
{
	// 1) parent = red, uncle = red
	// -> p = black, uncle = black, pp = red로 바꿈
	// 2) p = red, uncle = black (triangle)
	// -> 회전을 통해 case 3으로 바꿈
	// 3) p = red, uncle = black (list)
	// -> 색상 변경 + 회전

	// color-Flipping
	// 노드의 색을 변경하는 방법이며 해당 방법을 사용하기 위해선 두 가지 경우가 존재한다.

	// 1. 부모 노드와 삼촌 노드 모두 레드로 동일하다면 
	// 부모 노드와 삼촌 노드 모두 블랙으로 바꿔주고 조부모 노드를 레드로 바꿔주면 트리 변경은 종료된다.

	// 색상 변경
	//node->parent->color = Color::Black;
	//uncle->color = Color::Black;
	//node->parent->parent->color = Color::Red;

	// 2. 부모 노드가 레드, 삼촌 노드가 블랙일 때, 한 방향으로 이어진 리스트 타입이라면 
	// 부모 노드와 조부모 노드의 색을 각각 블랙, 레드로 변경해주고 회전을 진행한다.
	
	/*node->parent->color = Color::Black;
	node->parent->parent->color = Color::Red;
	LeftRotate(node->parent->parent);*/
	
	while (node->parent->color == Color::Red)
	{
		// 이런상황이었다
		//      [pp(B)]
		//   [p(R)]   [u(R)]
		//       [n(R)]
		// 현재 상태: parent가 왼쪽에 있는 상태
		if (node->parent == node->parent->parent->left)
		{
			Node* uncle = node->parent->parent->right;
			// 1)의 경우
			//    [pp(R)]
	        // [p(B)]   [u(B)]
	        //    [n(R)]
	        // case1의 경우 p = black, uncle = black, pp = red로 바꿈
			if (uncle->color == Color::Red)
			{
				// 색상 변경
				node->parent->color = Color::Black; // p
				uncle->color = Color::Black; // u
				node->parent->parent->color = Color::Red; // pp

				// 확인할 노드 변경
				node = node->parent->parent;
			}
			// 2)의 경우
			else // uncle->color == Color::Black 이었다면?
			{
				//   [pp(B)]
				//[p(R)]   [u(B)]
				//   [n(R)]
				
				// left rotation을 하면
				//      [pp(B)]
				//   [p(R)]   [u(B)]
				// [n(R)]
				
				// Triangle 타입
				if (node == node->parent->right)
				{
					node = node->parent;
					LeftRotate(node);
				}

				// List 타입
				//      [pp(B)]
				//   [p(R)]   [u(B)]
				// [n(R)]
				// 위 상태에서 아래 상태로 바꿔준다
				
				//      [pp(R)]
				//   [p(B)]   [u(B)]
				// [n(R)]

				// 그리고 RightRotate 해준다
				//      [p(B)]
				//   [n(R)]   [pp(R)]
				//               [u(B)]

				node->parent->color = Color::Black;
				node->parent->parent->color = Color::Red;
				RightRotate(node->parent->parent);
			}
		}
		else
		{
			// 현재 상태: parent가 오른쪽에 있는 상태
			//      [pp(B)]
			//   [u(R)]   [p(R)]
			//               [n(R)]
			// case1의 경우 p = black, uncle = black, pp = red로 바꿈
			Node* uncle = node->parent->parent->left;
			if (uncle->color == Color::Red)
			{
				// 색상 변경
				node->parent->color = Color::Black;
				uncle->color = Color::Black;
				node->parent->parent->color = Color::Red;

				// 확인할 노드 변경
				node = node->parent->parent;
			}
			else
			{
				// Triangle 타입
				
				// 현재 상태
				//      [pp(B)]
			    //   [u(B)]   [p(R)]
			    //          [n(R)]

				// RightRotate을 한다
				// 결과 : 
				//      [pp(B)]
				//   [u(B)]   [p(R)]
				//               [n(R)]
	
				if (node == node->parent->left)
				{
					node = node->parent;
					RightRotate(node);
				}

				// List 타입

				//      [pp(B)]
				//   [u(B)]   [p(R)]
				//               [n(R)]

				// 색상변경 pp를 R로, p를 B로
				//      [pp(R)]
				//   [u(B)]   [p(B)]
				//               [n(R)]
				
				// LeftRotate를 한다
				// 결과:
				//             [p(B)]
				//       [pp(R)]    [n(R)]
				//    [u(B)]
				node->parent->color = Color::Black;
				node->parent->parent->color = Color::Red;
				LeftRotate(node->parent->parent);
			}
		}
	}

	_root->color = Color::Black;
}

void BinarySearchTree::Delete(int key)
{
	Node* deleteNode = Search(_root, key);
	Delete(deleteNode);
}

// 먼저 BST 삭제 실행
//      [20]
//  [15(B)]    [30]
//           [25] [40]
void BinarySearchTree::Delete(Node* node)
{
	if (node == _nil)
		return;

	if (node->left == _nil)
	{
		Color color = node->color;
		Node* right = node->right;
		Replace(node, node->right);

		if (color == Color::Black)
			DeleteFixup(right);
	}
	else if (node->right == _nil)
	{
		Color color = node->color;
		Node* left = node->left;
		Replace(node, node->left);

		if (color == Color::Black)
			DeleteFixup(left);
	}
	else
	{
		// 자식 노드가 2개 모두 있을 때
		// 다음 데이터 찾기
		Node* next = Next(node);
		node->key = next->key;
		Delete(next);
	}
}

// 먼저 BST 삭제 실행
// - Case1) 삭제할 노드가 Red -> 그냥 삭제. 끝!
// - Case2) root가 DB -> 그냥 추가 Black 삭제. 끝!
// - Case3) DB의 sibling 노드가 Red
// -- s = black, p = red (s <-> p 색상 교환)
// -- DB 방향으로 rotate(p)
// -- goto other case
// - Case4) DB의 sibling 노드가 Black && sibling의 양쪽 자식도 black
// -- 추가 Black을 parent에게 이전
// --- p가 Red이면 Black 됨
// --- p가 Black이면 DB 됨
// -- s = red
// -- p를 대상으로 알고리즘 이어서 실행 (DB가 여전히 존재하면)
// - Case5) DB의 sibling 노드가 Black && sibling의 near child = red, far child = black
// -- s <-> near 색상 교환
// -- far 방향으로 rotate(s)
// -- goto Case6
// - Case6) DB의 sibling 노드가 Black && sibling의 far child = red
// - p <-> s 색상 교환
// - far = black
// - rotate(p) (DB 방향으로)
// - 추가 Black 제거
void BinarySearchTree::DeleteFixup(Node* node)
{
	// 삭제할 노드
	Node* x = node;

	// [Case1], [Case2]
	while (x != _root && x->color == Color::Black)
	{
		//       [p]
		// [x(DB)]   [s(?)]
		
		// LeftChild 일때
		if (x == x->parent->left)
		{
			// [Case3]
			Node* s = x->parent->right;
			// ->
			//       [p]
			// [x(DB)]   [s(R)]
			if (s->color == Color::Red)
			{
				s->color = Color::Black;
				x->parent->color = Color::Red;
				LeftRotate(x->parent);
				s = x->parent->right; // [1]
				// -> 색상 변경
		        //       [p(R)]
		        // [x(DB)]   [s(B)]
				//          [1]
				
				// LeftRotate후 
				//         [s(B)]
			    //     [P(R)]   
				// [x(DB)]
				
				// 만약에 형제에 자식이 있었다면
				//         [s(B)]
				//     [P(R)]   
				// [x(DB)] [1]
			}
		
			// [Case4]
			if (s->left->color == Color::Black && s->right->color == Color::Black)
			{
				s->color = Color::Red;
				x = x->parent;
			}
			else
			{
				// [Case5]

				// 처음은 이 상태
				//          [p]
				// [X(DB)]       [s(B)]
				//        [near(R)] [far(B)]
				
				// 색상 변경
				//          [p]
				// [X(DB)]       [s(R)]
				//        [near(B)] [far(B)]

				// 그 다음 far방향으로 오른쪽 회전
				//          [p]
				// [X(DB)]       [near(B)]
				//                    [s(R)]
				//                      [far(B)]
				// 그 이후 case6 실행
				if (s->right->color == Color::Black)
				{
					s->left->color = Color::Black;
					s->color = Color::Red;
					RightRotate(s);
					s = x->parent->right;
				}

				// [Case6]

				// 이 상황이 case6이다
				//          [p]
				// [X(DB)]       [s(B)]
				//                    [far(R)] 
				
				// - p <-> s 색상 교환
                // - far = black
                // - rotate(p) (DB 방향으로)
                // - 추가 Black 제거

				s->color = x->parent->color;
				x->parent->color = Color::Black;
				s->right->color = Color::Black;
				LeftRotate(x->parent);
				x = _root;
			}
		}
		// RightChild 일때
		else
		{
			// [Case3]
			Node* s = x->parent->left;
			if (s->color == Color::Red)
			{
				s->color = Color::Black;
				x->parent->color = Color::Red;
				RightRotate(x->parent);
				s = x->parent->left;
			}

			// [Case4]
			if (s->right->color == Color::Black && s->left->color == Color::Black)
			{
				s->color = Color::Red;
				x = x->parent;
			}
			else
			{
				// [Case5]
				if (s->left->color == Color::Black)
				{
					s->right->color = Color::Black;
					s->color = Color::Red;
					LeftRotate(s);
					s = x->parent->left;
				}

				// [Case6]
				s->color = x->parent->color;
				x->parent->color = Color::Black;
				s->left->color = Color::Black;
				RightRotate(x->parent);
				x = _root;
			}
		}
	}

	x->color = Color::Black;
}

// u 서브트리를 v 서브트리로 교체
void BinarySearchTree::Replace(Node* u, Node* v)
{
	if (u->parent == _nil)
		_root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;

	v->parent = u->parent;

	delete u;
}
// Rotation
// 부모 노드가 레드, 삼촌 노드가 블랙일 경우에 왼쪽 혹은 오른쪽으로 노드를 회전시키는 방법이다.
// 더블 레드가 발생했을 때, 노드의 모양을 보고 회전 방향이 결정된다. 
// 조부모와 부모, 부모와 자식 노드 간의 연결된 방향이 다르면(꺾인 모양이면) 트라이앵글(Triangle) 타입이라고 하며 기준이 되는 노드는 부모 노드이다. 꺾인 방향에 따라 왼쪽, 혹은 오른쪽으로 회전시킨다.
// 한 번 회전을 하게 되면 트리의 균형이 맞춰질 때도 있지만 한 방향으로 뻗어진 구조가 될 수도 있다.
// 이러한 타입을 리스트(List) 타입이라 하고 반대로 한 번 더 회전을 시켜준다.
// 오른쪽 회전 시 x의 오른쪽 자식 노드가 y, 즉 x의 부모 노드의 왼쪽 자식 노드로 변환되고 
// 왼쪽 회전은 반대로 되는 것을 알 수 있다.

// 기존 트리
//	    [y]
//   [x]   [3]
// [1] [2]

// RightRotate한 결과
//      [x]
//   [1]   [y]
//       [2] [3]

// 그리고 LeftRotate하면 원상복귀 된다
void BinarySearchTree::LeftRotate(Node* x)
{
	Node* y = x->right;

	x->right = y->left;	// [2];

	if (y->left != _nil)
		y->left->parent = x;

	// 원래 X의 부모였던 애가 Y의 부모가 되게끔 바꿔치기
	y->parent = x->parent;

	if (x->parent == _nil)
		_root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;

	y->left = x;
	x->parent = y;
}

// 기존 트리
//	    [y]
//   [x]   [3]
// [1] [2]

// RightRotate한 결과
//      [x]
//   [1]   [y]
//       [2] [3]

void BinarySearchTree::RightRotate(Node* y)
{
	Node* x = y->left;

	y->left = x->right;	// [2];

	if (x->right != _nil)
		x->right->parent = y;

	if (y->parent == _nil)
		_root = x;
	else if (y == y->parent->right)
		y->parent->right = x;
	else
		y->parent->left = x;

	x->right = y;
	y->parent = x;
}
