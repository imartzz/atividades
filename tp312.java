import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

public class tp312 {

    // Variáveis globais para o arquivo de log
    static int comp = 0;

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

    /* Parseia uma linha do CSV*/
    static void parseLinha(String linha) {
        Scanner sc = new Scanner(linha);
        sc.useDelimiter(",");
        id[total]           = sc.nextInt();
        nome[total]         = sc.next().trim();
        cidade[total]       = sc.next().trim();
        capacidade[total]   = sc.nextInt();
        avaliacao[total]    = Double.parseDouble(sc.next().trim());
        
        // Remove aspas se houver e troca ponto e virgula por vírgula espaço
        String cozinhaRaw = sc.next().replace("\"", "").trim();
        tiposCozinha[total] = cozinhaRaw.replace(";", ",");
        
        faixaPreco[total]   = sc.next().trim().length();
        
        sc.useDelimiter("[:\\-,]");
        horaAb[total]  = sc.nextInt();
        minAb[total]   = sc.nextInt();
        horaFe[total]  = sc.nextInt();
        minFe[total]   = sc.nextInt();
        dataAno[total] = sc.nextInt();
        dataMes[total] = sc.nextInt();
        dataDia[total] = sc.nextInt();
        
        sc.useDelimiter(",");
        aberto[total] = (sc.next().trim().charAt(0) == 't');
        sc.close();
        total++;
    }

    /* Le o arquivo CSV linha a linha */
    static void lerCsv() {
        try {
            Scanner sc = new Scanner(new File("/tmp/restaurantes.csv"));
            sc.nextLine(); // Pula cabeçalho
            while (sc.hasNextLine()) {
                parseLinha(sc.nextLine());
            }
            sc.close();
        } catch (IOException e) {
            System.err.println("Erro ao ler CSV: " + e.getMessage());
        }
    }

    /* Retorna o restaurante formatado  */
    static String formatarRestaurante(int i) {
        StringBuilder faixa = new StringBuilder();
        for (int j = 0; j < faixaPreco[i]; j++) {
            faixa.append("$");
        }
        
        String avaliacaoFormatada = String.format("%.1f", avaliacao[i]).replace(',', '.');
        
        return String.format("[%d ## %s ## %s ## %d ## %s ## [%s] ## %s ## %s ## %s ## %b]",
                id[i], nome[i], cidade[i], capacidade[i], avaliacaoFormatada,
                tiposCozinha[i], faixa.toString(),
                formatarHorario(i), formatarData(i), aberto[i]);
    }

    /* Encontra o índice global correspondente ao ID passado */
    static int obterIndicePorId(int busca) {
        for (int i = 0; i < total; i++) {
            if (id[i] == busca) return i;
        }
        return -1;
    }

    static class No {
        public int elementoIdx; // Guarda o índice do elemento nos arrays globais
        public No esq;
        public No dir;

        public No(int elementoIdx) {
            this.elementoIdx = elementoIdx;
            this.esq = null;
            this.dir = null;
        }
    }

    static class ArvoreBinaria {
        private No raiz;

        public ArvoreBinaria() {
            raiz = null;
        }

        // Insere um elemento na arvore usando o atributo nome como chave
        public void inserir(int idx) {
            raiz = inserir(raiz, idx);
        }

        private No inserir(No i, int idx) {
            if (i == null) {
                i = new No(idx);
            } else if (nome[idx].compareTo(nome[i.elementoIdx]) < 0) {
                i.esq = inserir(i.esq, idx);
            } else if (nome[idx].compareTo(nome[i.elementoIdx]) > 0) {
                i.dir = inserir(i.dir, idx);
            }
            // Se for igual, o enunciado pede para não inserir duplicados
            return i;
        }

        // Pesquisa se uma chave está na árvore mostrando o caminho de ponteiros
        public void pesquisar(String chave) {
            System.out.print("raiz ");
            if (pesquisar(raiz, chave)) {
                System.out.println("SIM");
            } else {
                System.out.println("NAO");
            }
        }

        private boolean pesquisar(No i, String chave) {
            if (i == null) {
                return false;
            }
            comp++;
            if (chave.equals(nome[i.elementoIdx])) {
                return true;
            } else if (chave.compareTo(nome[i.elementoIdx]) < 0) {
                System.out.print("esq ");
                return pesquisar(i.esq, chave);
            } else {
                System.out.print("dir ");
                return pesquisar(i.dir, chave);
            }
        }

        // Mostra os elementos usando caminhamento em ordem
        public void caminharEmOrdem() {
            caminharEmOrdem(raiz);
        }

        private void caminharEmOrdem(No i) {
            if (i != null) {
                caminharEmOrdem(i.esq);
                System.out.println(formatarRestaurante(i.elementoIdx));
                caminharEmOrdem(i.dir);
            }
        }
    }

    public static void main(String[] args) {
        lerCsv();

        Scanner scIn = new Scanner(System.in);
        ArvoreBinaria arvore = new ArvoreBinaria();

        // leitura dos IDs iniciais para preencher a árvore até encontrar -1
        while (scIn.hasNextInt()) {
            int idBusca = scIn.nextInt();
            if (idBusca == -1) {
                break;
            }
            int idx = obterIndicePorId(idBusca);
            if (idx != -1) {
                arvore.inserir(idx);
            }
        }
        
        // Consumir o fim de linha restante após o número -1
        if (scIn.hasNextLine()) {
            scIn.nextLine();
        }

        long inicio = System.currentTimeMillis();
        // leitura das chaves de pesquisa por nome até encontrar FIM
        while (scIn.hasNextLine()) {
            String chave = scIn.nextLine().trim();
            if (chave.equals("FIM")) {
                break;
            }
            arvore.pesquisar(chave);
        }
        long fim = System.currentTimeMillis();
        long tempo = fim - inicio;
        
        scIn.close();

        // apresenta os registros usando caminhamento em ordem
        arvore.caminharEmOrdem();

       
        try {
            PrintWriter log = new PrintWriter("matricula_arvore_binaria.txt");
            log.printf("888678\t%d\t%dms", comp, tempo);
            log.close();
        } catch (IOException e) {
            System.err.println("Erro ao criar log: " + e.getMessage());
        }
    }
}