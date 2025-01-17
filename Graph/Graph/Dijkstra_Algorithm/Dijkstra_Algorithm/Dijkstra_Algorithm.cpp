#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

// 다익스트라 알고리즘(Dijkstra Algorithm)이란 그래프 내의 노드 간 최단 경로를 구하는 알고리즘이다. BFS를 기반으로 진행되지만 간선마다 동일한 코스트가 아닐수 있기 때문에 
// 탐색 순서가 다를 수 있다는 차이점이 있다. 가중치를 계산하여 시작 노드부터 도착 노드까지의 최단 경로와 비용을 알 수 있다. 
// 즉, 발견한 후보 중에서 코스트가 가장 작은 길을 선택한다.
// 다음 코드는 다익스트라 알고리즘을 이용하여 0번 노드부터 모든 노드 간의 가중치(비용)를 구하는 기능을 구현한 것이다.

struct Vertex
{
    // int data;
};

vector<Vertex> vertices;
vector<vector<int>> adjacent; // 인접 행렬

void CreateGraph()
{
    vertices.resize(6);
    adjacent = vector<vector<int>>(6, vector<int>(6, -1));

    adjacent[0][1] = 15;
    adjacent[0][3] = 35;

    adjacent[1][0] = 15;
    adjacent[1][2] = 5;
    adjacent[1][3] = 10;

    adjacent[3][4] = 5;
    adjacent[5][4] = 5;
}

void Dijkstra(int here)
{
    struct VertexCost
    {
        int vertex;
        int cost;
        // pair를 이용해서 해도된다
    };
    list<VertexCost> discovered; // 발견 목록
    vector<int> best(6, INT32_MAX); // 각 정점별로 지금까지 발견한 최소거리
    vector<int> parent(6, -1);

    // 우선 현재 노드에서 목적지 노드까지의 비용을 정점과 함께 관리하기 위해 구조체를 만들었다. 
    // 일반적인 BFS와는 다르게 발견 목록을 큐로 관리할 수 없다. 
    // 발견한 노드와 탐색한 노드의 순서가 다를 수 있기 때문이다. 
    // 또한 최소 비용(거리)을 저장할 수 있는 동적 배열과 부모 노드의 경로를 알 수 있는 동적 배열을 선언해주었다.

    // 초기화
    // rvalue를 이용한 이동 연산
    discovered.push_back(VertexCost{ here,0 }); // 중괄호 초기화를 사용한 객체 생성 방식, 이동 연산으로 lvalue를 이용한 방법보다 더 효율적임

    // lvalue를 전달 했을 경우
    // VertexCost vc = { here, 0 };
    // discovered.push_back(vc);  // lvalue 전달
    // push_back(const T& value)가 호출, 복사 비용이 발생

    // 이것이 되는 이유는 push_back엔 오른값, 즉 임시값을 넣어야하기 때문에 VertexCost{ here,0 } 이라는 임시 객체를 생성해서 넣어줬기 떄문에 가능하다
    // 생성자가 정의되어 있다면 VertexCost(here, 0)과 같은 방식도 사용 가능
    best[here] = 0;
    parent[here] = here;

    while (discovered.empty() == false)
    {
        // 최단거리 찾기
        // 제일 좋은 후보를 찾는다.
        list<VertexCost>::iterator bestit;
        int bestCost = INT32_MAX;
        

        for (auto it = discovered.begin(); it != discovered.end(); it++)
        {
            if (it->cost < bestCost)
            {
                bestCost = it->cost;
                bestit = it;
            }
        }
        // 이 코드부턴 발견된 노드들이 전부 탐색될 때까지 무한 반복된다. 이터레이터를 이용해 지정한 노드에서 해당 노드까지 가장 적은 비용이 드는 경로를 저장해 줄 것이다. 
        // 발견한 노드 중 더 적은 비용이 드는 노드를 베스트로 지정한다.

        // 사실.
        // 그래프의 크기가 아주 크거나 최악의 경우 모든 노드를 순회한다고 가정하면 위의 코드(리스트를 이용한 코드)는 절대 좋은 코드가 아니다. 
        // 아무튼 가장 좋은 후보만 찾으면 되기 때문에 이러한 기능을 가지는 자료 구조를 사용하는 것이 훨씬 효율적일 것이다.

        // priority_queue 같은 경우 큐에 들어간 정보 중 가장 좋은(후보에 알맞는) 노드를 한 번에 찾아 주기 때문에 위와 같은 조건에 사용하면 훨씬 효율적일 것이다.

        // 다음 노드 선정
        int cost = bestit->cost;
        here = bestit->vertex;
        discovered.erase(bestit);
        // 최적의 경로를 찾았다면 다음 노드로 변경한다. 발견 목록에선 삭제하며 더 이상 탐색 후보에 오르지 않도록 한다.

        // 방문? 더 짧은 경로를 뒤늦게 찾았다면 스킵.
        if (best[here] < cost)
            continue;
        // 현재 best[here]은 내가 있는 노드(목적지)의 최소 비용을 의미한다. 
        // 이 비용이 내가 위에서 찾은 비용보다 더 적다면 아래의 코드는 무시(스킵)한다.

        // 노드 탐색
        // 방문!
        for (int there = 0; there < 6; there++)
        {
            // 연결되지 않았다면 스킵.
            if (adjacent[here][there] == -1)
                continue;

            // 더 좋은 경로를 과거에 찾았으면 스킵
            int nextCost = best[here] + adjacent[here][there];
            if (nextCost >= best[there])
                continue;

            discovered.push_back(VertexCost{ there, nextCost });
            best[there] = nextCost;
            parent[there] = here;
        }
        // here노드와 연결되어있는 there노드를 모두 찾는다. 연결되어있지 않으면 스킵하고, 연결되어있다면 그 노드와 연결되어있는 간선의 가중치(비용)를 구한다. 
        // 비용은 출발 노드(0번)에서 해당 노드까지의 총합을 의미한다. 만약 이 비용이 최소 비용보다 크다면 스킵한다.
       
        // 문제가 없다면 discovered에 새 구조체를 만들어 추가하고 최소 비용 배열의 값을 수정해준다.
        // 부모 노드 정보를 저장하는 배열도 수정해준다.이러한 과정을 모든 노드를 탐색할 때까지 진행하고 프로그램은 종료된다.
    }
}

