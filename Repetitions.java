import java.util.Scanner;
public class Repetitions {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        String s = sc.nextLine();
        long n = s.length();

        long maior = 1;
        long atual = 1;
        for(int i = 1;i<n;i++){
            if(s.charAt(i) == s.charAt(i-1)){
            atual++;
            if(atual > maior)
                maior = atual;
        }else {
                atual = 1;
            }
        
    }
     System.out.println(maior);
}
}
