import java.io.File;
import java.io.IOException;
import java.util.Scanner;

public class Main {

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

    /*
     * Parseia uma linha do CSV usando Scanner com useDelimiter.
     * Formato: id,nome,cidade,capacidade,avaliacao,tipos_cozinha,
     *          faixa_preco,horario,data_abertura,aberto
     */
    static void parseLinha(String linha) {
        Scanner sc = new Scanner(linha);

        // campos simples: delimitador virgula
        sc.useDelimiter(",");
        id[total]         = sc.nextInt();
        nome[total]       = sc.next();
        cidade[total]     = sc.next();
        capacidade[total] = sc.nextInt();
        avaliacao[total]  = sc.nextDouble();

        // tipos_cozinha: troca ';' por ',' para exibicao
        tiposCozinha[total] = sc.next().replace(';', ',');

        // faixa_preco: conta os '$'
        faixaPreco[total] = sc.next().length();

        // horario e data: delimitador cobre ':', '-' e ','
        sc.useDelimiter("[:\\-,]");
        horaAb[total]  = sc.nextInt();
        minAb[total]   = sc.nextInt();
        horaFe[total]  = sc.nextInt();
        minFe[total]   = sc.nextInt();
        dataAno[total] = sc.nextInt();
        dataMes[total] = sc.nextInt();
        dataDia[total] = sc.nextInt();

        // aberto: volta delimitador virgula e pega ultimo token
        sc.useDelimiter(",");
        aberto[total] = (sc.next().charAt(0) == 't');

        sc.close();
        total++;
    }

    /* Le o arquivo CSV linha a linha e parseia cada restaurante */
    static void lerCsv() {
        try {
            Scanner sc = new Scanner(new File("/tmp/restaurantes.csv"));
            sc.nextLine(); // descarta cabecalho
            while (sc.hasNextLine()) {
                parseLinha(sc.nextLine());
            }
            sc.close();
        } catch (IOException e) {
            System.err.println("Erro ao ler CSV: " + e.getMessage());
        }
    }

    /* Busca restaurante pelo id e retorna seu indice, ou -1 se nao encontrado */
    static int buscarPorId(int busca) {
        int resp = -1;
        for (int i = 0; i < total && resp == -1; i++) {
            if (id[i] == busca) resp = i;
        }
        return resp;
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

    /*
     * Le ids da entrada padrao e imprime o restaurante correspondente.
     * Encerra ao receber -1.
     */
    public static void main(String[] args) {
        lerCsv();
        Scanner sc = new Scanner(System.in);
        boolean continuar = true;
        while (continuar && sc.hasNextInt()) {
            int busca = sc.nextInt();
            if (busca == -1) {
                continuar = false;
            } else {
                int idx = buscarPorId(busca);
                if (idx != -1) System.out.println(formatarRestaurante(idx));
            }
        }
        sc.close();
    }
}