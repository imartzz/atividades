#include <stdio.h>
#include <string.h>

int main(){
    char string[1000000];
    scanf("%s", string);
    long long n = strlen(string);
    if(string[n-1] == '\n'){
        string[n-1] = '\0';
    }
    long long maior = 1;
    long long atual = 0;
    for(long long i=0;string[i] != '\0';i++){
        if (string[i] == string[i - 1]) {
            atual++;
            if (atual > maior)
                maior = atual;
        } else {
            atual = 1;
        }
    }
    printf("%lld", maior);
}