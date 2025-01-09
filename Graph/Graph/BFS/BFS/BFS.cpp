#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

// BFS
// 너비 우선 탐색(BFS : Breadth First Search)은 깊이 우선 탐색과는 반대로 루트 노드에서 가까운 거리순으로 탐색한다.
// 큐를 이용하여 탐색할 대기열을 설정할 수 있고 발견하는 노드와 탐색하는 노드의 실행 차이가 존재한다.

struct Vertex
{
    //int data;
};

vector<Vertex> vertices;
vector<vector<int>> adjacent;
vector<bool> discovered;

// 그래프를 만드는 방법이 크게 두가지가 있다.
// 인접 리스트를 만들거나 인접 행렬 방식으로 만들거나 이다.
void CreateGraph()
{
    vertices.resize(6);
    adjacent = vector<vector<int>>(6);
    // 인접 리스트
    /*adjacent[0].push_back(1);
    adjacent[0].push_back(3);
    adjacent[1].push_back(0);
    adjacent[1].push_back(2);
    adjacent[1].push_back(3);
    adjacent[3].push_back(4);
    adjacent[5].push_back(4);*/

    // 인접 행렬
    adjacent = vector<vector<int>>
    {
        {0,1,0,1,0,0},
        {1,0,1,1,0,0},
        {0,0,0,0,0,0},
        {0,0,0,0,1,0},
        {0,0,0,0,0,0},
        {0,0,0,0,1,0},
    };
}
// Queue는 말 그대로 내가 발견한 순서를 그대로 넣어주는 예약 시스템이라고 했다.
void Bfs(int here)
{
    // 누구에 의해 발견 되었는지?
    vector<int> parent(6, -1);
    // 시작점에서 얼만큼 떨어져 있는지?
    vector<int> distance(6, -1);

    // 발견한 노드 목록 저장
    queue<int> q;
    q.push(here);
    discovered[here] = true;

    parent[here] = here;
    distance[here] = 0;
    // 이렇게 하면 내가 어떤 시작점을 기준으로 이동해야 되는 거리를 이렇게 다 판별해준다. 그리고 이것을 이용하면 최단거리도 이런 식으로 찾아줄수 있다
    // 이것을 기반으로 길찾기 알고리즘을 만들수 있다

    while (q.empty() == false)
    {
        here = q.front();
        q.pop();
        // 방문
        cout << "Visited : " << here << endl;

        // 인접 리스트 버전
        // 인접한 노드 찾기
        /*for (int there : adjacent[here])
        {
            if (discovered[there])
                continue;
            q.push(there);
            discovered[there] = true;

            parent[there] = here;
            distance[there] = distance[here] + 1;
        }*/
        // 인접 행렬 버전
        for (int there = 0; there < 6; there++)
        {
            if (adjacent[here][there] == 0)
                continue;
            if (discovered[there])
                continue;

            q.push(there);
            discovered[there] = true;

            parent[there] = here;
            distance[there] = distance[here] + 1;
        }
    }
    //return; // 중단점
}
// DFS의 경우 발견한 순서랑 방문하는 순서랑 일치한다고 볼 수 있다.
// 하지만 BFS의 경우 발견만 하고 당장 방문은 하지 않을수 있다.

void BfsAll()
{
    discovered = vector<bool>(6, false);

    for (int i = 0; i < vertices.size(); i++)
    {
        if (!discovered[i])
            Bfs(i);
    }
}

int main()
{
    CreateGraph();
    //discovered = vector<bool>(6, false);
    //Bfs(0);
    // 결과
    // 0 -> 1 -> 3 -> 2 -> 4 -> 5
    BfsAll();

    // 이러한 BFS든 DFS든 외울 필요는 없고 이 원리에 대해서만 이해를 한다면 기억을 되살려서 만들수 있어야 된다
    // 그리고 BFS,DFS 코드를 직접 한번 안보고 몇번 만들어 보는 연습을 하는것이 좋다
    // 기술면접에서도 종종 나옴
}
