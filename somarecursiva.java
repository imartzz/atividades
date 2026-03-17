import java.util.Scanner;
public class somarecursiva {

    static int somarecursiva(int n,int soma){
        if(n == 0){
            return soma;      // retorna a soma final
        }
        soma += n % 10;
        return soma = somarecursiva(n/10, soma);  // chama recursivamente as outras somas
    }
    public static void main(String[] args){
        Scanner sc = new Scanner(System.in);
        while(sc.hasNextInt()){ // le enquanto tem inteiro 
        int n = sc.nextInt();
        int soma = 0; 
        soma = somarecursiva(n,soma);    
        System.out.println(soma);
        }
    }

}
