import java.io.File;
import java.io.FileWriter;
import java.util.*;

public class Heapsort { 

   
    static int comparacoes = 0; 
    static int movimentacoes = 0;
    static int n = 0; // Tamanho do array de jogos a serem ordenados
    static final String MATRICULA = "888678"; 

   
    static class Game { 
        int id;
        String name;
        String releaseDate;
        int estimatedOwners;
        float price;
        String[] supportedLanguages;
        int metacriticScore;
        float userScore;
        int achievements;
        String[] publishers;
        String[] developers;
        String[] categories;
        String[] genres;
        String[] tags;

        public Game() {}

        
        void setId(String s) { try { id = Integer.parseInt(s.trim()); } catch (NumberFormatException e) { id = 0; } }
        void setName(String s) { name = s; }
        void setReleaseDate(String s) { releaseDate = formatarData(s); }

        void setEstimatedOwners(String s) {
            try { estimatedOwners = Integer.parseInt(s.replaceAll("[^0-9]", "")); } catch (NumberFormatException e) { estimatedOwners = 0; }
        }

        void setPrice(String s) {
            try {
                s = s.replace(',', '.').trim();
                price = Float.parseFloat(s);
            } catch (NumberFormatException e) { price = 0.0f; }
        }

        void setLanguages(String s) { supportedLanguages = separar(s); }
        void setMetacritic(String s) { try { metacriticScore = Integer.parseInt(s.trim()); } catch (NumberFormatException e) { metacriticScore = 0; } }
        void setUserScore(String s) {
            try {
                s = s.replace(',', '.').trim();
                userScore = Float.parseFloat(s);
            } catch (NumberFormatException e) { userScore = 0.0f; }
        }

        void setAchievements(String s) { try { achievements = Integer.parseInt(s.trim()); } catch (NumberFormatException e) { achievements = 0; } }
        void setPublishers(String s) { publishers = separar(s); }
        void setDevelopers(String s) { developers = separar(s); }
        void setCategories(String s) { categories = separar(s); }
        void setGenres(String s) { genres = separar(s); }
        void setTags(String s) { tags = separar(s); }

        String[] separar(String s) {
            s = s.replace("[", "").replace("]", "").replace("\"", "").replace("'", "");
            String[] partes = s.split(",");
            for (int i = 0; i < partes.length; i++) partes[i] = partes[i].trim();
            return partes;
        }
    }
    
    public static String converteMesParaNumero(String mes) {
        switch (mes) {
            case "Jan": return "01";
            case "Feb": return "02";
            case "Mar": return "03";
            case "Apr": return "04";
            case "May": return "05";
            case "Jun": return "06";
            case "Jul": return "07";
            case "Aug": return "08";
            case "Sep": return "09";
            case "Oct": return "10";
            case "Nov": return "11";
            case "Dec": return "12";
            default: return "00";
        }
    }
    
    public static String formatarData(String dataOriginal) {
        String dataLimpa = dataOriginal.replace(",", "").trim();
        String[] partes = dataLimpa.split(" ");
        
        if (partes.length == 3) {
            String mes = converteMesParaNumero(partes[0].trim());
            String dia = partes[1].trim();
            String ano = partes[2].trim();
            
            if (dia.length() == 1) {
                dia = "0" + dia;
            }
            
            return dia + "/" + mes + "/" + ano;
        }
        
        return dataOriginal;
    }
    
    /**
     * Compara dois objetos Game para o Max-Heap.
     * Retorna true se g1 for MAIOR que g2 (deve ser o pai no heap).
     * Critério: Estimated Owners (principal, crescente) > ID (secundário, crescente).
     */
    public static boolean isMaior(Game g1, Game g2) {
        comparacoes++;
        // Chave primária: Estimated Owners (CRESCENTE)
        if (g1.estimatedOwners > g2.estimatedOwners) {
            return true; // g1 é MAIOR
        }
        if (g1.estimatedOwners < g2.estimatedOwners) {
            return false; // g1 é MENOR
        }

        // Chave secundária: ID (CRESCENTE, desempate)
        comparacoes++;
        return g1.id > g2.id; // g1 é MAIOR (ou seja, deve vir depois)
    }

    /**
     * Troca dois elementos no array.
     */
    private static void swap(Game[] array, int i, int j) {
        Game temp = array[i];
        array[i] = array[j];
        array[j] = temp;
        movimentacoes += 3; // 3 atribuições por troca
    }
    
    /**
     * Constrói o Max-Heap (parte inicial do Heapsort).
     */
    public static void buildMaxHeap(Game[] array) {
        for (int i = n / 2 - 1; i >= 0; i--) {
            maxHeapify(array, i, n);
        }
    }

    /**
     * Mantém a propriedade Max-Heap para um sub-árvore.
     */
    private static void maxHeapify(Game[] array, int i, int heapSize) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;

        // Compara com o filho da esquerda (verifica se o filho é MAIOR)
        if (left < heapSize && isMaior(array[left], array[largest])) {
            largest = left;
        }

        // Compara com o filho da direita (verifica se a direita é MAIOR que o atual 'largest')
        if (right < heapSize && isMaior(array[right], array[largest])) {
            largest = right;
        }

