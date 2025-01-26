#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

// 그래프/트리 응용
// 최소 스패닝 트리 (MST : Minimum Spanning Tree)

// 서로소 집합 or 상호 배타적 집합 (Disjoint Set) , 공통된 원소를 가지고 있지 않는 집합을 의미
// -> 유니온-파인드 (Union-Find) : 합치기-찾기

// Lineage Battleground (혼종!)
// 혈맹전 + 서바이벌
// 1인 팀 1000명 (팀id 0~999)
// 동맹 (1번팀 + 2번팀 = 1번팀)
// 이 자료 구조는 최소 스패닝 트리(Minimum Spanning Tree)를 만드는 데 사용된다.

void LineageBattleground()
{
    struct User
    {
        int teamId;
        // TODO
    };

    // TODO : UserManager
    vector<User> users;
    for (int i = 0; i < 1000; i++)
    {
        users.push_back(User{ i });
    }

    // 팀 동맹
    // users[1] <-> users[5]
    users[5].teamId = users[1].teamId;  // 1

    // teamId = 1인 팀과 teamId = 2인 팀이 통합
    for (User& user : users)
    {
        if (user.teamId == 1)
            user.teamId = 2;
    }

    // 찾기 연산 : O(1)
    // 합치기 연산 : O(N)
}

// 트리 구조를 이용한 상호 배타적 집합의 표현
struct Node
{
    Node* leader;
};

// 합병 : 트리 구조에 포함시킨다.
// 시간 복잡도 : 트리의 높이에 비례한 시간이 걸림
// -> O(Ackermann(n)) = O(1) (Disjoint Set 사용 시)
class NaiveDisjointSet
{
public:
    // 입력되는 n의 개수만큼 _parent의 배열 생성되고 생성자에서 반복문으로 배열에 요소를 집어 넣는다
    NaiveDisjointSet(int n) : _parent(n)
    {
        for (int i = 0; i < n; i++)
            _parent[i] = i;
    }

    // 서로소 집합 구조에서 원하는 노드를 찾는 Find 연산이다. 여기서 원하는 노드는 해당 노드의 가장 위에 있는 노드, 즉 나를 가지고 있는 루트 노드를 찾아서 반환해준다. 
    // 깊이가 깊은 노드라도 재귀적으로 함수를 호출하여 부모의 부모 노드를 찾아 타고 올라가는 방식으로 루트 노드를 찾는다.
    // 리더 찾기
    int Find(int u)
    {
        if (u == _parent[u])
            return u;

        return Find(_parent[u]); // 루트 노드의 값을 반환
    }

    // 서로 공통적인 요소를 가지고 있지 않는 트리 구조의 데이터들을 하나로 합쳐주는 Union 연산이다. 
    // 두 개의 노드를 합치게 되면 그 즉시 연산이 이루어진다.
    // u와 v를 병합 (u가 v 산하로)
    void Merge(int u, int v)
    {
        u = Find(u);
        v = Find(v);

        if (u == v)
            return;

        _parent[u] = v;
    }
    // 하지만 여기서 문제가 발생할 수 있다. 
    // 단 한 번도 루트 노드나 부모 노드와 병합하지 않고 리프 노드에만 병합을 시도하게 된다면 연결 리스트와 같은 구조의 트리 모양이 생성될 것이다. 
    // 그렇게 되면 시간 복잡도는 최악의 경우 O(N)을 가지게 된다.
private:
    vector<int> _parent;
};
// 초기화
// 서로소 집합의 초기화는 노드의 개수만큼 부모 노드를 담는 배열 _parent를 생성한다. 
// 집합을 생성할 때에는 부모 노드가 연결되어 있지 않으므로 자기 자신을 가리킨다. 그리고 모든 노드는 연결되어 있지 않는 상태로 생성이 된다.


// 트리가 한 쪽으로 기우는 문제를 해결하려면?
// 트리를 합칠 때, 항상 [높이가 낮은 트리]를 [높이가 높은 트리] 밑으로 이동
// (Union-By-Rank) 랭크에 의한 합치기 최적화
// 위에서 본 문제를 해결하기 위한 방법이 존재한다. 바로 경로 압축(Path Compression)과 유니온 바이 랭크(Union By Rank)라는 두 가지 방법이다.
class DisjointSet
{
public:
    DisjointSet(int n) : _parent(n), _rank(n, 1)
    {
        for (int i = 0; i < n; i++)
            _parent[i] = i;
    }

