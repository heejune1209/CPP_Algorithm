#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

// 트리 만들기
// 코드로 트리를 만들 때에는 노드를 이용한다.
using NodeRef = shared_ptr<struct Node>;

struct Node
{
    Node() {}
    Node(const string& data) : data(data) {}

    string          data;
    vector<NodeRef> children;
};
// 그리고 싶은 트리 구조에 따라 자식 노드를 children에 추가하면 쉽게 완성할 수 있다.

NodeRef CreateTree()
{
    NodeRef root = make_shared<Node>("R1 개발실");
    {
        NodeRef node = make_shared<Node>("디자인팀");
        root->children.push_back(node);
        {
            NodeRef leaf = make_shared<Node>("전투");
            node->children.push_back(leaf);
        }
        {
            NodeRef leaf = make_shared<Node>("경제");
            node->children.push_back(leaf);
        }
        {
            NodeRef leaf = make_shared<Node>("스토리");
            node->children.push_back(leaf);
        }
    }
    {
        NodeRef node = make_shared<Node>("프로그래밍팀");
        root->children.push_back(node);
        {
            NodeRef leaf = make_shared<Node>("서버");
            node->children.push_back(leaf);
        }
        {
            NodeRef leaf = make_shared<Node>("클라");
            node->children.push_back(leaf);
        }
        {
            NodeRef leaf = make_shared<Node>("엔진");
            node->children.push_back(leaf);
        }
    }
    {
        NodeRef node = make_shared<Node>("아트팀");
        root->children.push_back(node);
        {
            NodeRef leaf = make_shared<Node>("배경");
            node->children.push_back(leaf);
        }
        {
            NodeRef leaf = make_shared<Node>("캐릭터");
            node->children.push_back(leaf);
        }
    }

    return root;
}
// 트리 출력하기
void PrintTree(NodeRef root, int depth)
{
    for (int i = 0; i < depth; i++)
        cout << "-";

    cout << root->data << endl;

    for (NodeRef& child : root->children)
        PrintTree(child, depth + 1);
}
// 재귀적 속성을 이용하여 모든 노드에 순차적으로 접근할 수 있으며 깊이를 알고싶다면 깊이값을 사용하여 구할 수 있다.

// 깊이(Depth) : 루트에서 어떤 노드에 도달하기 위해 거쳐야 하는 간선의 수 (aka. 몇 층?)
// 높이(Height) : 가장 깊숙히 있는 노드의 깊이 (max(depth))

// 트리 높이 구하기
int GetHeight(NodeRef root)
{
    int height = 1;

    for (NodeRef& child : root->children)
        height = max(height, GetHeight(child) + 1); // return larger of _Left and _Right, 둘중 큰애를 반환

    return height;
}
// 높이를 구할 때에도 구조 출력과 동일하게 재귀적 특성을 이용한다. 
// 단, 깊이와는 다르게 루트 노드가 1부터 시작하고 가장 깊은(가장 숫자가 큰) 노드에 도달하면 해당 높이를 트리의 최종 높이로 반환한다.
int main()
{
    NodeRef root = CreateTree();
    PrintTree(root, 0);
    cout << "Tree Height : " << GetHeight(root) << endl;
}
