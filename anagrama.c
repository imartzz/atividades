#include <stdio.h>

int main() {
    char s1[10000], meio[100], s2[10000];

    // leio a  primeira string primeiro para verificar o "FIM"
    while (scanf("%s", s1) == 1) {
        if (s1[0] == 'F' && s1[1] == 'I' && s1[2] == 'M') return 0;

      
        scanf("%s", s2);

        int n1 = 0, n2 = 0;
        
        // Convertendo s1 para minúsculo e contando tamanho
        for (int i = 0; s1[i] != '\0'; i++) {
            if (s1[i] >= 'A' && s1[i] <= 'Z') s1[i] += 32;
            n1++;
        }
        // Convertendo s2 para minúsculo e contando tamanho
        for (int i = 0; s2[i] != '\0'; i++) {
            if (s2[i] >= 'A' && s2[i] <= 'Z') s2[i] += 32;
            n2++;
        }

        int igual = 0;

        // Se os tamanhos forem diferentes, nem precisa comparar
        if (n1 == n2) {
            for (int i = 0; i < n1; i++) {
                for (int j = 0; j < n2; j++) {
                    if (s1[i] == s2[j]) {
                        s2[j] = ' '; // Marca como usado
                        igual++;
                        break;
                    }
                }
            }
        }

        if (igual == n1 && n1 == n2) {
            printf("SIM\n");
        } else {
            printf("NAO\n");
        }
    }
    return 0;
}