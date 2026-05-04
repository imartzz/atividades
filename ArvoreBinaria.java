// ============================================================
//  Classes auxiliares para o Exercício Resolvido (8)
// ============================================================

class Celula {
    int elemento;
    Celula prox;
    Celula(int x) { elemento = x; prox = null; }
}

class CelulaDupla {
    int elemento;
    CelulaDupla prox;
    CelulaDupla ant;
    CelulaDupla(int x) { elemento = x; prox = null; ant = null; }
}

// ============================================================
//  Nó da Árvore Binária
// ============================================================

class No {
    int elemento;
    No esq;
    No dir;

    No(int x) {
        elemento = x;
        esq = null;
        dir = null;
    }
}

// ============================================================
//  Árvore Binária de Busca
// ============================================================

class ArvoreBinaria {

    No raiz;

    ArvoreBinaria() {
        raiz = null;
    }

    // ========================================================
    // INSERÇÃO — com retorno de referência
    // ========================================================

    void inserir(int x) throws Exception {
        raiz = inserir(x, raiz);
    }

    No inserir(int x, No i) throws Exception {
        if (i == null) {
            i = new No(x);
        } else if (x < i.elemento) {
            i.esq = inserir(x, i.esq);
        } else if (x > i.elemento) {
            i.dir = inserir(x, i.dir);
        } else {
            throw new Exception("Erro: elemento repetido!");
        }
        return i;
    }

    // ========================================================
    // INSERÇÃO — com passagem de pai
    // ========================================================

    void inserirPai(int x) throws Exception {
        if (raiz == null) {
            raiz = new No(x);
        } else {
            inserirPai(x, raiz, null);
        }
    }

    void inserirPai(int x, No i, No pai) throws Exception {
        if (i == null) {
            if (x < pai.elemento) {
                pai.esq = new No(x);
            } else {
                pai.dir = new No(x);
            }
        } else if (x < i.elemento) {
            inserirPai(x, i.esq, i);
        } else if (x > i.elemento) {
            inserirPai(x, i.dir, i);
        } else {
            throw new Exception("Erro: elemento repetido!");
        }
    }

    // ========================================================
    // PESQUISA
    // ========================================================

    boolean pesquisar(int x) {
        return pesquisar(x, raiz);
    }

    boolean pesquisar(int x, No i) {
        if (i == null) {
            return false;
        } else if (x == i.elemento) {
            return true;
        } else if (x < i.elemento) {
            return pesquisar(x, i.esq);
        } else {
            return pesquisar(x, i.dir);
        }
    }

    // ========================================================
    // CAMINHAMENTO CENTRAL (Em Ordem): esq → raiz → dir
    // ========================================================

    void caminharCentral() {
        caminharCentral(raiz);
        System.out.println();
    }

    void caminharCentral(No i) {
        if (i != null) {
            caminharCentral(i.esq);
            System.out.print(i.elemento + " ");
            caminharCentral(i.dir);
        }
    }

    // ========================================================
    // CAMINHAMENTO PRÉ-ORDEM: raiz → esq → dir
    // ========================================================

    void caminharPre() {
        caminharPre(raiz);
        System.out.println();
    }

    void caminharPre(No i) {
        if (i != null) {
            System.out.print(i.elemento + " ");
            caminharPre(i.esq);
            caminharPre(i.dir);
        }
    }

    // ========================================================
    // CAMINHAMENTO PÓS-ORDEM: esq → dir → raiz
    // ========================================================

    void caminharPos() {
        caminharPos(raiz);
        System.out.println();
    }

    void caminharPos(No i) {
        if (i != null) {
            caminharPos(i.esq);
            caminharPos(i.dir);
            System.out.print(i.elemento + " ");
        }
    }

    // ========================================================
    // EXERCÍCIO RESOLVIDO (1) — Retorna a altura da árvore
    // ========================================================

    int getAltura() {
        return getAltura(raiz, 0);
    }

    int getAltura(No i, int altura) {
        if (i == null) {
            altura--;
        } else {
            int alturaEsq = getAltura(i.esq, altura + 1);
            int alturaDir = getAltura(i.dir, altura + 1);
            altura = (alturaEsq > alturaDir) ? alturaEsq : alturaDir;
        }
        return altura;
    }

