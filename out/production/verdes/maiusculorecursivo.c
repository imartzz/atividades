#include <stdio.h>
#include <string.h>

void maiuscula(char string[]){
    int comp = 0;
for(int i = 0;string[i] != '\0';i++){
    if(string[i] >= 'A' && string[i] <= 'Z') comp++; 
}
printf("%d\n", comp);
}

int main(){
char string[1000];
while(fgets(string,sizeof(string), stdin)){
    string[strcspn(string, "\n")] = '\0';
    if(strcmp(string, "FIM") == 0) break;
    maiuscula(string);
}
return 0;
}