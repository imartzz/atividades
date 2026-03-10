import java.util.Scanner;

public class tpstringinvertida{
    public static void StringInvertida(String a,int tamanho){
    if(tamanho < 0) return;
    System.out.print(a.charAt(tamanho));
    StringInvertida(a, tamanho-1);
    }
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        while(true){
        String linha = scanner.nextLine();
        if(linha.equals("FIM")) break;
        StringInvertida(linha, linha.length()-1);
        System.out.println();
        }
        scanner.close();
    }
}