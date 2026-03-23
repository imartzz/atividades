#include <stdio.h>

void inserirfim(int vetor[], int x,int *m,int n){
    if(*m >= n) return;

    vetor[*m] = x;
    (*m)++;

}

void imprimir_completo(int vetor[], int n){
    for(int i = 0; i < n; i++){
        printf("[%d]: %d | ", i, vetor[i]);
    }
    printf("\n");
}

void inseririnicio(int vetor[], int x,int  *m,int n){
    if(*m >= n) return;

    for(int i = *m;i > 0;i--){
    vetor[i] = vetor[i-1];
    }

    vetor[0] = x;
    (*m)++;
}

void inserir(int vetor[], int x, int pos,int *m, int n){
    if( *m >= n || pos < 0 || pos > *m) return;

    for(int i = *m;i > pos;i--){
        vetor[i] = vetor[i-1];
    }

    vetor[pos] = x;
    (*m)++;
}

void removerFim(int vetor[],int *m){
    if(*m == 0) return;

     vetor[*m-1] = 0;
     (*m)--;
}

void removerInicio(int vetor[],int *m,int n){
    if(*m == 0) return;
    vetor[0] = 0;
    (*m)--;
    for(int i = 0;i <= *m;i++){
        vetor[i] = vetor[i+1];
    }
    vetor[*m] = 0;
}

void removerpos(int vetor[], int pos, int *m){
    if(pos >= *m || pos < 0 || *m == 0) return;
    (*m)--;
    for (int i = pos; i <= *m; i++) {
        vetor[i] = vetor[i + 1];
    }
    vetor[*m] = 0;
}
int main(){
    int n = 10;
    int m = 0;
    int vetor[10];

    for(int i = 0;i<n;i++){
        vetor[i] = 0;
    }

    inserirfim(vetor,5,&m,n);
    inserir(vetor,7,1,&m,n);
    inseririnicio(vetor,2,&m,n);
    inserir(vetor,11,3,&m,n);
    inserirfim(vetor,10,&m,n);
    inseririnicio(vetor,8,&m,n);
    removerFim(vetor,&m);
    removerInicio(vetor,&m,n);
    removerpos(vetor,1,&m);

    imprimir_completo(vetor,n);
    return 0;

}