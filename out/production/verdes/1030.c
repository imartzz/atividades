#include <stdio.h>

int josephus(int n, int k) {
    if (n == 1) return 0;
    return (josephus(n - 1, k) + k) % n;
}

int main() {
    int nc;
    scanf("%d", &nc);
    for (int i = 1; i <= nc; i++) {
        int n, k;
        scanf("%d %d", &n, &k);
        printf("Case %d: %d\n", i, josephus(n, k) + 1); 
    }
    return 0;
}