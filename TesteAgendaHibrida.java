import java.util.Scanner;

/**
 * 1. Classe Contato (O elemento a ser armazenado)
 * Atributos: nome, telefone, email (String) e CPF (int) [cite: 31, 32]
 */
class Contato {
    public String nome;
    public String telefone;
    public String email;
    public int cpf; 

    public Contato(String nome, String telefone, String email, int cpf) {
        this.nome = nome;
        this.telefone = telefone;
        this.email = email;
        this.cpf = cpf;
    }

    @Override
    public String toString() {
        return nome + " (Tel: " + telefone + ", CPF: " + cpf + ")";
    }
}

/**
 * 2. Classe Celula Dupla de Contato (Nó da Lista Duplamente Encadeada)
 * Contém o Contato e ponteiros 'prox' e 'ant' (Lista Dupla)
 */
class CelulaDuplaContato {
    public CelulaDuplaContato prox;
    public CelulaDuplaContato ant; // Para ser Lista Dupla
    public Contato contato; 

    public CelulaDuplaContato(Contato x) {
        this.contato = x;  
        this.prox = null;
        this.ant = null;
    }
}

/**
 * 3. Classe NoAlf (Nó da Árvore Binária)
 * Atributos: char letra, NoAlf esq, dir; CelulaDuplaContato primeiro e ultimo [cite: 34]
 */
class NoAlf {
    public char letra;       // Chave de pesquisa (primeira letra do nome)
    public NoAlf esq;
    public NoAlf dir;
    public CelulaDuplaContato primeiro;  // Sentinela: Início da Lista Dupla
    public CelulaDuplaContato ultimo;    // Sentinela: Fim da Lista Dupla

    public NoAlf(char letra) {
        this.letra = letra;
        this.esq = null;
        this.dir = null;
        
        // Inicializa a Lista Dupla com nós sentinela
        this.primeiro = new CelulaDuplaContato(null); 
        this.ultimo = new CelulaDuplaContato(null);   
        
        // Liga os sentinelas (Lista vazia)
        this.primeiro.prox = this.ultimo;
        this.ultimo.ant = this.primeiro;
    }

    // --- Métodos de Lista Dupla (Aninhados ao Nó) ---

    /**
     * Insere o Contato no FIM da Lista Dupla deste Nó.
     */
    public void inserirLista(Contato c) {
        CelulaDuplaContato nova = new CelulaDuplaContato(c);

        nova.prox = this.ultimo;
        nova.ant = this.ultimo.ant;

        this.ultimo.ant.prox = nova;
        this.ultimo.ant = nova;
    }

    /**
     * Remove um Contato da Lista Dupla deste Nó pelo nome.
     */
    public boolean removerLista(String nome) {
        for (CelulaDuplaContato i = primeiro.prox; i != ultimo; i = i.prox) {
            if (i.contato.nome.equalsIgnoreCase(nome)) {
                // Remove o nó 'i' da lista
                i.ant.prox = i.prox;
                i.prox.ant = i.ant;
                
                // Isola
                i.ant = i.prox = null;
                return true;
            }
        }
        return false;
    }
}

/**
 * 4. Classe Agenda (Controle Principal)
 * Atributos: NoAlf raiz [cite: 35]
 */
class Agenda {
    private NoAlf raiz; 
    
    public Agenda() {  
        this.raiz = null;
        // Inicializa a árvore com as 26 letras (A-Z) maiúsculas, sem acento [cite: 36]
        inicializarArvoreAlfabetica();
    }

    private void inicializarArvoreAlfabetica() {
        // Para uma árvore balanceada, geralmente se insere o elemento do meio (M ou N) primeiro.
        char[] letrasBalanceadas = {
            'M', 'G', 'T', 'D', 'I', 'Q', 'W', 'B', 'E', 'H', 'K', 'O', 'S', 'V', 'Z', 
            'A', 'C', 'F', 'J', 'L', 'N', 'P', 'R', 'U', 'X', 'Y'
        };
        
        for (char letra : letrasBalanceadas) {
            this.raiz = inserirNoArvore(this.raiz, letra);
        }
        System.out.println("Árvore de letras (A-Z) inicializada.");
    }

    // --- Métodos de Controle da Árvore ---

    private NoAlf inserirNoArvore(NoAlf i, char letra) {
        if (i == null) {
            i = new NoAlf(letra);
        } else if (letra < i.letra) {
            i.esq = inserirNoArvore(i.esq, letra);
        } else if (letra > i.letra) {
            i.dir = inserirNoArvore(i.dir, letra);
        }
        return i;
    }

    private NoAlf pesquisarNoArvore(NoAlf i, char letra) {
        if (i == null || letra == i.letra) {
            return i;
        } else if (letra < i.letra) {
            return pesquisarNoArvore(i.esq, letra);
        } else {
            return pesquisarNoArvore(i.dir, letra);
        }
    }
    
    // --- Métodos da Agenda (Híbridos) solicitados [cite: 35] ---

    /**
     * Insere um Contato na Lista Dupla correta da Árvore.
     * Complexidade: O(h), onde h é a altura da árvore. O(logN) (Melhor caso) ou O(N) (Pior caso).
     */
    public void inserir(Contato contato) throws Exception {
        if (contato == null || contato.nome == null || contato.nome.isEmpty()) {
            throw new Exception("Contato inválido.");
        }
        char primeiraLetra = Character.toUpperCase(contato.nome.charAt(0));
        
        NoAlf noLetra = pesquisarNoArvore(this.raiz, primeiraLetra);
        
        if (noLetra != null) {
            noLetra.inserirLista(contato);
            // System.out.println("-> Contato " + contato.nome + " inserido em " + noLetra.letra);
        } else {
            // Este caso só deve ocorrer se a letra não for de A-Z, o que não deve acontecer
            // devido à inicialização, mas é um bom ponto de controle.
            throw new Exception("Nó da letra '" + primeiraLetra + "' não existe na árvore.");
        }
    }

