#include <stdio.h>

void cesar(char *string, int i){
if(string[i] == '\0') return; // se chegar no final da string acaba a recursividade

if (string[i] != '\n') {
        string[i] = string[i] + 3; // atualiza o char
    }
cesar(string,i+1); // chama recursivamente pro proxima char
}
int main(){
    char string[10000];
    while(fgets(string, 10000,stdin) != EOF){
        if(string[0] == 'F' && string[1] && 'I' && string[2] && 'M' && string[3] == '\n') return 0; // verifica se é FIM se for retorna 
    
    
    cesar(string,0); // passa I = 0 pra começar da string[0]
    printf("%s", string);
}
return 0;
}