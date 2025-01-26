#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

// 최소 스패닝 트리
// 스패닝 트리(Spanning Tree)란, 간선의 수를 최소화하여 모든 정점을 연결한 트리를 의미한다. 
// 이 중에서도 가장 적은 가중치를 가지는 스패닝 트리를 최소 스패닝 트리(Minimum Spanning Tree)라고 한다. 
// 간선을 최소화 한다는 의미는 사이클(Cycle)이 생기지 않는다는 의미이다. 사이클이 있는 트리는 스패닝 트리에 속하지 않는다.

// 스패닝 트리의 특성

// 1. N개의 정점이 있을 때 간선은 N - 1개이다.
// 2. 사이클을 가지지 않는다.

// 크루스칼(Kruskal) 알고리즘
// 크루스칼 알고리즘(Kruskal Algorithm)은 최소 스패닝 트리를 만드는 데 사용되는 대표적인 알고리즘이다. 
// 탐욕적인(Greedy) 방법을 이용한다는 특성이 있다. 탐욕법은 현재 위치에서 가장 최적인 답을 선택하여 결과를 도출해내는 방법이다. 
// 그래프에서 가중치가 가장 적은 간선을 우선 순위로 두고 연결하며 최소 스패닝 트리를 만들게 된다.

// 해당 알고리즘은 이전 시간에 알아보았던 서로소 집합(Disjoint Set) 개념을 이용하여 쉽게 구현이 가능하다.
// 최소 스패닝 트리를 만들기 위해선 다음과 같은 과정을 거치게 되는데,
// 이 때 유니온 - 파인드(Union - Find) 알고리즘을 사용하여 간단하게 만들 수 있다.
// 유니온 - 파인드 알고리즘은 서로소 집합이 가지는 대표적인 특성이다.

//  최소 스패닝 트리 만들기

// 1. 간선의 정보(인접 노드와 가중치)를 담은 배열 생성 후 가중치 오름차순으로 정렬한다.

// 2. 디스조인트 셋을 이용하여 그래프의 노드 수만큼 집합을 만든다.
// - 이 때, 부모 노드는 각자 자기 자신을 가지고 있다.

// 3. 간선 배열을 순회하며 Find 연산을 이용해 연결이 되어있는지 확인한다.
// - 연결이 되어있다면, 즉 같은 루트 노드를 가지고 있다면 사이클이 발생하므로 병합하지 않는다.
// - 연결이 되어있지 않다면 Union 연산을 실행한다.

// 크루스칼 알고리즘 시간 복잡도
// union-find 알고리즘은 시간복잡도가 상수이므로 간선들을 가중치 기준으로 정렬하는 데 걸리는 시간에 의존한다.
// 그리고 간선을 가중치 기준으로 정렬하는데 소요되는 시간은 일반적으로 O(ElogE)이다. (E는 간선개수)
// 즉, 크루스칼 알고리즘의 시간 복잡도는 간선을 정렬한 뒤 Union-Find 연산을 수행하기 때문에 시간 복잡도는 O(ElogE) 이다.

struct Vertex
{

};

vector<Vertex> vertices;
vector<vector<int>> adjacent;   // 인접 행렬

void CreateGraph()
{
    vertices.resize(6);
    adjacent = vector<vector<int>>(6, vector<int>(6, -1));

    adjacent[0][1] = adjacent[1][0] = 15;
    adjacent[0][3] = adjacent[3][0] = 35;
    adjacent[1][2] = adjacent[2][1] = 5;
    adjacent[1][3] = adjacent[3][1] = 10;
    adjacent[3][4] = adjacent[4][3] = 5;
    adjacent[3][5] = adjacent[5][3] = 10;
    adjacent[4][5] = adjacent[5][4] = 5;
}

// Disjoint Set
class DisjointSet
{
public:
    DisjointSet(int n) : _parent(n), _rank(n, 1)
    {
        for (int i = 0; i < n; i++)
            _parent[i] = i;
    }

    int Find(int u)
    {
        if (u == _parent[u])
            return u;

        return _parent[u] = Find(_parent[u]);
    }

    void Merge(int u, int v)
    {
        u = Find(u);
        v = Find(v);

        if (u == v)
            return;

        if (_rank[u] > _rank[v])
            swap(u, v);

        _parent[u] = v;

        if (_rank[u] == _rank[v])
            _rank[v]++;
    }
private:
    vector<int> _parent;
    vector<int> _rank;
};

// 간선의 정보(인접 노드와 가중치)를 담은 배열 생성 후 가중치 오름차순으로 정렬한다.
struct CostEdge
{
    int cost;
    int u;
    int v;

    bool operator<(CostEdge& other)
    {
        return cost < other.cost;
    }
};

// 가중치의 합을 반환
int Kruskal(vector<CostEdge>& selected)
{
    int ret = 0;

    selected.clear();

    vector<CostEdge> edges;

    // 인접 행렬 인덱스 용도로 u,v를 만들어주고 해당 인덱스 값에 adjacent의 cost정보를 가지고 와서 CostEdge의 벡터에 cost, u,v 정보를 넣어준다
    for (int u = 0; u < adjacent.size(); u++)
    {
        for (int v = 0; v < adjacent[u].size(); v++)
        {
            // 중복 등록 방지
            if (u > v)
                continue;

            int cost = adjacent[u][v];
            if (cost == -1)
                continue;

            edges.push_back(CostEdge{ cost, u, v });
        }
    }

    std::sort(edges.begin(), edges.end());

    // 인접 노드와 가중치 정보를 담은 구조체 CostEdge를 만들어주고, 
    // 오퍼레이터 <까지 재정의 해주었다. 해당 대소 비교 연산자는 sort 함수를 사용할 때 사용될 것이다.

    // Kruskal 알고리즘 구현
    // 가장 적은 가중치부터 탐색

    // 디스조인트 셋을 이용하여 그래프의 노드 수만큼 집합을 만든다.
    DisjointSet sets(vertices.size());

    // DisjointSet은 이전 시간에 만들었던 클래스를 재활용하였고, 생성자 선언 시 값을 넣어주면 
    // 해당 개수만큼의 집합을 _parent = 자기 자신, _rank = 1로 초기화하여 생성해준다.

    // 간선 배열을 순회하며 Find 연산을 이용해 연결이 되어있는지 확인한다.
    for (CostEdge& edge : edges)
    {
        // 같은 그룹이면 스킵 (안 그러면 사이클 발생)
        if (sets.Find(edge.u) == sets.Find(edge.v))
            continue;

        // 두 그룹 병합
        sets.Merge(edge.u, edge.v);
        selected.push_back(edge);
        ret += edge.cost;
    }

    return ret;
}
// 디스조인트 셋의 Find 연산을 이용해 최상위 노드를 비교한다. 일치한다면 같은 집합 내에 있다는 뜻이므로 병합을 생략하고 다음 간선으로 넘어간다. 
// 다른 집합에 속해있는 각각의 그래프라면 병합을 실행한다. 디스조인트 셋 자체에서 만들어 낸 그래프는 높이가 변경되고 최적화 되어 서로 연결되는 노드가 다르겠지만, 
// 해당 알고리즘을 사용하여 만드는 트리는 인접 노드의 정보가 바뀌지 않기 때문에 그대로 잘 연결될 것이다.

// 결과
// 이렇게 되면 크루스칼 알고리즘을 실행했을 때 가중치 합의 최솟값이 반환될 것이고 그래프는 최소 스패닝 트리의 형태를 띠게 될 것이다.

int main()
{
    CreateGraph();

    vector<CostEdge> selected;
    int cost = Kruskal(selected);
}