    /**
     * Pesquisa um Contato por nome (Busca na Árvore e depois na Lista Dupla).
     * Complexidade: O(h + L), onde h é a altura da árvore e L é o tamanho da lista da letra.
     */
    public Contato pesquisar(String nome) {
        if (nome == null || nome.isEmpty()) return null;
        char primeiraLetra = Character.toUpperCase(nome.charAt(0));

        NoAlf noLetra = pesquisarNoArvore(this.raiz, primeiraLetra);

        if (noLetra != null) {
            // Pesquisa na lista dupla
            for (CelulaDuplaContato i = noLetra.primeiro.prox; i != noLetra.ultimo; i = i.prox) {
                if (i.contato.nome.equalsIgnoreCase(nome)) {
                    return i.contato;
                }
            }
        }
        return null;
    }
    
    /**
     * Pesquisa um Contato por CPF (Busca na Árvore e depois na Lista Dupla).
     * Complexidade: Pior caso O(N_total), onde N_total é o número total de contatos.
     * Necessita percorrer toda a árvore e todas as listas.
     */
    public Contato pesquisar(int cpf) {
        return pesquisarCPFRecursivo(this.raiz, cpf);
    }

    private Contato pesquisarCPFRecursivo(NoAlf i, int cpf) {
        if (i == null) {
            return null;
        }

        // 1. Pesquisa na lista do nó atual
        for (CelulaDuplaContato j = i.primeiro.prox; j != i.ultimo; j = j.prox) {
            if (j.contato.cpf == cpf) {
                return j.contato;
            }
        }

        // 2. Continua a busca nas subárvores
        Contato resultadoEsq = pesquisarCPFRecursivo(i.esq, cpf);
        if (resultadoEsq != null) return resultadoEsq;
        
        return pesquisarCPFRecursivo(i.dir, cpf);
    }
    
    /**
     * Remove um Contato por nome. [cite: 35]
     * Complexidade: O(h + L), onde h é a altura da árvore e L é o tamanho da lista.
     */
    public boolean remover(String nome) throws Exception {
        if (nome == null || nome.isEmpty()) return false;
        char primeiraLetra = Character.toUpperCase(nome.charAt(0));
        
        NoAlf noLetra = pesquisarNoArvore(this.raiz, primeiraLetra);
        
        if (noLetra != null) {
            return noLetra.removerLista(nome);
        }
        return false;
    }
    
    // --- Métodos Auxiliares ---
    
    public void mostrarAgenda() {
        System.out.println("--- Conteúdo da Agenda ---");
        mostrarAgendaRecursivo(this.raiz);
        System.out.println("--------------------------");
    }

    private void mostrarAgendaRecursivo(NoAlf i) {
        if (i != null) {
            mostrarAgendaRecursivo(i.esq);
            
            // Se a lista do nó não está vazia, mostra
            if (i.primeiro.prox != i.ultimo) {
                System.out.print("LETRA [" + i.letra + "]: ");
                System.out.print("[");
                for (CelulaDuplaContato j = i.primeiro.prox; j != i.ultimo; j = j.prox) {
                    System.out.print(j.contato.nome + " -> ");
                }
                System.out.println("]");
            }
            
            mostrarAgendaRecursivo(i.dir);
        }
    }
}

// 5. Classe de Teste (Exemplo de uso)
public class TesteAgendaHibrida {
    public static void main(String[] args) {
        Agenda agenda = new Agenda();

        try {
            // Criando Contatos (Note que a chave é a primeira letra)
            Contato c1 = new Contato("Maria", "1111", "m@a.com", 100);
            Contato c2 = new Contato("Monica", "2222", "mo@a.com", 200);
            Contato c3 = new Contato("Gustavo", "3333", "g@a.com", 300);
            Contato c4 = new Contato("Thiago", "4444", "t@a.com", 400);
            Contato c5 = new Contato("Gabriel", "5555", "ga@a.com", 500);
            
            // Inserindo na Agenda (Árvore + Lista Dupla)
            agenda.inserir(c1); 
            agenda.inserir(c2); // Monica e Maria vão para o nó 'M'
            agenda.inserir(c3); // Gustavo e Gabriel vão para o nó 'G'
            agenda.inserir(c5); 
            agenda.inserir(c4);
            
            agenda.mostrarAgenda();
            
            // Testando Pesquisa por Nome
            Contato resultadoNome = agenda.pesquisar("Monica");
            System.out.println("\nPesquisa por Nome (Monica): " + (resultadoNome != null ? resultadoNome : "Não encontrado."));
            
            // Testando Pesquisa por CPF
            Contato resultadoCPF = agenda.pesquisar(500);
            System.out.println("Pesquisa por CPF (500): " + (resultadoCPF != null ? resultadoCPF.nome : "Não encontrado."));
            
            // Testando Remoção
            boolean removido = agenda.remover("Maria");
            System.out.println("Remoção de Maria: " + (removido ? "Sucesso." : "Falha."));
            
            agenda.mostrarAgenda();

        } catch (Exception e) {
            System.err.println("Erro na execução: " + e.getMessage());
        }
    }
}