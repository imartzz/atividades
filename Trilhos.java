import java.util.Scanner;

public class Trilhos {
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
        while (sc.hasNextInt()) {
            int n = sc.nextInt();
            if (n == 0) break;

            int[] saida = new int[n];

            while (true) {
                saida[0] = sc.nextInt();
                if (saida[0] == 0) {
                    System.out.println();
                    break;
                }

                for (int i = 1; i < n; i++)
                    saida[i] = sc.nextInt();

                topo = -1;
                int next = 1;
                boolean possivel = true;

                for (int i = 0; i < n; i++) {
                    int quero = saida[i];
                    while (next <= quero && (vazia() || dados[topo] != quero))
                        push(next++);
                    if (dados[topo] == quero) {
                        pop();
                    } else {
                        possivel = false;
                        topo = -1;
                        break;
                    }
                }

                System.out.println(possivel && vazia() ? "Yes" : "No");
            }
        }
    }
}