// 인접 리스트와 pair로 vertexcost를 관리한 버전
//vector<Vertex> vertices;
//vector<vector<pair<int, int>>> adjacent;
//
//void createGraph()
//{
//    vertices.resize(6);
//    //adjacent = vector<vector<int>>(6, vector<int>(6, -1));
//
//    /*adjacent[0][1] = 15;
//    adjacent[0][3] = 35;
//    adjacent[1][0] = 15;
//    adjacent[1][2] = 5;
//    adjacent[1][3] = 10;
//    adjacent[3][4] = 5;
//    adjacent[5][4] = 5;*/
//
//    // 인접 리스트 버전
//    adjacent.resize(6);
//    adjacent[0].push_back({ 1,15 });
//    adjacent[0].push_back({ 3,35 });
//    adjacent[1].push_back({ 0,15 });
//    adjacent[1].push_back({ 2,5 });
//    adjacent[1].push_back({ 3,10 });
//    adjacent[3].push_back({ 4,5 });
//    adjacent[5].push_back({ 4,5 });
//}
//void Dijkstra(int here)
//{
//    /*struct VertexCost
//    {
//        int vertex;
//        int cost;
//    };*/
//
//    list<pair<int, int>> discovered; // first : vertex, second : cost
//    vector<int> best(6, INT32_MAX);
//    vector<int> parent(6, -1);
//
//    discovered.push_back({ here,0 });
//    best[here] = 0;
//    parent[here] = here;
//
//    while (discovered.empty() == false)
//    {
//        // 최단거리 찾기
//        // 가장 좋은 후보를 찾는다
//
//        list<pair<int, int>>::iterator bestit;
//        int bestcost = INT32_MAX;
//
//        for (auto it = discovered.begin(); it != discovered.end(); it++)
//        {
//            if (it->second < bestcost)
//            {
//                bestcost = it->second;
//                bestit = it;
//            }
//        }
//
//        // 다음 노드 선정
//        // 최적의 경로를 찾았다면 다음 노드로 변경, 발견 목록에서 삭제
//
//        int cost = bestit->second;
//        here = bestit->first;
//        discovered.erase(bestit);
//
//        // 노드 탐색
//        if (best[here] < cost)
//            continue;
//
//        for (auto edge : adjacent[here])
//        {
//            int there = edge.first;
//            int cost = edge.second;
//            // 더 좋은 경로를 찾았다면 스킵
//            int nextcost = best[here] + cost;
//            if (nextcost > best[there])
//                continue;
//
//            discovered.push_back({ there, nextcost });
//            best[there] = nextcost;
//            parent[there] = here;
//
//        }
//
//    }
//
//}
int main()
{
    CreateGraph();
    Dijkstra(0);
}
