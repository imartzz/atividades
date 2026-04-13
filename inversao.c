#include <stdio.h>

int contar(char string[], int n){
	for(int i = 0;string[i] != '\0';i++){
		n++;
         }
	return n;
}


int main(){
	char string[1000];
	while(fgets(string,1000,stdin)){
		if('F' && string[1] == 'I' && string[2] == 'M') return 0; // verifica se é FIM
		int n = 0;
		n = contar(string,n);
		for(int i = n-1;i >= 0;i--){   // printa a string ao inverso
			if(string[i] != '\n' && string[i] != '\0'){
			printf("%c", string[i]);  
			}
		}
		printf("\n");
	}
	

		return 0;
}


