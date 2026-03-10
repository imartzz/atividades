
/**
 * Classe principal para testar a implementação da ListaDupla.
 * Contém o método main.
 */
public class TesteListaDupla {

    public static void main(String[] args) {
        System.out.println("--- Testando a Lista Duplamente Encadeada ---");
        ListaDupla minhaLista = new ListaDupla();

        try {
            // 1. Inserindo elementos
            minhaLista.inserirInicio(10);
            minhaLista.mostrar(); // [ 10 ]
            minhaLista.inserirFim(50);
            minhaLista.mostrar(); // [ 10 50 ]
            minhaLista.inserirInicio(5);
            minhaLista.mostrar(); // [ 5 10 50 ]
            
            // Testando a ordem inversa
            minhaLista.mostrarInverso(); // [ (inverso) 50 10 5 ]

            // 2. Inserindo por posição
            // Inserir 20 na posição 2 (entre 10 e 50)
            minhaLista.inserir(20, 2);
            minhaLista.mostrar(); // [ 5 10 20 50 ]
            System.out.println("Tamanho atual: " + minhaLista.tamanho()); // Tamanho atual: 4

            // 3. Removendo elementos
            int removidoInicio = minhaLista.removerInicio();
            System.out.println("Removeu do início: " + removidoInicio); // Removeu do início: 5
            minhaLista.mostrar(); // [ 10 20 50 ]

            int removidoFim = minhaLista.removerFim();
            System.out.println("Removeu do fim: " + removidoFim); // Removeu do fim: 50
            minhaLista.mostrar(); // [ 10 20 ]
            minhaLista.mostrarInverso(); // [ (inverso) 20 10 ]

            // Adicionando mais para testar remoção por pos
            minhaLista.inserirFim(30);
            minhaLista.inserirFim(40);
            minhaLista.mostrar(); // [ 10 20 30 40 ]

            // 4. Removendo por posição
            // Remover da posição 1 (o elemento 20)
            int removidoPos = minhaLista.remover(1);
            System.out.println("Removeu da pos 1: " + removidoPos); // Removeu da pos 1: 20
            minhaLista.mostrar(); // [ 10 30 40 ]
            minhaLista.mostrarInverso(); // [ (inverso) 40 30 10 ]

            // 5. Testando erros
            System.out.println("\n--- Testando Erros ---");
            // Tentando remover de posição inválida
            try {
                minhaLista.remover(99);
            } catch (Exception e) {
                System.out.println("Erro capturado: " + e.getMessage());
            }

            // Esvaziando a lista
            minhaLista.removerInicio();
            minhaLista.removerInicio();
            minhaLista.removerInicio();
            minhaLista.mostrar(); // [ ]

            // Tentando remover da lista vazia
            try {
                minhaLista.removerFim();
            } catch (Exception e) {
                System.out.println("Erro capturado: " + e.getMessage());
            }

        } catch (Exception e) {
            System.out.println("Ocorreu um erro inesperado!");
            e.printStackTrace();
        }
    }
}

/**
 * Implementação da Lista Duplamente Encadeada com nós sentinela.
 */
class ListaDupla {

    // --- Célula Dupla (Inner Class) ---
    class CelulaDupla {
        public int elemento;
        public CelulaDupla ant; // Ponteiro para anterior
        public CelulaDupla prox; // Ponteiro para próximo

        public CelulaDupla() {
            this(0);
        }

        public CelulaDupla(int elemento) {
            this.elemento = elemento;
            this.ant = null;
            this.prox = null;
        }
    }

    // --- Atributos da Lista ---
    private CelulaDupla primeiro; // Nó sentinela cabeça
    private CelulaDupla ultimo;   // Nó sentinela cauda

    // --- Construtor ---
    public ListaDupla() {
        // Cria os sentinelas
        primeiro = new CelulaDupla();
        ultimo = new CelulaDupla();

        // Liga os sentinelas (lista vazia)
        primeiro.prox = ultimo;
        ultimo.ant = primeiro;
    }

    // --- Métodos de Inserção ---

    public void inserirInicio(int x) {
        CelulaDupla nova = new CelulaDupla(x);
        
        // Ajusta ponteiros da nova célula
        nova.ant = primeiro;
        nova.prox = primeiro.prox;

        // Ajusta ponteiros das células adjacentes
        primeiro.prox.ant = nova; // O antigo primeiro elemento aponta para a nova
        primeiro.prox = nova;     // O sentinela aponta para a nova

        nova = null;
    }

