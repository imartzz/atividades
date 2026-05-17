#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_RESTAURANTES 500
#define MAX_STRING 200

// variaveis globais para o arquivo de log e contagem
int comp = 0;

int dataAno[MAX_RESTAURANTES];
int dataMes[MAX_RESTAURANTES];
int dataDia[MAX_RESTAURANTES];

int horaAb[MAX_RESTAURANTES];
int minAb[MAX_RESTAURANTES];
int horaFe[MAX_RESTAURANTES];
int minFe[MAX_RESTAURANTES];

int     id[MAX_RESTAURANTES];
char    nome[MAX_RESTAURANTES][MAX_STRING];
char    cidade[MAX_RESTAURANTES][MAX_STRING];
int     capacidade[MAX_RESTAURANTES];
double  avaliacao[MAX_RESTAURANTES];
char    tiposCozinha[MAX_RESTAURANTES][MAX_STRING];
int     faixaPreco[MAX_RESTAURANTES];
bool    aberto[MAX_RESTAURANTES];
int     total = 0;

/* Remove espaços em branco antes e depois da string */
void trim(char* dest, const char* src) {
    while (*src == ' ' || *src == '\t') {
        src++;
    }
    int len = strlen(src);
    while (len > 0 && (src[len - 1] == ' ' || src[len - 1] == '\t' || src[len - 1] == '\r' || src[len - 1] == '\n')) {
        len--;
    }
    strncpy(dest, src, len);
    dest[len] = '\0';
}

/* Parseia uma linha do CSV */
void parseLinha(char* linha) {
    char tokens[10][MAX_STRING];
    int t = 0;
    char* start = linha;
    char* p = linha;
    
    // Divide por virgulas
    while (*p) {
        if (*p == ',') {
            int len = p - start;
            strncpy(tokens[t], start, len);
            tokens[t][len] = '\0';
            t++;
            start = p + 1;
        }
        p++;
    }
    strcpy(tokens[t], start);
    int len = strlen(tokens[t]);
    if (len > 0 && tokens[t][len-1] == '\n') tokens[t][len-1] = '\0';
    if (len > 1 && tokens[t][len-2] == '\r') tokens[t][len-2] = '\0';
    t++;

    id[total] = atoi(tokens[0]);
    trim(nome[total], tokens[1]);
    trim(cidade[total], tokens[2]);
    capacidade[total] = atoi(tokens[3]);
    avaliacao[total] = atof(tokens[4]);
    
    // Remove aspas se houver e troca ponto e vírgula por vírgula
    char cozinhaRaw[MAX_STRING];
    trim(cozinhaRaw, tokens[5]);
    int idxC = 0;
    for (int i = 0; cozinhaRaw[i] != '\0'; i++) {
        if (cozinhaRaw[i] != '"') {
            if (cozinhaRaw[i] == ';') {
                tiposCozinha[total][idxC++] = ',';
            } else {
                tiposCozinha[total][idxC++] = cozinhaRaw[i];
            }
        }
    }
    tiposCozinha[total][idxC] = '\0';
    
    char faixaRaw[MAX_STRING];
    trim(faixaRaw, tokens[6]);
    faixaPreco[total] = strlen(faixaRaw);
    
    sscanf(tokens[7], "%d:%d-%d:%d", &horaAb[total], &minAb[total], &horaFe[total], &minFe[total]);
    sscanf(tokens[8], "%d-%d-%d", &dataAno[total], &dataMes[total], &dataDia[total]);
    
    char abertoRaw[MAX_STRING];
    trim(abertoRaw, tokens[9]);
    aberto[total] = (abertoRaw[0] == 't');
    
    total++;
}

/* Lê o arquivo CSV linha a linha */
void lerCsv() {
    FILE* file = fopen("/tmp/restaurantes.csv", "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao ler CSV\n");
        return;
    }
    char linha[1000];
    if (fgets(linha, sizeof(linha), file) == NULL) { // Pula cabeçalho
        fclose(file);
        return;
    }
    while (fgets(linha, sizeof(linha), file) != NULL) {
        parseLinha(linha);
    }
    fclose(file);
}

