#include <stdio.h>

int main() {
    int r = 0;
    while (scanf("%d", &r) == 1 && r != 0) {
        int mark[r];
        int leti[r];
        int markk = -1;
        int letii = -1;
        int somamark = 0;
        int somaleti = 0;

      
        for (int i = 0; i < r; i++) {
            scanf("%d", &mark[i]);
            somamark += mark[i];
            if (i >= 2 && mark[i] == mark[i-1] && mark[i] == mark[i-2] && markk == -1) {
                markk = i;
            }
        }

        for (int i = 0; i < r; i++) {
            scanf("%d", &leti[i]);
            somaleti += leti[i];
            if (i >= 2 && leti[i] == leti[i-1] && leti[i] == leti[i-2] && letii == -1) {
                letii = i;
            }
        }

        if (markk != -1 || letii != -1) {
            if (letii == -1 || (markk != -1 && markk < letii)) {
                somamark += 30;
            } 
            else if (markk == -1 || (letii != -1 && letii < markk)) {
                somaleti += 30;
            }
        }

        
        if (somamark > somaleti) {
            printf("M\n");
        } else if (somaleti > somamark) {
            printf("L\n");
        } else {
            printf("T\n");
        }
    }
    return 0;
}