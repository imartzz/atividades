#include <stdio.h>
#include <string.h>

int main(){
    char string[1000];
    while(fgets(string, sizeof(string), stdin)){
        string[strcspn(string, "\n")] = '\0';
        
        if(strcmp(string, "FIM") == 0) break;
        
        int cont = 0;
        for(int i = 0; string[i] != '\0'; i++){
            if(string[i] >= 'A' && string[i] <= 'Z') cont++;
        }
        printf("%d\n", cont);
    }
}