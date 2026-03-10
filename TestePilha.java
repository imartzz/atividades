
/**
 * Classe principal para testar a implementação da Pilha Flexível.
 * Contém o método main.
 */
public class TestePilha {

    public static void main(String[] args) {
        
        System.out.println("--- Testando a Pilha Flexível ---");
        PilhaFlexivel minhaPilha = new PilhaFlexivel();

        try {
            // 1. Inserindo elementos (Empilhar - PUSH)
            System.out.println("Empilhando: 10, 20, 30");
            minhaPilha.empilhar(10);
            minhaPilha.empilhar(20);
            minhaPilha.empilhar(30);
            minhaPilha.mostrar(); // [ 30 20 10 ] -> 30 está no topo
            System.out.println("Topo atual (peek): " + minhaPilha.consultarTopo()); // 30

            // 2. Removendo elementos (Desempilhar - POP)
            int removido1 = minhaPilha.desempilhar();
            System.out.println("Desempilhou (Pop): " + removido1); // 30
            minhaPilha.mostrar(); // [ 20 10 ]
            
            int removido2 = minhaPilha.desempilhar();
            System.out.println("Desempilhou (Pop): " + removido2); // 20
            minhaPilha.mostrar(); // [ 10 ]
            System.out.println("Topo atual (peek): " + minhaPilha.consultarTopo()); // 10

            // 3. Testando o último elemento
            int removido3 = minhaPilha.desempilhar();
            System.out.println("Desempilhou (Pop): " + removido3); // 10
            minhaPilha.mostrar(); // [ ]
            System.out.println("Pilha vazia? " + minhaPilha.isVazia()); // true

            // 4. Testando erro (Desempilhar Pilha Vazia)
            System.out.println("\n--- Testando Erro ---");
            try {
                minhaPilha.desempilhar();
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
 * Implementação da Pilha Flexível (Encadeada) usando nós (células).
 * Apenas um ponteiro 'topo' é necessário, pois as operações são LIFO.
 */
class PilhaFlexivel {

    // --- Célula (Nó) ---
    class Celula {
        public int elemento;
        public Celula prox; // Ponteiro para o elemento abaixo na pilha

        public Celula(int elemento) {
            this.elemento = elemento;
            this.prox = null;
        }
    }

    // --- Atributo Principal ---
    private Celula topo; // Ponteiro que sempre aponta para a célula no topo da pilha

    // --- Construtor ---
    public PilhaFlexivel() {
        this.topo = null; // A pilha começa vazia
    }

    // --- Métodos de Controle ---

    public boolean isVazia() {
        return (topo == null);
    }

    // --- Operações da Pilha ---

    /**
     * PUSH: Insere um elemento no topo da pilha.
     * É análogo à 'inserirInicio' de uma lista encadeada.
     */
    public void empilhar(int x) {
        Celula nova = new Celula(x);
        
        // A nova célula aponta para a antiga célula do topo (ou null, se vazia)
        nova.prox = topo; 
        
        // Atualiza o ponteiro 'topo' para a nova célula
        topo = nova; 
        
        nova = null;
    }

    /**
     * POP: Remove e retorna o elemento do topo da pilha.
     * É análogo à 'removerInicio' de uma lista encadeada.
     */
    public int desempilhar() throws Exception {
        if (isVazia()) {
            throw new Exception("Erro ao desempilhar: Pilha vazia!");
        }

        Celula tmp = topo; // Guarda a célula do topo
        int elemento = topo.elemento;

        // Move o topo para o próximo elemento (o que estava abaixo)
        topo = topo.prox; 
        
        // Isola a célula removida (boa prática)
        tmp.prox = null;
        tmp = null;

        return elemento;
    }

    /**
     * PEEK: Retorna o elemento do topo da pilha sem removê-lo.
     */
    public int consultarTopo() throws Exception {
        if (isVazia()) {
            throw new Exception("Erro ao consultar: Pilha vazia!");
        }
        return topo.elemento;
    }

    // --- Método Auxiliar ---

    /**
     * Mostra os elementos da pilha, do topo para a base.
     */
    public void mostrar() {
        System.out.print("[ ");
        for (Celula i = topo; i != null; i = i.prox) {
            System.out.print(i.elemento + " ");
        }
        System.out.println("] (Topo <-> Base)");
    }
}