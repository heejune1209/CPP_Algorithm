#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

// 탐색은 어떤 특정 알고리즘에 의해 여기있는 모든 정점들을 한바퀴 순회하는걸 탐색이라 하는데, 시작을 정점 어디서 시작하든 상관 없다.
// DFS
// 깊이 우선 탐색(DFS : Depth First Search)은 그래프의 깊이를 우선시하는 탐색 방법이다.
// 루트 노드에서 탐색을 시작했을 때 다음 노드로 넘어가고, 해당 노드에서 또 연결된 노드가 있는지 탐색한다.
// 리프 노드에 다다랐을 때 다시 뒤로 돌아오며 길이 있으면 해당 노드로 넘어가 다시 탐색한다. 재귀함수를 이용하여 구현할 수 있다.

struct Vertex
{
    //int data;
};

vector<Vertex> vertices;
vector<vector<int>> adjacent;
vector<bool> visited;

// 그래프를 만드는 방법이 크게 두가지가 있다.
// 인접 리스트를 만들거나 인접 행렬 방식으로 만들거나 이다.
void CreateGraph()
{
    vertices.resize(6);
    adjacent = vector<vector<int>>(6);
    // 인접 리스트
    adjacent[0].push_back(1); // 0번 정점이 1번 정점과 연결
    adjacent[0].push_back(3); // adjacent[0]이 {1, 3}이라는 리스트를 가지게 된다는 뜻
    adjacent[1].push_back(0);
    adjacent[1].push_back(2);
    adjacent[1].push_back(3);
    adjacent[3].push_back(4);
    adjacent[5].push_back(4);
    // 인덱스를 추가하는 것이 아니라 실제 정점 번호를 추가한다고 이해하면 된다.

    // 인접 리스트를 다르게 표현한 것.
    /*
    adjacent = vector<vector<int>>
    {
        {1, 3},   // 0번 정점: 1번, 3번 정점과 연결
        {0, 2, 3},// 1번 정점: 0번, 2번, 3번 정점과 연결
        {},        // 2번 정점: 연결 없음
        {4},      // 3번 정점: 4번 정점과 연결
        {},        // 4번 정점: 연결 없음
        {4}       // 5번 정점: 4번 정점과 연결
    };
    */
    // 인접 행렬
    // 인접 행렬에서는 1은 두 정점이 연결되어 있음을 나타낸다
    /*adjacent = vector<vector<int>>
    {
        {0,1,0,1,0,0},
        {1,0,1,1,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,1,0},
        {0,0,0,0,0,0},
        {0,0,0,0,1,0},
    };*/
    // 행렬의 인덱스가 정점 번호를 나타낸다
    // 결론적으로 인접 리스트의 정점 번호와 인접 행렬의 인덱스는 동일한 그래프를 표현하기 위한 다른 방식일 뿐이다.
}

// DFS
void Dfs(int here) // here은 입구를 뜻함
{
    // 방문!
    visited[here] = true;
    cout << "Visted : " << here << endl;

    // 인접 리스트 ver
    // 모든 인접 정점을 순회한다.
    // there는 목적지
    for (int i = 0; i < adjacent[here].size(); i++)
    {
        int there = adjacent[here][i];
        if (visited[there] == false)
            Dfs(there);
    }

    // 인접 행렬 ver
    //for (int there = 0; there < 6; there++)
    //{
    //    if (adjacent[here][there] == 0)
    //        continue;
    //    // 아직 방문하지 않은 곳이 있으면 방문한다.
    //    if (visited[there] == false)
    //        Dfs(there);
    //}

    

}
// 끊어져있는 노드가 있을 때 탐색이 되지 않는 경우 방지
void DfsAll()
{
    visited = vector<bool>(6, false);
    for (int i = 0; i < 6; i++)
    {
        if (visited[i] == false)
        {
            Dfs(i);
        }
    }
}
int main()
{
    CreateGraph();
    
    //Dfs(0);
    DfsAll();
    // 결과 : 0->1->2->3->4->5

}

