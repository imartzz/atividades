#include <stdio.h>

int main(){
    double n = 0;
    int nota100 = 0;
    int nota50 = 0;
    int nota20 = 0;
    int nota10 = 0;
    int nota5 = 0;
    int nota2 = 0;
    int moeda1 = 0;
    int moeda50 = 0;
    int moeda25 = 0;
    int moeda10 = 0;
    int moeda5 = 0;
    int moeda01 = 0;
    scanf("%lf", &n);

    while(n > 0){
        if(n >= 100){
             n -= 100;
             nota100++;
        } else if(n >= 50){
            n -= 50;
            nota50++;        
        } else if(n >= 20){
            n -= 20;
            nota20++;
        }  else if(n >= 10){
            n -= 10;
            nota10++;
        }  else if(n >= 5){
            n -= 5;
            nota5++;
        }  else if(n >= 2){
            n -= 2;
            nota2++;
        } else if(n >= 1){
            n -= 1;
            moeda1++;
        } else if(n >= 0.50){
            n -= 0.50;
            moeda50++;
        } else if(n >= 0.25){
            n -= 0.25;
            moeda25++;
        } else if(n >= 0.10){
            n -= 0.10;
            moeda10++;
        } else if(n >= 0.05){
            n -= 0.05;
            moeda5++;
        } else if(n >= 0.01){
            n -= 0.01;
            moeda01++;
        } else {
            n = 0;
        }
    }
    printf("NOTAS:\n");
    printf("%d nota(s) de R$ 100.00\n", nota100);
    printf("%d nota(s) de R$ 50.00\n", nota50);
    printf("%d nota(s) de R$ 20.00\n", nota20);
    printf("%d nota(s) de R$ 10.00\n", nota10);
    printf("%d nota(s) de R$ 5.00\n", nota5);
    printf("%d nota(s) de R$ 2.00\n", nota2);
    printf("MOEDAS:\n");
    printf("%d moeda(s) de R$ 1.00\n", moeda1);
    printf("%d moeda(s) de R$ 0.50\n", moeda50);
    printf("%d moeda(s) de R$ 0.25\n", moeda25);
    printf("%d moeda(s) de R$ 0.10\n", moeda10);
    printf("%d moeda(s) de R$ 0.05\n", moeda5);
    printf("%d moeda(s) de R$ 0.01\n", moeda01);


    return 0;
}