    // ========================================================
    // EXERCÍCIO RESOLVIDO (2) — Análise de complexidade:
    // Insere 100000 elementos aleatórios e mostra, a cada
    // 1000 inserções, o número de nós, log2(n) e a altura.
    // (Chamado no main)
    // ========================================================

    // ========================================================
    // EXERCÍCIO RESOLVIDO (3) — Retorna a soma dos elementos
    // ========================================================

    int soma() {
        return soma(raiz);
    }

    int soma(No i) {
        int resp = 0;
        if (i != null) {
            resp = i.elemento + soma(i.esq) + soma(i.dir);
        }
        return resp;
    }

    // ========================================================
    // EXERCÍCIO RESOLVIDO (4) — Retorna o número de elementos pares
    // ========================================================

    int numPares() {
        return numPares(raiz);
    }

    int numPares(No i) {
        int resp = 0;
        if (i != null) {
            resp = ((i.elemento % 2 == 0) ? 1 : 0) + numPares(i.esq) + numPares(i.dir);
        }
        return resp;
    }

    // ========================================================
    // EXERCÍCIO RESOLVIDO (5) — Verifica se duas árvores são iguais
    // ========================================================

    static boolean igual(ArvoreBinaria a1, ArvoreBinaria a2) {
        return igual(a1.raiz, a2.raiz);
    }

    private static boolean igual(No i1, No i2) {
        boolean resp;
        if (i1 != null && i2 != null) {
            resp = (i1.elemento == i2.elemento)
                    && igual(i1.esq, i2.esq)
                    && igual(i1.dir, i2.dir);
        } else if (i1 == null && i2 == null) {
            resp = true;
        } else {
            resp = false;
        }
        return resp;
    }

    // ========================================================
    // EXERCÍCIO RESOLVIDO (6) — Retorna true se há elemento divisível por 11
    // ========================================================

    boolean hasDiv11() {
        return hasDiv11(raiz);
    }

    boolean hasDiv11(No i) {
        boolean resp = false;
        if (i != null) {
            resp = (i.elemento % 11 == 0) || hasDiv11(i.esq) || hasDiv11(i.dir);
        }
        return resp;
    }

    // ========================================================
    // EXERCÍCIO RESOLVIDO (7) — TreeSort
    // Insere os elementos do array na árvore e usa o
    // caminhamento central para devolvê-los em ordem crescente.
    // Complexidade: Θ(n lg n) para árvore balanceada.
    // ========================================================

    static int[] treeSort(int[] entrada) throws Exception {
        ArvoreBinaria ab = new ArvoreBinaria();
        for (int x : entrada) {
            try { ab.inserir(x); } catch (Exception e) { /* ignora repetidos */ }
        }
        int[] resultado = new int[entrada.length];
        ab.treeSortHelper(ab.raiz, resultado, new int[]{0});
        return resultado;
    }

    private void treeSortHelper(No i, int[] array, int[] idx) {
        if (i != null) {
            treeSortHelper(i.esq, array, idx);
            array[idx[0]++] = i.elemento;
            treeSortHelper(i.dir, array, idx);
        }
    }

    // ========================================================
    // EXERCÍCIO RESOLVIDO (8) — toAB(Celula p1, CelulaDupla p2)
    // Recebe o nó cabeça de uma lista simples e de uma dupla,
    // intercala os elementos das duas listas e insere na árvore,
    // retornando o nó raiz da árvore criada.
    // ========================================================

    static No toAB(Celula p1, CelulaDupla p2) throws Exception {
        No resp = null;
        p1 = p1.prox;
        p2 = p2.prox;

        while (p1 != null && p2 != null) {
            resp = inserirEx8(resp, p1.elemento);
            resp = inserirEx8(resp, p2.elemento);
            p1 = p1.prox;
            p2 = p2.prox;
        }
        while (p1 != null) {
            resp = inserirEx8(resp, p1.elemento);
            p1 = p1.prox;
        }
        while (p2 != null) {
            resp = inserirEx8(resp, p2.elemento);
            p2 = p2.prox;
        }
        return resp;
    }

