import java.util.Random;
import java.util.Scanner;

public class alteracaorandom {
    
   
    public static Random gerador = new Random();
    static boolean testar(String str){
        if(str.length() == 3 && str.charAt(0) == 'F' && str.charAt(1) == 'I' && str.charAt(2) == 'M'){
            return false;
        } return true;
    }
    public static void main(String[] args) {
        // Configura a seed como 4 para a correção automática
        gerador.setSeed(4);
        
        Scanner sc = new Scanner(System.in);

        while (sc.hasNextLine()) {
            String str = sc.nextLine();
            if(testar(str) == false) return;
             

           
            // Sorteia a primeira letra (a que será substituída)
            char letra1 = (char) ('a' + (Math.abs(gerador.nextInt()) % 26));
            
            // Sorteia a segunda letra (a que entrará no lugar)
            char letra2 = (char) ('a' + (Math.abs(gerador.nextInt()) % 26));

            StringBuilder sb = new StringBuilder(str);
            for(int i = 0;i < str.length();i++){
                if(str.charAt(i) == letra1){
                    sb.setCharAt(i, letra2);
                }
            }
            System.out.println(sb.toString());
        }

        sc.close();
    }
}