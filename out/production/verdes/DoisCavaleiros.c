#include <stdio.h>

int main(){
    long long n = 0;
    scanf("%lld", &n);

    for(int i = 1;i<=n;i++){
    long long k = i;
    k = k*k*(k*k-1)/2 - 4*(k-1)*(k-2);
    printf("%lld\n", k);
    }
    return 0;
}