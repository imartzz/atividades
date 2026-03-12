#include <stdio.h>
#include <string.h>
int main(){
    char string[1000];
    char stringa[1000];
    char stringb[1000];
    scanf(" %s", string);
    scanf(" %s", stringa);
    scanf(" %s", stringb);
    int vetor[26];
    for(int i = 0; i < 26; i++){
        vetor[i] = 0;
    }
    for(int i = 0; i < strlen(string); i++){
        vetor[string[i]-'A']++;
    }
    for(int i = 0; i < strlen(stringa); i++){
        vetor[stringa[i]-'A']++;
    }
    for(int i = 0; i < strlen(stringb); i++){
        vetor[stringb[i]-'A']--;
    }
    for(int i = 0; i < 26; i++){
        if(vetor[i] != 0){
            printf("NO\n");
            return 0;
        }
    }
    printf("YES\n");
    return 0;
}