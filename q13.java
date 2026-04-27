import java.io.File;
import java.io.IOException;
import java.util.Scanner;

public class q13 {

    static int dataAno[] = new int[500];
    static int dataMes[] = new int[500];
    static int dataDia[] = new int[500];

    /* Retorna data do indice i formatada como DD/MM/YYYY */
    static String formatarData(int i) {
        return String.format("%02d/%02d/%04d", dataDia[i], dataMes[i], dataAno[i]);
    }

    static int horaAb[] = new int[500];
    static int minAb[]  = new int[500];
    static int horaFe[] = new int[500];
    static int minFe[]  = new int[500];

    /* Retorna horario do indice i formatado como HH:mm-HH:mm */
    static String formatarHorario(int i) {
        return String.format("%02d:%02d-%02d:%02d", horaAb[i], minAb[i], horaFe[i], minFe[i]);
    }

    static int     id[]           = new int[500];
    static String  nome[]         = new String[500];
    static String  cidade[]       = new String[500];
    static int     capacidade[]   = new int[500];
    static double  avaliacao[]    = new double[500];
    static String  tiposCozinha[] = new String[500];
    static int     faixaPreco[]   = new int[500];
    static boolean aberto[]       = new boolean[500];
    static int     total          = 0;

    /* parseia uma linha do CSV e armazena no indice total */
    static void parseLinha(String linha) {
        Scanner sc = new Scanner(linha);
        sc.useDelimiter(",");
        id[total]           = sc.nextInt();
        nome[total]         = sc.next();
        cidade[total]       = sc.next();
        capacidade[total]   = sc.nextInt();
        avaliacao[total]    = sc.nextDouble();
        tiposCozinha[total] = sc.next().replace(';', ',');
        faixaPreco[total]   = sc.next().length();
        sc.useDelimiter("[:\\-,]");
        horaAb[total]  = sc.nextInt();
        minAb[total]   = sc.nextInt();
        horaFe[total]  = sc.nextInt();
        minFe[total]   = sc.nextInt();
        dataAno[total] = sc.nextInt();
        dataMes[total] = sc.nextInt();
        dataDia[total] = sc.nextInt();
        sc.useDelimiter(",");
        aberto[total] = (sc.next().charAt(0) == 't');
        sc.close();
        total++;
    }

    /* Le o arquivo CSV linha a linha e parseia cada restaurante */
    static void lerCsv() {
        try {
            Scanner sc = new Scanner(new File("/tmp/restaurantes.csv"));
            sc.nextLine();
            while (sc.hasNextLine()) parseLinha(sc.nextLine());
            sc.close();
        } catch (IOException e) {
            System.err.println("Erro ao ler CSV: " + e.getMessage());
        }
    }

    /* retorna o restaurante do indice i formatado conforme especificacao */
    static String formatarRestaurante(int i) {
        StringBuilder faixa = new StringBuilder();
        for (int j = 0; j < faixaPreco[i]; j++) faixa.append("$");
        return String.format("[%d ## %s ## %s ## %d ## %.1f ## [%s] ## %s ## %s ## %s ## %b]",
                id[i], nome[i], cidade[i], capacidade[i], avaliacao[i],
                tiposCozinha[i], faixa.toString(),
                formatarHorario(i), formatarData(i), aberto[i]);
    }

    /* Busca id no array global e retorna seu indice, ou -1 se nao encontrado */
    static int buscarPorId(int busca) {
        int resp = -1;
        int i = 0;
        while (i < total && resp == -1) {
            if (id[i] == busca) resp = i;
            i++;
        }
        return resp;
    }

    // ==================== FILA CIRCULAR ====================

    static final int CAPACIDADE = 5;
    static int fila[]  = new int[CAPACIDADE + 1];
    static int inicio  = 0;
    static int fim     = 0;
    static int qtd     = 0;

    /* retorna true se a fila esta cheia */
    static boolean filaCheia() {
        return qtd == CAPACIDADE;
    }

    /* retorna true se a fila esta vazia */
    static boolean filaVazia() {
        return qtd == 0;
    }

    /* Enfileira restaurante no fim da fila */
    static void enfileirar(int idx) {
        fila[fim] = idx;
        fim = (fim + 1) % (CAPACIDADE + 1);
        qtd++;
    }

    /* desenfileira e retorna indice do primeiro restaurante */
    static int desenfileirar() {
        int resp = fila[inicio];
        inicio = (inicio + 1) % (CAPACIDADE + 1);
        qtd--;
        return resp;
    }

    /* calcula media arredondada do ano de abertura dos elementos na fila */
    static int mediaAnoAbertura() {
        int soma = 0, i = inicio, cont = 0;
        while (cont < qtd) {
            soma += dataAno[fila[i]];
            i = (i + 1) % (CAPACIDADE + 1);
            cont++;
        }
        return (int) Math.round((double) soma / qtd);
    }

    /* Enfileira com verificacao de fila cheia e imprime saida */
    static void inserir(int idx) {
        if (filaCheia()) {
            int removido = desenfileirar();
            System.out.println("(R)" + nome[removido]);
        }
        enfileirar(idx);
        System.out.println("(I)" + mediaAnoAbertura());
    }

    /*
     *  le ids e enfileira cada restaurante
     *  executa comandos I e R
     *  (I) media_ano, (R) nome e fila final
     */
    public static void main(String[] args) {
        lerCsv();

        Scanner sc = new Scanner(System.in);

        /* le ids ate -1 e enfileira */
        boolean continuar = true;
        while (continuar && sc.hasNextInt()) {
            int busca = sc.nextInt();
            if (busca == -1) {
                continuar = false;
            } else {
                int idx = buscarPorId(busca);
                if (idx != -1) inserir(idx);
            }
        }

        /*  le n comandos e executa */
        int n = sc.nextInt();
        for (int i = 0; i < n; i++) {
            String cmd = sc.next();
            if (cmd.compareTo("I") == 0) {
                int idx = buscarPorId(sc.nextInt());
                if (idx != -1) inserir(idx);
            } else if (cmd.compareTo("R") == 0) {
                if (!filaVazia()) {
                    System.out.println("(R)" + nome[desenfileirar()]);
                }
            }
        }

        sc.close();

        /* imprime fila do primeiro ao ultimo */
        for (int i = 0; i < qtd; i++) {
            int idx = fila[(inicio + i) % (CAPACIDADE + 1)];
            System.out.println(formatarRestaurante(idx));
        }
    }
}