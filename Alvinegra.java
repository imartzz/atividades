import java.io.File;
import java.io.FileWriter;
import java.util.*;

public class Alvinegra {

    // Variáveis globais de estatística
    static int comparacoes = 0;
    static long inicioTempo = 0;
    static final String MATRICULA = "888678";

    // Constantes de cor
    static final boolean PRETO = false;
    static final boolean VERMELHO = true;

    
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
            try { s = s.replace(',', '.').trim(); price = Float.parseFloat(s); } catch (NumberFormatException e) { price = 0.0f; }
        }
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

    
    
    static class No {
        Game elemento;
        No esq, dir;
        boolean cor; // VERMELHO = true, PRETO = false

        public No(Game elemento) {
            this(elemento, null, null, VERMELHO);
        }

        public No(Game elemento, No esq, No dir, boolean cor) {
            this.elemento = elemento;
            this.esq = esq;
            this.dir = dir;
            this.cor = cor;
        }
    }

    static class ArvoreAlvinegra {
        private No raiz;

        public ArvoreAlvinegra() {
            raiz = null;
        }

        

        private boolean ehVermelho(No no) {
            if (no == null) return false;
            return no.cor == VERMELHO;
        }

        // Rotação para a esquerda: Move o link vermelho do lado direito para o lado esquerdo
        private No rotacionarEsquerda(No h) {
            No x = h.dir;
            h.dir = x.esq;
            x.esq = h;
            x.cor = h.cor;
            h.cor = VERMELHO;
            return x;
        }

        // Rotação para a direita: Corrige o duplo vermelho à esquerda
        private No rotacionarDireita(No h) {
            No x = h.esq;
            h.esq = x.dir;
            x.dir = h;
            x.cor = h.cor;
            h.cor = VERMELHO;
            return x;
        }

        // Inversão de cores: Move o vermelho para o nó pai
        private void inverterCores(No h) {
            h.cor = !h.cor;
            if (h.esq != null) h.esq.cor = !h.esq.cor;
            if (h.dir != null) h.dir.cor = !h.dir.cor;
        }

        
        
        public void inserir(Game elemento) throws Exception {
            raiz = inserir(elemento, raiz);
            raiz.cor = PRETO;
        }

        private No inserir(Game elemento, No h) throws Exception {
            // Caso base: Árvore vazia ou folha Insere como VERMELHO
            if (h == null) return new No(elemento);

            // Comparação de chaves (Name)
            int cmp = elemento.name.compareTo(h.elemento.name);

            if (cmp < 0) {
                h.esq = inserir(elemento, h.esq);
            } else if (cmp > 0) {
                h.dir = inserir(elemento, h.dir);
            } else {
                // Chave duplicada
                throw new Exception("Chave duplicada: " + elemento.name);
            }

            
            
            //  Corrigir um link vermelho à direita 
            if (ehVermelho(h.dir) && !ehVermelho(h.esq)) {
                h = rotacionarEsquerda(h);
            }

            //  Corrigir dois links vermelhos seguidos à esquerda
            if (ehVermelho(h.esq) && ehVermelho(h.esq.esq)) {
                h = rotacionarDireita(h);
            }

            //  Dividir um nó 4-nó (ambos filhos vermelhos)
            if (ehVermelho(h.esq) && ehVermelho(h.dir)) {
                inverterCores(h);
            }

            return h;
        }

        
        
        public boolean pesquisar(String nome) {
            System.out.print(nome + ": =>raiz");
            return pesquisar(nome, raiz);
        }

        private boolean pesquisar(String nome, No h) {
            if (h == null) {
                return false;
            }

            int cmp = nome.compareTo(h.elemento.name);
            comparacoes++; // Conta a comparação de chaves

            if (cmp == 0) {
                return true;
            } else if (cmp < 0) {
                System.out.print(" esq");
                return pesquisar(nome, h.esq);
            } else {
                System.out.print(" dir");
                return pesquisar(nome, h.dir);
            }
        }
    }

   

    
    public static void main(String[] args) throws Exception {
        ArrayList<Game> todosOsJogos = new ArrayList<>();
        ArvoreAlvinegra arvore = new ArvoreAlvinegra();

        //  Leitura do CSV
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

        //  Leitura dos IDs para inserção
        while (true) {
            String idString = sc.nextLine();
            if (idString.equalsIgnoreCase("FIM")) break;

            try {
                int idNum = Integer.parseInt(idString.trim());
                for (Game jogoCSV : todosOsJogos) {
                    if (jogoCSV.id == idNum) {
                        try {
                            arvore.inserir(jogoCSV);
                        } catch (Exception e) {
                            // Ignora duplicatas conforme enunciado
                        }
                        break; 
                    }
                }
            } catch (NumberFormatException e) {
                // Ignora erros
            }
        }

        //  Leitura dos Nomes para pesquisa
        while (true) {
            String nomePesquisa = sc.nextLine();
            if (nomePesquisa.equalsIgnoreCase("FIM")) break;

            boolean encontrou = arvore.pesquisar(nomePesquisa.trim());
            
            // Finaliza a linha de saída
            System.out.println(encontrou ? " SIM" : " NAO");
        }
        
        sc.close();

        //  Arquivo de Log
        long fimTempo = new Date().getTime();
        String nomeArquivo = MATRICULA + "_alvinegra.txt";
        
        try (FileWriter logWriter = new FileWriter(nomeArquivo)) {
            // Matrícula <tab> Tempo (ms) <tab> Comparacoes
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