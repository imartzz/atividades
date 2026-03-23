#include <stdio.h>
#include <string.h>


int main(){

    char stringA[1000];
    char stringB[1000];
    while(scanf("%s %s", stringA, stringB) == 2){
        int lenA = strlen(stringA);
        int lenB = strlen(stringB);
   int i = 0;
  while(i < lenA || i < lenB) {
            if(i < lenA) printf("%c", stringA[i]);
            if(i < lenB) printf("%c", stringB[i]);
            i++;
        }
        printf("\n");
}
    return 0;
}