#include <iostream>
#include <vector>
using namespace std;

// 벡터 구현 연습
template<typename T>
class Vector
{
public:
    Vector()
    {
        
    }
    ~Vector()
    {
        if (_data)
            delete[] _data;
    }
    void push_back(const T& value)
    {
        if (_size == _capacity)
        {
            // 증설 작업
            int newCapacity = static_cast<int>(_capacity * 1.5);
            
            if (newCapacity ==_capacity)
                newCapacity++;

            reserve(newCapacity);
        }

        // 데이터 저장
        _data[_size] = value;

        // 데이터 개수 증가
        _size++;
    }
    
    // 인수로 원하는 용량을 받아주면 늘려주는 역할
    void reserve(int capacity)
    {
        if (_capacity >= capacity)
            return;

        _capacity = capacity;

        T* newData = new T[_capacity];
        
        // 데이터 복사
        for (int i = 0; i < _size; i++)
        {
            newData[i] = _data[i];
        }

        if (_data)
            delete[] _data;

        // 교체
        _data = newData;
    }
    // position을 요구하면 데이터의 포지션을 뱉어주는 기능
    T& operator[](const int pos) { return _data[pos]; }

    int size() { return _size; }
    int capacity() { return _capacity; }

    void clear()
    {
        if (_data)
        {
            delete[] _data;
            _data = new T[_capacity];
        }
        _size = 0;
    }


private:
    T* _data = nullptr;
    int _size = 0;
    int _capacity = 0;
};

int main()
{
    Vector<int> v;

    v.reserve(100);
    cout << v.size() << " " << v.capacity() << endl;
    for (int i = 0; i < 100; i++)
    {
        v.push_back(i);
        cout << v.size() << " " << v.capacity() << endl;
    }

    v.clear();
    cout << v.size() << " " << v.capacity() << endl;

    return 0;
}

