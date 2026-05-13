public class ArvoreDeArvore{
    No raiz;

    ArvoreDeArvore() {
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
        i.esq = inserirInterno(i.esq, palavra);
    } else if (palavra.compareTo(i.palavra) > 0) {
        i.dir =  inserirInterno(i.dir, palavra);
    }
    return i;
}

    void inserir(String palavra){
        char inicial = palavra.charAt(0);

        raiz = inserirExterno(raiz, inicial);

        No no = pesquisarExterno(inicial, raiz);
        
        no.raiz = inserirInterno(no.raiz, palavra);
    }


    void mostrar(No i) {
    if(i == null) return;
    mostrar(i.esq);
    mostrarInterno(i.raiz);
    mostrar(i.dir);
}
void mostrarInterno(No2 i) {
    if(i == null) return;
    mostrarInterno(i.esq);
   System.out.println(i.palavra);
   mostrarInterno(i.dir);
}
    void mostrar() {
    mostrar(raiz);
    }
    
    int contarInterno(No2 i, char ultimo) {
    if (i == null) return 0;
    
    int soma = 0;
    soma += contarInterno(i.esq, ultimo);
    
    if (i.palavra.charAt(i.palavra.length() - 1) == ultimo) 
        soma++;
    
    soma += contarInterno(i.dir, ultimo);
    return soma;
}


int contarPalavras(char primeiro, char ultimo) {
    No i = pesquisarExterno(primeiro, raiz);
    if (i != null)       
        return contarInterno(i.raiz, ultimo);
    return 0;
}

boolean pesquisarGeral(String s){
    return pesquisarGeral(raiz,s);
}

boolean pesquisarInterno(No2 i, String s) {
    if (i == null) return false;
    if (i.palavra.equals(s)) return true;

    
    return pesquisarInterno(i.esq, s) || pesquisarInterno(i.dir, s);
}


boolean pesquisarGeral(No i,String s){
    if(i == null) return false;
    boolean resp = false;
    resp = pesquisarInterno(i.raiz,s);
    if(resp == true) return resp;
    resp = pesquisarGeral(i.esq, s);
    if(resp == true) return resp;
    resp = pesquisarGeral(i.dir, s);
    return resp;
}

int contarInternoSimples(No2 i){
    int soma = 0;
    if(i == null) return soma;
    soma++;
    soma += contarInternoSimples(i.esq);
    soma += contarInternoSimples(i.dir);
    return soma;
}

public int contarPelaLetra(char letra) {
    No no = pesquisarExterno(letra, raiz);
    if (no == null) return 0;
    return contarInternoSimples(no.raiz);
}




int contarPalavrasTam(String padrao) {
    No no = pesquisarExterno(padrao.charAt(0), raiz); 
    if (no == null) return 0;
    return pesquisarInternoTam(no.raiz, padrao.length());
} 

int pesquisarInternoTam(No2 i, int tamanho) {
    int soma = 0;
    if (i == null) return soma;
    if (i.palavra.length() == tamanho) soma++;
    soma += pesquisarInternoTam(i.esq, tamanho);
    soma += pesquisarInternoTam(i.dir, tamanho);
    return soma;
}

public double mediaTamanho(char letra) {
    No no = pesquisarExterno(letra, raiz);
    if (no == null) return 0;
    
    int totalPalavras = contarInternoSimples(no.raiz);
    if (totalPalavras == 0) return 0;
    
    int somaCaracteres = somarCaracteresInterno(no.raiz);
    
    return (double) somaCaracteres / totalPalavras;
}

private int somarCaracteresInterno(No2 i) {
   int soma = 0;
   if(i == null) return soma;
   soma += i.palavra.length();
   soma += somarCaracteresInterno(i.dir);
   soma += somarCaracteresInterno(i.esq);
   return soma;
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
        ArvoreDeArvore arv = new ArvoreDeArvore();

        arv.inserir("TE");
        arv.inserir("FE");
        arv.inserir("TA");
        arv.inserir("ZE");
        arv.inserir("VA");
        arv.inserir("TO");
        arv.mostrar();
        System.out.println(arv.contarPalavras('T', 'A'));
        System.out.println(arv.contarPalavras('T', 'R'));
        System.out.println(arv.contarPalavras('V', 'A'));
        System.out.println(arv.pesquisarGeral("VA"));
        System.out.println(arv.contarPelaLetra('T'));
        System.out.println(arv.contarPalavrasTam("TE"));
    }
}
