#include <stdio.h>

int main() {
    char string[10000];
    char meio[100];
    char string1[10000];

    while (scanf("%s %s %s", string, meio, string1) == 3) {
        if (string[0] == 'F' && string[1] == 'I' && string[2] == 'M' && string[3] == '\0') return 0;

        int igual = 0;
        int n = 0; // tamanho da string 0
        int tam2 = 0; // tamanho da string 1

        // Contando tamanhos individualmente
        while (string[n] != '\0') n++;
        while (string1[tam2] != '\0') tam2++;

        // Convertendo a primeira palavra
        for (int i = 0; i < n; i++) {
            if (string[i] >= 'A' && string[i] <= 'Z') string[i] += 32;
        }
        // Convertendo a segunda palavra
        for (int i = 0; i < tam2; i++) {
            if (string1[i] >= 'A' && string1[i] <= 'Z') string1[i] += 32;
        }

        // Comparação iterativa
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < tam2; j++) {
                if (string[i] == string1[j]) {
                    string1[j] = ' '; 
                    igual++;
                    break;
                }
            }
        }

        
        if (igual == n && tam2 == n) {
            printf("SIM\n");
        } else {
            printf("NÃO\n"); 
        }
    }
    return 0;
}