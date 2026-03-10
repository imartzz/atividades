import java.io.File;
import java.io.FileWriter;
import java.util.*;

public class HashRehash {

    // Contadores globais
    static int comparacoes = 0;
    static long inicioTempo = 0;
    static final String MATRICULA = "888678";
    // CLASSE GAME
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
        void setEstimatedOwners(String s) { try { estimatedOwners = Integer.parseInt(s.replaceAll("[^0-9]", "")); } catch (NumberFormatException e) { estimatedOwners = 0; } }
        void setPrice(String s) { try { s = s.replace(',', '.').trim(); price = Float.parseFloat(s); } catch (NumberFormatException e) { price = 0.0f; } }
        void setLanguages(String s) { supportedLanguages = separar(s); }
        void setMetacritic(String s) { try { metacriticScore = Integer.parseInt(s.trim()); } catch (NumberFormatException e) { metacriticScore = 0; } }
        void setUserScore(String s) { try { s = s.replace(',', '.').trim(); userScore = Float.parseFloat(s); } catch (NumberFormatException e) { userScore = 0.0f; } }
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
        
        public static String converteMesParaNumero(String mes) {
            switch (mes) {
                case "Jan": return "01"; case "Feb": return "02"; case "Mar": return "03";
                case "Apr": return "04"; case "May": return "05"; case "Jun": return "06";
                case "Jul": return "07"; case "Aug": return "08"; case "Sep": return "09";
                case "Oct": return "10"; case "Nov": return "11"; case "Dec": return "12";
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
                if (dia.length() == 1) dia = "0" + dia;
                return dia + "/" + mes + "/" + ano;
            }
            return dataOriginal;
        }
    }
    // TABELA HASH COM REHASH
    static class TabelaHash {
        Game[] tabela;
        int tamTab; 

        public TabelaHash() {
            this.tamTab = 21; // Tamanho fixo solicitado
            this.tabela = new Game[tamTab];
            for(int i = 0; i < tamTab; i++){
                tabela[i] = null;
            }
        }

        // Calcula a soma dos caracteres ASCII
        private int getAsciiSum(String nome) {
            int soma = 0;
            for (int i = 0; i < nome.length(); i++) {
                soma += (int) nome.charAt(i);
            }
            return soma;
        }

        // Função Hash 1: (ASCII Name) mod tamTab
        public int hash1(String nome) {
            return getAsciiSum(nome) % tamTab;
        }

        // Função Hash 2 (Rehash): (ASCII Name + 1) mod tamTab
        public int hash2(String nome) {
            return (getAsciiSum(nome) + 1) % tamTab;
        }

        public void inserir(Game g) {
            if (g == null) return;
            
            //  Tenta a primeira função hash
            int pos = hash1(g.name);
            if (tabela[pos] == null) {
                tabela[pos] = g;
            } else {
                //. Colisão: Tenta a segunda função hash (Rehash)
                pos = hash2(g.name);
                if (tabela[pos] == null) {
                    tabela[pos] = g;
                }
                // Se ambas estiverem ocupadas, o elemento é descartado nesta implementação simples
            }
        }

        public int pesquisar(String nome) {
            //  Verifica Hash 1
            int pos = hash1(nome);
            if (tabela[pos] != null) {
                comparacoes++;
                if (tabela[pos].name.equals(nome)) {
                    return pos;
                }
            }

            //  Verifica Hash 2 (Rehash)
            pos = hash2(nome);
            if (tabela[pos] != null) {
                comparacoes++;
                if (tabela[pos].name.equals(nome)) {
                    return pos;
                }
            }

            return -1; // Não encontrou
        }
    }

    // MAIN
    public static void main(String[] args) throws Exception {
        ArrayList<Game> todosOsJogos = new ArrayList<>();
        TabelaHash tabela = new TabelaHash();

        //  Leitura do arquivo 
        try (Scanner scArquivo = new Scanner(new File("/tmp/games.csv"), "UTF-8")) {
            if (scArquivo.hasNextLine()) scArquivo.nextLine(); 

            while (scArquivo.hasNextLine()) {
                String linha = scArquivo.nextLine();
                ArrayList<String> dados = splitCsvAspas(linha);

                Game g = new Game();
                g.setId(dados.get(0));
                g.setName(dados.get(1).trim());
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

        Scanner sc = new Scanner(System.in);
        inicioTempo = new Date().getTime();

        //  Inserção na Tabela Hash
        while (true) {
            String idString = sc.nextLine();
            if (idString.equalsIgnoreCase("FIM")) break;

            try {
                int idNum = Integer.parseInt(idString.trim());
                for (Game jogoCSV : todosOsJogos) {
                    if (jogoCSV.id == idNum) {
                        tabela.inserir(jogoCSV);
                        break; 
                    }
                }
            } catch (NumberFormatException e) {}
        }

        //  Pesquisa
        while (true) {
            String nomePesquisa = sc.nextLine();
            if (nomePesquisa.equalsIgnoreCase("FIM")) break;

            String nomeLimpo = nomePesquisa.trim();
            int posEncontrada = tabela.pesquisar(nomeLimpo);
            
            // Se encontrar, mostra a posição real.
            // Se NÃO encontrar, mostra a posição onde deveria estar inicialmente (Hash 1).
            int posOutput = (posEncontrada != -1) ? posEncontrada : tabela.hash1(nomeLimpo);
            String resultado = (posEncontrada != -1) ? "SIM" : "NAO";

            System.out.println(nomeLimpo + ": (Posicao: " + posOutput + ") " + resultado);
        }
        
        sc.close();

        //  Gravação do Log
        long fimTempo = new Date().getTime();
        String nomeArquivoLog = MATRICULA + "_hashRehash.txt";
        try (FileWriter logWriter = new FileWriter(nomeArquivoLog)) {
            // Matrícula <tab> Tempo <tab> Comparacoes
            logWriter.write(String.format("%s\t%.4f\t%d", MATRICULA, (double)(fimTempo - inicioTempo) / 1000.0, comparacoes));
        } catch (Exception e) {
            System.err.println("Erro log: " + e.getMessage());
        }
    }

    // Auxiliar de parsing
    static ArrayList<String> splitCsvAspas(String linha) {
        ArrayList<String> partes = new ArrayList<>();
        StringBuilder atual = new StringBuilder();
        boolean aspas = false;
        for (int i = 0; i < linha.length(); i++) {
            char c = linha.charAt(i);
            if (c == '"') aspas = !aspas;
            else if (c == ',' && !aspas) { partes.add(atual.toString()); atual.setLength(0); }
            else atual.append(c);
        }
        partes.add(atual.toString());
        return partes;
    }
}