#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <windows.h>
using namespace std;

// 동적 계획법 (DP : Dynamic Programming)
// 종종 코딩 면접에 나오는 경우도 있음
// 보통 동적 계획법은 굉장히 로우하게 들어가야지만 사용할 일이 생긴다

// 메모이제이션 (Memoization)
int cache[50][50];  // [n][r] // 결과물을 저장하는 임시 메모리

// Combination : 이향 계수
// Combination(n,k) = (n-1,k-1) + (n-1,k-1)
int Combination(int n, int r)
{
    // 기저 사례
    if (r == 0 || r == n)
        return 1;

    // 이미 답을 구한 적 있으면 바로 반환
    int& ret = cache[n][r];
    if (ret != -1)
        return ret;

    // 새로 답을 구해서 캐시에 저장
    return ret = Combination(n - 1, r - 1) + Combination(n - 1, r);
}
// 다이나믹 프로그래밍에서 가장 핵심적인 내용은 쪼개가지고 계산을 하는데 우리가 결과를 구한게 있으면 그거를 반복해서 계산을 하는것이 아니라 
// 메모이제이션이라고 해서 캐시를 이용해서 임시 저장을 한다음에 그 값을 재사용한다

// DP코들르 만들때는 각자 스타일을 하나 정해서 그것을 계속 지키는 것이 좋다
// 처음에는 기저 사례, 예외적인 케이스를 먼저 챙기고 그다음에 두번째로 메모이제이션, 즉 캐시를 먼저 확인을 하는 식으로 만들면 좋다
// 3번째로는 새로 답을 구해서 캐시에 저장한다. 이렇게 이루어 주면 된다

int main()
{
    // 캐시 초기화, 초기값을 -1로 초기화
    ::memset(cache, -1, sizeof(cache));

    __int64 start = GetTickCount64();
    int lotto = Combination(45, 6);
    __int64 end = GetTickCount64();

    cout << end - start << " ms" << endl;
}