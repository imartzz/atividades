#include <stdio.h>

int main(){
    int n1 = 0;
    int n2 = 0;
    int n3 = 0;
    int n4 = 0;
    scanf("%d %d %d %d", &n1, &n2, &n3, &n4);
    int media = (n1 * 4 + n2 *3 + n3 * 2 + n4) / 10;
    printf("%d", media);
    return 0;
}