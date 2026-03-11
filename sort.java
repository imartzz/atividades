import java.util.Scanner;
public class sort{
static void BubbleSort(int[] arr,int n,int m){
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


    public static void main(String[] args) {
        
        Scanner sc = new Scanner(System.in);
        while(sc.hasNextInt()){
        int n = sc.nextInt();
        int m = sc.nextInt();
        System.out.println(n + " " + m);
        int[] vetor = new int[n];

        for(int i = 0;i<n;i++){
            vetor[i] = sc.nextInt();
        }
        BubbleSort(vetor, n, m);

        for(int i = 0;i<n;i++){
            System.out.println(vetor[i]);
        }
        }

    }
}