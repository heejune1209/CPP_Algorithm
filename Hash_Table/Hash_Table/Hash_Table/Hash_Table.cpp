#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <thread>
using namespace std;

// 해시 테이블

// 해시 테이블(Hash Table)이란 Key와 Value로 이루어진 자료 구조 중의 하나이다. 
// 키와 밸류를 매핑하여 키 값 만으로 데이터를 빠르게 추가, 탐색, 삭제할 수 있다. 
// 앞서 보았던 이진 탐색 트리(Binary Search Tree)가 O(log N)의 시간 복잡도를 가지는데, 
// 해시 테이블은 키 값만 알면 데이터를 바로 찾을 수 있기 때문에 O(1)의 시간 복잡도를 가진다. 대신에 메모리 소모는 다른 자료 구조보다 훨씬 많다.

// Hash Function을 이용하여 나온 키의 해시값을 버킷의 인덱스(Index)와 매핑한다. 데이터는 버킷(Bucket)이라는 이름의 테이블(배열)에 저장된다.

// 키의 해시값을 사용하는 이유?
// 우선, 키 값의 원본 자체를 사용한다고 가정하면 탐색 시 모든 키 값을 탐색해야 하기 때문에 시간 복잡도는 O(N)이 된다.
// 하지만 해시 함수를 이용하면 키 값에 해당하는 해시값만 계산하여 데이터를 바로 찾을 수 있기 때문에 속도 측면에서 굉장히 빨라지게 된다.
// 또한 키 값의 크기가 아무리 커져도 해시 함수를 거치면 일정한 길이 이하로만 결과가 도출되기 때문에 사이즈 관리에 훨씬 효율적이라고 볼 수 있다.
// 그래서 해시 테이블이라는 용어가 붙은거는 테이블로 관리를 하되 해시값을 추출해가지고 키값을 어디다가 활용할지를 고르기 떄문에 용어가 이렇게 붙은것이다

// Q) map vs hash map (C++11 표준 unordered map) 

// map : Red-Black Tree 구조로 이루어져있음
// - 추가/탐색/삭제 O(log N)
// 맵은 레드블랙 트리 소위 균형 이진 트리 구조로 만들어져 있어 가지고 트리 구조로 관리를 하고 그러면서 이제 데이터가 추가되거나 삭제가 되면은 이진 트리를
// 유지를 하지만 거기다 균형을 또 맞춰 줘 가지고 한쪽으로 쏠리는 걸 예방하는 형태로 되어 있다 그리고 시간 복잡도는 이제 logN 을 따른다.
// 하지만 속도적인 측면에서 보면은 해시맵이 훨씬 더 빠르다
// 해시맵은 메모리를 내주고 그 대신 속도를 취하는 방법이기 때문에 속도 측면에서 보면은 이제 충돌이 일어나지 않는다고 가정을 하면 해시맵이 훨씬 더 빠르다라는 부분까지만 
// 일단은 꼭 인지를 하고 그거를 대답할 수 있어야 됩니다
// 그리고 로직에 대해서 어떤 원리로 동작하는지에 대해서만 이해를 하는게 중요

// C# dictionary = C++ map (X)
// C# dictionary = C++ unordered map (O)

// hash map (unordered map)
// - 추가/탐색/삭제 O(1)

// 레드 블랙 트리처럼 정렬을 해주고 데이터가 크게 따라가지고 맞춰주고 하는 개념은 아니고 원하는 키값을 굉장히 빠르게 찾아주는데만 최적화가 되어있다

// 메모리를 내주고 속도를 취한다

// 결론적으로 맵과 해시 맵은 전혀 다른 구조로 되어있기 때문에 같은 맵 형식의 컨테이너일지라도 데이터의 추가, 탐색, 삭제에서 시간 복잡도의 차이가 크다는 것을 알 수 있다.

// O(1)
void TestTable()
{
    struct User
    {
        int userId = 0; // 1 ~ 999
        string username;
    };

    vector<User> users;
    users.resize(1000);

    // 777번 유저 정보 세팅
    users[777] = User{ 777, "Dandi" };

    // 777번 유저 이름은?
    string name = users[777].username;
    cout << name << endl;

    // 테이블
    // 키를 알면 데이터를 단번에 찾을 수 있다.

    // 문제의 상황
    // int32_max (3억~)
}

