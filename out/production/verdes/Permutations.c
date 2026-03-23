#include <stdio.h>

int main(){
    long long n;
    scanf("%lld", &n);

    if(n == 2 || n == 3){
        printf("NO SOLUTION");
        return 0; 
    }

    for(long long i = 2; i <= n; i += 2)
        printf("%lld ", i);

    for(long long i = 1; i <= n; i += 2)
        printf("%lld ", i);

    return 0;
}