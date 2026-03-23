import java.util.Scanner;

public class EstacionamentoLinear {

    static int[] dados = new int[100000];
    static int topo = -1;
    static int tamanho = 0;

    static boolean vazia() {
        return topo == -1;
    }

    static boolean cheio() {
        return tamanho == 100000 - 1;
    }

    static void empilhar(int carro) {
        if (cheio()) return;
        tamanho++;
        dados[++topo] = carro;
    }

    static int desempilhar() {
        if (vazia()) return -1;
        tamanho--;
        return dados[topo--];
    }

    static public class evento {
        int tempo;
        char acao;
        int carro;

        public evento(int tempo, char acao, int carro) {
            this.tempo = tempo;
            this.acao = acao;
            this.carro = carro;
        }

        static void ordenar(evento[] eventos, int m) {
            boolean swapped;
            for (int i = 0; i < m - 1; i++) {
                swapped = false;
                for (int j = 0; j < m - i - 1; j++) {
                    if (eventos[j].tempo > eventos[j + 1].tempo) {
                        evento temp = eventos[j];
                        eventos[j] = eventos[j + 1];
                        eventos[j + 1] = temp;
                        swapped = true;
                    }
                }
                if (!swapped) break;
            }
        }
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        while (sc.hasNextInt()) {
            int n = sc.nextInt();
            int k = sc.nextInt();
            if (n == 0 && k == 0) break;


            topo = -1;
            tamanho = 0;

            evento[] eventos = new evento[100000];
            int m = 0;

            for (int i = 1; i <= n; i++) {
                int tempo1 = sc.nextInt();
                int tempo2 = sc.nextInt();
                eventos[m++] = new evento(tempo1, 'e', i);
                eventos[m++] = new evento(tempo2, 'd', i);
            }

            evento.ordenar(eventos, m);

            boolean valido = true;
            for (int i = 0; i < m; i++) {
                if (eventos[i].acao == 'e') {
                    if (tamanho < k) {
                        empilhar(eventos[i].carro);
                    } else {
                        valido = false;
                        break;
                    }
                } else {
                    if (desempilhar() != eventos[i].carro) {
                        valido = false;
                        break;
                    }
                }
            }

            System.out.println(valido ? "Sim" : "Nao");
        }
    }
}