public class pilhaflex {
    public static class celula {
    int elemento;
    celula prox;

    celula(int elemento) {
        this.elemento = elemento;
        this.prox = null;
    }
    }
    public static celula topo;
    public pilhaflex(){
        topo = null;
    }
    public void empilhar(int x){
        celula tmp = new celula(x);
        tmp.prox = topo;
        topo = tmp;
        tmp = null;
    }

    public int desempilhar(){
    if(topo == null) return -1;
    int elemento = topo.elemento;
    celula tmp = topo;
    topo = topo.prox;
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
    for (celula i = topo; i != null; i = i.prox) {
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
public void mostrarinversaointerativa(){
pilhaflex tmp = new pilhaflex();
for(celula i = topo;i != null;i = i.prox){
    tmp.empilhar(i.elemento);
}
tmp.mostrar();
}
public static void main(String[] args) throws Exception {
        
       
        pilhaflex p = new pilhaflex();

      
        p.empilhar(3);
        p.empilhar(5);
        p.empilhar(7);
       
        p.mostrar();
        p.mostrarrecursivo(topo);
        p.mostrarinversa(topo);
        System.out.println(p.soma());
        System.out.println(p.somarecursiva(0, topo));
        System.out.println(p.maiorelemento());
        System.out.println(p.maiorelementorecursivo(topo.prox,topo.elemento));

        
        int removido = p.desempilhar();
        System.out.println("Removido: " + removido);
         removido = p.desempilhar();
        System.out.println("Removido: " + removido);

        
        p.mostrar();

        
    }
}
