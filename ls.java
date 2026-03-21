import java.util.Scanner;
public class ls {

    static boolean verificar(String str) {
        return !(str.length() >= 3 && str.charAt(0) == 'F' && str.charAt(1) == 'I' && str.charAt(2) == 'M');
    }

    static boolean isVogal(char c) {
        return c=='A'||c=='a'||c=='E'||c=='e'||c=='I'||c=='i'||c=='O'||c=='o'||c=='U'||c=='u';
    }

    static String vogal(String str) {
        for (int i = 0; i < str.length(); i++) {
            if (!isVogal(str.charAt(i))) return "NAO";
        }
        return "SIM";
    }

    static String consoante(String str) {
        for (int i = 0; i < str.length(); i++) {
            char c = str.charAt(i);
            if (!(c >= 'A' && c <= 'z')) return "NAO"; // não é letra
            if (isVogal(c)) return "NAO";              // é vogal
        }
        return "SIM";
    }

    static String inteiro(String str) {
        for (int i = 0; i < str.length(); i++) {
            if (str.charAt(i) < '0' || str.charAt(i) > '9') return "NAO";
        }
        return "SIM";
    }

    static String real(String str) {
        boolean achouSeparador = false;
        for (int i = 0; i < str.length(); i++) {
            char c = str.charAt(i);
            if (c == ',' || c == '.') {  // verifica se tem . ou ,
                if (achouSeparador) return "NAO"; // segunda vírgula/ponto = invalido
                achouSeparador = true;
            } else if (c < '0' || c > '9') {
                return "NAO";
            }
        }
        return "SIM";
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        while (sc.hasNextLine()) {
            String str = sc.nextLine();
            if (!verificar(str)) return;
            System.out.println(vogal(str) + " " + consoante(str) + " " + inteiro(str) + " " + real(str));
        }
    }
}