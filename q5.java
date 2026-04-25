import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

public class q5 {

    // ==================== ARRAYS DE DATA ====================
    static int dataAno[] = new int[500];
    static int dataMes[] = new int[500];
    static int dataDia[] = new int[500];

    /* Retorna data do indice i formatada como DD/MM/YYYY */
    static String formatarData(int i) {
        return String.format("%02d/%02d/%04d", dataDia[i], dataMes[i], dataAno[i]);
    }

    // ==================== ARRAYS DE HORA ====================
    static int horaAb[] = new int[500];
    static int minAb[]  = new int[500];
    static int horaFe[] = new int[500];
    static int minFe[]  = new int[500];

    /* Retorna horario do indice i formatado como HH:mm-HH:mm */
    static String formatarHorario(int i) {
        return String.format("%02d:%02d-%02d:%02d", horaAb[i], minAb[i], horaFe[i], minFe[i]);
    }

    // ==================== ARRAYS DE RESTAURANTE ====================
    static int     id[]           = new int[500];
    static String  nome[]         = new String[500];
    static String  cidade[]       = new String[500];
    static int     capacidade[]   = new int[500];
    static double  avaliacao[]    = new double[500];
    static String  tiposCozinha[] = new String[500];
    static int     faixaPreco[]   = new int[500];
    static boolean aberto[]       = new boolean[500];
    static int     total          = 0;

    /* Parseia uma linha do CSV e armazena no indice total */
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
            while (sc.hasNextLine()) {
                parseLinha(sc.nextLine());
            }
            sc.close();
        } catch (IOException e) {
            System.err.println("Erro ao ler CSV: " + e.getMessage());
        }
    }

    // ==================== SUBCOLECAO ====================

    static int subIdx[] = new int[500];
    static int subTotal = 0;

    /* Busca id no array global e adiciona indice encontrado na subcolecao */
    static void adicionarPorId(int busca) {
        int i = 0;
        boolean achou = false;
        while (i < total && !achou) {
            if (id[i] == busca) {
                subIdx[subTotal++] = i;
                achou = true;
            }
            i++;
        }
    }

    // ==================== PESQUISA SEQUENCIAL ====================

    static long comp = 0;

    /*
     * Pesquisa o nome buscado na subcolecao sequencialmente
     * Conta cada compareTo como 1 comparacao
     * Retorna true se encontrado, false caso contrario
     */
    static boolean pesquisaSequencial(String buscado) {
        boolean achou = false;
        int i = 0;
        while (i < subTotal && !achou) {
            comp++;
            if (nome[subIdx[i]].compareTo(buscado) == 0) {
                achou = true;
            }
            i++;
        }
        return achou;
    }

    /*
     * Le ids da entrada (parte 1), monta subcolecao
     * le nomes a buscar (parte 2) e imprime SIM/NAO para cada um
     * Gera arquivo de log com matricula, comparacoes e tempo
     */
    public static void main(String[] args) {
        lerCsv();

        Scanner sc = new Scanner(System.in);

        // parte 1: le ids ate -1
        boolean continuar = true;
        while (continuar && sc.hasNextInt()) {
            int busca = sc.nextInt();
            if (busca == -1) {
                continuar = false;
            } else {
                adicionarPorId(busca);
            }
        }

        // consome o '\n'
        sc.nextLine();

        // parte 2: le nomes ate FIM e pesquisa cada um
        long inicio = System.currentTimeMillis();
        boolean lendo = true;
        while (lendo && sc.hasNextLine()) {
            String buscado = sc.nextLine();
            if (buscado.compareTo("FIM") == 0) {
                lendo = false;
            } else {
                if (pesquisaSequencial(buscado)) {
                    System.out.println("SIM");
                } else {
                    System.out.println("NAO");
                }
            }
        }
        long fim = System.currentTimeMillis();
        double tempo = fim - inicio;

        sc.close();

        // gera log: matricula\tcomparacoes\ttempo
        try {
            PrintWriter log = new PrintWriter("matricula_sequencial.txt");
            log.printf("1593190\t%d\t%.2fms%n", comp, tempo);
            log.close();
        } catch (IOException e) {
            System.err.println("Erro ao gravar log: " + e.getMessage());
        }
    }
}