#include <stdio.h>

int main(){
    long long n = 0;
    scanf("%lld", &n);
    long long vetor[n];
    for(long long i = 0;i<n;i++){
        scanf("%lld", &vetor[i]);
    }
    long long contador = 0;
    for(long long i = 0;i<n-1;i++){
        if(vetor[i] > vetor[i+1]){
            while(vetor[i] > vetor[i+1]){
                vetor[i+1] += 1;
                contador++;
            }
        }
    }
    printf("%lld", contador);
}