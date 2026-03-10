import java.util.Random;

public class Arvore {

    private static class Node {
        int elemento;
        Node esquerda;
        Node direita;

        public Node(int elemento) {
            this.elemento = elemento;
            this.esquerda = null;
            this.direita = null;
        }
    }

    Node raiz;
    // private int totalElementos = 0; 

    public Arvore() {
        this.raiz = null;
    }

    public void insert(int elemento) {
        raiz = inserirRecursivo(raiz, elemento);
        //totalElementos++; 
    }

    private Node inserirRecursivo(Node atual, int elemento) {
        if (atual == null) {
            return new Node(elemento);
        }

        if (elemento < atual.elemento) {
            atual.esquerda = inserirRecursivo(atual.esquerda, elemento);
        } else if (elemento > atual.elemento) {
            atual.direita = inserirRecursivo(atual.direita, elemento);
        }
        
        return atual;
    }

    public int getAltura() {
        return getAltura(raiz, 0);
    }

    private int getAltura(Node node, int altura) {
        if (node == null) {
            return altura - 1;
        } else {
            int alturaEsq = getAltura(node.esquerda, altura + 1);
            int alturaDir = getAltura(node.direita, altura + 1);
            return Math.max(alturaEsq, alturaDir);
        }
    }
    
    public void caminharCentral() {
        caminharCentral(raiz);
        System.out.println();
    }

    public void caminharPos() {
        caminharPos(raiz);
        System.out.println();
    }

    public void caminharPre() {
        caminharPre(raiz);
        System.out.println();
    }
    public void getSoma() {
        int somaTotal = calcularSomaRecursivo(raiz);
        System.out.println("Soma Total dos Elementos: " + somaTotal);
    }


    public void getPares(){
        int totalPares = numPares(raiz);
        System.out.println("Quantos Pares: " + totalPares);
    }

    public void getDivisivelOnze(){
        boolean temDivisivel = numDivisiveis(raiz);
        System.out.println("Tem Divisiveis 11: " + temDivisivel);
    }

    private boolean numDivisiveis(Node node){
        boolean divisiveis = false;
         if(node != null){
        divisiveis = (node.elemento % 11 == 0) || numDivisiveis(node.esquerda) || numDivisiveis(node.direita);
    }
    return divisiveis;
    }
    

    private int numPares(Node node){
    int pares = 0;
    if(node != null){
        pares = ((node.elemento % 2 == 0) ? 1 : 0) + numPares(node.esquerda) + numPares(node.direita);
    }
    
    
    return pares;
    }
    private int calcularSomaRecursivo(Node node) {
        if (node == null) {
            return 0; // Caso base: subárvore nula contribui com 0 para a soma
        }
        
        // Retorna: Valor do nó atual + soma da subárvore esquerda + soma da subárvore direita
        return node.elemento 
             + calcularSomaRecursivo(node.esquerda) 
             + calcularSomaRecursivo(node.direita);
    }
    private void caminharPre(Node node) {
        if (node != null) {
            System.out.print(node.elemento + " ");
            caminharPre(node.esquerda);
            caminharPre(node.direita);
        }
    }

    private void caminharCentral(Node node) {
        if (node != null) {
            caminharCentral(node.esquerda);
            System.out.print(node.elemento + " ");
            caminharCentral(node.direita);
        }
    }

    private void caminharPos(Node node) {
        if (node != null) {
            caminharPos(node.esquerda);
            caminharPos(node.direita);
            System.out.print(node.elemento + " ");
        }
    }

    public static void main(String[] args) {
        Arvore arvore = new Arvore();
        Random random = new Random();
        random.setSeed(0);
        arvore.insert(10);
        arvore.insert(20);
        arvore.insert(30);
        arvore.insert(40);
        arvore.insert(50);
        arvore.insert(60);
        arvore.insert(110);
        // int totalInsercoes = 100000;

        // for (int i = 1; i <= totalInsercoes; i++) {
            // int elemento = random.nextInt(1000000);
            // arvore.insert(elemento);

            //int num = arvore.totalElementos;
            //double log2 = Math.log(num) / Math.log(2);
            //int altura = arvore.getAltura();

            //System.out.printf("Inserção %d | Elementos: %d | log2(n): %.2f | Altura: %d%n",
                    //i, num, log2, altura);
        //}

        //System.out.println("\nAltura final da árvore: " + arvore.getAltura());

    arvore.getSoma();
    arvore.getPares();
    arvore.getDivisivelOnze();
    }
}
