#include <stdio.h>

main(){
int dia;
scanf("%d", &dia);
int ano = 0;
int mes = 0;
int dias = 0;

while (dia > 0)
{
    if (dia - 365 >= 0)
    {
        dia = dia - 365;
        ano++;
    } else if(dia - 30 >= 0){
        dia = dia - 30;
        mes++;
    } else {
        dia = dia -1;
        dias++;
    } 
}


    printf("%d ano(s)", ano);
    printf("%d mes(es)", mes);
    printf("%d dia(s)", dias);
    return 0;
}