public class buble {

    public static void bubbleSort(int[] arr) {
        int n = arr.length;
        int temp = 0;

        // Loop externo para as passagens
        for (int i = 0; i < n - 1; i++) {
            // Loop interno para comparar e trocar
            for (int j = 0; j < n - i - 1; j++) {
                // Se o elemento atual for maior que o próximo, troca
                if (arr[j] > arr[j + 1]) {
                    temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
    }

    public static void main(String[] args) {
        int[] lista = {64, 34, 25, 12, 22, 11, 90};

        System.out.print("Lista original: ");
        for (int i : lista) {
            System.out.print(i + " ");
        }
        System.out.println();

        bubbleSort(lista);

        System.out.print("Lista ordenada: ");
        for (int i : lista) {
            System.out.print(i + " ");
        }
        System.out.println();
    }
}

public class MinhaPilha {
    private int[] elementos;
    private int topo; // Índice do topo

    // Construtor
    public MinhaPilha(int capacidade) {
        elementos = new int[capacidade];
        topo = -1; // Pilha vazia
    }

    // Empilha (adiciona no topo)
    public void empilhar(int valor) {
        if (topo < elementos.length - 1) {
            topo++;
            elementos[topo] = valor;
        } else {
            System.out.println("Erro: Pilha cheia!");
        }
    }

    // Desempilha (remove do topo)
    public int desempilhar() {
        if (topo >= 0) {
            int valor = elementos[topo];
            topo--;
            return valor;
        } else {
            System.out.println("Erro: Pilha vazia!");
            return -1; // Valor de erro
        }
    }
    
    // Mostra os elementos
    public void mostrar() {
        if (topo == -1) {
            System.out.println("Pilha vazia.");
            return;
        }
        System.out.print("[ ");
        for (int i = 0; i <= topo; i++) {
            System.out.print(elementos[i] + " ");
        }
        System.out.println("]");
    }
}





public class MinhaFila {
    private int[] elementos;
    private int frente;
    private int fim;
    private int tamanho;

    // Construtor
    public MinhaFila(int capacidade) {
        elementos = new int[capacidade];
        frente = 0;
        fim = -1;
        tamanho = 0;
    }

    // Enfileira (adiciona no fim)
    public void enfileirar(int valor) {
        if (tamanho < elementos.length) {
            fim = (fim + 1) % elementos.length;
            elementos[fim] = valor;
            tamanho++;
        } else {
            System.out.println("Erro: Fila cheia!");
        }
    }

    // Desenfileira (remove da frente)
    public int desenfileirar() {
        if (tamanho > 0) {
            int valor = elementos[frente];
            frente = (frente + 1) % elementos.length;
            tamanho--;
            return valor;
        } else {
            System.out.println("Erro: Fila vazia!");
            return -1; // Valor de erro
        }
    }
    
    // Mostra os elementos
    public void mostrar() {
        if (tamanho == 0) {
            System.out.println("Fila vazia.");
            return;
        }
        System.out.print("[ ");
        for (int i = 0; i < tamanho; i++) {
            System.out.print(elementos[(frente + i) % elementos.length] + " ");
        }
        System.out.println("]");
    }
}









public class MinhaLista {
    private int[] elementos;
    private int n; // Contador de elementos

    // Construtor
    public MinhaLista(int capacidade) {
        elementos = new int[capacidade];
        n = 0;
    }

    // Adiciona no final
    public void adicionar(int valor) {
        if (n < elementos.length) {
            elementos[n] = valor;
            n++;
        } else {
            System.out.println("Erro: Lista cheia!");
        }
    }

    // Adiciona no início (exige mover todos)
    public void adicionarInicio(int valor) {
        if (n < elementos.length) {
            for (int i = n; i > 0; i--) {
                elementos[i] = elementos[i - 1];
            }
            elementos[0] = valor;
            n++;
        } else {
            System.out.println("Erro: Lista cheia!");
        }
    }
    
    // Remove do final
    public int removerFim() {
        if (n > 0) {
            n--;
            return elementos[n];
        } else {
            System.out.println("Erro: Lista vazia!");
            return -1; // Valor de erro
        }
    }
    
    // Mostra os elementos
    public void mostrar() {
        System.out.print("[ ");
        for (int i = 0; i < n; i++) {
            System.out.print(elementos[i] + " ");
        }
        System.out.println("]");
    }
}