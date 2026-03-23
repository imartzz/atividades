#include <stdio.h>

int substringMaisLonga(char *str) {
    int visto[256];
    
    // inicializo o vetor com -1
    for (int i = 0; i < 256; i++) {
        visto[i] = -1;
    }

    int inicio = 0;
    int maior = 0;
    
    // substitui o strlen
    int n = 0;
    while (str[n] != '\0') {
        n++;
    }

    for (int i = 0; i < n; i++) {
        char c = str[i];

        if (visto[(int)c] >= inicio) {
            inicio = visto[(int)c] + 1;     // se o char já foi visto eu tiro ele da parte não vista
        }

        visto[(int)c] = i; // anoto quando a letra foi vista

        int tamanho_atual = i - inicio + 1; // atualiza o tamanho    
        if (tamanho_atual > maior) {      
            maior = tamanho_atual;        // se esse é o maior tamanho ele vira o maior
        }
    }

    return maior;
}

int main() {
    char str[1000];
    while (scanf("%s", str) != EOF) {
        if(str[0] == 'F' && str[1] == 'I' && str[2] == 'M') return 0;
        printf("%d\n", substringMaisLonga(str));
    }
    return 0;
}