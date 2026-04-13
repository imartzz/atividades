#include <stdio.h>

int partition(int arr[],int left,int right){
    int pivot = arr[right];
    int i = left-1;
    for(int j = left;j < right;j++){
        if(arr[j] <= pivot){
            i++;
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    }
    int tmp = arr[i+1];
    arr[i+1] = arr[right];
    arr[right] = tmp;
    return i+1;
}


void quicksort(int arr[],int left,int right){
    if(left < right){
        int pi = partition(arr,left,right);
        quicksort(arr,left,pi-1);
        quicksort(arr,pi+1,right);
    }
}

int main(){
int arr[] = {7,2,1,6,5,3,4};
int tamanhoarr = sizeof(arr) / sizeof(arr[0]);
quicksort(arr,0,tamanhoarr-1);
for(int j = 0;j<tamanhoarr;j++){
    printf("%d ", arr[j]);
}
return 0;
}