public class Fila {
    static int[] dados = new int[1000];
    static int inicio = -1;
    static int fim = -1;
    static int tamanho = 0;

    static boolean vazia(){
        return inicio == -1;
    }
    static boolean cheia(){
        return fim == 998;
    }

    static void enfileirar(int x){
        if(cheia()) return;
        if(vazia()) inicio = 0;
        dados[++fim] = x;
        tamanho++;
    }
    static int desenfileirar(){
        if(vazia()) return -1;
        int valor = dados[inicio];
        if(inicio == fim){
            inicio = fim = -1;
        } else {
            tamanho--;
            inicio++;
        }
        return valor;
    }
    static void imprimir(){
        for(int i = inicio;i <= fim;i++){
            System.out.print(dados[i] + " ");
        }
    }
    public static void main(String[] args){
        enfileirar(10);
        enfileirar(20);
        enfileirar(30);
        desenfileirar();
        imprimir();
    }
}
