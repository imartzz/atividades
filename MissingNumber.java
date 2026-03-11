import java.util.Scanner;
public class MissingNumber {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        long n = sc.nextLong();
        long soma = (long) n*(n+1)/2;
        long[] vetor = new long[(int) (n-1)];
        for(int i = 0;i<n-1;i++){
            vetor[i] = sc.nextLong();
            soma -= vetor[i];
        }
        System.out.println(soma);
    }
}
