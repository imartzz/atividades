#include <stdio.h>

#define MAX 51

int dados[MAX];
int inicio = 0;
int fim = 0;
int tamanho = 0;

int vazia()  { return tamanho == 0; }
int cheia()  { return tamanho == MAX; }

void enfileirar(int valor) {
    if (cheia()) return;
    dados[fim] = valor;
    fim = (fim + 1) % MAX;
    tamanho++;
}

int desenfileirar() {
    if (vazia()) return -1;
    int valor = dados[inicio];
    inicio = (inicio + 1) % MAX;
    tamanho--;
    return valor;
}

int main(){
    int n = 0;
    while(scanf("%d", &n) == 1 && n != 0){
        inicio = fim = tamanho = 0;
        int descartadas[n];
        int l = 0;

        for(int i = 1; i <= n; i++)
            enfileirar(i);

        while(tamanho >= 2){
            descartadas[l++] = desenfileirar();
            int valor = desenfileirar();
            enfileirar(valor);
        }

        printf("Discarded cards: ");
        for(int m = 0; m < l; m++){
            if(m > 0) printf(", ");
            printf("%d", descartadas[m]);
        }
        printf("\n");
        printf("Remaining card: %d\n", desenfileirar());
    }
    return 0;
}