#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

// 우선순위 큐
// 우선순위 큐(Priority Queue) 는 힙 트리 구조를 가지는 큐이다.
// 일반적인 큐와 다르게 우선순위를 지정하여 알맞은 조건에따라 대기열을 재배치하고 값을 반환한다.

// 컨테이너는 나중에 유동적으로 선택활 수 있게끔 typename Container 이렇게 만들어 주고 기본적으로는 벡터를 받아줄것이기 때문에 디폴트 값은 벡터로 놓는다.
// 오름차순으로 반환하고 싶으면 세 번째 인자를 설정해주면 된다.
template<typename T, typename Container = vector<T>, typename Predicate = less<T>>
class PriorityQueue
{
public:
	void push(const T& data)
	{
		// 우선 힙 구조부터 맞춰주기
		_heap.push_back(data);

		// 도장깨기 시작
		// 대소 비교하며 정리하기
		int now = static_cast<int>(_heap.size()) - 1;	// 방금 넣은 데이터의 인덱스
		// 루트 노드까지 반복
		while (now > 0)
		{
			// 부모 노드와 비교해서 더 작으면 패배
			int parent = (now - 1) / 2;
			if (_predicate(_heap[now], _heap[parent]))
				break;
			// _predicate가 std::less<T>일 때(최대 힙일때)
			// _predicate(a, b) => a < b

			// _predicate가 std::greater<T>일 때(최소 힙일때)
			// _predicate(a, b) => a > b

			// 데이터 교체
			::swap(_heap[now], _heap[parent]);
			now = parent;
		}
	}

	void pop()
	{
		// 최상위 노드 제거 + 최하위 노드를 최상위로 올리기
		_heap[0] = _heap.back();
		_heap.pop_back();

		// 최상위 노드부터 시작
		int now = 0;
		while (true)
		{
			int left = (2 * now) + 1;
			int right = (2 * now) + 2;

			// 리프에 도달한 경우
			if (left >= _heap.size())
				break;

			int child = now;

			// 왼쪽과 비교
			if (_predicate(_heap[child], _heap[left]))
				child = left;

			// 둘 중 승자를 오른쪽과 비교
			if (right < _heap.size() && _predicate(_heap[child], _heap[right]))
				child = right;

			// 왼쪽/오른쪽 둘 다 현재 값보다 작으면 종료
			if (child == now)
				break;

			// 데이터 교체
			::swap(_heap[now], _heap[child]);
			now = child;
		}
	}

	T& top()
	{
		return _heap[0];
	}

	bool empty()
	{
		return _heap.empty();
	}
private:
	Container _heap = {};
	Predicate _predicate = {};
};

int main()
{
    // priority_queue<int, vector<int>, greater<int>> pq;	// 최소 힙(min heap), 이러면 작은 순서대로 반환된다
	priority_queue<int> pq; // 최대 힙(max heap) , // Container = vector<int>, Predicate = less<int>

    pq.push(100);
    pq.push(300);
    pq.push(200);
    pq.push(500);
    pq.push(400);

    while (pq.empty() == false)
    {
        int value = pq.top(); 
        // top은 우선순위 큐에서 가장 우선순위가 높은 요소를 반환한다.
        // 기본적으로 가장 큰 값이 반환되며, 사용자 정의 비교 함수를 사용해 우선순위를 변경할 수 있다.
        // top은 현재 우선순위 큐의 최우선 요소를 읽는다. 반환된 요소는 읽기 전용이며, 수정할 수 없다.
        pq.pop();

        cout << value << endl;
        // 위와 같은 코드가 있다고 했을 때, 결과값은 선입선출 순이 아닌 내림차순으로 반환된다.
        // 기본적으로 힙 트리 구조는 최대 힙(Max Heap) 트리 구조이다. (내림차순)

		// 우선순위 큐의 시간 복잡도는 O(log N)이다. 그리고 push와 pop도 log n이다
		// 왜 push와 pop도 log n이냐면, 데이터를 하나 추가하면 추가된 데이터부터 시작을 해서 도장깨기를 위로 한다. 
		// 이 도장깨기 횟수가 연산해야되는 횟수라고 볼 수 있는건데 이 트리구조를 보면
		// 데이터가 아무리 많아도 층 자체는 생각보다 느리게 증가한다. 왜냐면 트리라는 것 자체가 데이터가 2배씩 증가하고 있기 때문이다. 
		// 그렇기 때문에 이런식으로 계속 2배가 늘어난다라는 구조가 있을 때는 로그를 생각해 주면 된다

		// 그래서 앞으로 이제 뭐 그냥 제일 작은 값 혹은 제일 큰 값 둘 중 하나를 데이터 중에서 
		// 계속 추출해야 되는 그런 필요성이 생긴다고 하면은 이 priority_queue를 우선적으로 떠올려 주면 된다.
		
		// 우선 순위 큐의 경우 사실 안보고도 한번 연습 삼아서 구현해보는것도 괜찮다
    }
}