    // 경로 압축(Path Compression)은 말 그대로 트리의 높이를 줄여 원하는 노드까지 도달하는 경로를 단축시켜주는 방법이다. 
    // Find 연산에서 재귀적으로 함수를 호출할 때마다 해당 노드의 부모 노드를 설정해주면 
    // 연결 리스트처럼 생성된 선형 트리의 구조에서 자식, 자손 노드들은 모두 루트 노드로 붙게되며 경로가 줄어들게 될 것이다.
    // 리더 찾기
    int Find(int u)
    {
        if (u == _parent[u])
            return u;

        return _parent[u] = Find(_parent[u]);
    }
    // return 부분의 코드에서 부모 노드의 값을 바꿔주면 쉽게 트리를 최적화할 수 있다.
     
    
    // 유니온 바이 랭크(Union By Rank)는 Union 연산 시 사용되는 방법이며 
    // 말 그대로 합칠 때마다 랭크(순위)를 부여하여 비교해서 더 나은 방향으로 트리를 합치도록 방향을 제시해주는 방법이다. 
    // 보통 랭크로는 트리의 사이즈(Size) 혹은 높이(Height)를 설정해줄 수 있는데, 
    // 여기서는 랭크를 높이로 설정하여 최적화하는 방법에 대해 알아보자.
    // u와 v를 병합 (u가 v 산하로)
    void Merge(int u, int v)
    {
        // 합쳐질 두 노드의 루트 노드를 찾아주는 Find 연산을 통해 각각의 트리 구조에서 경로 압축이 실행, u와 v는 루트 노드로 변경
        u = Find(u);
        v = Find(v);

        // 두 값이 일치하면 동일한 루트 노드를 가지는 노드들이므로 바로 종료
        if (u == v)
            return;

        // 산하로 들어갈 u 노드의 트리가 v 노드의 트리보다 높다면 서로 값을 교환해주는 예외 처리
        if (_rank[u] > _rank[v])
            swap(u, v);

        // rank[u] <= rank[v] 보장됨

        // u가 v 산하로
        _parent[u] = v;

        // 두 트리의 높이가 동일하다면 한 트리가 산하로 들어가면서 높이가 1 증가하게 되니 랭크의 값도 1 증가
        if (_rank[u] == _rank[v])
            _rank[v]++; // v의 랭크를 1올림
    }
private:
    vector<int> _parent;
    vector<int> _rank;
};
// 이렇게 트리의 최적화를 해주게 되면 최악의 상황인 O(N)의 시간 복잡도가 나오지 않고 
// 트리는 계속 재구성되며 그리 높지 않은 구조를 유지하게 될 것이다. 
// 이러한 과정을 거쳐 평평한 트리가 완성되면 Find 연산은 O(1)의 시간 복잡도를 갖게 될 것이다.
// 정확하게 말하자면 Find 연산은 호출할 때마다 수행 시간이 변한다(트리의 경로 압축, 재구성 때문). 
// 따라서 Find 연산의 실제 시간 복잡도는 O(Ackermann(N))이다

// 결국 DisjointSet 서로 팀을 나눈 다음에 팀끼리 합쳐질 때 정말로 그 역할에 최적화되어 있는 알고리즘.
// 그래서 만약에 우리가 이 리니지 배틀그라운드라는 게임을 만들 때 서로 모두 같은 팀이 아닌 서로 다른 팀인 상태에서 막 팀들이 둘둘둘둘 합쳐지는 상황이 계속 빈번하게 발생을 한다고 가정을 하면은 
// 요런 DisjointSet라는 걸 이제 적극적으로 활용할 수도 있다 라는 얘기가 된다

int main()
{
    // 7의 부모를 1로 만들어버림
    /*NaiveDisjointSet team(10);
    team.Merge(7, 1);
    int teamid = team.Find(7);
    int teamid2 = team.Find(1);*/

    DisjointSet teams(20);

    teams.Merge(10, 1);
    int teamId = teams.Find(1);
    int teamId2 = teams.Find(10);

    teams.Merge(3, 2);
    int teamId3 = teams.Find(3);
    int teamId4 = teams.Find(2);

    teams.Merge(1, 3);
    int teamId6 = teams.Find(1);
    int teamId7 = teams.Find(3);
    int teamId8 = teams.Find(10);

    teams.Merge(5, 7);
    int teamId9 = teams.Find(5);
    int teamId10 = teams.Find(7);

    teams.Merge(9, 11);
    teams.Merge(7, 9);

    teams.Merge(5, 2);
}