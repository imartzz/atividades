public class arvoreDeArvore{
    No raiz;

    arvoreDeArvore() {
        raiz = null;
    }
     
    No inserirExterno(No i, char letra) {
    if (i == null) return new No(letra); 
    if (letra < i.letra) i.esq = inserirExterno(i.esq, letra);
    else if (letra > i.letra) i.dir = inserirExterno(i.dir, letra);
    return i; 
}


    No pesquisarExterno(char letra, No i){
        if(i == null) return i;
        if(letra == i.letra) return i;
        if(letra < i.letra) {
            return pesquisarExterno(letra, i.esq);
        } else {
        return pesquisarExterno(letra, i.dir); 
        }
    }
    
    No2 inserirInterno(No2 i, String palavra) {
    if (i == null) {
        return new No2(palavra);
    } else if (palavra.compareTo(i.palavra) < 0) {
        return inserirInterno(i.esq, palavra);
    } else if (palavra.compareTo(i.palavra) > 0) {
        return inserirInterno(i.dir, palavra);
    }
    return i;
}

    void inserir(String palavra){
        char inicial = palavra.charAt(0);

        raiz = inserirExterno(raiz, inicial);

        No no = pesquisarExterno(inicial, raiz);
        






    }


    class No {
        char letra;
        No dir;
        No esq;
        No2 raiz;
        No(char letra){
            this.letra = letra;
            dir = null;
            esq = null;
            raiz = null;
        }
    }
    class No2{
        String palavra;
        No2 esq;
        No2 dir;

        No2(String palavra){
            this.palavra = palavra;
            dir = null;
            esq =  null;
        }
    }

    public static void main(String[] args) {
        
    }
}
