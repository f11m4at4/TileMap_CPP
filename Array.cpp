// --------------------------------------
// -------------- �������� ���� ---------
// --------------------------------------
#include <iostream>
using namespace std;

void main()
{
    int arr[] = { 2, 0, 3, 1, 4, 6, 9, 7, 8, 5 };
    int n = sizeof(arr) / sizeof(arr[0]);

    // ���� ������ ����� �������� ����
    for (int i = 0; i < n - 1; i++) {
        cout << "i : " << i << " -> ";
        for (int j = 0; j < n - 1 - i; j++) {
            // ��������: ���� ���� ������ ���� ��
            if (arr[j] > arr[j + 1]) {  
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }

            cout << "j[" << j << "]" << arr[j] << ", ";
        }
        cout << endl;
    }

    // ���ĵ� �迭 ���
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
    cout << endl;
}

