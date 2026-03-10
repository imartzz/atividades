import java.util.Scanner;

public class espelho {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int i = sc.nextInt();
        int j = sc.nextInt();
        for(int n = i;n <= j;n++){
        System.out.print(n);
        }
        for(int n = j;n >=i;n--){
            System.out.print(n);
        }
    }
}
