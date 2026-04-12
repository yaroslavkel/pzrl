#include <iostream>
using namespace std;
void merge(int* arr, int* temp, int left, int mid, int right){
	int i = left;
	int j = mid + 1;
	int k = left;
	while (i <= mid && j <= right){
		if (arr[i] <= arr[j]){
			temp[k++] = arr[i++];
		}
		else {
			temp[k++] = arr[j++];
		}
	}
	while (i <= mid){
		temp[k++] = arr[i++];
	}
	while (j <= right){
		temp[k++] = arr[j++];
	}
	for (i = left; i <= right; i++){
		arr[i] = temp[i];
	}
}
void merge_sort(int* arr, int* temp, int left, int right){
	if (left < right){
		int mid = left + (right - left) / 2;
		merge_sort(arr, temp, left, mid);
		merge_sort(arr, temp, mid + 1, right);
		merge(arr, temp, left, mid, right);
	}
}
int* merge_sort(int* arr, int size){
	int* temp = new int[size];
	int* sorted = new int[size];
	for (int i = 0; i < size; i++){
		sorted[i] = arr[i];
	}
	merge_sort(sorted, temp, 0, size - 1);
	delete[] temp;
	return sorted;
}
void print_arr(int* arr, int size){
	cout << "Array: " << endl;
	for (int i = 0; i < size; i++){
		cout << arr[i] << " ";
	}
	cout << endl;
}
int main(){
	int* arr = new int[10];
	for (int i = 0; i < 10; i++){
		if (i % 2 == 0){
			arr[i] = i - 5;
		}
		else {
			arr[i] = i + 5;
		}
	}
	int* sorted = merge_sort(arr, 10);
	print_arr(arr, 10);
	print_arr(sorted, 10);
	delete[] arr;
	delete[] sorted;
}
