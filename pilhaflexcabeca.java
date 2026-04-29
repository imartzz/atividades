public class pilhaflexcabeca {
    public static class celula {
    int elemento;
    celula prox;

    celula(int elemento) {
        this.elemento = elemento;
        this.prox = null;
    }
    }
    public static celula topo;
    public pilhaflexcabeca(){
         topo = new celula(0);
        topo.prox = null;
    }
    public void empilhar(int x){
        celula tmp = new celula(x);
        tmp.prox = topo.prox;
        topo.prox = tmp;
        tmp = null;
    }

    public int desempilhar(){
    if(topo.prox == null) return -1;
    int elemento = topo.prox.elemento;
    celula tmp = topo.prox;
    topo.prox = topo.prox.prox;
    tmp.prox = null;
    tmp = null;
    return elemento;
    }
    
    public int soma(){
        int soma = 0;
        for(celula i = topo;i != null;i=i.prox){
            soma+= i.elemento;
        }
        return soma;
    }

    public int somarecursiva(int soma,celula i){
    if(i == null) return soma;
    soma+= i.elemento;
    return somarecursiva(soma, i.prox);
    }

    public int maiorelemento(){
        if(topo == null) return -1;
        int maior = topo.elemento;
        for(celula i = topo.prox;i != null;i = i.prox){
            if(i.elemento > maior){
                maior = i.elemento;
            }
        }
        return maior;   

    }
    public int maiorelementorecursivo(celula i,int maior){
        if(i == null) return maior;
        if(i.elemento > maior) maior = i.elemento;
        return maiorelementorecursivo(i.prox, maior);
    }

    public void mostrar() {
    System.out.print("[ ");
    for (celula i = topo.prox; i != null; i = i.prox) {
        System.out.print(i.elemento + " ");
    }
    System.out.println("]");
}

public int mostrarrecursivo(celula i){
    if(i == null){
        System.out.println();
        return -1;
    }
    System.out.print(i.elemento + " ");
    return mostrarrecursivo(i.prox);
}
public void mostrarinversa(celula i){
    if(i == null){
        return;
    }
    mostrarinversa(i.prox);
    System.out.print(i.elemento + " ");
}
public static void main(String[] args) throws Exception {
        
       
        pilhaflexcabeca p = new pilhaflexcabeca();

      
        p.empilhar(3);
        p.empilhar(5);
        p.empilhar(7);
       
        p.mostrar();

        
        int removido = p.desempilhar();
        System.out.println("Removido: " + removido);
         removido = p.desempilhar();
        System.out.println("Removido: " + removido);

        
        p.mostrar();

        
    }
}
