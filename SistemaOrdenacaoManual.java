import java.util.InputMismatchException;
import java.util.Scanner;

public class SistemaOrdenacaoManual {

    
    static class Aluno {
        private String nome;
        private int idade;

        public Aluno(String nome, int idade) {
            this.nome = nome;
            this.idade = idade;
        }

        public String getNome() {
            return nome;
        }

        public int getIdade() {
            return idade;
        }

        @Override
        public String toString() {
            return "Nome: " + nome + ", Idade: " + idade;
        }
    }

    public static void ordenarAlunos(Aluno[] alunos) {
        int n = alunos.length;
        Aluno temp;

        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - 1 - i; j++) {
                
                // 1. Comparar por Nome
                int comparacaoNome = alunos[j].getNome().compareTo(alunos[j + 1].getNome());

                if (comparacaoNome > 0) {
                    // Se o nome do j for "maior" (vem depois), troca
                    temp = alunos[j];
                    alunos[j] = alunos[j + 1];
                    alunos[j + 1] = temp;
                } else if (comparacaoNome == 0) {
                    // 2. Se os nomes são IGUAIS, comparar por Idade
                    if (alunos[j].getIdade() > alunos[j + 1].getIdade()) {
                        // Se a idade do j for maior, troca
                        temp = alunos[j];
                        alunos[j] = alunos[j + 1];
                        alunos[j + 1] = temp;
                    }
                }
            }
        }
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int quantidadeAlunos = 0;

        try {
            System.out.print("Quantos alunos você deseja inserir? ");
            quantidadeAlunos = scanner.nextInt();
        } catch (InputMismatchException e) {
            System.out.println("Entrada inválida. Por favor, digite um número.");
            scanner.close();
            return;
        }
        
        // Consumir a nova linha pendente após nextInt()
        scanner.nextLine(); 

        // 1. Criar o array de Alunos
        Aluno[] vetorAlunos = new Aluno[quantidadeAlunos];

        // 2. Leitura dos dados com Scanner
        System.out.println("\n--- Inserção dos Alunos ---");
        for (int i = 0; i < quantidadeAlunos; i++) {
            System.out.println("\nAluno #" + (i + 1));
            
            String nome;
            do {
                System.out.print("Nome: ");
                nome = scanner.nextLine().trim();
                if (nome.isEmpty()) {
                    System.out.println("O nome não pode ser vazio. Tente novamente.");
                }
            } while (nome.isEmpty());
            
            int idade = -1;
            while (idade < 0) {
                try {
                    System.out.print("Idade: ");
                    idade = scanner.nextInt();
                    if (idade < 0) {
                         System.out.println("A idade deve ser um número positivo.");
                    }
                } catch (InputMismatchException e) {
                    System.out.println("Entrada inválida. Digite um número inteiro para a idade.");
                    scanner.next(); // Limpa o buffer de entrada
                }
            }
            scanner.nextLine(); // Consumir a nova linha após nextInt()

            // Criar e inserir o objeto no array
            vetorAlunos[i] = new Aluno(nome, idade);
        }

        // 3. Ordenação do array usando o Bubble Sort customizado
        ordenarAlunos(vetorAlunos);

        // 4. Impressão dos alunos ordenados
        System.out.println("\n--- Lista de Alunos Ordenada (Nome e depois Idade) ---");
        for (Aluno aluno : vetorAlunos) {
            System.out.println(aluno);
        }

        scanner.close();
    }
}