#include <stdio.h>


int partition(int arr[], int left, int right);

void quicksort(int arr[], int left, int right) {
    if (left < right) {
        int pi = partition(arr, left, right);
        quicksort(arr, left, pi - 1);
        quicksort(arr, pi + 1, right); 
    }
}

int partition(int arr[], int left, int right) {
    int pivot = arr[right]; 
    int i = (left - 1);     
    
    for (int j = left; j < right; j++) {
        if (arr[j] <= pivot) {
            i = i + 1;
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    }
    
   
    int temp = arr[i + 1];      
    arr[i + 1] = arr[right];    
    arr[right] = temp;
    
    return i + 1;
}

void sortColors(int* nums, int numsSize) {
    quicksort(nums, 0, numsSize - 1); 
}