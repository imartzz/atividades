import java.io.File;
import java.io.FileWriter;
import java.util.*;

public class Merge { 

    static int comparacoes = 0; 
    static int movimentacoes = 0;
    static int n = 0; 

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
        return switch (mes) {
            case "Jan" -> "01";
            case "Feb" -> "02";
            case "Mar" -> "03";
            case "Apr" -> "04";
            case "May" -> "05";
            case "Jun" -> "06";
            case "Jul" -> "07";
            case "Aug" -> "08";
            case "Sep" -> "09";
            case "Oct" -> "10";
            case "Nov" -> "11";
            case "Dec" -> "12";
            default -> "00";
        };
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

    public static boolean isMenorOuIgual(Game g1, Game g2) {
        comparacoes++; 
        if (g1.price < g2.price) {
            return true;
        } 
        if (g1.price > g2.price) {
            return false;
        }
        
        comparacoes++;
        return g1.id <= g2.id;
    }


    public static void sort(Game[] array) {
        mergesort(array, 0, n-1);
    }

    private static void mergesort(Game[] array, int esq, int dir) {
        if (esq < dir){
            int meio = esq + (dir - esq) / 2; 
            mergesort(array, esq, meio);
            mergesort(array, meio + 1, dir);
            intercalar(array, esq, meio, dir);
        }
    }

    public static void intercalar(Game[] array, int esq, int meio, int dir){
        int n1 = meio - esq + 1;
        int n2 = dir - meio;

        Game[] a1 = new Game[n1];
        Game[] a2 = new Game[n2];

        for(int i = 0; i < n1; i++){
            a1[i] = array[esq + i];
            movimentacoes++; 
        }

        for(int j = 0; j < n2; j++){
            a2[j] = array[meio + j + 1];
            movimentacoes++; 
        }

        int i = 0, j = 0;
        int k = esq;
        
        while(i < n1 && j < n2){
            if (isMenorOuIgual(a1[i], a2[j])){
                array[k] = a1[i++];
            } else {
                array[k] = a2[j++];
            }
            movimentacoes++; 
            k++;
        }

        while(i < n1){
            array[k++] = a1[i++];
            movimentacoes++; 
        }

        while(j < n2){
            array[k++] = a2[j++];
            movimentacoes++; 
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
        } // scArquivo é fechado automaticamente aqui

        // Leitura de entrada do usuário
        try (Scanner sc = new Scanner(System.in)) {
            while (true) {
                String idString = sc.nextLine();

                if (idString.equals("FIM")) break;

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
        } // sc é fechado automaticamente aqui


        // Ordenação
        n = jogosInclusos.size();
        Game[] array = jogosInclusos.toArray(new Game[n]);
        
        long inicioOrdenacao = now(); 
        sort(array);
        long fimOrdenacao = now();   
        
        double tempoOrdenacao = (double)(fimOrdenacao - inicioOrdenacao); 

        // Saída 
        System.out.println("| 5 precos mais caros |");
        int countMax = 0;
        for (int i = n - 1; i >= 0 && countMax < 5; i--) {
            printGame(array[i]);
            countMax++;
        }
        System.out.println();

        System.out.println("| 5 precos mais baratos |");
        for (int i = 0; i < n && i < 5; i++) {
            printGame(array[i]);
        }
       

        // Geração do Arquivo de LOG
        String matricula = "888678";
        String nomeArquivo = matricula + "_mergesort.txt";

        try (FileWriter logWriter = new FileWriter(nomeArquivo)) {
            int compTotal = comparacoes;
            int movTotal = movimentacoes;

            String logLine = String.format("%s\t%d\t%d\t%.4f", matricula, compTotal, movTotal, tempoOrdenacao);
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