/* Retorna o restaurante formatado em uma string destino */
void formatarRestaurante(int i, char* resultado) {
    char faixa[10] = "";
    for (int j = 0; j < faixaPreco[i]; j++) {
        strcat(faixa, "$");
    }
    
    char strAberto[6];
    if (aberto[i]) {
        strcpy(strAberto, "true");
    } else {
        strcpy(strAberto, "false");
    }
    
    sprintf(resultado, "[%d ## %s ## %s ## %d ## %.1f ## [%s] ## %s ## %02d:%02d-%02d:%02d ## %02d/%02d/%04d ## %s]",
            id[i], nome[i], cidade[i], capacidade[i], avaliacao[i],
            tiposCozinha[i], faixa,
            horaAb[i], minAb[i], horaFe[i], minFe[i],
            dataDia[i], dataMes[i], dataAno[i], strAberto);
}

/* Encontra o índice global correspondente ao ID passado */
int obterIndicePorId(int busca) {
    for (int i = 0; i < total; i++) {
        if (id[i] == busca) return i;
    }
    return -1;
}

// Estrutura do Nó da Árvore Binária
typedef struct No {
    int elementoIdx; // Guarda o índice do elemento nos arrays globais
    struct No* esq;
    struct No* dir;
} No;

No* novoNo(int idx) {
    No* n = (No*)malloc(sizeof(No));
    n->elementoIdx = idx;
    n->esq = NULL;
    n->dir = NULL;
    return n;
}

// i    nsere um elemento na árvore usando o atributo nome como chave
No* inserir(No* i, int idx) {
    if (i == NULL) {
        return novoNo(idx);
    }
    int compRes = strcmp(nome[idx], nome[i->elementoIdx]);
    if (compRes < 0) {
        i->esq = inserir(i->esq, idx);
    } else if (compRes > 0) {
        i->dir = inserir(i->dir, idx);
    }
    // Se for igual, o enunciado pede para não inserir duplicados
    return i;
}

// pesquisa se uma chave está na árvore mostrando o caminho de ponteiros
bool pesquisar(No* i, const char* chave) {
    if (i == NULL) {
        return false;
    }
    comp++;
    int compRes = strcmp(chave, nome[i->elementoIdx]);
    if (compRes == 0) {
        return true;
    } else if (compRes < 0) {
        printf("esq ");
        return pesquisar(i->esq, chave);
    } else {
        printf("dir ");
        return pesquisar(i->dir, chave);
    }
}

void pesquisarArvore(No* raiz, const char* chave) {
    printf("raiz ");
    if (pesquisar(raiz, chave)) {
        printf("SIM\n");
    } else {
        printf("NAO\n");
    }
}

// mostra os elementos usando caminhamento em ordem
void caminharEmOrdem(No* i) {
    if (i != NULL) {
        caminharEmOrdem(i->esq);
        char buffer[1000];
        formatarRestaurante(i->elementoIdx, buffer);
        printf("%s\n", buffer);
        caminharEmOrdem(i->dir);
    }
}

int main() {
    lerCsv();

    No* raiz = NULL;
    int idBusca;

    // Leitura dos IDs iniciais para preencher a árvore até encontrar -1
    while (scanf("%d", &idBusca) == 1) {
        if (idBusca == -1) {
            break;
        }
        int idx = obterIndicePorId(idBusca);
        if (idx != -1) {
            raiz = inserir(raiz, idx);
        }
    }
    
    // consumir o fim de linha restante após o número -1
    char c;
    while ((c = getchar()) != '\n' && c != EOF);

    clock_t inicio = clock();
    
    // leitura das chaves de pesquisa por nome até encontrar FIM
    char linhaChave[MAX_STRING];
    while (fgets(linhaChave, sizeof(linhaChave), stdin) != NULL) {
        char chave[MAX_STRING];
        trim(chave, linhaChave);
        if (strcmp(chave, "FIM") == 0) {
            break;
        }
        pesquisarArvore(raiz, chave);
    }
    
    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000.0;

    // apresenta os registros usando caminhamento em ordem
    caminharEmOrdem(raiz);

    // gerar arquivo de log específico para arvore binária
    FILE* log = fopen("matricula_arvore_binaria.txt", "w");
    if (log != NULL) {
        fprintf(log, "888678\t%d\t%dms", comp, (int)tempo);
        fclose(log);
    }

    return 0;
}