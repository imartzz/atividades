#include <stdio.h>

int main(){
    long long n = 0;
    scanf("%lld", &n);
    long long vetor[n-1];
    long long soma = n*(n+1)/2;
    for(int i = 0;i<n-1;i++){
    scanf("%lld", &vetor[i]);
    soma = soma-vetor[i];
    }
    printf("%lld", soma);
    return 0;
}