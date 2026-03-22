#include <stdio.h>

#define MAX 10

int dados[MAX];
int front = -1;   /* índice de onde sai, -1 = fila vazia */
int rear  = -1;   /* índice de onde entra */

/* ----- funções ----- */

int vazia() {
    return front == -1;
}

int cheia() {
    return rear == MAX - 1;
}

void enqueue(int valor) {
    if (cheia()) { printf("Fila cheia!\n"); return; }
    if (vazia())
        front = 0;       /* primeiro elemento: inicializa front */
    dados[++rear] = valor;
}

int dequeue() {
    if (vazia()) { printf("Fila vazia!\n"); return -1; }
    int valor = dados[front];
    if (front == rear)   /* era o último elemento */
        front = rear = -1;
    else
        front++;
    return valor;
}

int frente() {
    if (vazia()) { printf("Fila vazia!\n"); return -1; }
    return dados[front];
}

void imprimir() {
    if (vazia()) { printf("Fila: vazia\n"); return; }
    printf("Fila (front -> rear): ");
    for (int i = front; i <= rear; i++)
        printf("%d ", dados[i]);
    printf("  (tamanho: %d)\n", rear - front + 1);
}

/* ----- main ----- */

int main() {
    enqueue(10);
    enqueue(20);
    enqueue(30);
    imprimir();                            /* 10 20 30 */

    printf("Frente:  %d\n", frente());     /* 10 — só espia */
    printf("Dequeue: %d\n", dequeue());    /* 10 — saiu o primeiro */
    printf("Dequeue: %d\n", dequeue());    /* 20 */
    imprimir();                            /* 30 */

    return 0;
}