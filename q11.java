import java.io.File;
import java.io.IOException;
import java.util.Scanner;

public class q11 {

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

    /* busca id no array global e retorna seu indice, ou -1 se nao encontrado */
    static int buscarPorId(int busca) {
        int resp = -1;
        int i = 0;
        while (i < total && resp == -1) {
            if (id[i] == busca) resp = i;
            i++;
        }
        return resp;
    }

    // ==================== LISTA SEQUENCIAL ====================

    static int lista[]  = new int[1000];
    static int tamanho  = 0;

    /* insere restaurante no inicio da lista, deslocando os demais para a direita */
    static void inserirInicio(int idx) {
        for (int i = tamanho; i > 0; i--) lista[i] = lista[i - 1];
        lista[0] = idx;
        tamanho++;
    }

    /* insere restaurante na posicao informada, deslocando os demais para a direita */
    static void inserir(int idx, int posicao) {
        for (int i = tamanho; i > posicao; i--) lista[i] = lista[i - 1];
        lista[posicao] = idx;
        tamanho++;
    }

    /* Insere restaurante no fim da lista */
    static void inserirFim(int idx) {
        lista[tamanho++] = idx;
    }

    /* Remove e retorna indice do primeiro restaurante, deslocando os demais para a esquerda */
    static int removerInicio() {
        int resp = lista[0];
        for (int i = 0; i < tamanho - 1; i++) lista[i] = lista[i + 1];
        tamanho--;
        return resp;
    }

    /* Remove e retorna indice do restaurante na posicao, deslocando os demais para a esquerda */
    static int remover(int posicao) {
        int resp = lista[posicao];
        for (int i = posicao; i < tamanho - 1; i++) lista[i] = lista[i + 1];
        tamanho--;
        return resp;
    }

    /* Remove e retorna indice do ultimo restaurante da lista */
    static int removerFim() {
        return lista[--tamanho];
    }

    /*
     *  le ids e insere no fim da lista
     *  executa comandos de insercao/remocao
     * saida =  nome de cada removido e lista final completa
     */
    public static void main(String[] args) {
        lerCsv();

        Scanner sc = new Scanner(System.in);

        /*  le ids ate -1 e insere no fim */
        boolean continuar = true;
        while (continuar && sc.hasNextInt()) {
            int busca = sc.nextInt();
            if (busca == -1) {
                continuar = false;
            } else {
                int idx = buscarPorId(busca);
                if (idx != -1) inserirFim(idx);
            }
        }

        /*  le n comandos e executa cada um */
        int n = sc.nextInt();
        for (int i = 0; i < n; i++) {
            String cmd = sc.next();

            if (cmd.compareTo("II") == 0) {
                int idx = buscarPorId(sc.nextInt());
                if (idx != -1) inserirInicio(idx);

            } else if (cmd.compareTo("I*") == 0) {
                int posicao = sc.nextInt();
                int idx     = buscarPorId(sc.nextInt());
                if (idx != -1) inserir(idx, posicao);

            } else if (cmd.compareTo("IF") == 0) {
                int idx = buscarPorId(sc.nextInt());
                if (idx != -1) inserirFim(idx);

            } else if (cmd.compareTo("RI") == 0) {
                System.out.println("(R)" + nome[removerInicio()]);

            } else if (cmd.compareTo("R*") == 0) {
                System.out.println("(R)" + nome[remover(sc.nextInt())]);

            } else if (cmd.compareTo("RF") == 0) {
                System.out.println("(R)" + nome[removerFim()]);
            }
        }

        sc.close();

        /* imprime lista do primeiro ao ultimo */
        for (int i = 0; i < tamanho; i++) {
            System.out.println(formatarRestaurante(lista[i]));
        }
    }
}