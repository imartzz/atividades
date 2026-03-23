#include <stdio.h>
#include <string.h>
int main() {
 
    int n = 0;
    scanf("%d", &n);

    for(int i = 0;i<n;i++){
        int alunos;
        int trocas = 0;
        scanf("%d", &alunos);
        int notas[alunos];
        int notas2[alunos];  
        for(int j = 0; j < alunos;j++){
        scanf("%d", &notas[j]);
        notas2[j] = notas[j];
        }
        for(int m = 0;m< alunos-1;m++){
        for(int k = 0;k < alunos-1 -m ;k++){
            if(notas[k] < notas[k+1]){
            int tmp = notas[k];
            notas[k] = notas[k+1];
            notas[k+1] = tmp;
            }
        }
    }
        for(int l = 0;l< alunos;l++){
            if(notas[l] == notas2[l]){
                trocas++;
            }
        }
        printf("%d\n", trocas);
    }

    return 0;
}