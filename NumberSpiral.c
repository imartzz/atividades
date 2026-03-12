#include <stdio.h>

int main() {
    int t;
    scanf("%d", &t);
    
    while (t--) {
        long long y, x;
        scanf("%lld %lld", &y, &x);
        
        long long k = y > x ? y : x;
        long long ans;
        
        if (k % 2 == 1) {
            
            if (x == k) {
                ans = k * k - (y - 1);         
            } else {
                ans = k * k - (k - 1) - (k - x); 
            }
        } else {
            if (y == k) {
                ans = k * k - (x - 1);        
            } else {
                ans = k * k - (k - 1) - (k - y); 
            }
        }
        
        printf("%lld\n", ans);
    }
    
    return 0;
}