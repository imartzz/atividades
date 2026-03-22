import java.util.Scanner;
public class stringrecursiva {
    static boolean verificar(String str){
        if(str.charAt(0) == 'F' && str.charAt(1) == 'I' && str.charAt(2) == 'M') return false; // se for FIM return false
        return true;
    }
    static int stringinvertida(String str,int i){
        if(i < 0) return 0;            // se acabar a string return 0
        System.out.print(str.charAt(i));         // printa o char atual
        return stringinvertida(str,i-1);        // chama pro proximo char
    }

    public static void main(String[] args){
        Scanner sc = new Scanner(System.in);
        while(sc.hasNextLine()) {
            String str = sc.nextLine();
            if (!verificar(str)) return;
            int i = str.length();
            stringinvertida(str,i-1);
            System.out.println();
        }
    }
}
