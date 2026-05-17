import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

public class tp311 {

    //contadores para o arquivo de log
    static int comp = 0;
    static int mov = 0;

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

    /* Parseia uma linha do CSV */
    static void parseLinha(String linha) {
        Scanner sc = new Scanner(linha);
        sc.useDelimiter(",");
        id[total]           = sc.nextInt();
        nome[total]         = sc.next().trim();
        cidade[total]       = sc.next().trim();
        capacidade[total]   = sc.nextInt();
        
        // Evita uso de Locale
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

    /* Retorna o restaurante formatado */
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

    static class CelulaDupla {
        public int elementoIdx;
        public CelulaDupla ant;
        public CelulaDupla prox;

        public CelulaDupla(int elementoIdx) {
            this.elementoIdx = elementoIdx;
            this.ant = null;
            this.prox = null;
        }
    }

    static class ListaDupla {
        private CelulaDupla primeiro;
        private CelulaDupla ultimo;
        private int tamanho;

        public ListaDupla() {
            primeiro = new CelulaDupla(-1); // Nó cabeça
            ultimo = primeiro;
            tamanho = 0;
        }

        // Insere no fim da lista dupla
        public void inserirFim(int idx) {
            ultimo.prox = new CelulaDupla(idx);
            ultimo.prox.ant = ultimo;
            ultimo = ultimo.prox;
            tamanho++;
        }

        // Retorna a célula em uma posição específica
        private CelulaDupla getCelula(int pos) {
            CelulaDupla atual = primeiro.prox;
            for (int i = 0; i < pos; i++) {
                atual = atual.prox;
            }
            return atual;
        }

        // Mostra os elementos do inicio ao fim
        public void mostrar() {
            for (CelulaDupla i = primeiro.prox; i != null; i = i.prox) {
                System.out.println(formatarRestaurante(i.elementoIdx));
            }
        }


        public void ordenarQuicksort() {
            quicksort(0, tamanho - 1);
        }

        private void quicksort(int esq, int dir) {
            if (esq < dir) {
                int i = esq, j = dir;
                int pivoIdx = getCelula((esq + dir) / 2).elementoIdx;

                while (i <= j) {
                    while (comparar(getCelula(i).elementoIdx, pivoIdx) < 0) i++;
                    while (comparar(getCelula(j).elementoIdx, pivoIdx) > 0) j--;
                    
                    if (i <= j) {
                        swap(i, j);
                        i++;
                        j--;
                    }
                }
                if (esq < j) quicksort(esq, j);
                if (i < dir) quicksort(i, dir);
            }
        }

        /* * Compara dois elementos
         * Primario: Avaliação Ordem Crescente
         * Secundario Desempate Nome Ordem Alfabética
         */
        private int comparar(int idx1, int idx2) {
            comp++;
            if (avaliacao[idx1] == avaliacao[idx2]) {
                return nome[idx1].compareTo(nome[idx2]);
            }
            return Double.compare(avaliacao[idx1], avaliacao[idx2]);
        }

        // Troca os dados entre duas células 
        private void swap(int i, int j) {
            CelulaDupla celI = getCelula(i);
            CelulaDupla celJ = getCelula(j);
            int temp = celI.elementoIdx;
            celI.elementoIdx = celJ.elementoIdx;
            celJ.elementoIdx = temp;
            mov += 3;
        }
    }

    public static void main(String[] args) {
        lerCsv();

        Scanner scIn = new Scanner(System.in);
        ListaDupla lista = new ListaDupla();

        // Leitura dos IDs iniciais
        while (scIn.hasNextInt()) {
            int idBusca = scIn.nextInt();
            if (idBusca == -1) {
                break;
            }
            int idx = obterIndicePorId(idBusca);
            if (idx != -1) {
                lista.inserirFim(idx);
            }
        }
        
        scIn.close();

        // Ordenação e calculo de tempo
        long inicio = System.currentTimeMillis();
        lista.ordenarQuicksort();
        long fim = System.currentTimeMillis();
        long tempo = fim - inicio;

        // Mostrar estado final ordenado
        lista.mostrar();

        // Gerar arquivo de log
        try {
            PrintWriter log = new PrintWriter("matricula_quicksort2.txt");
            log.printf("888678\t%d\t%d\t%dms", comp, mov, tempo);
            log.close();
        } catch (IOException e) {
            System.err.println("Erro ao criar log: " + e.getMessage());
        }
    }
}