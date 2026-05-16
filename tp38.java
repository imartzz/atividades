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
        // separa por virgula, tratando corretamente as aspas do CSV
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
        
        // Horários
        String[] horarios = partes[7].split("-");
        this.horarioAbertura = horarios[0].trim();
        this.horarioFechamento = horarios[1].trim();
        
        // data e Status
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

class CelulaDupla {
    public Restaurante elemento;
    public CelulaDupla ant;
    public CelulaDupla prox;

    public CelulaDupla() {
        this(null);
    }

    public CelulaDupla(Restaurante elemento) {
        this.elemento = elemento;
        this.ant = null;
        this.prox = null;
    }
}


class ListaDupla {
    private CelulaDupla primeiro;
    private CelulaDupla ultimo;

    public ListaDupla() {
        primeiro = new CelulaDupla(); // Nó cabeça 
        ultimo = primeiro;
    }

    public void inserirInicio(Restaurante x) {
        CelulaDupla tmp = new CelulaDupla(x);
        tmp.ant = primeiro;
        tmp.prox = primeiro.prox;
        primeiro.prox = tmp;
        if (primeiro == ultimo) {
            ultimo = tmp;
        } else {
            tmp.prox.ant = tmp;
        }
        tmp = null;
    }

    public void inserirFim(Restaurante x) {
        ultimo.prox = new CelulaDupla(x);
        ultimo.prox.ant = ultimo;
        ultimo = ultimo.prox;
    }

    public void inserir(Restaurante x, int pos) throws Exception {
        int tamanho = tamanho();
        if (pos < 0 || pos > tamanho) {
            throw new Exception("Erro ao inserir: posição inválida!");
        } else if (pos == 0) {
            inserirInicio(x);
        } else if (pos == tamanho) {
            inserirFim(x);
        } else {
            CelulaDupla i = primeiro;
            for (int j = 0; j < pos; j++, i = i.prox);
            
            CelulaDupla tmp = new CelulaDupla(x);
            tmp.prox = i.prox;
            tmp.ant = i;
            tmp.prox.ant = tmp;
            i.prox = tmp;
            tmp = null;
            i = null;
        }
    }

    public Restaurante removerInicio() throws Exception {
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
        Restaurante resp = tmp.elemento;
        tmp.prox = tmp.ant = null;
        tmp = null;
        return resp;
    }

    public Restaurante removerFim() throws Exception {
        if (primeiro == ultimo) {
            throw new Exception("Erro ao remover: lista vazia!");
        }
        Restaurante resp = ultimo.elemento;
        ultimo = ultimo.ant;
        ultimo.prox.ant = null;
        ultimo.prox = null;
        return resp;
    }

    public Restaurante remover(int pos) throws Exception {
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
            Restaurante resp = i.elemento;
            i.prox = i.ant = null;
            i = null;
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
            System.out.println("[" + pos + "] " + i.elemento.imprimir());
            pos++;
        }
    }
}


public class tp38 {
    public static void main(String[] args) {
        Restaurante[] colecao = new Restaurante[600];
        int tamanhoColecao = 0;
        
        //  Carga da base de dados do CSV
        try {
            Scanner scCsv = new Scanner(new File("/tmp/restaurantes.csv"));
            if (scCsv.hasNextLine()) {
                scCsv.nextLine(); // Pular linha de cabeçalho
            }
            while (scCsv.hasNextLine()) {
                String linha = scCsv.nextLine();
                if (!linha.trim().isEmpty()) {
                    Restaurante r = new Restaurante();
                    r.ler(linha);
                    colecao[tamanhoColecao++] = r;
                }
            }
            scCsv.close();
        } catch (IOException e) {
            System.out.println("Erro ao ler o arquivo: " + e.getMessage());
            return;
        }

        Scanner scIn = new Scanner(System.in);
        ListaDupla lista = new ListaDupla();

        //  Leitura dos IDs iniciais inseridos no fim da lista até encontrar "-1"
        while (scIn.hasNext()) {
            String entrada = scIn.next();
            if (entrada.equals("-1")) {
                break;
            }

            int idBusca = Integer.parseInt(entrada);
            for (int i = 0; i < tamanhoColecao; i++) {
                if (colecao[i].getId() == idBusca) {
                    lista.inserirFim(colecao[i]);
                    break;
                }
            }
        }

        //  Processamento das entradas
        if (scIn.hasNextInt()) {
            int numComandos = scIn.nextInt();
            
            for (int i = 0; i < numComandos; i++) {
                String comando = scIn.next();

                try {
                    if (comando.equals("II")) {
                        int idBusca = scIn.nextInt();
                        lista.inserirInicio(buscarRestaurante(colecao, tamanhoColecao, idBusca));
                    } else if (comando.equals("IF")) {
                        int idBusca = scIn.nextInt();
                        lista.inserirFim(buscarRestaurante(colecao, tamanhoColecao, idBusca));
                    } else if (comando.equals("I*")) {
                        int pos = scIn.nextInt();
                        int idBusca = scIn.nextInt();
                        lista.inserir(buscarRestaurante(colecao, tamanhoColecao, idBusca), pos);
                    } else if (comando.equals("RI")) {
                        Restaurante r = lista.removerInicio();
                        System.out.println("(R) " + r.getNome());
                    } else if (comando.equals("RF")) {
                        Restaurante r = lista.removerFim();
                        System.out.println("(R) " + r.getNome());
                    } else if (comando.equals("R*")) {
                        int pos = scIn.nextInt();
                        Restaurante r = lista.remover(pos);
                        System.out.println("(R) " + r.getNome());
                    }
                } catch (Exception e) {
                    System.out.println(e.getMessage());
                }
            }
        }
        
        scIn.close();

        //  Mostrar estado final da lista encadeada
        lista.mostrar();
    }

    private static Restaurante buscarRestaurante(Restaurante[] colecao, int tamanho, int id) {
        for (int i = 0; i < tamanho; i++) {
            if (colecao[i].getId() == id) {
                return colecao[i];
            }
        }
        return null;
    }
}