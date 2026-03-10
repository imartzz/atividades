import java.io.File;
import java.io.FileWriter;
import java.util.*;

public class ArvoreDeArvore {

   
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

    

    
    static class No2 {
        Game elemento;
        No2 esq, dir;

        public No2(Game elemento) {
            this.elemento = elemento;
            this.esq = null;
            this.dir = null;
        }
    }

    
    static class No1 {
        int elemento; // Chave numérica (0 a 14)
        No1 esq, dir;
        No2 outro;    // Ponteiro para a raiz da segunda árvore

        public No1(int elemento) {
            this.elemento = elemento;
            this.esq = null;
            this.dir = null;
            this.outro = null;
        }
    }

    
    static class ArvoreArvore {
        private No1 raiz;

        public ArvoreArvore() throws Exception {
            raiz = null;
            
            int[] chaves = {7, 3, 11, 1, 5, 9, 13, 0, 2, 4, 6, 8, 10, 12, 14};
            for (int chave : chaves) {
                inserirNaPrimeiraArvore(chave);
            }
        }

        
        private void inserirNaPrimeiraArvore(int x) throws Exception {
            raiz = inserirNaPrimeiraArvore(x, raiz);
        }

        private No1 inserirNaPrimeiraArvore(int x, No1 i) throws Exception {
            if (i == null) {
                i = new No1(x);
            } else if (x < i.elemento) {
                i.esq = inserirNaPrimeiraArvore(x, i.esq);
            } else if (x > i.elemento) {
                i.dir = inserirNaPrimeiraArvore(x, i.dir);
            }
            return i;
        }

        
        public void inserir(Game g) throws Exception {
            //  Calcular a chave da primeira árvore
            int chave1 = g.estimatedOwners % 15;

            //  Pesquisar o nó correspondente na primeira árvore
            No1 no1 = pesquisarNo1(chave1, raiz);

            if (no1 != null) {
                //  Inserir na segunda árvore associada a este nó
                no1.outro = inserirNaSegundaArvore(g, no1.outro);
            }
        }

        private No1 pesquisarNo1(int chave, No1 i) {
            if (i == null) return null;
            if (chave == i.elemento) return i;
            else if (chave < i.elemento) return pesquisarNo1(chave, i.esq);
            else return pesquisarNo1(chave, i.dir);
        }

        
        private No2 inserirNaSegundaArvore(Game g, No2 i) throws Exception {
            if (i == null) {
                i = new No2(g);
            } else if (g.name.compareTo(i.elemento.name) < 0) {
                i.esq = inserirNaSegundaArvore(g, i.esq);
            } else if (g.name.compareTo(i.elemento.name) > 0) {
                i.dir = inserirNaSegundaArvore(g, i.dir);
            } else {
                // Chave duplicada na segunda árvore, não inserir
                
            }
            return i;
        }

      
        // A pesquisa percorre a primeira árvore (Mostrar) e busca na segunda
        public boolean pesquisar(String nome) {
            System.out.print("=> "+ nome + " => raiz "); // Início do caminho
            return pesquisarGlobal(nome, raiz);
        }

        // Percorre a primeira árvore (Pre-order ou In-order modificado para busca)
        // Se encontrar na sub-árvore, retorna true e para.
        private boolean pesquisarGlobal(String nome, No1 no1) {
            if (no1 == null) return false;

            //  Tenta pesquisar na árvore secundária deste nó
            if (pesquisarNaSegundaArvore(nome, no1.outro)) {
                return true;
            }

            //  Se não encontrou, continua caminhando na primeira árvore
            
            // Tenta Esquerda
            System.out.print("ESQ ");
            if (pesquisarGlobal(nome, no1.esq)) {
                return true;
            }

            // Tenta Direita
            System.out.print("DIR ");
            if (pesquisarGlobal(nome, no1.dir)) {
                return true;
            }

            return false;
        }

        private boolean pesquisarNaSegundaArvore(String nome, No2 no2) {
            if (no2 == null) return false;

            int cmp = nome.compareTo(no2.elemento.name);
            comparacoes++; // Comparação de nomes 

            if (cmp == 0) {
                return true;
            } else if (cmp < 0) {
                System.out.print("esq "); 
                return pesquisarNaSegundaArvore(nome, no2.esq);
            } else {
                System.out.print("dir ");
                return pesquisarNaSegundaArvore(nome, no2.dir);
            }
        }
    }

  
    public static void main(String[] args) throws Exception {
        ArrayList<Game> todosOsJogos = new ArrayList<>();
        
        // Inicializa a Estrutura de Árvore de Árvore (Já cria a estrutura primária 0..14)
        ArvoreArvore arvore = new ArvoreArvore();

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
                        // Insere na Árvore de Árvore
                        arvore.inserir(jogoCSV);
                        break; 
                    }
                }
            } catch (Exception e) {
                // Ignora erros de parse ou duplicatas
            }
        }

        //  Leitura dos Nomes para pesquisa
        while (true) {
            String nomePesquisa = sc.nextLine();
            if (nomePesquisa.equalsIgnoreCase("FIM")) break;

            // O método pesquisar imprime o caminho
            boolean encontrou = arvore.pesquisar(nomePesquisa.trim());
            System.out.println(encontrou ? " SIM" : " NAO");
        }
        
        sc.close();

        //  Arquivo de Log
        long fimTempo = new Date().getTime();
        try (FileWriter logWriter = new FileWriter(MATRICULA + "_arvoreBinaria.txt")) {
            logWriter.write(String.format("%s\t%.4f\t%d", MATRICULA, (double)(fimTempo - inicioTempo), comparacoes));
        } catch (Exception e) {
            System.err.println("Erro log: " + e.getMessage());
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