        // Se o maior não for o nó atual (i), troca e continua a descida recursivamente
        if (largest != i) {
            swap(array, i, largest);
            maxHeapify(array, largest, heapSize);
        }
    }

    /**
     * Algoritmo de ordenacao Heapsort.
     */
    public static void sort(Game[] array) {
        // 1. Constrói o Max-Heap
        buildMaxHeap(array);

        // 2. Extrai o elemento máximo sequencialmente para ordenar
        for (int i = n - 1; i > 0; i--) {
            // Troca o nó raiz (maior elemento) com o último elemento não ordenado (i)
            swap(array, 0, i);
            
            // Reorganiza o heap reduzido
            maxHeapify(array, 0, i);
        }
    }
    
   
    
    public static long now(){
        return new Date().getTime(); 
    }

   
    public static void printGame(Game g) {
         System.out.println("=> " + g.id + " ## " + g.name + " ## " + g.releaseDate + " ## " +
            g.estimatedOwners + " ## " + g.price + " ## " + Arrays.toString(g.supportedLanguages) +
            " ## " + g.metacriticScore + " ## " + g.userScore + " ## " + g.achievements +
            " ## " + Arrays.toString(g.publishers) + " ## " + Arrays.toString(g.developers) +
            " ## " + Arrays.toString(g.categories) + " ## " + Arrays.toString(g.genres) +
            " ## " + Arrays.toString(g.tags) + " ##"); 
    }

    public static void main(String[] args) throws Exception {
        ArrayList<Game> todosOsJogos = new ArrayList<>();
        ArrayList<Game> jogosInclusos = new ArrayList<>();

        // Pré-leitura do CSV
        try (Scanner scArquivo = new Scanner(new File("games.csv"), "UTF-8")) {
            if (scArquivo.hasNextLine()) {
                scArquivo.nextLine(); // Pula o cabeçalho
            }

            while (scArquivo.hasNextLine()) {
                String linha = scArquivo.nextLine();
                ArrayList<String> dados = splitCsvAspas(linha);

                Game g = new Game();
                g.setId(dados.get(0));
                g.setName(dados.get(1));
                g.setReleaseDate(dados.get(2));
                g.setEstimatedOwners(dados.get(3));
                g.setPrice(dados.get(4));
                g.setLanguages(dados.get(5));
                g.setMetacritic(dados.get(6));
                g.setUserScore(dados.get(7));
                g.setAchievements(dados.get(8));
                g.setPublishers(dados.get(9));
                g.setDevelopers(dados.get(10));
                g.setCategories(dados.get(11));
                g.setGenres(dados.get(12));
                g.setTags(dados.get(13));
                
                todosOsJogos.add(g);
            }
        } 

        // Leitura de IDs de entrada
        try (Scanner sc = new Scanner(System.in)) {
            while (true) {
                String idString = sc.nextLine();

                if (idString.equalsIgnoreCase("FIM")) break;

                try {
                    int idNum = Integer.parseInt(idString.trim());
                    
                    Game jogoEncontrado = null;
                    for (Game jogoCSV : todosOsJogos) {
                        if (jogoCSV.id == idNum) {
                            jogoEncontrado = jogoCSV; 
                            break;
                        }
                    }
                    
                    if (jogoEncontrado != null) {
                        jogosInclusos.add(jogoEncontrado); 
                    }
                } catch (NumberFormatException e) {
                    // Ignora entradas não numéricas
                }
            }
        } 


        // Conversão para array e Ordenação
        n = jogosInclusos.size();
        Game[] array = jogosInclusos.toArray(new Game[n]);
        
        long inicioOrdenacao = now(); 
        sort(array); // Chama o Heapsort
        long fimOrdenacao = now();   
        
        double tempoOrdenacao = (double)(fimOrdenacao - inicioOrdenacao); 

        // print
        for (int i = 0; i < n; i++) {
            printGame(array[i]);
        }
       

        // GERAÇÃO DO ARQUIVO DE LOG
        String nomeArquivo = MATRICULA + "_heapsort.txt";

        
        try (FileWriter logWriter = new FileWriter(nomeArquivo)) {
            int compTotal = comparacoes;
            int movTotal = movimentacoes;

            
            String logLine = String.format("%s\t%d\t%d\t%.4f", MATRICULA, compTotal, movTotal, tempoOrdenacao);
            logWriter.write(logLine);
        } catch (Exception e) {
            System.err.println("Erro ao escrever no arquivo de log: " + e.getMessage());
        }
    }

    /**
     * Quebra uma linha de CSV em colunas, respeitando aspas, usando StringBuilder.
     */
    static ArrayList<String> splitCsvAspas(String linha) {
        ArrayList<String> partes = new ArrayList<>();
        StringBuilder atual = new StringBuilder();
        boolean aspas = false;

        for (int i = 0; i < linha.length(); i++) {
            char c = linha.charAt(i);

            if (c == '"') {
                aspas = !aspas;
            } else if (c == ',' && !aspas) {
                partes.add(atual.toString());
                atual.setLength(0);
            } else {
                atual.append(c);
            }
        }

        partes.add(atual.toString());
        return partes;
    }
}