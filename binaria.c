#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
void preenchervetor(int vetor[], int n){
    for(int i = 0;i<n;i++){
        vetor[i] = rand() % 1000000;
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
int comparar(const void *a, const void *b){
    return (*(int*)a - *(int*)b);
    }
int main(){
    int comp = 0;
    bool flag = false;
    int n;
    scanf("%d", &n);
   int *vetor = malloc(n * sizeof(int)); 
    if (vetor == NULL) {
        printf("Erro ao alocar memoria\n");
        return 1;
    }
    preenchervetor(vetor,n);
    qsort(vetor, n, sizeof(int), comparar);
    // bubbleSort(vetor,n);
    int esq = 0;
    int dir = n-1;
    int meio = 0;
    int busca;
    scanf("%d", &busca);
    while(esq <= dir){
        meio = (esq + dir) / 2;
        comp++;
        if(vetor[meio] == busca){
            esq = n;
            flag = true;
        } else if(busca > vetor[meio]){
            comp++;
        esq = meio+1;
        } else {
            comp++;
            dir = meio-1;
        }
    }
    if(flag == true){
        printf("TRUE\n");
    } else {
        printf("FALSE\n");
    }
    printf("COMPARAÇOES = %d", comp);
    free(vetor);
return 0;
}