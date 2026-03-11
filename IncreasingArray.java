import java.util.Scanner;
public class IncreasingArray {
    public static void main(String[] args){
    Scanner sc = new Scanner(System.in);
    long n = sc.nextLong();
    long[] vetor = new long[(int)(n)];
    for(int i = 0;i<n;i++){
    vetor[i] = sc.nextLong();
    }
    long contador = 0;
    for(int i = 0;i<n-1;i++){
    if(vetor[i] > vetor[i+1]){
        contador += vetor[i] - vetor[i+1];
        vetor[i+1] = vetor[i];
    }
    }
    System.out.println(contador);

    }
}
