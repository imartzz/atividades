import java.util.Scanner;

public class Parenteses {
    static int[] dados = new int[1000];
    static int topo = -1;

    static boolean vazia() { return topo == -1; }
    static boolean cheia() { return topo == 998; }

    static void push(int valor) {
        if (cheia()) return;
        dados[++topo] = valor;
    }

    static int pop() {
        if (vazia()) return -1;
        return dados[topo--];
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        while (sc.hasNextLine()) {
            String linha = sc.nextLine();
            topo = -1;
            boolean invalido = false;

            for (int i = 0; i < linha.length(); i++) {
                char c = linha.charAt(i);
                if (c == '(') {
                    push(i);
                } else if (c == ')') {
                    if (vazia()) {
                        invalido = true;
                        break;
                    }
                    pop();
                }
            }

            System.out.println(!invalido && vazia() ? "correct" : "incorrect");
        }
    }
}