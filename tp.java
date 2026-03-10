import java.util.Scanner;
class tp {

   
    public static int charToInt(char c) {
        return c - '0';
    }

    
    public static int lastIndexOf(String str, char ch) {
        for (int i = str.length() - 1; i >= 0; i--) {
            if (str.charAt(i) == ch) {
                return i;
            }
        }
        return -1;
    }

    public static String substring(String str, int start, int end) {
        String result = "";
        for (int i = start; i < end; i++) {
            result += str.charAt(i);
        }
        return result;
    }

  
    public static boolean avaliarExpressao(String line) {
        // 1. Extrair o número de variáveis (n)
        int n = charToInt(line.charAt(0));
        
        // Se n for 0, não há entradas (embora o caso de teste possa ter expressão)
        if (n == 0) {
             // Lógica para expressão sem variáveis, se necessário.
        }

        // 2. Extrair os valores das entradas (A, B, C...)
        boolean[] valores = new boolean[n];
        int pos = 2; // Posição inicial dos valores na string (após n e espaço)
        for (int i = 0; i < n; i++) {
            valores[i] = (line.charAt(pos) == '1');
            pos += 2; // Pula o valor e o espaço
        }

        // 3. Extrair a expressão e substituir as variáveis pelos seus valores
        String expressao = substring(line, pos, line.length());
        String exprComValores = "";
        for (int i = 0; i < expressao.length(); i++) {
            char c = expressao.charAt(i);
            if (c >= 'A' && c <= 'Z') {
                // Substitui a letra (A, B, ...) pelo valor correspondente
                exprComValores += (valores[c - 'A'] ? '1' : '0');
            } else if (c != ' ') {
                // Remove espaços e mantém o resto (parênteses, vírgulas, operadores)
                exprComValores += c;
            }
        }

        // 4. Loop de avaliação iterativa
        while (exprComValores.length() > 1) {
            int p_abertura = lastIndexOf(exprComValores, '(');
            int p_fechamento = -1;

            // Encontra o ')' correspondente
            for (int i = p_abertura + 1; i < exprComValores.length(); i++) {
                if (exprComValores.charAt(i) == ')') {
                    p_fechamento = i;
                    break;
                }
            }

            // Extrai os argumentos da subexpressão mais interna
            String args = substring(exprComValores, p_abertura + 1, p_fechamento);

            // Extrai o operador (and, or, not)
            int inicio_op = p_abertura - 1;
            while (inicio_op >= 0 && exprComValores.charAt(inicio_op) >= 'a' && exprComValores.charAt(inicio_op) <= 'z') {
                inicio_op--;
            }
            inicio_op++;
            
            String operador = substring(exprComValores, inicio_op, p_abertura);
            char resultadoChar;

            // Avalia a subexpressão
            if (operador.equals("not")) {
                resultadoChar = (args.charAt(0) == '0') ? '1' : '0';
            } else {
                boolean resultadoBool;
                if (operador.equals("and")) {
                    resultadoBool = true;
                    for (int i = 0; i < args.length(); i++) {
                        if (args.charAt(i) == '0') {
                            resultadoBool = false;
                            break;
                        }
                    }
                } else { // or
                    resultadoBool = false;
                    for (int i = 0; i < args.length(); i++) {
                        if (args.charAt(i) == '1') {
                            resultadoBool = true;
                            break;
                        }
                    }
                }
                resultadoChar = resultadoBool ? '1' : '0';
            }

            // Substitui a subexpressão avaliada pelo seu resultado
            String antes = substring(exprComValores, 0, inicio_op);
            String depois = substring(exprComValores, p_fechamento + 1, exprComValores.length());
            exprComValores = antes + resultadoChar + depois;
        }

        return exprComValores.charAt(0) == '1';
    }

    public static void main(String[] args) {
        // Objeto para ler a entrada padrão
        Scanner scanner = new Scanner(System.in);
        String line;

        // Lê linha por linha até encontrar a condição de parada "0"
        while (!(line = scanner.nextLine()).equals("0")) {
            boolean resultado = avaliarExpressao(line);
            if (resultado) {
                System.out.println("1");
            } else {
                System.out.println("0");
            }
        }
        scanner.close();
    }
}