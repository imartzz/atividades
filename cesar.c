#include <stdio.h>

void cesar(char *string, int i){
if(string[i] == '\0') return;

if (string[i] != '\n') {
        string[i] = string[i] + 3;
    }
cesar(string,i+1);
}
int main(){
    char string[10000];
    while(fgets(string, 10000,stdin) != EOF){
    
    
    cesar(string,0);
    printf("%s", string);
}
return 0;
}