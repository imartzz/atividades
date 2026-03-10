import java.util.EmptyStackException;

/**
 * TAD Pilha Flexível (Dinâmica) implementado com Lista Encadeada.
 * A Pilha segue o princípio LIFO (Last-In, First-Out).
 * <T> torna a pilha genérica, aceitando qualquer tipo de objeto.
 */
public class PilhaFlexivel<T> {

    // --- 1. CLASSE INTERNA: No (Célula) ---
    /**
     * Classe interna que representa um Nó (Célula) da Lista Encadeada.
     */
    private static class No<T> {
        public T dado;       // O elemento a ser armazenado
        public No<T> proximo; // Ponteiro para o próximo nó (o que está abaixo)

        /**
         * Construtor do Nó.
         */
        public No(T dado) {
            this.dado = dado;
            this.proximo = null;
        }

        /**
         * Exibe o dado armazenado.
         */
        public void exibir() {
            System.out.print(this.dado);
        }
    }
    // ------------------------------------

    private No<T> topo; // Ponteiro para o nó do topo da pilha

    /**
     * Construtor da Pilha: Inicializa a pilha vazia.
     */
    public PilhaFlexivel() {
        this.topo = null;
    }

    /**
     * Verifica se a pilha está vazia.
     * @return true se a pilha estiver vazia, false caso contrário.
     */
    public boolean isEmpty() {
        return (this.topo == null);
    }

    /**
     * Insere (empilha) um novo elemento no topo da pilha. (PUSH)
     * Complexidade: O(1).
     * @param dado O elemento a ser inserido.
     */
    public void push(T dado) {
        No<T> novoNo = new No<>(dado);
        
        // O novo nó aponta para o nó que era o antigo topo
        novoNo.proximo = this.topo; 
        
        // O novo nó se torna o novo topo
        this.topo = novoNo;
        
        System.out.println("PUSH: Elemento '" + dado + "' empilhado.");
    }

    /**
     * Remove (desempilha) e retorna o elemento do topo da pilha. (POP)
     * Complexidade: O(1).
     * @return O elemento que estava no topo da pilha.
     * @throws EmptyStackException se a pilha estiver vazia.
     */
    public T pop() {
        if (isEmpty()) {
            throw new EmptyStackException();
        }

        // 1. Guarda o dado do topo atual
        T dadoRemovido = this.topo.dado;

        // 2. O novo topo é o próximo nó (o que estava abaixo)
        this.topo = this.topo.proximo; 

        System.out.println("POP: Elemento '" + dadoRemovido + "' desempilhado.");
        return dadoRemovido;
    }

    /**
     * Retorna o elemento do topo da pilha sem removê-lo. (PEEK)
     * Complexidade: O(1).
     * @return O elemento que está no topo da pilha.
     * @throws EmptyStackException se a pilha estiver vazia.
     */
    public T peek() {
        if (isEmpty()) {
            throw new EmptyStackException();
        }
        return this.topo.dado;
    }

    /**
     * Exibe todos os elementos da pilha (do topo para a base).
     */
    public void exibirPilha() {
        if (isEmpty()) {
            System.out.println("A pilha está vazia.");
            return;
        }

        System.out.print("\n--- Conteúdo da Pilha (Topo -> Base) ---\nTOPO -> ");
        No<T> atual = this.topo;
        while (atual != null) {
            atual.exibir();
            if (atual.proximo != null) {
                System.out.print(" -> ");
            }
            atual = atual.proximo;
        }
        System.out.println(" <- BASE\n");
    }

    // --- Método Main para Teste ---
    public static void main(String[] args) {
        // Criando uma pilha que armazena Strings
        PilhaFlexivel<String> pilhaStrings = new PilhaFlexivel<>();

        System.out.println("Pilha de Strings vazia? " + pilhaStrings.isEmpty()); // true

        // 1. Inserir elementos (PUSH)
        pilhaStrings.push("Primeiro");
        pilhaStrings.push("Meio");
        pilhaStrings.push("Topo Atual");

        pilhaStrings.exibirPilha();

        // 2. Verificar o topo (PEEK)
        try {
            System.out.println("Elemento no Topo (Peek): " + pilhaStrings.peek());
        } catch (EmptyStackException e) {
            System.out.println("Erro: Pilha vazia para Peek.");
        }

        // 3. Remover elementos (POP)
        pilhaStrings.pop(); // Remove "Topo Atual"
        
        pilhaStrings.exibirPilha();
        
        // Exemplo de outra pilha (para Inteiros)
        PilhaFlexivel<Integer> pilhaInteiros = new PilhaFlexivel<>();
        pilhaInteiros.push(42);
        pilhaInteiros.push(100);
        System.out.println("Topo da Pilha de Inteiros: " + pilhaInteiros.peek());

        // 4. Esvaziar e testar exceção
        pilhaStrings.pop(); 
        pilhaStrings.pop(); 

        System.out.println("Pilha de Strings vazia após esvaziar? " + pilhaStrings.isEmpty()); // true

        try {
            pilhaStrings.pop();
        } catch (EmptyStackException e) {
            System.out.println("\nSUCCESS: Exceção tratada ao tentar POP em pilha vazia.");
        }
    }
}