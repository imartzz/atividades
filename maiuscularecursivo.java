import java.util.Scanner;

public class maiuscularecursivo {
    static int maiusculaRecursivo(String s, int i) {
        if (i >= s.length()) return 0;
        int atual = (s.charAt(i) >= 'A' && s.charAt(i) <= 'Z') ? 1 : 0;
        return atual + maiusculaRecursivo(s, i + 1);
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        while (sc.hasNextLine()) {
            String linha = sc.nextLine();
            if (linha.equals("FIM")) break;
            System.out.println(maiusculaRecursivo(linha, 0));
        }
    }
}