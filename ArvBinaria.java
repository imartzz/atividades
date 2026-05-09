public class ArvBinaria {
     No raiz;

    public ArvBinaria() {
        raiz = null;
    }

    void inserir(int x){
        raiz = inserir(x, raiz);
    }

    No inserir(int x, No i){
        if(i == null){
        i = new No(x);
        } else if(x < i.elemento){
        i.esq = inserir(x, i.esq);
        } else if(x > i.elemento){
            i.dir = inserir(x, i.dir);
        } else {
            // erro
        }
        return i;
    }


    boolean pesquisar(int x){
        return pesquisar(x, raiz);
    }

    boolean pesquisar(int x, No i){
        if(i == null) return false;
        if(x == i.elemento) return true;
        if(x < i.elemento) return pesquisar(x, i.esq);
        return pesquisar(x, i.dir);
    }
    
    void CaminharCentral(No i){
        if(i != null){
            CaminharCentral(i.esq);
            System.out.print(i.elemento);
            CaminharCentral(i.dir);
        }
    }

    void CaminharPre(No i){
        if(i != null){
            System.out.print(i.elemento);
            CaminharPre(i.esq);
            CaminharPre(i.dir);
        }
    }

    void CaminharPos(No i){
        if(i != null){
            CaminharPos(i.esq);
            CaminharPos(i.dir);
            System.out.print(i.elemento);
        }
    }
    public int getQuantidade(){
        int soma = quantidade(raiz);
        return soma;
    }
    int quantidade(No i){
        if(i == null){
            return 0;
        }
        return 1 + quantidade(i.esq) + quantidade(i.dir);
    }

    public int somarElementos(){
    int soma = somarElementos(raiz);
    return soma;
    }
    int somarElementos(No i){
        if(i == null) return 0;
        return i.elemento + somarElementos(i.esq) + somarElementos(i.dir);
    }

    
    public int getAltura() {
        return getAltura(raiz);
    }

    int getAltura(No i) {
    if (i == null) {
        return -1; 
    } else {
        int alturaEsq = getAltura(i.esq);
        int alturaDir = getAltura(i.dir);

      
        return 1 + Math.max(alturaEsq, alturaDir);
    }
}

public int getPares(){
    return getPares(raiz);
}

int getPares(No i){
    if(i == null) return 0;
    if(i.elemento % 2 == 0) {
        return 1 + getPares(i.esq) + getPares(i.dir);
    } else {
        return 0 + getPares(i.esq) + getPares(i.dir);
    }
}
public int getMaior(){
    return getMaior(raiz);
}
int getMaior(No i){
    if(i == null) return -1;
    while(i.dir != null){
        i = i.dir;
    }
    return i.elemento;
}

public int getPai(int x) {
    if (raiz == null || raiz.elemento == x) return -1; // Raiz não tem pai
    return getPai(x, raiz);
}
int getPai(int x, No i){
    if(x < i.elemento && i.esq != null){
        if(x == i.esq.elemento) {
            return i.elemento;
        } else {
            return getPai(x, i.esq);
        }
    }
    if(x > i.elemento && i.dir != null){
        if(x == i.dir.elemento){
            return i.elemento;
        } else {
            return getPai(x,i.dir);
        }
    }
    return -1;
}

public int getNivel(int x) {
    return getNivel(x, raiz, 0);
}

int getNivel(int x,No i,int nivel){
    if(i == null) return -1;
    if(x == i.elemento) return nivel;
    if( x <i.elemento) {
        return getNivel(x,i.esq,nivel + 1);
    } else {
        return getNivel(x,i.dir,nivel + 1);
    }
}
   


     

    class No{
      int elemento;
      No dir;
      No esq;

      No(int x){
        this.elemento = x;
        dir = null;
        esq = null;
      }
     }


     public static void main(String[] args) {
         ArvBinaria ab = new ArvBinaria();
         ab.inserir(3);
         ab.inserir(5);
         ab.inserir(1);
         ab.inserir(8);
         ab.inserir(2);
         ab.inserir(4);
         ab.inserir(9);
         System.out.print("Central: ");
        ab.CaminharCentral(ab.raiz); // 1 2 3 4 5 8

        System.out.print("\nPré:     ");
        ab.CaminharPre(ab.raiz);     // 3 1 2 5 4 8

        System.out.print("\nPós:     ");
        ab.CaminharPos(ab.raiz);     // 2 1 4 8 5 3

        System.out.println("\nTem 4? " + ab.pesquisar(4)); // true
        System.out.println("Tem 9? " + ab.pesquisar(9)); // false

        System.out.println(ab.getQuantidade());
        System.out.println(ab.somarElementos());
        System.out.println(ab.getAltura());
        System.out.println(ab.getPares());
        System.out.println(ab.getMaior());
        System.out.println(ab.getPai(8));
        System.out.println(ab.getNivel(9));
        



     }
    
}
