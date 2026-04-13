import java.util.Arrays;
public class buble {

    public static void bubbleSort(int[] arr) {
        int n = arr.length;
        boolean ordenado = false;
       
        for (int i = 0; i < n - 1; i++) {
            ordenado = false;
            System.out.println(Arrays.toString(arr));
            for (int j = 0; j < n - i - 1; j++) {
               
                if (arr[j] > arr[j + 1]) {
                    ordenado = true;
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
            if(ordenado == false){
                return;
            }
        }
    }

    public static void main(String[] args) {
    int[] array = new int[5];
    array = new int[]{1,2,3,5,4};
    bubbleSort(array);
    }
}