    public void inserirFim(int x) {
        CelulaDupla nova = new CelulaDupla(x);

        // Ajusta ponteiros da nova célula
        nova.prox = ultimo;
        nova.ant = ultimo.ant;

        // Ajusta ponteiros das células adjacentes
        ultimo.ant.prox = nova; // O antigo último elemento aponta para a nova
        ultimo.ant = nova;      // O sentinela cauda aponta para a nova

        nova = null;
    }

    // --- Métodos de Remoção ---

    /**
     * Helper para verificar se a lista está vazia
     */
    public boolean isVazia() {
        return (primeiro.prox == ultimo);
    }

    public int removerInicio() throws Exception {
        if (isVazia()) {
            throw new Exception("Erro ao remover: Lista vazia!");
        }

        CelulaDupla tmp = primeiro.prox; // Célula a ser removida
        int elemento = tmp.elemento;

        // Faz o "bypass" da célula tmp
        primeiro.prox = tmp.prox;
        tmp.prox.ant = primeiro;

        // Isola tmp (boa prática)
        tmp.ant = tmp.prox = null;
        tmp = null;

        return elemento;
    }

    public int removerFim() throws Exception {
        if (isVazia()) {
            throw new Exception("Erro ao remover: Lista vazia!");
        }

        CelulaDupla tmp = ultimo.ant; // Célula a ser removida
        int elemento = tmp.elemento;

        // Faz o "bypass" da célula tmp
        ultimo.ant = tmp.ant;
        tmp.ant.prox = ultimo;

        // Isola tmp
        tmp.ant = tmp.prox = null;
        tmp = null;

        return elemento;
    }

    // --- Métodos por Posição ---

    public int tamanho() {
        int tamanho = 0;
        // Começa do primeiro elemento (primeiro.prox) e para antes do sentinela cauda
        for (CelulaDupla i = primeiro.prox; i != ultimo; i = i.prox) {
            tamanho++;
        }
        return tamanho;
    }

    public void inserir(int x, int pos) throws Exception {
        int tamanho = tamanho();
        if (pos < 0 || pos > tamanho) {
            throw new Exception("Erro ao inserir: Posição inválida!");
        } else if (pos == 0) {
            inserirInicio(x);
        } else if (pos == tamanho) {
            inserirFim(x);
        } else {
            // Anda até a célula *anterior* à posição de inserção
            CelulaDupla i = primeiro.prox; // Começa no primeiro elemento
            for (int j = 0; j < pos - 1; j++) {
                i = i.prox;
            }
            // 'i' agora é a célula na posição (pos-1)
            
            CelulaDupla nova = new CelulaDupla(x);
            
            // Conecta a nova célula
            nova.ant = i;
            nova.prox = i.prox;

            // Conecta as células adjacentes à nova
            nova.ant.prox = nova;
            nova.prox.ant = nova;

            nova = i = null;
        }
    }

    public int remover(int pos) throws Exception {
        int tamanho = tamanho();
        int elemento;

        if (isVazia() || pos < 0 || pos >= tamanho) {
            throw new Exception("Erro ao remover: Lista vazia ou posição inválida!");
        } else if (pos == 0) {
            elemento = removerInicio();
        } else if (pos == tamanho - 1) {
            elemento = removerFim();
        } else {
            // Anda até a célula *da posição* a ser removida
            CelulaDupla i = primeiro.prox; // Começa no primeiro elemento (pos 0)
            for (int j = 0; j < pos; j++) {
                i = i.prox;
            }
            // 'i' é a célula a ser removida

            elemento = i.elemento;

            // Faz o "bypass" da célula 'i'
            i.ant.prox = i.prox;
            i.prox.ant = i.ant;

            // Isola 'i'
            i.prox = i.ant = null;
            i = null;
        }
        return elemento;
    }

    // --- Métodos Auxiliares ---

    /**
     * Mostra os elementos do início ao fim.
     */
    public void mostrar() {
        System.out.print("[ ");
        for (CelulaDupla i = primeiro.prox; i != ultimo; i = i.prox) {
            System.out.print(i.elemento + " ");
        }
        System.out.println("]");
    }

    /**
     * Mostra os elementos do fim ao início (prova do ponteiro 'ant').
     */
    public void mostrarInverso() {
        System.out.print("[ (inverso) ");
        for (CelulaDupla i = ultimo.ant; i != primeiro; i = i.ant) {
            System.out.print(i.elemento + " ");
        }
        System.out.println("]");
    }
}