#include <stdio.h>


	void bubbleSort(int arr[], int n, int m) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) { 
		if(arr[j] % m > arr[j + 1] % m) {

			temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
		
		} else if(arr[j] % m == arr[j+1] % m){
		if(arr[j] % 2 == 0 && arr[j+1] % 2 == 1){
			temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                } else if(arr[j] % 2 == 1 && arr[j+1] %2 == 1 && arr[j] < arr[j+1]){
				temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
               } else if(arr[j] % 2 == 0 && arr[j+1] % 2 == 0 && arr[j] > arr[j+1]){
		       
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
		}
		}       
    }
}
}
int main(){
	int n = 0;
	int m = 0;
	while(scanf("%d %d", &n, &m) == 2){
	printf("%d %d\n", n,m);
	int vetor[n];

	for(int i = 0;i< n;i++){
		scanf("%d", &vetor[i]);
	}

	bubbleSort(vetor,n,m);

	for(int i = 0;i < n;i++){
		printf("%d\n", vetor[i]);
	}
}
	return 0;
}


