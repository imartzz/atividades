using System;

class Program {
    static void Main() {
        int m = int.Parse(Console.ReadLine());
        int a = int.Parse(Console.ReadLine());
        int b = int.Parse(Console.ReadLine());

        int c = m - (a + b);

        if(c > a && c > b) Console.WriteLine(c);
        if(b > a && b > c) Console.WriteLine(b);
        if(a > b && a > c) Console.WriteLine(a);
    }
}