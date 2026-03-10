import java.util.Scanner;

public class Exec3 {

    public static void main(String[] args) {
        ascendente();
    }

    public static void ascendente() {
        Scanner scanner = new Scanner(System.in);

        int n = scanner.nextInt();
        int m = scanner.nextInt();

       
        while (n != 0 || m != 0) {
            
            
            System.out.println(n + " " + m);

            int[] numeros = new int[n];
            for (int i = 0; i < n; i++) {
                numeros[i] = scanner.nextInt();
            }

           
            for (int i = 0; i < n - 1; i++) {
                for (int j = 0; j < n - 1 - i; j++) {
                    
                    int num_j = numeros[j];
                    int num_j1 = numeros[j + 1];
                    int mod_j = num_j % m;
                    int mod_j1 = num_j1 % m;

                    boolean precisaTrocar = false;

    

                   
                    if (mod_j > mod_j1) {
                        precisaTrocar = true;
                    
              
                    } else if (mod_j == mod_j1) {
                        
                        boolean j_eh_impar = (num_j % 2 != 0);
                        boolean j1_eh_impar = (num_j1 % 2 != 0);

                       
                        if (!j_eh_impar && j1_eh_impar) {
                            precisaTrocar = true;
                        }
                       
                        else if (j_eh_impar && j1_eh_impar) {
                            if (num_j < num_j1) {
                                precisaTrocar = true;
                            }
                        }
                       
                        else if (!j_eh_impar && !j1_eh_impar) {
                            if (num_j > num_j1) {
                                precisaTrocar = true;
                            }
                        }
                    }
     

                    if (precisaTrocar) {
                        int tempNumero = numeros[j];
                        numeros[j] = numeros[j + 1];
                        numeros[j + 1] = tempNumero;
                    }
                }
            }

           
            for (int i = 0; i < n; i++) {
                System.out.println(numeros[i]);
            }

       
            n = scanner.nextInt();
            m = scanner.nextInt();
        }
        
        
        System.out.println("0 0");

        scanner.close();
    }
}