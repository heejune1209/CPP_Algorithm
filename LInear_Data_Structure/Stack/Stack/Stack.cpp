#include <iostream>
#include <vector>
#include <list>
#include <stack>
using namespace std;

// Stack (LIFO Last-In-First-Out 후입선출)

// 활용
// - 되돌리기 같은 기능을 구현할 때 (Ctrl+Z)

// Stack 구현
// 동적 배열 사용 방식과 연결 리스트 사용 방식으로 나눌 수 있다.
// 디폴트 값으로 만약에 그냥 벡터로 놓고 얘를 이제 옵션으로 설정할 수 있다
// 기본 상태에선 벡터로 놓고 옵션으로 설정할 수 있다
template<typename T, typename Container = vector<T>>
class Stack
{
public:
    void push(const T& value)
    {
        _container.push_back(value);
    }

    /*
    // 성능 때문에 잘 사용하지 않는다. -> top 사용 선호
    T pop()
    {
        T ret = _data[_size - 1];
        _size--;
        return ret; // T(const T&)
    }
    */
    // C++에선 top을하고 pop을 하고 두번에 거쳐서 데이터를 꺼내 쓰는게 낫다

    void pop()
    {
        _container.pop_back();
    }

    T& top()
    {
        return _container.back();
    }

    bool empty() { return _container.empty(); }
    int size() { return _container.size(); }
private:
    //vector<T> _container;
    //list<T> _container; // 동일한 동작을 한다. -> STL 컨테이너의 장점(인터페이스 통일화)
    Container _container;
};


int main()
{
    // 이렇게 기본 상태에선 벡터를 이용
    //Stack<int> s;
    
    // 연결 리스트 방식으로 동작
    Stack<int, list<int>> s;

    // 삽입
    s.push(1);
    s.push(2);
    s.push(3);

    // 스택이 비었는지 확인
    while (s.empty() == false)
    {
        // 최상위 원소
        int data = s.top(); // top은 스택의 가장 위(마지막)에 있는 요소를 반환한다
        // 최상위 원소 삭제
        s.pop();

        cout << data << endl;
    }
    // 스택 사이즈
    int size = s.size();

}

