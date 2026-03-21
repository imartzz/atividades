#include <stdio.h>
#include <stdlib.h>

int vogal(char string[], int i){
    if(string[i] == '\0' || string[i] == '\n') return 1;    // se chegar na final return 1
    if(string[i]=='A'||string[i]=='a'||string[i]=='E'||string[i]=='e'||
       string[i]=='I'||string[i]=='i'||string[i]=='O'||string[i]=='o'||
       string[i]=='U'||string[i]=='u'){
        return vogal(string, i + 1); // recursão
    }
    return 0;
}

int consoantes(char string[], int i){
    if(string[i] == '\0' || string[i] == '\n') return 1; // final return 1
    if(string[i] >= 'A' && string[i] <= 'z'){
        if(string[i]!='A'&&string[i]!='a'&&string[i]!='E'&&string[i]!='e'&&
           string[i]!='I'&&string[i]!='i'&&string[i]!='O'&&string[i]!='o'&&
           string[i]!='U'&&string[i]!='u'){
            return consoantes(string, i + 1);  // recursao 
        }
        return 0; 
    }
    return 0;
}

int inteiro(char string[], int i){
    if(string[i] == '\0' || string[i] == '\n') return 1; // se chegar no final
    if(string[i] >= '0' && string[i] <= '9'){        // se for numero
        return inteiro(string, i + 1);
    }
    return 0;
}

int digitos_ou_fim(char string[], int i){
    if(string[i] == '\0' || string[i] == '\n') return 1; // se chegar no final
    if(string[i] >= '0' && string[i] <= '9'){     // se for numero
        return digitos_ou_fim(string, i + 1);
    }
    return 0;
}

int real(char string[], int i){
    if(string[i] == '\0' || string[i] == '\n') return 1;
    if(string[i] == ',' || string[i] == '.'){
        return digitos_ou_fim(string, i + 1);          // se for ',' ou '.' chama funçao pra ver se tem numero depois da ,
    }
    if(string[i] >= '0' && string[i] <= '9'){     // se for numero chama recursão
        return real(string, i + 1);
    }
    return 0;
}

int verificar(char string[]){
    if(string[0]=='F' && string[1]=='I' && string[2]=='M') return 1; // se for FIM return 1
    return 0;
}

int main(){
    char string[1000];
    while(fgets(string, 1000, stdin)){
        
        if(verificar(string) == 1) return 0;
        printf(vogal(string,0)     ? "SIM " : "NAO ");
        printf(consoantes(string,0)? "SIM " : "NAO ");
        printf(inteiro(string,0)   ? "SIM " : "NAO ");
        printf(real(string,0)      ? "SIM\n": "NAO\n");
    }
    return 0;
}