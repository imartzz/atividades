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
    char linha[1000];
    while(fgets(linha, sizeof(linha), stdin) != NULL) {
        int invalido = 0;
        topo = -1;

        for(int i = 0; linha[i] != '\0'; i++){
            if(linha[i] == '('){
                push(i);
            } else if(linha[i] == ')'){
                if(vazia()){
                    invalido = 1;
                    break;        
                }
                pop();
            }
        }

        if(!invalido && vazia())
            printf("correct\n");
        else
            printf("incorrect\n");

        topo = -1;
    }
    return 0;
}