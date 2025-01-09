#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

// Queue (FIFO First-in-First-out 선입선출)

// front << [1][2][3][4] << rear(back)
// 예시) 대기열

template<typename T>
class ArrayQueue // 순환 구조 큐를 이용
{
public:
    ArrayQueue()
    {
        //_container.resize(100);
    }
    void push(const T& value)
    {
        // TODO : 다 찼는지 체크
        if (_size == _container.size())
        {
            // 증설 작업
            int newSize = max(1, _size * 2); // max 함수는 둘 중 큰 값을 리턴
            vector<T> newData;
            newData.resize(newSize);

            // 데이터 복사
            for (int i = 0; i < _size; i++)
            {
                int index = (_front + i) % _container.size();
                newData[i] = _container[index];
            }
            // 교체
            _container.swap(newData); // swap은 인수로 집어넣는 벡터의 요소들로 교체하는 함수
            _front = 0;
            _back = _size;
        }

        _container[_back] = value;
        // 나머지 연산으로 빠져나가지 않게 범위 안에 돌게 한다
        _back = (_back + 1) % _container.size();
        _size++;
    }
    void pop()
    {
        _front = (_front + 1) % _container.size();
        _size--;
    }
    T& front()
    {
        return _container[_front];
    }
    bool empty()
    {
        return _size == 0;
    }
    int size()
    {
        return _size;
    }
private:
    vector<T> _container;
    
    int _front = 0;
    int _back = 0;
    int _size = 0;

};

//template<typename T>
//class ListQueue
//{
//public:
//    void push(const T& value)
//    {
//        _container.push_back(value);
//    }
//    void pop()
//    {
//        // 효율이 좋지 않다. (동적 배열에서의 앞 요소 추가 및 삭제)
//        // _container.erase(_container.begin());
//        // 리스트 사용
//        _container.pop_front();
//    }
//    T& front()
//    {
//        return _container.front();
//    }
//    bool empty()
//    {
//        return _container.empty();
//    }
//    int size()
//    {
//        return _container.size();
//    }
//private:
//    list<T> _container;
//    // 덱(deque)을 사용해도 리스트와 비슷한 속도로 동작된다.(원래 코드는 덱으로 되어있음)
//};

int main()
{
    //ListQueue<int> q;
    ArrayQueue<int> q;
    for (int i = 0; i < 100; i++)
    {
        q.push(i);
    }
    while (q.empty() == false)
    {
        int value = q.front();
        q.pop();
        cout << value << endl;
    }
    int size = q.size();
}

