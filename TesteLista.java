
/**
 * Classe principal para testar a implementação da Lista.
 * Contém o método main.
 */
public class TesteLista {

    public static void main(String[] args) {
        
        System.out.println("--- Testando a Lista ---");
        Lista minhaLista = new Lista();

        try {
            // 1. Inserindo elementos
            minhaLista.inserirInicio(10);
            minhaLista.mostrar(); // [ 10 ]
            minhaLista.inserirFim(50);
            minhaLista.mostrar(); // [ 10 50 ]
            minhaLista.inserirInicio(5);
            minhaLista.mostrar(); // [ 5 10 50 ]

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
            
            // Adicionando mais para testar remoção por pos
            minhaLista.inserirFim(30);
            minhaLista.inserirFim(40);
            minhaLista.mostrar(); // [ 10 20 30 40 ]

            // 4. Removendo por posição
            // Remover da posição 1 (o elemento 20)
            int removidoPos = minhaLista.remover(1);
            System.out.println("Removeu da pos 1: " + removidoPos); // Removeu da pos 1: 20
            minhaLista.mostrar(); // [ 10 30 40 ]

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
 * Implementação da Lista Encadeada com nó sentinela (cabeça).
 * Esta classe NÃO deve ser 'public' pois está no mesmo arquivo que 'TesteLista'.
 */
class Lista {

    // --- Célula (Inner Class) ---
    class Celula {
        public int elemento;
        public Celula prox;

        public Celula() {
            this(0);
        }

        public Celula(int elemento) {
            this.elemento = elemento;
            this.prox = null;
        }
    }

    // --- Atributos da Lista ---
    private Celula primeiro, ultimo;

    // --- Construtor ---
    public Lista() {
        primeiro = new Celula(); // Nó sentinela
        ultimo = primeiro;
    }

    // --- Métodos de Inserção ---
    public void inserirInicio(int x) {
        Celula tmp = new Celula(x);
        tmp.prox = primeiro.prox;
        primeiro.prox = tmp;
        if (primeiro == ultimo) {
            ultimo = tmp;
        }
        tmp = null;
    }

    public void inserirFim(int x) {
        ultimo.prox = new Celula(x);
        ultimo = ultimo.prox;
    }

    // --- Métodos de Remoção (CORRIGIDOS) ---

    public int removerInicio() throws Exception {
        if (primeiro == ultimo) {
            throw new Exception("Erro ao remover: Lista vazia!");
        }

        Celula tmp = primeiro.prox; // A célula a ser removida
        int resp = tmp.elemento;
        primeiro.prox = tmp.prox; // Faz o sentinela "pular" a célula removida

        if (tmp == ultimo) { // Se removemos o último elemento
            ultimo = primeiro; // O sentinela se torna o último
        }
        
        tmp.prox = null;
        tmp = null;
        return resp;
    }

    public int removerFim() throws Exception {
        if (primeiro == ultimo) {
            throw new Exception("Erro ao remover: Lista vazia!");
        }

        Celula i;
        // Encontra o penúltimo elemento
        for (i = primeiro; i.prox != ultimo; i = i.prox);

        int elemento = ultimo.elemento;
        ultimo = i; // O penúltimo agora é o último
        i.prox = null; // Desconecta o antigo último
        
        return elemento;
    }

    // --- Métodos por Posição (CORRIGIDOS) ---

    public int tamanho() {
        int tamanho = 0;
        // Começa do primeiro *elemento* (primeiro.prox), não do sentinela
        for (Celula i = primeiro.prox; i != null; i = i.prox) {
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
            Celula i = primeiro;
            for (int j = 0; j < pos; j++, i = i.prox);
            
            Celula tmp = new Celula(x);
            tmp.prox = i.prox;
            i.prox = tmp;
            tmp = i = null;
        }
    }

    public int remover(int pos) throws Exception {
        int elemento;
        int tamanho = tamanho();

        // Verificação de erro completa e estrutura do if
        if (primeiro == ultimo || pos < 0 || pos >= tamanho) {
            throw new Exception("Erro ao remover: Lista vazia ou posição inválida!");
        } else if (pos == 0) {
            elemento = removerInicio();
        } else if (pos == tamanho - 1) {
            elemento = removerFim();
        } else {
            // Anda até a célula *anterior* à posição de remoção
            Celula i = primeiro;
            for (int j = 0; j < pos; j++) {
                i = i.prox;
            }
            
            Celula tmp = i.prox; // Célula a ser removida
            elemento = tmp.elemento;
            i.prox = tmp.prox; // "Pula" a célula
            tmp.prox = null;
            i = tmp = null;
        }
        // Retorno movido para fora do bloco if/else
        return elemento;
    }

    // --- Método Auxiliar ---
    public void mostrar() {
        System.out.print("[ ");
        for (Celula i = primeiro.prox; i != null; i = i.prox) {
            System.out.print(i.elemento + " ");
        }
        System.out.println("]");
    }
}