class Contato {
    String nome;
    String telefone;

    Contato(String nome, String telefone) {
        this.nome = nome;
        this.telefone = telefone;
    }
}

class Celula {
    Contato contato;
    Celula prox;

    Celula(Contato contato) {
        this.contato = contato;
        this.prox = null;
    }
}

class No {
    char letra;
    No esq, dir;
    Celula primeiro, ultimo; // Início e fim da lista encadeada

    No(char letra) {
        this.letra = letra;
        this.esq = this.dir = null;
        this.primeiro = new Celula(null);
        this.ultimo = this.primeiro;
    }
}

public class ArvoreDeListas {
    No raiz;

    public ArvoreDeListas() {
        raiz = null;
    }

    // Método principal de inserção
    public void inserir(Contato contato) {
        char letra = contato.nome.toUpperCase().charAt(0);
        raiz = inserir(raiz, letra, contato);
    }

    private No inserir(No i, char letra, Contato contato) {
        if (i == null) {
            i = new No(letra);
            i.ultimo.prox = new Celula(contato);
            i.ultimo = i.ultimo.prox;
        } else if (letra < i.letra) {
            i.esq = inserir(i.esq, letra, contato);
        } else if (letra > i.letra) {
            i.dir = inserir(i.dir, letra, contato);
        } else {
            i.ultimo.prox = new Celula(contato);
            i.ultimo = i.ultimo.prox;
        }
        return i;
    }

    // Método para mostrar a agenda completa (Caminhamento Central)
    public void mostrar() {
        mostrar(raiz);
    }

    private void mostrar(No i) {
        if (i != null) {
            mostrar(i.esq);
            System.out.println("Letra: " + i.letra);
            for (Celula c = i.primeiro.prox; c != null; c = c.prox) {
                System.out.println("  -> " + c.contato.nome + " (" + c.contato.telefone + ")");
            }
            mostrar(i.dir);
        }
    }

    No pesquisarInterno(No i, char a){
        if(i == null) return null;
        if(a > i.letra) return pesquisarInterno(i.dir, a);
        else if (a == i.letra) return i;
        else return pesquisarInterno(i.esq, a);
    }

    boolean pesquisar(String nome){
        No no = pesquisarInterno(raiz, nome.toUpperCase().charAt(0));
        if (no == null) return false;
        
        for(Celula c = no.primeiro.prox; c != null; c = c.prox){
            if(c.contato.nome.equals(nome)) return true;
        }
        return false;
    }

    int contarDDDNaLetra(char letra, String ddd){
    No no = pesquisarInterno(raiz, letra);

    if(no == null) return 0;
    int contador = 0;
    for(Celula c = no.primeiro.prox; c != null;c = c.prox){
        if(c.contato.telefone.charAt(1) == ddd.charAt(0) && c.contato.telefone.charAt(2) == ddd.charAt(1)){
        contador ++;
        }
    }
    return contador;
    }
    int contarDDDNaArvore(String ddd){
        return contarDDDNaArvore(ddd, raiz);
    }
     int contarDDDNaArvore(String ddd, No i){
        if(i == null) return 0;
        int contador = 0;

        for(Celula c = i.primeiro.prox;c != null;c = c.prox){
        if(c.contato.telefone.charAt(1) == ddd.charAt(0) && c.contato.telefone.charAt(2) == ddd.charAt(1)){
        contador++;    
        }
    }
    contador += contarDDDNaArvore(ddd, i.esq);
    contador += contarDDDNaArvore(ddd, i.dir);
    return contador;
     }

    public static void main(String[] args) {
        ArvoreDeListas al = new ArvoreDeListas();

        // Dados com DDD misturados para podermos testar a contagem
        al.inserir(new Contato("Ana", "(31) 91111-1111"));
        al.inserir(new Contato("Bruno", "(11) 92222-2222"));
        al.inserir(new Contato("Alice", "(31) 93333-3333"));
        al.inserir(new Contato("Carlos", "(41) 94444-4444"));
        al.inserir(new Contato("Zeca", "(31) 95555-5555"));
        al.inserir(new Contato("Zuleica", "(11) 96666-6666"));

        System.out.println("Pesquisar Zeca: " + al.pesquisar("Zeca"));
        System.out.println("Total de contatos com DDD 31 na letra A: " + al.contarDDDNaLetra('A', "31"));
         System.out.println("Total de contatos com DDD 31 na Arvore: " + al.contarDDDNaArvore("11"));
    


        System.out.println("\n--- Agenda de Contatos ---");
        al.mostrar();
        
        // Aqui você vai chamar os métodos de contar depois que criá-los!
        // System.out.println("\nTotal de contatos com DDD 31 na árvore toda: " + al.contarContatosComDDD("31"));
    }
}