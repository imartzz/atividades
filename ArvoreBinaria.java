import java.io.File;
import java.io.FileWriter;
import java.util.*;

public class ArvoreBinaria {

    // Contadores globais para o log
    static int comparacoes = 0;
    static long inicioTempo = 0;
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
        
        // Auxiliares de data
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

 
    
    // Classe No (Nó da árvore)
    static class No {
        Game elemento;
        No esq, dir;

        public No(Game elemento) {
            this(elemento, null, null);
        }

        public No(Game elemento, No esq, No dir) {
            this.elemento = elemento;
            this.esq = esq;
            this.dir = dir;
        }
    }

    // Classe da Árvore
    static class ArvoreBST {
        private No raiz;

        public ArvoreBST() {
            raiz = null;
        }

        
        public void inserir(Game x) throws Exception {
            raiz = inserir(x, raiz);
        }

        private No inserir(Game x, No i) throws Exception {
            if (i == null) {
                i = new No(x);
            } else if (x.name.compareTo(i.elemento.name) < 0) {
                i.esq = inserir(x, i.esq);
            } else if (x.name.compareTo(i.elemento.name) > 0) {
                i.dir = inserir(x, i.dir);
            } else {
                // Elemento já existe (x.name == i.elemento.name)
              
                throw new Exception("Erro ao inserir: chave duplicada!");
            }
            return i;
        }

        
        // Exibe o caminho e retorna true se encontrou
        public boolean pesquisar(String nome) {
            System.out.print(nome + ": =>raiz"); // Começa sempre na raiz
            return pesquisar(nome, raiz);
        }

        private boolean pesquisar(String nome, No i) {
            if (i == null) {
                // Chegou ao fim e não encontrou
                return false;
            }

            int comparacao = nome.compareTo(i.elemento.name);
            comparacoes++; // Conta a comparação de chaves

            if (comparacao == 0) {
                // Encontrou
                return true;
            } else if (comparacao < 0) {
                // Vai para a esquerda
                System.out.print(" esq");
                return pesquisar(nome, i.esq);
            } else {
                // Vai para a direita
                System.out.print(" dir");
                return pesquisar(nome, i.dir);
            }
        }
    }

    
    public static void main(String[] args) throws Exception {
        ArrayList<Game> todosOsJogos = new ArrayList<>();
        ArvoreBST arvore = new ArvoreBST();

        //  Leitura do CSV
        try (Scanner scArquivo = new Scanner(new File("/tmp/games.csv"), "UTF-8")) {
            if (scArquivo.hasNextLine()) scArquivo.nextLine(); // Pula cabeçalho

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

        //  Leitura dos IDs para inserção na Árvore
        while (true) {
            String idString = sc.nextLine();
            if (idString.equalsIgnoreCase("FIM")) break;

            try {
                int idNum = Integer.parseInt(idString.trim());
                // Busca sequencial na lista completa para achar o objeto
                for (Game jogoCSV : todosOsJogos) {
                    if (jogoCSV.id == idNum) {
                        try {
                            // Tenta inserir na árvore (chave = Name)
                            arvore.inserir(jogoCSV);
                        } catch (Exception e) {
                            // Ignora duplicatas conforme enunciado
                        }
                        break; 
                    }
                }
            } catch (NumberFormatException e) {
                // Ignora linhas inválidas
            }
        }

        //  Leitura dos Nomes para pesquisa na Árvore
        while (true) {
            String nomePesquisa = sc.nextLine();
            if (nomePesquisa.equalsIgnoreCase("FIM")) break;

            // O método pesquisar já imprime o caminho (=>raiz esq dir...)
            boolean encontrou = arvore.pesquisar(nomePesquisa.trim());
            
            if (encontrou) {
                System.out.println(" SIM");
            } else {
                System.out.println(" NAO");
            }
        }
        
        sc.close();

        // 4. Gravação do Log
        long fimTempo = new Date().getTime();
        double tempoExecucao = (double)(fimTempo - inicioTempo) / 1000.0; // segundos 
        
        

        String nomeArquivoLog = MATRICULA + "_arvoreBinaria.txt";
        try (FileWriter logWriter = new FileWriter(nomeArquivoLog)) {
            // Matrícula <tab> Tempo <tab> Comparacoes
            logWriter.write(String.format("%s\t%.4f\t%d", MATRICULA, (double)(fimTempo - inicioTempo), comparacoes));
        } catch (Exception e) {
            System.err.println("Erro ao criar log: " + e.getMessage());
        }
    }

    
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