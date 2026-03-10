#include <stdio.h>
#include <string.h>
int main(){
char string[1000];

int entrada = 0;
scanf("%d", &entrada);
getchar();
for(int j = 0;j < entrada;j++){

fgets(string, 1000, stdin);
int m = strlen(string);
if(string[m-1] == '\n'){
    string[m-1] = '\0';
    m--;
}
char tmp;
int n = strlen(string);
for(int i = 0;i < n;i++){
if(string[i] >= 'a' && string[i] <= 'z'){
    string[i] = string[i] + 3;
}
if(string[i] >= 'A' && string[i] <= 'Z'){
    string[i] = string[i] + 3;
}
}
for(int i = 0;i < n/2;i++){
    tmp = string[i];
    string[i] = string[n - i - 1];
    string[n - i - 1] = tmp;
}
for(int i = n/2;i < n;i++){

    string[i] = string[i] - 1;
}
printf("%s\n", string);
}
}

