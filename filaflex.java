
public class filaflex {
    Celula primeiro, ultimo;

    public filaflex(){
        primeiro = new Celula();
        ultimo = primeiro;
    }
    
    public class Celula{
        int elemento;
        Celula prox;
        public Celula(){
            prox = null;
        }
        public Celula(int x){
            this.elemento = x;
            prox = null;
            
        }
    }
    public void inserir(int x){
        ultimo.prox = new Celula(x);
        ultimo = ultimo.prox;
    }

    public int remover(){
        if (primeiro == ultimo) return -1;
    Celula tmp = primeiro;
    primeiro = primeiro.prox;           
    int elemento = primeiro.elemento;   
    tmp.prox = null; tmp = null;        
    return elemento;
    }

    public int maior(){
        if(primeiro == ultimo) return -1; 
        int maior = primeiro.prox.elemento;
        for(Celula i = primeiro.prox.prox;i != null;i = i.prox){
        if(maior < i.elemento) maior = i.elemento;
        }
        return maior;
    }

    public int terceiro(){
        return primeiro.prox.prox.prox.elemento;
    }

    public int somar(){
        if(primeiro == ultimo) return-1;
        int soma = 0;
        for(Celula i = primeiro.prox;i != null;i = i.prox){
        soma += i.elemento;
        }
        return soma;
    }

    public void inverter(){
        
    }

    public void mostrar(){
        for(Celula i = primeiro.prox;i != null;i = i.prox){
            System.out.print(i.elemento + " ");
        }
        System.out.println();
    }
    public static void main(String[] args) {
        filaflex fila = new filaflex();
        fila.inserir(30);
        fila.inserir(20);
        fila.inserir(40);
        fila.inserir(38);
        fila.mostrar();
        System.out.println(fila.maior());
        System.out.println(fila.terceiro());
        System.out.println(fila.somar());
    }
}
