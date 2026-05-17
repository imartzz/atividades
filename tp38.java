import java.io.File;
import java.io.IOException;
import java.util.Scanner;

public class tp38 {

    
    static int dataAno[] = new int[500];
    static int dataMes[] = new int[500];
    static int dataDia[] = new int[500];

    /* retorna data do indice i formatada como DD/MM/YYYY */
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

    /* parseia uma linha do CSV */
    static void parseLinha(String linha) {
        Scanner sc = new Scanner(linha);
        sc.useDelimiter(",");
        id[total]           = sc.nextInt();
        nome[total]         = sc.next().trim();
        cidade[total]       = sc.next().trim();
        capacidade[total]   = sc.nextInt();
        avaliacao[total]    = sc.nextDouble();
        
        // remove aspas se houver e troca ponto e virgula por vírgula espaço
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

    /* Le o arquivo CSV linha a linha*/
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
        public int elementoIdx; // Guarda o índice do elemento nos arrays globais
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

        public ListaDupla() {
            primeiro = new CelulaDupla(-1); // Nó cabeça 
            ultimo = primeiro;
        }

        public void inserirInicio(int idx) {
            CelulaDupla tmp = new CelulaDupla(idx);
            tmp.ant = primeiro;
            tmp.prox = primeiro.prox;
            primeiro.prox = tmp;
            if (primeiro == ultimo) {
                ultimo = tmp;
            } else {
                tmp.prox.ant = tmp;
            }
        }

        public void inserirFim(int idx) {
            ultimo.prox = new CelulaDupla(idx);
            ultimo.prox.ant = ultimo;
            ultimo = ultimo.prox;
        }

        public void inserir(int idx, int pos) throws Exception {
            int tamanho = tamanho();
            if (pos < 0 || pos > tamanho) {
                throw new Exception("Erro ao inserir: posição inválida!");
            } else if (pos == 0) {
                inserirInicio(idx);
            } else if (pos == tamanho) {
                inserirFim(idx);
            } else {
                CelulaDupla i = primeiro;
                for (int j = 0; j < pos; j++, i = i.prox);
                
                CelulaDupla tmp = new CelulaDupla(idx);
                tmp.prox = i.prox;
                tmp.ant = i;
                tmp.prox.ant = tmp;
                i.prox = tmp;
            }
        }

        public int removerInicio() throws Exception {
            if (primeiro == ultimo) {
                throw new Exception("Erro ao remover: lista vazia!");
            }
            CelulaDupla tmp = primeiro.prox;
            primeiro.prox = tmp.prox;
            if (primeiro.prox != null) {
                primeiro.prox.ant = primeiro;
            } else {
                ultimo = primeiro;
            }
            int resp = tmp.elementoIdx;
            tmp.prox = tmp.ant = null;
            return resp;
        }

        public int removerFim() throws Exception {
            if (primeiro == ultimo) {
                throw new Exception("Erro ao remover: lista vazia!");
            }
            int resp = ultimo.elementoIdx;
            ultimo = ultimo.ant;
            ultimo.prox.ant = null;
            ultimo.prox = null;
            return resp;
        }

        public int remover(int pos) throws Exception {
            int tamanho = tamanho();
            if (primeiro == ultimo || pos < 0 || pos >= tamanho) {
                throw new Exception("Erro ao remover: posição inválida!");
            } else if (pos == 0) {
                return removerInicio();
            } else if (pos == tamanho - 1) {
                return removerFim();
            } else {
                CelulaDupla i = primeiro.prox;
                for (int j = 0; j < pos; j++, i = i.prox);
                
                i.ant.prox = i.prox;
                i.prox.ant = i.ant;
                int resp = i.elementoIdx;
                i.prox = i.ant = null;
                return resp;
            }
        }

        public int tamanho() {
            int cont = 0;
            for (CelulaDupla i = primeiro.prox; i != null; i = i.prox) {
                cont++;
            }
            return cont;
        }

        public void mostrar() {
            int pos = 0;
            for (CelulaDupla i = primeiro.prox; i != null; i = i.prox) {
                System.out.println(formatarRestaurante(i.elementoIdx));
                pos++;
            }
        }
    }


    public static void main(String[] args) {
        lerCsv();

        Scanner scIn = new Scanner(System.in);
        ListaDupla lista = new ListaDupla();

        // Leitura dos IDs iniciais até encontrar -1
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

        //  Processamento das operações dinâmicas
        if (scIn.hasNextInt()) {
            int numComandos = scIn.nextInt();
            
            for (int i = 0; i < numComandos; i++) {
                String comando = scIn.next();

                try {
                    if (comando.equals("II")) {
                        int idBusca = scIn.nextInt();
                        lista.inserirInicio(obterIndicePorId(idBusca));
                    } else if (comando.equals("IF")) {
                        int idBusca = scIn.nextInt();
                        lista.inserirFim(obterIndicePorId(idBusca));
                    } else if (comando.equals("I*")) {
                        int pos = scIn.nextInt();
                        int idBusca = scIn.nextInt();
                        lista.inserir(obterIndicePorId(idBusca), pos);
                    } else if (comando.equals("RI")) {
                        int idx = lista.removerInicio();
                        System.out.println("(R)" + nome[idx]);
                    } else if (comando.equals("RF")) {
                        int idx = lista.removerFim();
                        System.out.println("(R)" + nome[idx]);
                    } else if (comando.equals("R*")) {
                        int pos = scIn.nextInt();
                        int idx = lista.remover(pos);
                        System.out.println("(R)" + nome[idx]);
                    }
                } catch (Exception e) {
                    System.err.println(e.getMessage());
                }
            }
        }
        
        scIn.close();

        //  Mostrar estado final da lista encadeada
        lista.mostrar();
    }
}