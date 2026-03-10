#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool sequencial(int vetor[], int x, int n){
    int comp = 0;
    bool flag = false;
    for(int i = 0; i < n; i++){
        comp++;
        if(vetor[i] == x){
            flag = true;
            i = n;
        }
        printf("Numero de comp e %d\n", comp);
    }
    printf("Numero de comp e %d\n", comp);
    return flag;
}

int main(){
    long long n;
    scanf("%lld", &n);

    int *vetor = malloc(n * sizeof(int)); 
    if (vetor == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }

    for(int i = 0; i < n; i++){
        vetor[i] = rand() % 1000000;
    }

    int numero;
    scanf("%d", &numero);

    if(sequencial(vetor, numero, n)){
        printf("TRUE\n");
    } else {
        printf("FALSE\n");
    }

    free(vetor); 
    return 0;
}