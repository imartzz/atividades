#include <stdio.h>


int somainteiro(int n,int x){
    if(n == 0){

	    return x; // retorna a soma no caso base
    }
    x += n%10;    // faz a soma
    return x = somainteiro(n/10,x);  // chama recursivamente o resto da soma

}
int main(){

	int n = 0;
	while(scanf("%d", &n) == 1){
	int soma = 0;
        	
	soma = somainteiro(n,soma); // chama a recursao
	printf("%d\n", soma); 
	}
	return 0;

}
