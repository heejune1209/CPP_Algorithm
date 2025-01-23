#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <thread>
using namespace std;

// 힙 정렬과 병합 정렬

// 힙 정렬
// 힙 정렬(Heap Sort)는 힙의 특성을 이용해 정렬하는 방식이다. 
// 힙 트리 구조에서는 우선순위 큐로 자연스럽게 가장 큰 수 혹은 가장 작은 수를 뽑아 접근하는 방식이다. 
// 배열의 모든 데이터를 우선순위 큐에 담고 큐가 빌 때까지 다시 배열에 재배치 하는 방식을 사용한다.
void HeapSort(vector<int>& v)
{
    priority_queue<int, vector<int>, greater<int>> pq;

    // O(Nlog N)
    for (int num : v)
        pq.push(num);

    v.clear();

    // O(Nlog N)
    while (!pq.empty())
    {
        v.push_back(pq.top());
        pq.pop();
    }

    cout << "HeapSort : ";
    for (int i : v)
        cout << i << " ";
    cout << endl;

    // 2Nlog N => 결과적으로 O(Nlog N)
}
// 작은 수부터 오름차순으로 정렬해야 하기 때문에 템플릿의 greater를 이용하였다(최소 힙 사용). 
// 힙 구조의 시간 복잡도는 O(log N)이고 데이터 수만큼 반복 실행하기 때문에 힙 정렬의 시간 복잡도는 O(Nlog N)이 된다.

// 병합 정렬
// 병합 정렬(Merge Sort, 혹은 합병 정렬)은 분할 정복(Divide and Conquer) 방식을 이용해 배열의 데이터를 정렬하는 방식이다. 
// 우선 데이터를 반씩 분할하여 정렬 후 다시 합치는 과정을 거치게 된다. 병합 정렬도 힙 정렬과 마찬가지로 O(Nlog N)의 시간 복잡도를 가진다.
// 이해할때 까지 디버깅으로 분석

// 병합 정렬은 나중에 멀티스레드 환경에서도 굉장히 유용해진다

// 분할 정복 (Divide and Conquer)
// - 분할 (Divide) : 문제를 더 단순하게 분할
// - 정복 (Conquer) : 분할된 문제를 해결
// - 결합 (Combine) : 결과를 취합하여 마무리
// - 복사(Copy) : 위 과정 모두 임시 배열에 저장하고 있는데, 이 배열을 원래 배열에 복사한다.

// 예시 풀어보기
// 정렬된 두 배열을 정렬된 채로 합치는 함수 만들어보기
vector<int> Merge(vector<int> a, vector<int> b)
{
    vector<int> temp;

    int leftIdx = 0;
    int rightIdx = 0;

    while (leftIdx < a.size() && rightIdx < b.size())
    {
        if (a[leftIdx] >= b[rightIdx])
        {
            temp.push_back(b[rightIdx]);
            rightIdx++;
        }
        else
        {
            temp.push_back(a[leftIdx]);
            leftIdx++;
        }
    }

    if (leftIdx >= a.size())
    {
        while (rightIdx < b.size())
        {
            temp.push_back(b[rightIdx]);
            rightIdx++;
        }
    }
    else
    {
        while (leftIdx < a.size())
        {
            temp.push_back(a[leftIdx]);
            leftIdx++;
        }
    }

    return temp;
}

// MergeResult : 여기서 배열의 해당 범위만큼 정렬 및 값 복사가 이뤄진다
// O(Nlog N) : 데이터가 N개가 있다고 한다면 N번을 비교해서 정렬해 줄테니까 기본적으로 N이 붙는다. 그리고 데이터가 많아지면 많아질수록 둘둘 비교하는부분이 계속 늘어나게 되고
// 합쳐지는것도 2배씩 데이터가 증가하는 형태로 합쳐지기 때문에 logN이다
void MergeResult(vector<int>& v, int left, int mid, int right)
{
    cout << "MR(" << left << ", " << mid << ", " << right << ")" << endl;
    int leftIdx = left;
    int rightIdx = mid + 1;

    vector<int> temp;
    while (leftIdx <= mid && rightIdx <= right)
    {
        if (v[leftIdx] <= v[rightIdx])
        {
            temp.push_back(v[leftIdx]);
            leftIdx++;
        }
        else
        {
            temp.push_back(v[rightIdx]);
            rightIdx++;
        }
    }

    // 왼쪽이 먼저 끝났으면 오른쪽 나머지 데이터 복사
    if (leftIdx > mid)
    {
        while (rightIdx <= right)
        {
            temp.push_back(v[rightIdx]);
            rightIdx++;
        }
    }
    else
    {
        while (leftIdx <= mid)
        {
            temp.push_back(v[leftIdx]);
            leftIdx++;
        }
    }

    for (int i = 0; i < temp.size(); i++)
        v[left + i] = temp[i];
}
// MergeSort : 분할하고 정렬하라는 큰 알고리즘
void MergeSort(vector<int>& v, int left, int right)
{
    cout << "Now MS(" << left << ", " << right << ")" << endl;
    if (left >= right)
    {
        cout << "Return MS(" << left << ", " << right << ")" << endl;
        return;
    }

    int mid = (left + right) / 2;
    MergeSort(v, left, mid); 
    MergeSort(v, mid + 1, right);

    MergeResult(v, left, mid, right);
}

int main()
{
    //vector<int> v{ 1, 5, 3, 2, 4, 6, 8, 7 };
    
    vector<int> v;

    srand(time(0));
    for (int i = 0; i < 50; i++)
    {
        int randValue = rand() % 100;
        v.push_back(randValue);
    }
    

    // HeapSort(v);
    cout << "--------------------------------\n";
    MergeSort(v, 0, v.size() - 1);

    cout << "MergeSort : ";
    for (int i : v)
        cout << i << " ";
    cout << endl;
    cout << "--------------------------------\n";
    //return 0;
    // 적용 문제
    vector<int> a{ 1, 3, 5, 7 };
    vector<int> b{ 2, 4, 6, 8 };
    vector<int> result = Merge(a, b);
    cout << "Merge Result" << endl;
    for (int num : result)
        cout << num << " ";
    cout << endl;
}