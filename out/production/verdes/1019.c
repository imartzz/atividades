#include  <stdio.h>

int main(){
    int n = 0;
    scanf("%d", &n);
    int hora = 0;
    int minutos = 0;
    int segundos = 0;
    while(n != 0){
        if(n >= 3600){
        hora++;
        n-=3600;
        } else if(n >= 60){
            minutos++;
            n-=60;
        } else{
            segundos += n;
            n = 0;
        }
    }

    printf("%d:%d:%d\n", hora,minutos,segundos);



    return 0;
}