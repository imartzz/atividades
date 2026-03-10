import java.io.*;
import java.text.*;
import java.util.*;


class Game {

	private int id;
	private String name;
	private String releaseDate;
	private int estimatedOwners;
	private float price;
	private String[] supportedLanguages;
	private int metacriticScore;
	private float userScore;
	private int achievements;
	private String[] publishers;
	private String[] developers;
	private String[] categories;
	private String[] genres;
	private String[] tags;

	// Construtor vazio de games
	public Game() {}

	// Construtor com parametros de games
	public Game (
			int id, String name, String releaseDate, int estimatedOwners, float price, String[] supportedLanguages, 
			int metacriticScore, float userScore, int achievements, String[] publishers, String[] developers, 
			String[] categories, String[] genres, String[] tags
		) {
		this.id = id;
		this.name = name;
		this.releaseDate = releaseDate;
		this.estimatedOwners = estimatedOwners;
		this.price = price;
		this.supportedLanguages = supportedLanguages;
		this.metacriticScore = metacriticScore;
		this.userScore = userScore;
		this.achievements = achievements;
		this.publishers = publishers;
		this.developers = developers;
		this.categories = categories;
		this.genres = genres;
		this.tags = tags;
	}

	// Gets
	public int getId() { return this.id; }
	public String getName() { return this.name; }
	public String getReleaseDate() { return this.releaseDate; }
	public int getEstimatedOwners() { return this.estimatedOwners; }
	public float getPrice() { return this.price; }
	public String[] getSupportedLanguages() { return this.supportedLanguages; }
	public int getMetacriticScore() { return this.metacriticScore; }
	public float getUserScore() { return this.userScore; }
	public int getAchievements() { return this.achievements; }
	public String[] getPublishers() { return this.publishers; }
	public String[] getDevelopers() { return this.developers; }
	public String[] getCategories() { return this.categories; }
	public String[] getGenres() { return this.genres; }
	public String[] getTags() { return this.tags; }

	// Sets 
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
		if(s == null) return new String[0];
		s = s.replace("[", "").replace("]", "").replace("\"", "").replace("'", "");
		String[] partes = s.split(",");
		for (int i = 0; i < partes.length; i++) partes[i] = partes[i].trim();
		return partes;
	}

	public static int obterTamanho(String caminho) {
		int linhas = 0;
		try (BufferedReader br = new BufferedReader(new FileReader(caminho))) {
			br.readLine();
			while (br.readLine() != null) {
				linhas++;
			}
		} catch (IOException e) {
			System.out.println("Erro ao contar linhas: " + e.getMessage());
		}
		return linhas;
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
        if (dataOriginal == null || dataOriginal.trim().isEmpty()) {
			return "01/01/0001";
		}
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
        } else if (dataOriginal.matches("\\d{4}")) {
			return "01/01/" + dataOriginal;
		} else {
			try {
                SimpleDateFormat formatoAmericano = new SimpleDateFormat("MMM dd yyyy", Locale.ENGLISH);
				Date dataObj = formatoAmericano.parse(dataOriginal.replace(",", ""));
				SimpleDateFormat formatoFinal = new SimpleDateFormat("dd/MM/yyyy");
				return formatoFinal.format(dataObj);
			} catch (ParseException e) {
				return "01/01/0001";
			}
		}
    }
    
	public ArrayList<Game> preencherTodosGames(String caminho) {
		ArrayList<Game> todosOsJogos = new ArrayList<>();
        try (Scanner scArquivo = new Scanner(new File(caminho), "UTF-8")) {
            if (scArquivo.hasNextLine()) {
                scArquivo.nextLine(); // Pula o cabeçalho
            }

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
        } catch (FileNotFoundException e) {
			System.err.println("Arquivo não encontrado: " + caminho);
		} catch (Exception e) {
			System.err.println("Erro durante a pré-leitura do CSV: " + e.getMessage());
		}
		return todosOsJogos;
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
	
	

	// Procurar jogo pelo codigo
	public Game ProcurarPorCodigo(ArrayList<Game> todosOsGames, int id) {
		for (Game jogo : todosOsGames) {
			if (jogo.getId() == id) {
				return jogo;
			}
		}
		return null;
	}

	public void inserirInicio(Game[] game, Game aux, int n){
    	if(n >= game.length){
    		System.out.println("Erro ao inserir!");
			return;
		} 

		for(int i = n; i > 0; i--){
			game[i] = game[i-1];
		}

		game[0] = aux;
	}

	public void inserir(Game[] game, Game aux, int pos, int n){
      if(n >= game.length || pos < 0 || pos > n){
        System.out.println("Erro ao inserir!");
        return;
      }

      for(int i = n; i > pos; i--){
        game[i] = game[i-1];
      }

      game[pos] = aux;
	}

	public void inserirFim(Game[] game, Game aux, int n){
      if(n >= game.length){
        System.out.println("Erro ao inserir!");
        return;
      }

      game[n] = aux;
	}

	public Game removerInicio(Game[] game, int n){
      if (n == 0) {
        System.out.println("Erro ao remover!");
        return null;
      }

      Game resp = game[0];

      for(int i = 0; i < n - 1; i++){
        game[i] = game[i+1];
      }

      return resp;
	}

	public Game remover(Game[] game, int pos, int n){
      if (n == 0 || pos < 0 || pos >= n) {
        System.out.println("Erro ao remover!");
        return null;
      }

      Game resp = game[pos];

      for(int i = pos; i < n - 1; i++){
        game[i] = game[i+1];
      }

      return resp;
	}

	public Game removerFim(Game[] game, int n){
      if (n == 0) {
        System.out.println("Erro ao remover!");
        return null;
      }

      return game[n - 1];
	}


	public void printGame(int index) {
		System.out.println(
			"[" + index + "] => " +
			this.id + " ## " +
			this.name + " ## " +
			this.releaseDate + " ## " +
			this.estimatedOwners + " ## " +
			this.price + " ## " +
			Arrays.toString(this.getSupportedLanguages()) + " ## " +
			this.metacriticScore + " ## " +
			this.userScore + " ## " +
			this.achievements + " ## " +
			Arrays.toString(this.getPublishers()) + " ## " +
			Arrays.toString(this.getDevelopers()) + " ## " +
			Arrays.toString(this.getCategories()) + " ## " +
			Arrays.toString(this.getGenres()) + " ## " +
			Arrays.toString(this.getTags()) + " ##"
		);
	}
}


