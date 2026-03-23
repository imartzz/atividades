#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
int comp = 0;
void preenchervetor(int vetor[], int n){
    for(int i = 0;i<n;i++){
        vetor[i] = rand() % 1000;
    }
}
void bubbleSort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n-1; i++) {
        
        int trocou = 0;
        for (j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
                trocou = 1;
            }
        }
        
        if (trocou == 0) break;
    }
}
bool binariarecursiva(int vetor[],int esq,int dir,int x){
    if(esq > dir){
        return false;
    } else {
    int meio = (esq + dir) / 2;
    comp++;
    if(x == vetor[meio]){
        return true;
    } else if(x > vetor[meio]){
        comp++;
        return binariarecursiva(vetor,meio+1,dir,x);
    } else {
        comp ++;
        return binariarecursiva(vetor,esq,meio-1,x);
    }
    }
}
int main(){
    int n;
    scanf("%d", &n);
    int vetor[n];

    
    preenchervetor(vetor,n);
    bubbleSort(vetor,n);
    int x;
    scanf("%d", &x);
    int esq = 0;
    int dir = n-1;
    if(binariarecursiva(vetor,esq,dir,x) == true){
        printf("TRUE\n");
    } else {
        printf("FALSE\n");
    }
    printf("%d", comp);
}