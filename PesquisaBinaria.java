import java.io.File;
import java.io.FileWriter;
import java.util.*;

public class PesquisaBinaria {

    static int comparacoesPesquisa = 0;
    static int n = 0;
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
     * Compara duas Strings lexicograficamente, de forma manual.
     * Retorna:
     * - Um valor negativo se s1 for menor que s2.
     * - 0 se s1 for igual a s2.
     * - Um valor positivo se s1 for maior que s2.
     */
    public static int stringCompareManual(String s1, String s2) {
        int len1 = s1.length();
        int len2 = s2.length();
        int lim = Math.min(len1, len2);
        char[] v1 = s1.toCharArray();
        char[] v2 = s2.toCharArray();

        int k = 0;
        while (k < lim) {
            char c1 = v1[k];
            char c2 = v2[k];
            if (c1 != c2) {
                // A diferença no valor do caractere (ASCII) determina a ordem lexicográfica
                return c1 - c2; 
            }
            k++;
        }
        // Se uma string é prefixo da outra, a diferença de tamanho define qual é maior
        return len1 - len2;
    }


    /**
     * Realiza a pesquisa binária por nome e conta as comparações,
     * usando uma comparação de String manual.
     * Retorna true se o nome for encontrado.
     */
    public static boolean pesquisaBinaria(Game[] array, String chave) {
        int esq = 0;
        int dir = n - 1;
        boolean encontrado = false;

        while (esq <= dir) {
            int meio = esq + (dir - esq) / 2;
            
            // Uso da função de comparação manual
            int comparacao = stringCompareManual(chave, array[meio].name); 
            comparacoesPesquisa++; // Conta a comparação principal

            if (comparacao == 0) {
                // Encontrado
                encontrado = true;
                esq = dir + 1; // Força a saída do loop
            } else if (comparacao < 0) {
                // A chave está na metade esquerda
                dir = meio - 1;
            } else {
                // A chave está na metade direita
                esq = meio + 1;
            }
        }

        return encontrado;
    }

    public static long now(){
        return new Date().getTime();
    }


    public static void main(String[] args) throws Exception {
        ArrayList<Game> todosOsJogos = new ArrayList<>();
        ArrayList<Game> jogosInclusos = new ArrayList<>();

        //Pré-leitura do CSV
        try (Scanner scArquivo = new Scanner(new File("/tmp/games.csv"), "UTF-8")) {
            if (scArquivo.hasNextLine()) {
                scArquivo.nextLine(); // Pula o cabeçalho
            }

            while (scArquivo.hasNextLine()) {
                String linha = scArquivo.nextLine();
                ArrayList<String> dados = splitCsvAspas(linha);

                Game g = new Game();
                g.setId(dados.get(0));
                g.setName(dados.get(1).trim()); // Garante que o nome não tenha espaços extras
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

        //Leitura da Primeira Parte da Entrada (IDs)
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

            //Preparação (Ordenação)
            n = jogosInclusos.size();
            Game[] array = jogosInclusos.toArray(new Game[n]);

            // Ordena o array pela chave de pesquisa (Name)

            Arrays.sort(array, (g1, g2) -> g1.name.compareTo(g2.name));


            //Leitura da Segunda Parte da Entrada (Nomes para Pesquisa)

            // Inicializa a contagem de tempo para a Pesquisa Binária
            long inicioPesquisa = now();

            while (true) {

                String nomePesquisa = sc.nextLine();

                if (nomePesquisa.equalsIgnoreCase("FIM")) break;

                // A chave de pesquisa deve ser limpa (trim) para a pesquisa binária funcionar corretamente
                boolean resultado = pesquisaBinaria(array, nomePesquisa.trim());


                System.out.println(resultado ? " SIM" : " NAO");
            }

            long fimPesquisa = now();
            double tempoPesquisa = (double)(fimPesquisa - inicioPesquisa);


            String nomeArquivo = MATRICULA + "_binaria.txt";

            try (FileWriter logWriter = new FileWriter(nomeArquivo)) {

                String logLine = String.format("%s\t%.4f\t%d", MATRICULA, tempoPesquisa, comparacoesPesquisa);
                logWriter.write(logLine);
            } catch (Exception e) {
                System.err.println("Erro ao escrever no arquivo de log: " + e.getMessage());
            }

        }
    }

    /**
     * Quebra uma linha de CSV em colunas, respeitando aspas.
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