public class ListaSequencial {
	public static void main(String[] args) {
		Scanner scanner = new Scanner(System.in);
		String caminho = "/tmp/games.csv";
		
		Game util = new Game(); 
		
		//  Pré-leitura de todos os jogos do CSV 
		ArrayList<Game> todosOsGames = util.preencherTodosGames(caminho);

        // Define o tamanho máximo do vetor da lista sequencial
		Game[] vetorGames = new Game[todosOsGames.size()]; 
		int utilizados = 0;

		//  Leitura da Primeira Parte da Entrada (IDs) - 'FIM' como parada
		while (true) {
			String idString = scanner.nextLine();

			if (idString.equalsIgnoreCase("FIM")) break;

			try {
				int idNum = Integer.parseInt(idString.trim());
				
				//  Procurar o jogo pelo ID
				Game jogoParaInserir = util.ProcurarPorCodigo(todosOsGames, idNum);
				
				// Inserir no final da lista sequencial 
				if (jogoParaInserir != null && utilizados < vetorGames.length) {
					vetorGames[utilizados] = jogoParaInserir;
					utilizados++;
				}
			} catch (NumberFormatException e) {
				// Ignora entradas não numéricas/inválidas.
			} catch (NoSuchElementException e) {
                // Caso a entrada termine de forma inesperada 
                break;
            }
		}
		
		// Lendo o número de operações
		int numOperacoes = 0;
		if(scanner.hasNextInt()) {
			numOperacoes = scanner.nextInt();
			scanner.nextLine(); // Consome a quebra de linha após o número
		}

		// Processamento das operações
		for(int i = 0; i < numOperacoes; i++){
			String linhaOperacao = scanner.nextLine();
			String[] operacao = linhaOperacao.split(" ");
			Game jogo;
			Game removido = null;
			int id;
			int posicao;

			switch (operacao[0]) {
				case "II":
					id = Integer.parseInt(operacao[1]);
					jogo = util.ProcurarPorCodigo(todosOsGames, id);
					if (jogo != null && utilizados < vetorGames.length) {
						util.inserirInicio(vetorGames, jogo, utilizados);
						utilizados++;
					} else if (jogo != null) {
                        // Se não houver espaço no array
                        System.out.println("Erro ao inserir!");
                    }
					break;
				case "I*":
					posicao = Integer.parseInt(operacao[1]);
					id = Integer.parseInt(operacao[2]);
					jogo = util.ProcurarPorCodigo(todosOsGames, id);
					if (jogo != null && posicao >= 0 && posicao <= utilizados && utilizados < vetorGames.length) {
						util.inserir(vetorGames, jogo, posicao, utilizados);
						utilizados++;
					} else if (jogo != null) {
                        System.out.println("Erro ao inserir!");
                    }
					break;
				case "IF":
					id = Integer.parseInt(operacao[1]);
					jogo = util.ProcurarPorCodigo(todosOsGames, id);
					if (jogo != null && utilizados < vetorGames.length) {
						util.inserirFim(vetorGames, jogo, utilizados);
						utilizados++;
					} else if (jogo != null) {
                        System.out.println("Erro ao inserir!");
                    }
					break;
				case "RI":
					removido = util.removerInicio(vetorGames, utilizados);
					if (removido != null) {
						System.out.println("(R) " + removido.getName());
						utilizados--;
					}
					break;
				case "R*":
					posicao = Integer.parseInt(operacao[1]);
					removido = util.remover(vetorGames, posicao, utilizados);
					if (removido != null) {
						System.out.println("(R) " + removido.getName());
						utilizados--;
					}
					break;
				case "RF":
					removido = util.removerFim(vetorGames, utilizados); 
					if (removido != null) {
						System.out.println("(R) " + removido.getName());
						utilizados--;
					}
					break;
			}
		}

		// Impressão final dos elementos
		for (int i = 0; i < utilizados; i++) {
			vetorGames[i].printGame(i);
		}

	    scanner.close();
	}
}