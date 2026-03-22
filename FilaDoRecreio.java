import java.util.Scanner;
public class FilaDoRecreio{
    
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        for(int i = 0;i<n;i++){
            int alunos = sc.nextInt();
            int trocas = 0;
            int[] notas = new int[alunos];
            int[] notas2 = new int[alunos];
            for(int j = 0;j < alunos;j++){
                notas[j] = sc.nextInt();
                notas2[j] = notas[j];
            }
            for(int m = 0;m< alunos-1;m++){
            for(int k = 0;k < alunos-1 -m ;k++){
                if(notas[k] < notas[k+1]){
                int tmp = notas[k];
                notas[k] = notas[k+1];
                notas[k+1] = tmp;
            }
        }
        }
            for(int l = 0;l < alunos;l++){
            if(notas[l] == notas2[l]) trocas++;
            }
            System.out.println(trocas);
    }
}
}
