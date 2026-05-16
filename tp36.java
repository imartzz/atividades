import java.io.File;
import java.io.IOException;
import java.util.Scanner;


class Restaurante {
    private int id;
    private String nome;
    private String cidade;
    private int capacidade;
    private double avaliacao;
    private String tiposCozinha;
    private int faixaPreco;
    private String horarioAbertura;
    private String horarioFechamento;
    private String dataAbertura;
    private boolean aberto;

    public Restaurante() {}

    public void ler(String linha) {
        // separa por virgula ignorando vírgulas dentro de aspas duplas
        String[] partes = linha.split(",(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)");
        
        this.id = Integer.parseInt(partes[0].trim());
        this.nome = partes[1].trim();
        this.cidade = partes[2].trim();
        this.capacidade = Integer.parseInt(partes[3].trim());
        this.avaliacao = Double.parseDouble(partes[4].trim());
        
        // tratamento da lista de cozinhas
        this.tiposCozinha = partes[5].replace("\"", "").replace(";", ", ");
        if (!this.tiposCozinha.startsWith("[")) this.tiposCozinha = "[" + this.tiposCozinha + "]";
        
        this.faixaPreco = partes[6].trim().length();
        
        // Horarios
        String[] horarios = partes[7].split("-");
        this.horarioAbertura = horarios[0].trim();
        this.horarioFechamento = horarios[1].trim();
        
        // Data e Status
        this.dataAbertura = formatarDataString(partes[8].trim());
        this.aberto = partes[9].trim().equalsIgnoreCase("true");
    }

    
    private String formatarDataString(String dataIn) {
        String[] p = dataIn.split("-");
        if (p.length == 3) {
            return p[2] + "/" + p[1] + "/" + p[0];
        }
        return dataIn;
    }

    public String imprimir() {
        StringBuilder sb = new StringBuilder();
        sb.append("[").append(id).append(" ## ").append(nome).append(" ## ")
          .append(cidade).append(" ## ").append(capacidade).append(" ## ")
          .append(avaliacao).append(" ## ").append(tiposCozinha).append(" ## ");
        
        for (int i = 0; i < faixaPreco; i++) sb.append("$");
        
        sb.append(" ## ").append(horarioAbertura).append("-").append(horarioFechamento)
          .append(" ## ").append(dataAbertura).append(" ## ").append(aberto).append("]");
        return sb.toString();
    }

    public int getId() { return id; }
    public String getNome() { return nome; }
}


class Celula {
    public Restaurante elemento;
    public Celula prox;

    public Celula(Restaurante elemento) {
        this.elemento = elemento;
        this.prox = null;
    }
}

class Pilha {
    private Celula topo;

    public Pilha() {
        topo = null;
    }

    public void inserir(Restaurante x) {
        Celula tmp = new Celula(x);
        tmp.prox = topo;
        topo = tmp;
    }

    public Restaurante remover() throws Exception {
        if (topo == null) {
            throw new Exception("Erro: Pilha vazia!");
        }
        Restaurante resp = topo.elemento;
        topo = topo.prox;
        return resp;
    }

    // A tem q exibir do fundo para o topo
    // usei a recursão para inverter a ordem de exibição sem desempilhar
    public void mostrar() {
        mostrarRecursivo(topo, tamanho() - 1);
    }

    private void mostrarRecursivo(Celula i, int pos) {
        if (i != null) {
            mostrarRecursivo(i.prox, pos - 1);
            System.out.println("[" + pos + "] " + i.elemento.imprimir());
        }
    }

    private int tamanho() {
        int cont = 0;
        for (Celula i = topo; i != null; i = i.prox) {
            cont++;
        }
        return cont;
    }
}


public class tp36 {
    public static void main(String[] args) {
       
        Restaurante[] colecao = new Restaurante[500];
        int tamanhoColecao = 0;
        
        // carregar dados do CSV com Scanner
        try {
            Scanner scCsv = new Scanner(new File("/tmp/restaurantes.csv"));
            if (scCsv.hasNextLine()) {
                scCsv.nextLine(); // Pular a primeira linha de cabeçalho
            }
            while (scCsv.hasNextLine()) {
                String linha = scCsv.nextLine();
                Restaurante r = new Restaurante();
                r.ler(linha);
                colecao[tamanhoColecao++] = r;
            }
            scCsv.close();
        } catch (IOException e) {
            System.out.println("Erro ao ler o arquivo: " + e.getMessage());
            return;
        }

        Scanner scIn = new Scanner(System.in);
        Pilha pilha = new Pilha();

        //. Leitura dos IDs iniciais para empilhar até encontrar "-1"
        while (scIn.hasNext()) {
            String entrada = scIn.next();
            if (entrada.equals("-1")) {
                break;
            }

            int idBusca = Integer.parseInt(entrada);
            for (int i = 0; i < tamanhoColecao; i++) {
                if (colecao[i].getId() == idBusca) {
                    pilha.inserir(colecao[i]);
                    break;
                }
            }
        }

        //  Leitura do número de comandos
        if (scIn.hasNextInt()) {
            int numComandos = scIn.nextInt();
            
            for (int i = 0; i < numComandos; i++) {
                String comando = scIn.next();

                if (comando.equals("I")) {
                    int idBusca = scIn.nextInt();
                    for (int j = 0; j < tamanhoColecao; j++) {
                        if (colecao[j].getId() == idBusca) {
                            pilha.inserir(colecao[j]);
                            break;
                        }
                    }
                } else if (comando.equals("R")) {
                    try {
                        Restaurante r = pilha.remover();
                        System.out.println("(R)" + r.getNome());
                    } catch (Exception e) {
                        System.out.println(e.getMessage());
                    }
                }
            }
        }
        
        scIn.close();

        //  Exibir o estado final da pilha
        pilha.mostrar();
    }
}