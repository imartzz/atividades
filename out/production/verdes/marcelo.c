#include <stdio.h>

int main() {
    int n;
    
    while (scanf("%d", &n) && n != 0) {
        int seq[n];
        for (int i = 0; i < n; i++)
            scanf("%d", &seq[i]);
        
        long long inversions = 0;
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - 1 - i; j++) {
                if (seq[j] > seq[j + 1]) {
                    int tmp = seq[j];
                    seq[j] = seq[j + 1];
                    seq[j + 1] = tmp;
                    inversions++;
                }
            }
        }
        
        if (inversions % 2 == 1)
            printf("Marcelo\n");
        else
            printf("Carlos\n");
    }
    
    return 0;
}