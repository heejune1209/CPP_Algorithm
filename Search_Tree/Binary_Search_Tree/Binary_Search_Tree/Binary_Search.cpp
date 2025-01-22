#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include "Binary_Search_Tree.h"
using namespace std;

// ���� Ž�� Ʈ��

// ���� Ž��(Binary Search)�� ��ü �����͸� ��ȸ�ϴ� ����� �ƴ� ������ ���� Ư���� ���� ��ġ�� ã�� �˰����̴�. 
// ���ĵ� �����Ͷ�� �������� ��Ȳ�� �����ϰ�, �߰����� ������ ������ �����Ͽ� ��� �񱳸� ���� Ž�� ������ ���������� ����̴�.

// ��Ȳ) �迭�� �����Ͱ� ���ĵǾ� �ִ�.
// [1][8][15][23][32][44][56][63][81][91]

// Q) 82��� ���ڰ� �迭�� �ֽ��ϱ�?
// A) 
// - ������ Ž��
//   - �ð� ���⵵�� O(N)�� �� ���̴�. -> ��ȿ����
// - ��� �� (���� Ž��)
//   - �ð� ���⵵�� O(log N)�� �� ���̴�. -> ȿ����

// ���ĵ� �迭�̱� ������ ���� Ž�� ����
// -- �迭�� ����
// --- �߰� ����/������ ������. => �ذ��ϱ� ���� ���� Ž�� "Ʈ��"�� ����
// - ���ĵ� ���� ����Ʈ�δ� �Ұ� (���� ���� X)
// �� Ž�� Ʈ���� �����Ͱ� ���������� ȿ���� ��û���� Ŀ����

vector<int> numbers;

void BinarySearch(int n)
{
    int left = 0;
    int right = (int)numbers.size() - 1;

    while (left <= right)
    {
        //cout << "Ž�� ����: " << left << "-" << right << endl;

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