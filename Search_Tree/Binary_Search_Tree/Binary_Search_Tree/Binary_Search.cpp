#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include "Binary_Search_Tree.h"
using namespace std;

// 이진 탐색 트리

// 이진 탐색(Binary Search)는 전체 데이터를 순회하는 방법이 아닌 조건을 정해 특정한 값의 위치를 찾는 알고리즘이다. 
// 정렬된 데이터라는 제한적인 상황이 존재하고, 중간값을 임의의 값으로 선택하여 대소 비교를 통해 탐색 범위를 좁혀나가는 방식이다.

// 상황) 배열에 데이터가 정렬되어 있다.
// [1][8][15][23][32][44][56][63][81][91]

// Q) 82라는 숫자가 배열에 있습니까?
// A) 
// - 순차적 탐색
//   - 시간 복잡도는 O(N)이 될 것이다. -> 비효율적
// - 대소 비교 (이진 탐색)
//   - 시간 복잡도는 O(log N)이 될 것이다. -> 효율적

// 정렬된 배열이기 때문에 이진 탐색 가능
// -- 배열의 단점
// --- 중간 삽입/삭제가 느리다. => 해결하기 위해 이진 탐색 "트리"를 만듦
// - 정렬된 연결 리스트로는 불가 (임의 접근 X)
// 이 탐색 트리는 데이터가 많아질수록 효율이 엄청나게 커진다

vector<int> numbers;

void BinarySearch(int n)
{
    int left = 0;
    int right = (int)numbers.size() - 1;

    while (left <= right)
    {
        //cout << "탐색 범위: " << left << "-" << right << endl;

        int mid = (left + right) / 2;

        if (n < numbers[mid])
        {
            //cout << n << "<" << numbers[mid] << endl;
            right = mid - 1;
        }
        else if (n > numbers[mid])
        {
            //cout << n << ">" << numbers[mid] << endl;
            left = mid + 1;
        }
        else
        {
            //cout << "Found!" << endl;
            break;
        }
    }
}
//   [20]
// [10]  [30]
//[9]  [25]  [40]
//        [26]  [50]

int main()
{
    numbers = vector<int>{ 1, 8, 15, 23, 32, 44, 56, 63, 81, 91 };
    BinarySearch(82);

    BinarySearchTree bst;
    bst.Insert(20);
    bst.Insert(10);
    bst.Insert(30);
    bst.Insert(25);
    bst.Insert(26);
    bst.Insert(40);
    bst.Insert(50);
    bst.Insert(9);

    bst.Delete(26);

    bst.Print();
    //bst.Delete(26);

}