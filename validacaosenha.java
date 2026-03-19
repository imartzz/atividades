import java.util.Scanner;

public class validacaosenha{

    static boolean testar(String str){
        if(str.length() == 3 && str.charAt(0) == 'F' && str.charAt(1) == 'I' && str.charAt(2) == 'M') return false;
        return true;         // se for FIM return false se não for return true e continua o programa
    }
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        while(sc.hasNextLine()){
        String str = sc.nextLine();
        if(testar(str) == false) return;     
        int maiuscula = 0;
        int caracterespecial = 0;
        int minuscula = 0;
        int numero = 0;
        for(int i = 0;i < str.length();i++){
        if(str.charAt(i) >= 'a' && str.charAt(i)<= 'z') minuscula++;     // ++ se for minuscula e etc
        if(str.charAt(i) >= 'A' && str.charAt(i) <= 'Z') maiuscula++;
        if(str.charAt(i) >= '0' && str.charAt(i) <= '9') numero++;
        if(str.charAt(i) >= '!' && str.charAt(i) <= '/') caracterespecial++;
        if(str.charAt(i) >= ':' && str.charAt(i) <= '@') caracterespecial++;
        if(str.charAt(i) >= '[' && str.charAt(i) <= '`') caracterespecial++;
        if(str.charAt(i) >= '{' && str.charAt(i) <= '~') caracterespecial++;

        }
        if(maiuscula > 0 && caracterespecial > 0 && minuscula >0 && numero > 0 && str.length() >= 8){ // se tiver os requisitos printa SIM else NAO
            System.out.println("SIM");
        } else {
            System.out.println("NAO");
        }
        }


    }
}