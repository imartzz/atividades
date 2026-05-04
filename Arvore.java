class Arvore {

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

    No raiz;

    Arvore() {
        raiz = null;
    }

    // INSERÇÃO — com retorno de referência
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

    // INSERÇÃO — com passagem de pai
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

    // PESQUISA
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

    // CAMINHAMENTO CENTRAL (Em Ordem): esq → raiz → dir
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

    // CAMINHAMENTO PRÉ-ORDEM: raiz → esq → dir
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

    // CAMINHAMENTO PÓS-ORDEM: esq → dir → raiz
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
}

class Main {
    public static void main(String[] args) throws Exception {
        Arvore ab = new Arvore();
    }
}