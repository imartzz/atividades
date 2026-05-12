class Lista {
    CelulaLista inicio;
    CelulaLista fim;

    Lista() {
        inicio = null;
        fim = null;
    }


    void adicionarPilha(int[] elementos) {
    CelulaLista nova = new CelulaLista();

    
    for (int x : elementos) {
        CelulaPilha novaCelula = new CelulaPilha(x);
        novaCelula.prox = nova.topo;
        nova.topo = novaCelula;
    }

    
    if (inicio == null) {
        inicio = nova;
        fim = nova;
    } else {
        fim.prox = nova;
        fim = nova;
    }
}

    int tamanhoPilha(CelulaPilha i){
        int tam = 0;
        while(i != null){
            tam++;
            i = i.prox;        
        }
        return tam;

    }

    CelulaLista maiorPilha(){
        CelulaLista tmp = inicio.prox;
        CelulaLista maior = inicio;
        int tamMaior = tamanhoPilha(inicio.topo);


        while (tmp != null){
        int tamAtual = tamanhoPilha(tmp.topo);
        if(tamAtual > tamMaior){
            tamMaior = tamAtual;
            maior = tmp;
        }
            tmp = tmp.prox;
        }
        return maior;
    }

    public static void main(String[] args) {
    Lista lista = new Lista();

    lista.adicionarPilha(new int[]{1, 2});          
    lista.adicionarPilha(new int[]{3, 4, 5, 6});   
    lista.adicionarPilha(new int[]{7, 8, 9});       

    CelulaLista maior = lista.maiorPilha();
    System.out.println("Tamanho da maior pilha: " + lista.tamanhoPilha(maior.topo));
    
}
}

class CelulaLista {
    CelulaPilha topo;
    CelulaLista prox;

    CelulaLista() {
        topo = null;
        prox = null;
    }
}

class CelulaPilha {
    int elemento;
    CelulaPilha prox;

    CelulaPilha(int elemento) {
        this.elemento = elemento;
        prox = null;
    }
}