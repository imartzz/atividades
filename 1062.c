#include <stdio.h>

#define MAX 1000

int dados[MAX];
int topo = -1;

int vazia(){
    return topo == -1;
}
int cheia(){
return topo == MAX - 1;
}

void push(int valor){
    if(cheia()) return;
    dados[++topo] = valor; 
}

int pop(){
if(vazia()) return -1;
return dados[topo--];
}

int main(){
    int n = 0;
    while(scanf("%d", &n) == 1 && n != 0){
        int saida[n];
        int possivel;

       
        while(1){
            scanf("%d", &saida[0]);
            if(saida[0] == 0){
                printf("\n");
                break;
            }  

            for(int i = 1; i < n; i++)
                scanf("%d", &saida[i]);

            
            topo = -1;
            int next = 1;
            possivel = 1;

            for(int i = 0; i < n; i++){
                int quero = saida[i];
                while(next <= quero && (vazia() || dados[topo] != quero))
                    push(next++);
                if(dados[topo] == quero){
                    pop();
                } else {
                    possivel = 0;
                    topo = -1;
                    break;
                }
            }

            printf(possivel && vazia() ? "Yes\n" : "No\n");
        }
    }
}