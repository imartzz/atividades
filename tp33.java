import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

public class tp33 {

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

    /* le o arquivo CSV linha a linha e parseia cada restaurante */
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

    /* Retorna o restaurante do indice i formatado conforme especificacao */
    static String formatarRestaurante(int i) {
        StringBuilder faixa = new StringBuilder();
        for (int j = 0; j < faixaPreco[i]; j++) faixa.append("$");
        return String.format("[%d ## %s ## %s ## %d ## %.1f ## [%s] ## %s ## %s ## %s ## %b]",
                id[i], nome[i], cidade[i], capacidade[i], avaliacao[i],
                tiposCozinha[i], faixa.toString(),
                formatarHorario(i), formatarData(i), aberto[i]);
    }

    static int subIdx[] = new int[500];
    static int subTotal = 0;

    /* Busca id no array global e adiciona indice encontrado na subcolecao */
    static void adicionarPorId(int busca) {
        int i = 0;
        boolean achou = false;
        while (i < total && !achou) {
            if (id[i] == busca) { subIdx[subTotal++] = i; achou = true; }
            i++;
        }
    }

    static long comp = 0;
    static long mov  = 0;

    /*
     * compara dois restaurantes por avaliacao desempate por nome
     * retorna negativo se i < j, zero se igual positivo se i > j
     */
    static int comparar(int i, int j) {
        int resp;
        if (avaliacao[subIdx[i]] < avaliacao[subIdx[j]]) {
            resp = -1;
        } else if (avaliacao[subIdx[i]] > avaliacao[subIdx[j]]) {
            resp = 1;
        } else {
            resp = nome[subIdx[i]].compareTo(nome[subIdx[j]]);
        }
        return resp;
    }

    /*
     * pivo no meio, dois ponteiros i e j dos dois lados
     * parcial so recursiona lado direito se i < k
     */
    static void quicksort(int esq, int dir, int k) {
        int i = esq, j = dir;
        int pivoIdx = (esq + dir) / 2;

        while (i <= j) {
            comp++;
            while (comparar(i, pivoIdx) < 0) { i++; comp++; }
            comp++;
            while (comparar(j, pivoIdx) > 0) { j--; comp++; }
            if (i <= j) {
                int temp  = subIdx[i];
                subIdx[i] = subIdx[j];
                subIdx[j] = temp;
                mov += 3;
                if (pivoIdx == i) pivoIdx = j;
                else if (pivoIdx == j) pivoIdx = i;
                i++;
                j--;
            }
        }

        if (esq < j) quicksort(esq, j, k);
        if (i < dir && i < k) quicksort(i, dir, k);
    }

    /*
     * le ids da entrada ordena parcialmente por avaliacao (k=10)
     * gera arquivo de log
     */
    public static void main(String[] args) {
        lerCsv();

        Scanner sc = new Scanner(System.in);
        boolean continuar = true;
        while (continuar && sc.hasNextInt()) {
            int busca = sc.nextInt();
            if (busca == -1) continuar = false;
            else adicionarPorId(busca);
        }
        sc.close();

        long inicio = System.currentTimeMillis();
        quicksort(0, subTotal - 1, 10);
        long fim = System.currentTimeMillis();
        double tempo = fim - inicio;

        for (int i = 0; i < subTotal; i++) {
            System.out.println(formatarRestaurante(subIdx[i]));
        }

        try {
            PrintWriter log = new PrintWriter("matricula_quicksort_parcial.txt");
            log.printf("888678\t%d\t%d\t%.2fms%n", comp, mov, tempo);
            log.close();
        } catch (IOException e) {
            System.err.println("Erro ao gravar log: " + e.getMessage());
        }
    }
}