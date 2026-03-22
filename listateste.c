#include <stdio.h>
#include <stdlib.h>
 
#define MAXTAM 6
 
int array[MAXTAM];
int n;
 

 
/* insere no início e empurra os demais para frente */
void inserirInicio(int x) {
    if (n >= MAXTAM) { printf("Erro ao inserir!\n"); exit(1); }
    for (int i = n; i > 0; i--)
        array[i] = array[i - 1];
    array[0] = x;
    n++;
}
 
/* insere no fim */
void inserirFim(int x) {
    if (n >= MAXTAM) { printf("Erro ao inserir!\n"); exit(1); }
    array[n] = x;
    n++;
}
 
/* insere em posição arbitrária e empurra os demais para frente */
void inserir(int x, int pos) {
    if (n >= MAXTAM || pos < 0 || pos > n) { printf("Erro ao inserir!\n"); exit(1); }
    for (int i = n; i > pos; i--)
        array[i] = array[i - 1];
    array[pos] = x;
    n++;
}
 
/* remove do início e puxa os demais para trás */
int removerInicio() {
    if (n == 0) { printf("Erro ao remover!\n"); exit(1); }
    int resp = array[0];
    n--;
    for (int i = 0; i < n; i++)
        array[i] = array[i + 1];
    return resp;
}
 
/* remove do fim */
int removerFim() {
    if (n == 0) { printf("Erro ao remover!\n"); exit(1); }
    return array[--n];
}
 
/* remove em posição arbitrária e puxa os demais para trás */
int remover(int pos) {
    if (n == 0 || pos < 0 || pos >= n) { printf("Erro ao remover!\n"); exit(1); }
    int resp = array[pos];
    n--;
    for (int i = pos; i < n; i++)
        array[i] = array[i + 1];
    return resp;
}
 
void mostrar() {
    printf("[ ");
    for (int i = 0; i < n; i++)
        printf("%d ", array[i]);
    printf("]\n");
}
 
int main() {
    printf("==== LISTA ESTATICA ====\n");
    int n = 0;
 
    inserirFim(10);
    inserirFim(20);
    inserirFim(30);
    printf("Apos inserirFim (10,20,30):    "); mostrar();   /* [ 10 20 30 ] */
 
    inserirInicio(5);
    printf("Apos inserirInicio (5):        "); mostrar();   /* [ 5 10 20 30 ] */
 
    inserir(99, 2);
    printf("Apos inserir (99, pos 2):      "); mostrar();   /* [ 5 10 99 20 30 ] */
 
    int x1 = removerInicio();
    printf("removerInicio: %d             ", x1);  mostrar();   /* [ 10 99 20 30 ] */
 
    int x2 = removerFim();
    printf("removerFim: %d               ", x2);   mostrar();   /* [ 10 99 20 ] */
 
    int x3 = remover(1);
    printf("remover (pos 1): %d           ", x3);  mostrar();   /* [ 10 20 ] */
 
    return 0;
}