
#include <iostream>
using namespace std;
//void Interchange_Sort(int a[], int n) {
//	int i, j;
//	for (i = 0; i < n - 1; i++) {
//		for (j = i + 1; j < n; j++) {
//			if (a[i] > a[j]) {
//				swap(a[i], a[j]);
//			}
//		}
//	}
//}
//void selectionSort(int a[], int n) {
//	int i, j, min, temp;
//	for (i = 0; i < n - 1; i++) {
//		min = i;
//		for (j = i + 1; j < n; j++) {
//			if (a[j] < a[min]) min = j;
//		}
//		swap(a[i], a[min]);
//	}
// void bubbleSort(int a[], int n) {
//		int i, j;
//		for (i = (n - 1); i >= 0; i--) {
//			for (j = 1; j <= i; j++) {
//				if (a[j - 1] > a[j])
//					swap(a[j - 1], a[j]);
//			}
//		}
//	}
void QuickSort(int a[], int left, int right,int n) {
	int i, j, x;
	x = a[(left + right) / 2];
	i = left; j = right;
	do
	{
		while (a[i] < x); i++;
		while (a[j] > x); j--;
		if (i <= j)
		{
			swap(a[i], a[j]);
			i++; j--;
			for (int i = 0; i < n; i++)
			{
				cout << a[i] << " ";
			}
			cout << endl;
		}
	} while (i <= j);
	if (left < j)
		QuickSort(a, left, j,n);
	if (i < right)
		QuickSort(a, i, right,n);
}
int main() {
	int a[9] = { 10 , 9 , 7 , 6 , 1 , 3 , 5 , 7 , 11 };
	cout << "Mang sau khi sap xep la:" << endl;
	QuickSort(a, 0, 7, 9);
	for (int i = 0; i < 9; i++) {
		cout << a[i] << "";
	}
	system("pause");
}