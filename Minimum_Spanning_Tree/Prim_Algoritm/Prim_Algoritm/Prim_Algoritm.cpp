#include <iostream>
#include <vector>
#include <queue>
#include <climits> // INT_MAX 사용
using namespace std;

// 간선을 표현하는 구조체
struct Edge 
{
    int vertex; // 연결된 정점 번호
    int cost; // 간선의 가중치
    Edge(int v, int c) : vertex(v), cost(c) {}
    // 생성자 안쓰고, 중괄호 초기화로 해도됨

    // 우선순위 큐에서 최소 힙을 유지하기 위한 비교 연산자
    bool operator>(const Edge& e) const 
    {
        return cost > e.cost;
    }
};

// 프림 알고리즘
int primMST(int n, vector<vector<Edge>>& graph) 
{
    // MST에 포함 여부를 나타내는 벡터
    vector<bool> inMST(n, false);
    // 최소 가중치 우선순위 큐 (최소 힙)
    priority_queue<Edge, vector<Edge>, greater<Edge>> pq;

    int totalcost = 0; // MST의 전체 가중치
    int start = 0;       // 시작 정점 (0번 정점)

    // 시작 정점의 간선 추가
    pq.push(Edge(start, 0));

    while (!pq.empty()) 
    {
        Edge current = pq.top();
        pq.pop();

        int cv = current.vertex;
        int c = current.cost;

        // 이미 MST에 포함된 정점이면 무시
        if (inMST[cv]) 
            continue;

        // 정점을 MST에 추가
        inMST[cv] = true;
        totalcost += c;

        // 현재 정점에 연결된 모든 간선을 검사
        for (const Edge& edge : graph[cv]) 
        {
            int v = edge.vertex;
            if (!inMST[v]) 
            {
                pq.push(edge);
            }
        }
    }

    return totalcost;
}

int main() 
{
    int n = 5; // 정점 개수
    vector<vector<Edge>> graph(n);

    // 그래프 간선 추가 (양방향 그래프), 인접 리스트를 활용
    graph[0].push_back(Edge(1, 2));
    graph[1].push_back(Edge(0, 2));

    graph[0].push_back(Edge(3, 3));
    graph[3].push_back(Edge(0, 3));

    graph[1].push_back(Edge(2, 1));
    graph[2].push_back(Edge(1, 1));

    graph[1].push_back(Edge(3, 4));
    graph[3].push_back(Edge(1, 4));

    graph[2].push_back(Edge(4, 5));
    graph[4].push_back(Edge(2, 5));

    graph[3].push_back(Edge(4, 6));
    graph[4].push_back(Edge(3, 6));

    // MST의 최소 가중치 출력
    int cost = primMST(n, graph);
    cout << "최소 신장 트리의 가중치: " << cost << endl;

    return 0;
}
