import java.util.Scanner;



public class PIlha {
    static int[] dados = new int[1000];
    static int topo = -1;

    static boolean vazia(){
        return topo == -1;
    }
    static boolean cheia(){
        return topo == 998;
    }

    static void push(int x){
        if(cheia()) return;
        dados[++topo] = x;
    }

    static int pop(){
        if(vazia()) return -1;

        return dados[topo--];
    }

    static void imprimir(){
        for(int i = topo; i >= 0;i--){
            System.out.print(dados[i] + " ");
        }
    }

    public static void main(String[] args){
        push(10);
        push(20);
        push(30);
        push(40);
        pop();
        pop();
        imprimir();
    }


}
