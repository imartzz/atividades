#include <stdio.h>

#define MAX 5

int dados[MAX];
int head     = 0;   /* índice de onde sai */
int tail     = 0;   /* índice de onde entra */
int tamanho  = 0;

/* ----- funções ----- */

int vazia() {
    return tamanho == 0;
}

int cheia() {
    return tamanho == MAX;
}

void enqueue(int valor) {
    if (cheia()) { printf("Fila cheia!\n"); return; }
    dados[tail] = valor;
    tail = (tail + 1) % MAX;   /* avança em círculo */
    tamanho++;
}

int dequeue() {
    if (vazia()) { printf("Fila vazia!\n"); return -1; }
    int valor = dados[head];
    head = (head + 1) % MAX;   /* avança em círculo */
    tamanho--;
    return valor;
}

int frente() {
    if (vazia()) { printf("Fila vazia!\n"); return -1; }
    return dados[head];
}

void imprimir() {
    printf("Fila circular (%d/%d): ", tamanho, MAX);
    for (int i = 0; i < tamanho; i++)
        printf("%d ", dados[(head + i) % MAX]);
    printf("\n");
}

/* ----- main ----- */

int main() {
    enqueue(10);
    enqueue(20);
    enqueue(30);
    imprimir();                             /* 10 20 30 */

    printf("Dequeue: %d\n", dequeue());     /* 10 — liberou posição 0 */
    printf("Dequeue: %d\n", dequeue());     /* 20 — liberou posição 1 */

    enqueue(40);
    enqueue(50);
    /* tail voltou ao início — reutilizou as posições 0 e 1 */
    imprimir();                             /* 30 40 50 */

    enqueue(60);
    enqueue(70);
    imprimir();                             /* 30 40 50 60 70 — array cheio */

    enqueue(80);                            /* "Fila cheia!" */

    return 0;
}