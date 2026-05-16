import java.util.Scanner;


class Celula {
    public int elemento;
    public Celula inf;
    public Celula sup;
    public Celula esq;
    public Celula dir;

    public Celula() {
        this(0);
    }

    public Celula(int elemento) {
        this.elemento = elemento;
        this.inf = this.sup = this.esq = this.dir = null;
    }
}


class Matriz {
    private Celula inicio;
    private int linha;
    private int coluna;

    public Matriz(int linha, int coluna) {
        this.linha = linha;
        this.coluna = coluna;

        // Alocar a primeira célula da matriz (canto superior esquerdo)
        inicio = new Celula();
        Celula linhaAtual = inicio;

        // criar a primeira linha da matriz conectando horizontalmente
        for (int j = 1; j < coluna; j++) {
            Celula nova = new Celula();
            linhaAtual.dir = nova;
            nova.esq = linhaAtual;
            linhaAtual = nova;
        }

        // Criar as linhas subsequentes conectando verticalmente e horizontalmente
        Celula linhaAnterior = inicio;
        for (int i = 1; i < linha; i++) {
            Celula novaLinhaInicio = new Celula();
            linhaAnterior.inf = novaLinhaInicio;
            novaLinhaInicio.sup = linhaAnterior;

            Celula atualAnterior = linhaAnterior.dir;
            Celula atualNova = novaLinhaInicio;

            for (int j = 1; j < coluna; j++) {
                Celula nova = new Celula();
                atualNova.dir = nova;
                nova.esq = atualNova;

                atualAnterior.inf = nova;
                nova.sup = atualAnterior;

                atualAnterior = atualAnterior.dir;
                atualNova = nova;
            }
            linhaAnterior = novaLinhaInicio;
        }
    }

    public Celula getInicio() {
        return this.inicio;
    }

    // efetua a soma da matriz corrente com a matriz m 
    public Matriz somar(Matriz m) {
        if (this.linha != m.linha || this.coluna != m.coluna) {
            return null;
        }

        Matriz resp = new Matriz(this.linha, this.coluna);
        Celula iA = this.inicio, iB = m.inicio, iC = resp.inicio;

        while (iA != null) {
            Celula jA = iA, jB = iB, jC = iC;
            while (jA != null) {
                jC.elemento = jA.elemento + jB.elemento;
                jA = jA.dir; 
                jB = jB.dir; 
                jC = jC.dir;
            }
            iA = iA.inf; 
            iB = iB.inf; 
            iC = iC.inf;
        }
        return resp;
    }

    // efetua a multiplicação da matriz corrente pela matriz m
    public Matriz multiplicar(Matriz m) {
        if (this.coluna != m.linha) {
            return null;
        }

        Matriz resp = new Matriz(this.linha, m.coluna);
        Celula rowC = resp.inicio;
        Celula rowA = this.inicio;

        while (rowA != null) {
            Celula colC = rowC;
            Celula colB = m.inicio; 

            while (colB != null) {
                int soma = 0;
                Celula jA = rowA;
                Celula jB = colB;

                while (jA != null && jB != null) {
                    soma += jA.elemento * jB.elemento;
                    jA = jA.dir;
                    jB = jB.inf;
                }

                colC.elemento = soma;
                colC = colC.dir;
                colB = colB.dir; 
            }
            rowA = rowA.inf;
            rowC = rowC.inf;
        }
        return resp;
    }

    // imprime os elementos da diagonal principal 
    public void mostrarDiagonalPrincipal() {
        if (linha == coluna) {
            Celula i = inicio;
            while (i != null) {
                System.out.print(i.elemento + " ");
                if (i.inf != null) {
                    i = i.inf.dir;
                } else {
                    i = null;
                }
            }
            System.out.println();
        }
    }

    // imprime os elementos da diagonal secundária
    public void mostrarDiagonalSecundaria() {
        if (linha == coluna) {
            Celula i = inicio;
            // Desloca para o último elemento da primeira linha
            while (i.dir != null) {
                i = i.dir;
            }
            // desloca em diagonal para baixo e para a esquerda
            while (i != null) {
                System.out.print(i.elemento + " ");
                if (i.inf != null) {
                    i = i.inf.esq;
                } else {
                    i = null;
                }
            }
            System.out.println();
        }
    }

    // imprime a matriz completa conforme o padrão de saída esperado
    public void mostrar() {
        for (Celula i = inicio; i != null; i = i.inf) {
            for (Celula j = i; j != null; j = j.dir) {
                System.out.print(j.elemento + " ");
            }
            System.out.println();
        }
    }
}


public class tp39 {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        if (sc.hasNextInt()) {
            int numCasos = sc.nextInt(); // Número de casos de teste 

            for (int caso = 0; caso < numCasos; caso++) {
                //  Leitura e criação da Primeira Matriz 
                int l1 = sc.nextInt();
                int c1 = sc.nextInt();
                Matriz m1 = new Matriz(l1, c1);
                
                for (Celula i = m1.getInicio(); i != null; i = i.inf) {
                    for (Celula j = i; j != null; j = j.dir) {
                        j.elemento = sc.nextInt();
                    }
                }

                //  Leitura e criação da Segunda Matriz 
                int l2 = sc.nextInt();
                int c2 = sc.nextInt();
                Matriz m2 = new Matriz(l2, c2);
                
                for (Celula i = m2.getInicio(); i != null; i = i.inf) {
                    for (Celula j = i; j != null; j = j.dir) {
                        j.elemento = sc.nextInt();
                    }
                }

                // Exibição dos resultados 
                m1.mostrarDiagonalPrincipal();     // Diagonal principal da primeira 
                m2.mostrarDiagonalSecundaria();    // Diagonal secundária da segunda 

                Matriz soma = m1.somar(m2);        // Matriz resultante da Soma 
                if (soma != null) {
                    soma.mostrar();
                }

                Matriz multiplicacao = m1.multiplicar(m2); // Matriz resultante da Multiplicação 
                if (multiplicacao != null) {
                    multiplicacao.mostrar();
                }
            }
        }
        sc.close();
    }
}