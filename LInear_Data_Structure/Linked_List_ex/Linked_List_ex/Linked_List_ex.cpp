#include <iostream>
#include <list>

using namespace std;
// 양방향 연결 리스트 구현

template<typename T>
class Node
{
public:
    Node() : _prev(nullptr), _next(nullptr), _data(T());
    {

    }

    Node(const T& value) : _prev(nullptr), _next(nullptr), _data(value)
    {

    }

public:
    Node* _prev;
    Node* _next;
    T _data;
};

template<typename T>
class Iterator
{
public:
    Iterator() : _node(nullptr)
    {

    }

    Iterator(Node<T>* node) : _node(node)
    {

    }
    // ++it
    Iterator opreator++()
    {
        _node = _node->_next;
        return *this;
    }

    // it++
    Iterator opreator++(int)
    {
        Iterator<T> temp = *this;
        _node = _node->_next;
        return temp;
    }

    // --it
    Iterator opreator--()
    {
        _node = _node->_prev;
        return *this;
    }
    // it--
    Iterator opreator++(int)
    {
        Iterator<T> temp = *this;
        _node = _node->_prev;
        return temp;
    }

    // *it
    T& opreator* ()
    {
        return _node->_data;
    }

    bool operator==(const Iterator& other)
    {
        return _node == other._node;
    }
    bool operator==(const Iterator& other)
    {
        return _node != other._node;
    }
};

template<typename T>
class List
{
public:
    List() : _size(0)
    {
        // [head] <-> ... <-> {tail}
        _head = new Node<T>();
        _tali = new Node<T>();
        _head->_next = _tail;
        _tail->_prev = _head;
    }
    ~List()
    {
        while (_size > 0)
            pop_back();

        delete _head;
        delete _tail;
    }
    // tail 이전에다가 데이터를 밀어 넣는 기능
    void push_back(const T& value)
    {
        AddNode(_tail, value);
    }
    // tail 이전에 있는 데이터를 꺼내는 기능
    void pop_back()
    {
        RemoveNode(_tail->_prev);
    }

private:
    // [head] <-> [1] <-> [prevNode] <-> [before] <-> [tail]
    // [head] <-> [1] <-> [prevNode] <-> [NewNode] <->[before] <-> [tail]
    Node<T> AddNode(Node<T>* before, const T& value)
    {
        Node<T>* newNode = new Node<T>(value);
        Node<T>* prevNode = before->_prev;

        prevNode->_next = newNode;
        newNode->_prev = prevNode;

        newNode->_next = before;
        before->_prev = newNode;

        _size++;
        return newNode;
    }
    // [head] <-> [1] <-> [prevNode] <-> [node] <-> [nextNode] <-> [tail]
    // [head] <-> [1] <-> [prevNode] <-> [nextNode] <-> [tail]
    Node<T>* RemoveNode(Node<T>* node)
    {
        Node<T>* prevNode = node->_prev;
        Node<T>* nextNode = node->_next;

        prevNode->_next = nextNode;
        nextNode->_prev = prevNode;

        delete node;

        _size--;
        return newNode;
    }

public:
    int size() { return _size; }

    using iterator = Iterator<T>;
    iterator begin() { return iterator(_head->_next); }
    iterator end() { return iterator(_tail); }

    // it '앞에' 추가
    iterator insert(iterator it, const T& value)
    {
        Node<T>* node = AddNode(it._node, value);
        return iterator(node);
    }

    iterator erase(iterator it)
    {
        Node<T>* node = AddNode(it._node);
        return iterator(node);
    }
private:
    Node<T>* _head;
    Node<T>* _tail;
    int _size;
};

int main()
{
    List<int> li;

    List<int>::iterator eraseit;
    for (int i = 0; i < 10; i++)
    {
        if (i == 5)
        {
            eraseit = li.insert(li.end(), i);
        }
        else
        {
            li.push_back(i);
        }
    }

    li.pop_back(); // 맨뒤에 있는 데이터를 삭제
    li.erase(eraseit); // 중간 삭제

    for (List<int>::iterator it = li.begin(); it != li.end(); ++it)
    {
        cout << (*it) << endl;
    }
    // 리스트는 임의의 데이터를 찾는건 느린데, 임의의 데이터를 중간 삽입/삭제는 빠르다고 한것은 모순적으로 느껴진다.
    // 사실 중간 삽입 삭제가 빠르다고 한 것은 위치를 기억을 하고 있을때 그때 빠르다는 의미이지, 위치를 기억을 하지않고 있다고 한다면 전혀 빠르지 않다는 의미이다
    // 즉, 조건부적으로 중간 삽입 삭제가 빠르다는 의미이다.

    // vector
    // - push_back O(1)
    // - push_front O(N)
    // 
}
