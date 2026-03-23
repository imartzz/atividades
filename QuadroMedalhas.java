import java.util.Scanner;
public class QuadroMedalhas {
    public static class Pais{
        int ouro;
        int prata;
        int bronze;
        String nome;
        public Pais(String nome,int ouro,int prata,int bronze) {
            this.nome = nome;
            this.ouro = ouro;
            this.prata = prata;
            this.bronze = bronze;
        }
    }
    public static void ordenar(Pais[] listaDePaises, int n) {
        for (int i = 0; i < n - 1; i++) {
            boolean swap = false;
            for (int j = 0; j < n - i - 1; j++) {


                if (deveTrocar(listaDePaises[j], listaDePaises[j + 1])) {

                    Pais temp = listaDePaises[j];
                    listaDePaises[j] = listaDePaises[j + 1];
                    listaDePaises[j + 1] = temp;
                    swap = true;
                }
            }
            if (!swap) break;
        }
    }


    public static boolean deveTrocar(Pais p1, Pais p2) {

        if (p1.ouro < p2.ouro) return true;
        if (p1.ouro > p2.ouro) return false;


        if (p1.prata < p2.prata) return true;
        if (p1.prata > p2.prata) return false;


        if (p1.bronze < p2.bronze) return true;
        if (p1.bronze > p2.bronze) return false;



        return p1.nome.compareTo(p2.nome) > 0;
    }
    public static void main(String[] args){
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        Pais[] listaDePaises = new Pais[n];
        for(int i = 0; i < n;i++){
            String nome = sc.next();
            int ouro = sc.nextInt();
            int prata = sc.nextInt();
            int bronze = sc.nextInt();
            listaDePaises[i] = new Pais(nome, ouro, prata, bronze);
        }
        ordenar(listaDePaises,n);
        for (Pais p : listaDePaises) {
            System.out.println(p.nome + " " + p.ouro + " " + p.prata + " " + p.bronze);
        }
    }
}
