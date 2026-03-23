#include <stdio.h>

#define MAX 10

int dados[MAX];
int topo = -1;   /* -1 = pilha vazia */

/* ----- funções ----- */

int vazia() {
    return topo == -1;
}

int cheia() {
    return topo == MAX - 1;
}

void push(int valor) {
    if (cheia()) { printf("Pilha cheia!\n"); return; }
    dados[++topo] = valor;   /* incrementa topo e insere */
}

int pop() {
    if (vazia()) { printf("Pilha vazia!\n"); return -1; }
    return dados[topo--];    /* retorna e decrementa topo */
}

int peek() {
    if (vazia()) { printf("Pilha vazia!\n"); return -1; }
    return dados[topo];
}

void imprimir() {
    printf("Pilha (topo -> base): ");
    for (int i = topo; i >= 0; i--)
        printf("%d ", dados[i]);
    printf("  (tamanho: %d)\n", topo + 1);
}

/* ----- main ----- */

int main() {
    push(10);
    push(20);
    push(30);
    imprimir();                      /* 30 20 10 */

    printf("Topo: %d\n", peek());    /* 30 — só espia */
    printf("Pop:  %d\n", pop());     /* 30 — remove */
    printf("Pop:  %d\n", pop());     /* 20 */
    imprimir();                      /* 10 */

    return 0;
}