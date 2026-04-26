import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

public class q9 {

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

    /* retorna horario do indice i formatado como HH:mm-HH:mm */
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
     * compara data_abertura dos restaurantes subIdx[i] e subIdx[j].
     * criterio: ano, mes, dia empate desempata pelo nome
     * retorna negativo se i < j, zero se igual, positivo se i > j.
     */
    static int comparar(int i, int j) {
        comp++;
        int idxI = subIdx[i];
        int idxJ = subIdx[j];
        int resp;

        if (dataAno[idxI] != dataAno[idxJ]) {
            resp = dataAno[idxI] - dataAno[idxJ];
        } else if (dataMes[idxI] != dataMes[idxJ]) {
            resp = dataMes[idxI] - dataMes[idxJ];
        } else if (dataDia[idxI] != dataDia[idxJ]) {
            resp = dataDia[idxI] - dataDia[idxJ];
        } else {
            comp++;
            resp = nome[idxI].compareTo(nome[idxJ]);
        }
        return resp;
    }

    /*
     * Reconstroi heap maximo a partir do no raiz dentro de [raiz..fim].
     * O maior elemento sobe para a raiz
     */
    static void heapify(int raiz, int fim) {
        int maior = raiz;
        int esq   = 2 * raiz + 1;
        int dir   = 2 * raiz + 2;

        if (esq <= fim && comparar(esq, maior) > 0) maior = esq;
        if (dir <= fim && comparar(dir, maior) > 0) maior = dir;

        if (maior != raiz) {
            int temp      = subIdx[raiz];
            subIdx[raiz]  = subIdx[maior];
            subIdx[maior] = temp;
            mov += 3;
            heapify(maior, fim);
        }
    }

    /*
     * Ordena subIdx por data_abertura (desempate nome) usando heapsort
     * constroi heap maximo
     *  extrai maior repetidamente para o final
     */
    static void heapsort() {
      
        for (int i = subTotal / 2 - 1; i >= 0; i--) {
            heapify(i, subTotal - 1);
        }
     
        for (int i = subTotal - 1; i > 0; i--) {
            int temp  = subIdx[0];
            subIdx[0] = subIdx[i];
            subIdx[i] = temp;
            mov += 3;
            heapify(0, i - 1);
        }
    }

    /*
     * Le ids da entrada, imprime restaurantes ordenados por data_abertura
     * desempate nome via heapsort e gera arquivo de log.
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
        heapsort();
        long fim = System.currentTimeMillis();
        double tempo = fim - inicio;

        for (int i = 0; i < subTotal; i++) {
            System.out.println(formatarRestaurante(subIdx[i]));
        }

        try {
            PrintWriter log = new PrintWriter("matricula_heapsort.txt");
            log.printf("888678\t%d\t%d\t%.2fms%n", comp, mov, tempo);
            log.close();
        } catch (IOException e) {
            System.err.println("Erro ao gravar log: " + e.getMessage());
        }
    }
}