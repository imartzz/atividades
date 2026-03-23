#include <stdio.h>

long long merge_count(int arr[], int tmp[], int left, int right) {
    if (right - left <= 1)
        return 0;
    
    int mid = (left + right) / 2;
    long long inversions = 0;
    
    inversions += merge_count(arr, tmp, left, mid);
    inversions += merge_count(arr, tmp, mid, right);
    
    // merge
    int i = left, j = mid, k = left;
    while (i < mid && j < right) {
        if (arr[i] <= arr[j]) {
            tmp[k++] = arr[i++];
        } else {
            tmp[k++] = arr[j++];
            inversions += mid - i; // todos os restantes da esquerda formam inversão
        }
    }
    while (i < mid) tmp[k++] = arr[i++];
    while (j < right) tmp[k++] = arr[j++];
    
    for (int i = left; i < right; i++)
        arr[i] = tmp[i];
    
    return inversions;
}

int main() {
    int n;
    
    while (scanf("%d", &n) && n != 0) {
        int seq[n], tmp[n];
        for (int i = 0; i < n; i++)
            scanf("%d", &seq[i]);
        
        long long inversions = merge_count(seq, tmp, 0, n);
        
        if (inversions % 2 == 1)
            printf("Marcelo\n");
        else
            printf("Carlos\n");
    }
    
    return 0;
}