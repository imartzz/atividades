#include <stdio.h>

void buble(int arr[],int tamanhoarr){
int ordenado = -1;

for(int i = 0;i < tamanhoarr;i++){
    ordenado = -1;
    for(int j = 0;j < tamanhoarr-i-1;j++){
        if(arr[j] >= arr[j+1]){
            ordenado = 1;
            int tmp = arr[j];
            arr[j] = arr[j+1];
            arr[j+1] = tmp;
        }
    }
    if(ordenado == -1) return;
}
}

int main(){
int arr[] = {7,2,1,6,5,3,4};
int tamanhoarr = sizeof(arr) / sizeof(arr[0]);
buble(arr,tamanhoarr);
for(int j = 0;j<tamanhoarr;j++){
    printf("%d ", arr[j]);
}
return 0;
}