    private static No inserirEx8(No i, int x) throws Exception {
        if (i == null) {
            i = new No(x);
        } else if (x < i.elemento) {
            i.esq = inserirEx8(i.esq, x);
        } else if (x > i.elemento) {
            i.dir = inserirEx8(i.dir, x);
        } else {
            throw new Exception("Erro ao inserir: " + x);
        }
        return i;
    }
}

// ============================================================
//  Main
// ============================================================

class Main {
    public static void main(String[] args) throws Exception {

        // --- Árvore base ---
        ArvoreBinaria ab = new ArvoreBinaria();
        ab.inserir(6); ab.inserir(3); ab.inserir(9);
        ab.inserir(1); ab.inserir(5); ab.inserir(8); ab.inserir(10);
        ab.inserir(2); ab.inserir(4); ab.inserir(7); ab.inserir(11);

        System.out.println("=== Ex (1) Altura ===");
        System.out.println("Altura: " + ab.getAltura()); // 3

        System.out.println("\n=== Ex (2) Complexidade (100000 elementos) ===");
        ArvoreBinaria ab2 = new ArvoreBinaria();
        java.util.Random gerador = new java.util.Random();
        gerador.setSeed(0);
        for (int i = 1; i <= 100000; i++) {
            int valor;
            do { valor = Math.abs(gerador.nextInt()); }
            while (ab2.pesquisar(valor));
            ab2.inserir(valor);
            if (i % 1000 == 0) {
                double log2 = (Math.log(i) / Math.log(2)) * 1.39;
                System.out.println("Nós = " + i + " | log2 ≈ " + (int) log2 + " | h = " + ab2.getAltura());
            }
        }

        System.out.println("\n=== Ex (3) Soma ===");
        System.out.println("Soma: " + ab.soma()); // 1+2+3+4+5+6+7+8+9+10+11 = 66

        System.out.println("\n=== Ex (4) Número de pares ===");
        System.out.println("Pares: " + ab.numPares()); // 2,4,6,8,10 = 5

        System.out.println("\n=== Ex (5) Árvores iguais ===");
        ArvoreBinaria ab3 = new ArvoreBinaria();
        ab3.inserir(6); ab3.inserir(3); ab3.inserir(9);
        System.out.println("ab == ab3? " + ArvoreBinaria.igual(ab, ab3)); // false
        ArvoreBinaria ab4 = new ArvoreBinaria();
        ab4.inserir(6); ab4.inserir(3); ab4.inserir(9);
        ab4.inserir(1); ab4.inserir(5); ab4.inserir(8); ab4.inserir(10);
        ab4.inserir(2); ab4.inserir(4); ab4.inserir(7); ab4.inserir(11);
        System.out.println("ab == ab4? " + ArvoreBinaria.igual(ab, ab4)); // true

        System.out.println("\n=== Ex (6) Divisível por 11 ===");
        System.out.println("Tem div por 11? " + ab.hasDiv11()); // true (11)

        System.out.println("\n=== Ex (7) TreeSort ===");
        int[] entrada = {5, 3, 8, 1, 4, 7, 2, 6};
        int[] ordenado = ArvoreBinaria.treeSort(entrada);
        System.out.print("Ordenado: ");
        for (int x : ordenado) System.out.print(x + " ");
        System.out.println();

        System.out.println("\n=== Ex (8) toAB (lista simples + lista dupla → árvore) ===");
        Celula c = new Celula(-1); // cabeça
        Celula aux = c;
        for (int x : new int[]{10, 20, 30}) { aux.prox = new Celula(x); aux = aux.prox; }

        CelulaDupla d = new CelulaDupla(-1); // cabeça
        CelulaDupla daux = d;
        for (int x : new int[]{15, 25}) { daux.prox = new CelulaDupla(x); daux = daux.prox; }

        No raizAB = ArvoreBinaria.toAB(c, d);
        ArvoreBinaria abEx8 = new ArvoreBinaria();
        abEx8.raiz = raizAB;
        System.out.print("Central (intercalado): ");
        abEx8.caminharCentral(); // 10 15 20 25 30
    }
}