import java.util.Scanner;

public class tp2 {

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        String linha = "";
        do {
            linha = scanner.nextLine(); 

            if (saoIguais(linha, "FIM")) {
                break;
            }

            String[] stringsSeparadas = separarPorHifen(linha);

            if (stringsSeparadas != null && stringsSeparadas.length == 2) {
                if (saoAnagramas(stringsSeparadas[0], stringsSeparadas[1])) {
                    System.out.println("SIM");
                } else {
                    System.out.println("NÃO");
                }
            } else {
                System.out.println("Entrada invalida");
            }

        } while (true);
        scanner.close();
    }

    public static boolean saoIguais(String str1, String str2) {
        if (str1.length() != str2.length()) {
            return false;
        }
        for (int i = 0; i < str1.length(); i++) {
            if (str1.charAt(i) != str2.charAt(i)) {
                return false;
            }
        }
        return true;
    }

    public static String[] separarPorHifen(String str) {
        int length = str.length();
        int indiceHifen = -1;
        
        for (int i = 0; i < length; i++) {
            if (str.charAt(i) == '-') {
                indiceHifen = i;
                break;
            }
        }
        
        if (indiceHifen <= 0 || indiceHifen >= length - 1) {
            return null;
        }
        
        String primeiraParte = "";
        for (int i = 0; i < indiceHifen; i++) {
            if (str.charAt(i) != ' ') {
                primeiraParte += str.charAt(i);
            }
        }

        String segundaParte = "";
        for (int i = indiceHifen + 1; i < length; i++) {
            if (str.charAt(i) != ' ') {
                segundaParte += str.charAt(i);
            }
        }
        
        return new String[]{primeiraParte, segundaParte};
    }
    
    private static boolean saoAnagramas(String str1, String str2) {
        int[] contagemCaracteres = new int[256]; 

        for (int i = 0; i < str1.length(); i++) {
            char c = str1.charAt(i);
            if (c >= 'a' && c <= 'z') {
                contagemCaracteres[c]++;
            } else if (c >= 'A' && c <= 'Z') {
                contagemCaracteres[c + 32]++;
            }
        }

        for (int i = 0; i < str2.length(); i++) {
            char c = str2.charAt(i);
            if (c >= 'a' && c <= 'z') {
                contagemCaracteres[c]--;
            } else if (c >= 'A' && c <= 'Z') {
                contagemCaracteres[c + 32]--;
            }
        }

        for (int i = 0; i < 256; i++) {
            if (contagemCaracteres[i] != 0) {
                return false;
            }
        }
        return true;
    }
}