void TestHash()
{
    struct User
    {
        int userId = 0; // 1 ~ int32_max
        string username;
    };

    vector<User> users;
    users.resize(1000);

    const int userId = 123456789;
    int key = (userId % 1000);  // hash < 고유번호, 한번더 변환을 해서 사용

    // 123456789번 유저 정보 세팅
    users[key] = User{ userId, "heejune" };

    // 123456789번 유저 이름은?
    User& user = users[key];
    if (user.userId == userId)
    {
        string name = users[key].username;
        cout << name << endl;
    }
    // Hash Collision
    // 이렇게 키 값을 해싱하여 나온 값들을 배열의 인덱스로 사용하게 되는데, 그러다보면 키의 해시값이 겹치는, 즉 충돌하는 상황이 발생할 수 있는 가능성도 존재한다.
    // 이렇게 키의 해시값이 충돌할 때 해결할 수 있는 방법에는 오픈 어드레싱(Open Addressing)과 체이닝(Chaining)이 존재한다.

    // 고로 해시 함수(Hash Function)를 설계할 때 효율적으로 잘 설계해야 한다.

    // 충돌 문제
    // 충돌이 발생한 자리를 대신해서 다른 빈자리를 찾아나서면 된다.

    // Open Addressing
    // 해당 기법은 충돌이 발생한 자리를 대신하여 다른 빈자리를 찾아나서는 기법이다.
    // 대표적으로 선형 조사법(Linear Probing)과 이차 조사법(Quadratic Probing)이 있다.
    
    // - 선형 조사법 (Linear Probing)
    // 선형 조사법은 해시값 충돌시 바로 다음 인덱스 값을 확인하여 비어있으면 데이터를 추가, 
    // 비어있지 않으면 또다시 인덱스 값을 증가시켜 확인하여 데이터를 추가하는 방법이다. 인덱스의 증가값이 일정하다.
    // 비어있는 자리를 찾을 때까지 버킷을 순회한다.
    // hash(key) + 1 -> hash(key) + 2
    
    // - 이차 조사법 (Quadratic Probing)
    // 이차 조사법은 해시값이 충돌할 때마다 인덱스의 증가값을 제곱하여 탐색하는 조사법이다. 
    // 첫 충돌시 1의 제곱만큼 인덱스 증가, 두 번째 충돌시 2의 제곱만큼 인덱스 증가하는 식으로 인덱스를 증가시킨다.
    // hash(key) + 1^2 -> hash(key) + 2^2

    // 체이닝
    // 해당 기법은 말 그대로 충돌시 기존의 데이터의 다음으로 추가할 데이터를 연결시키는 기법이다. 
    // 키의 해시값이 겹칠 경우를 대비하여 테이블 구조를 이차 배열로 만들게 되면 키의 해시값이 충돌하여도 데이터를 추가하는 덴 문제가 없을 것이다.
    // 다만, 이런식의 자료 구조는 O(1)의 시간 복잡도를 보장할 수 없고 해당 해시값과 연결된 모든 리스트를 탐색해야 하게 될 것이다.
}

// O(1)
void TestHashTableChaining()
{
    struct User
    {
        int userId = 0; // 1 ~ int32_max
        string username;
    };

    vector<vector<User>> users; // 테이블 구조를 이차 배열로
    users.resize(1000); // 버킷 사이즈를 1000개로

    const int userId1 = 123456789; // 첫 번째 유저 ID
    const int userId2 = 223456789; // 두 번째 유저 ID (충돌 발생)
    
    int key1 = userId1 % 1000;  // 첫 번째 해시값
    int key2 = userId2 % 1000;  // 두 번째 해시값 (key1과 동일한 값을 가지도록 설정)

    // 유저 정보 세팅 (충돌 처리)
    users[key1].push_back(User{ userId1, "heejune" });
    users[key2].push_back(User{ userId2, "heejune2" });

    // 첫 번째 유저 이름 출력
    vector<User>& bucket1 = users[key1];
    cout << "Bucket for key " << key1 << ":" << endl;
    for (User& user : bucket1)
    {
        cout << "UserId: " << user.userId << ", Username: " << user.username << endl;
    }

    // 두 번째 유저 이름 확인 (같은 버킷에 저장되어 있는지 확인)
    vector<User>& bucket2 = users[key2];
    cout << "Retrieving users from bucket with collisions:" << endl;
    for (User& user : bucket2)
    {
        if (user.userId == userId2)
        {
            string name = user.username;
            cout << "Found userId: " << user.userId << " with username: " << name << endl;
        }
    }
}

int main()
{
    // TestTable();
    // TestHash();
    TestHashTableChaining();
}