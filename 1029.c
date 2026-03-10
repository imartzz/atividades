#include <stdio.h>

int fibonacci(int n, int *calls){
    (*calls)++;
if(n == 0) return 0;
if(n == 1) return 1;
return fibonacci(n-1,calls) + fibonacci(n-2,calls);
}

int main(){
int num = 0;
scanf("%d", &num);  
for(int i = 0;i<num;i++){
    int n = 0;
    scanf("%d", &n);
    int calls = 0;
    int result = fibonacci(n, &calls); 
    printf("fib(%d) = %d calls = %d\n", n,calls-1,result);
}
return 0;
}