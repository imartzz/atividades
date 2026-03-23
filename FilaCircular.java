public class FilaCircular {

    static int[] dados = new int[1000];
    static int inicio = 0;
    static int fim = 0;
    static int tamanho = 0;
    static boolean vazia(){
        return tamanho == 0;
    }
    static boolean cheia(){
        return fim == 1000;
    }
    static void enfileirar(int x){
        if(cheia()) return;
        dados[fim] = x;
        fim = (fim + 1) % 1000;
        tamanho++;
    }

    static int desenfileirar(){
        if(vazia()) return -1;
        int valor = dados[inicio];
        inicio = (inicio + 1) % 1000;
        tamanho--;
        return valor;
    }
    static void imprimir(){
        for(int i = 0;i< tamanho;i++){
            System.out.print(dados[inicio + i % 1000] + " ");
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
