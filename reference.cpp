#include <iostream>

#define LOG(x) std::cout << x << std::endl;

void fillArr_point(int* arr, int len, int n) {
    for (int i = 0; i < len; i++)
        arr[i] = n;
}

void printArr_point(int* arr, int len) {
    for (int i = 0; i < len; i++)
        LOG(arr[i]);
}

void fillArr_ref(int* arr, int len, int n) {
    for (int i = 0; i < len; i++)
        arr[i] = n;
}

void printArr_ref(int* arr, int len) {
    for (int i = 0; i < len; i++)
        LOG(arr[i]);
}

int main() {
    int* arr_point = new int[10];

    fillArr_point(arr_point, 10, 4);
    printArr_point(arr_point, 10);

    int arr_ref[10];

    fillArr_ref(arr_point, 10, 10);
    printArr_ref(arr_point, 10);

    delete[] arr_point;
}