import java.util.Scanner;

public class maiuscula {
    static int maiusculaIterativo(String s) {
        int comp = 0;
        for (int i = 0; i < s.length(); i++) {
            if (s.charAt(i) >= 'A' && s.charAt(i) <= 'Z') comp++;
        }
        return comp;
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        while (sc.hasNextLine()) {
            String linha = sc.nextLine();
            if (linha.equals("FIM")) break;
            System.out.println(maiusculaIterativo(linha));
        }
    }
}