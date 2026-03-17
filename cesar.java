import java.util.Scanner;

public class cesar {
    static boolean verificar(String s){
        if (s.length() == 3 && s.charAt(0) == 'F' && s.charAt(1) == 'I' && s.charAt(2) == 'M') return false;
        return true;
    } // verifica se a palavra é FIM
    static String novastring(String s) {
    StringBuilder sb = new StringBuilder();
    
    for (int i = 0; i < s.length(); i++) {
        // Pega o caractere, soma 3 e transforma de volta em char
        char novoChar = (char) (s.charAt(i) + 3);
        sb.append(novoChar);
    }
    
    return sb.toString(); // Retorna a nova string montada
}
public static void main(String[] args) {
    Scanner sc = new Scanner(System.in);
    while(sc.hasNextLine()){     // roda enquanto tem linha pra ler
    String s = sc.nextLine();
    if(verificar(s) != true) return; // chama pra verificar se é FIM
    String s1 = novastring(s); // Cria a nova string de cesar
    System.out.println(s1); 
    }
}
}
