#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <time.h> 

#define MATRICULA "888678" 
#define MAX_ATTRIBUTES 14
#define MAX_LIST_ITEMS 100
#define MAX_STRING_LEN 4000
#define MAX_NOME_LEN 100
#define MAX_DATA_LEN 100
#define MAX_SUB_STRING_LEN 50
#define CACHE_SIZE 5000

// Struct para armazenar os dados de um jogo
typedef struct Game {
    // Note: O campo 'atributos' foi removido desta struct para otimização, 
    // mas a lógica de parsing agora usa um array auxiliar na main/função de parsing.
    int AppID;
    char nome[MAX_NOME_LEN];
    char releaseDate[MAX_DATA_LEN];
    int estimatedOwners;
    float preco;
    char supLanguages[MAX_LIST_ITEMS][MAX_SUB_STRING_LEN];
    int metacriticScore;
    float userScore;
    int achievements;
    char publishers[MAX_LIST_ITEMS][MAX_SUB_STRING_LEN];
    char developers[MAX_LIST_ITEMS][MAX_SUB_STRING_LEN];
    char categories[MAX_LIST_ITEMS][MAX_SUB_STRING_LEN];
    char genres[MAX_LIST_ITEMS][MAX_SUB_STRING_LEN];
    char tags[MAX_LIST_ITEMS][MAX_SUB_STRING_LEN];
} Game;

// No da Arvore AVL
typedef struct No {
    Game elemento;
    struct No *esq, *dir;
    int altura; 
} No;

// Variável global para contagem de comparações
int comparacoes = 0;
Game JogosCache[CACHE_SIZE]; 
int nJogos = 0;

// ==============================================================================
// 1. PROTOTIPOS E FUNCOES DE PARSING
// ==============================================================================

// Prototipos
void limparNaoNumericos(char *entrada, char *saida);
void parseListaString(char* entrada, char saida[][MAX_SUB_STRING_LEN]);
void parseDataJogo(char *dataOriginal, char *dataFinal);
// Novo protótipo para a função de separação, aceitando o array de atributos auxiliar
void parseCSVLinha(Game *Jogo, char *linha, char atributos[][MAX_STRING_LEN]);
void atribuirCamposJogo(Game *Jogo, char atributos[][MAX_STRING_LEN]);
bool isIdNaLista(char *linhaCSV, char listaID[][20], int listaTam);


// Logica de limpar nao numericos
void limparNaoNumericos(char *entrada, char *saida) {
    int j = 0;
    for (int i = 0; entrada[i] != '\0'; i++) {
        if (entrada[i] >= '0' && entrada[i] <= '9') {
            saida[j++] = entrada[i];
        }
    }
    saida[j] = '\0';
}

// Logica de parsear listas de string
void parseListaString(char* entrada, char saida[][MAX_SUB_STRING_LEN]) {
    int tokenCount = 0;
    for (int a = 0; a < MAX_LIST_ITEMS; a++) saida[a][0] = '\0';
    
    char str_limpa[MAX_STRING_LEN];
    strncpy(str_limpa, entrada, MAX_STRING_LEN - 1);
    str_limpa[MAX_STRING_LEN - 1] = '\0';
    
    // Remove colchetes, aspas e espacos no inicio e fim
    int len = strlen(str_limpa);
    if(len <= 2) return;
    
    int inicio = 0;
    while(inicio < len && (str_limpa[inicio] == '[' || str_limpa[inicio] == '\'' || str_limpa[inicio] == '"' || str_limpa[inicio] == ' ')) inicio++;
    int fim = len - 1;
    while(fim > inicio && (str_limpa[fim] == ']' || str_limpa[fim] == '\'' || str_limpa[fim] == '"' || str_limpa[fim] == ' ')) fim--;
    str_limpa[fim + 1] = '\0';
    
    char *ptr_limpo = &str_limpa[inicio];
    char *token = strtok(ptr_limpo, ",");
    
    while(token != NULL && tokenCount < MAX_LIST_ITEMS) {
        int start_token = 0;
        // Remove espacos e aspas no inicio do token
        while(token[start_token] == ' ' || token[start_token] == '\'' || token[start_token] == '"') start_token++;
        int end_token = strlen(token) - 1;
        // Remove espacos e aspas no final do token
        while(end_token >= start_token && (token[end_token] == ' ' || token[end_token] == '\'' || token[end_token] == '"')) end_token--;
        token[end_token + 1] = '\0';
        char *token_final = token + start_token;

        if (strlen(token_final) > 0 && strlen(token_final) < MAX_SUB_STRING_LEN) {
            strcpy(saida[tokenCount], token_final);
            tokenCount++;
        }
        token = strtok(NULL, ",");
    }
}

// Logica de parsear a data
void parseDataJogo(char *dataOriginal, char *dataFinal) {
    char mesStr[10] = "", diaStr[10] = "", anoStr[10] = "";
    int dia = 1, mes = 1, ano = 1900;
    char limpa[50];
    int j = 0;
    
    // Remove virgulas
    for (int i = 0; dataOriginal[i] != '\0'; i++) {
        if (dataOriginal[i] != ',') limpa[j++] = dataOriginal[i];
    }
    limpa[j] = '\0';
    
    int items = sscanf(limpa, "%s %s %s", mesStr, diaStr, anoStr);
    
    // Mapeamento de meses
    if (strcmp(mesStr, "Jan") == 0) mes = 1; else if (strcmp(mesStr, "Feb") == 0) mes = 2;
    else if (strcmp(mesStr, "Mar") == 0) mes = 3; else if (strcmp(mesStr, "Apr") == 0) mes = 4;
    else if (strcmp(mesStr, "May") == 0) mes = 5; else if (strcmp(mesStr, "Jun") == 0) mes = 6;
    else if (strcmp(mesStr, "Jul") == 0) mes = 7; else if (strcmp(mesStr, "Aug") == 0) mes = 8;
    else if (strcmp(mesStr, "Sep") == 0) mes = 9; else if (strcmp(mesStr, "Oct") == 0) mes = 10;
    else if (strcmp(mesStr, "Nov") == 0) mes = 11; else if (strcmp(mesStr, "Dec") == 0) mes = 12;
    
    if (items == 3) { dia = atoi(diaStr); ano = atoi(anoStr); }
    else if (items == 2) { dia = 1; ano = atoi(diaStr); }
    else if (items == 1) { if(isdigit(mesStr[0])){ ano = atoi(mesStr); dia = 1; mes = 1; } }
    
    if (ano == 0) ano = 1900;
    sprintf(dataFinal, "%02d/%02d/%04d", dia, mes, ano);
}

// Logica de separar atributos do CSV
void parseCSVLinha(Game *Jogo, char *linha, char atributos[][MAX_STRING_LEN]) {
    int i = 0, j = 0, campo = 0;
    bool entre_aspas = false;
    int len = strlen(linha);
    for (int a = 0; a < MAX_ATTRIBUTES; a++) atributos[a][0] = '\0';

    while (i < len && campo < MAX_ATTRIBUTES) {
        j = 0;
        char token[MAX_STRING_LEN] = "";
        while(i < len && linha[i] == ' ') i++; 
        if(i >= len) break;

        if (linha[i] == '"') {
            entre_aspas = true;
            i++;
        }

        while (i < len && j < (MAX_STRING_LEN - 1)) {
            if (entre_aspas) {
                if (linha[i] == '"') {
                    if (i + 1 < len && linha[i+1] == '"') { // Aspas duplas escapadas
                        token[j++] = '"';
                        i += 2;
                    } else { // Fim das aspas
                        entre_aspas = false;
                        i++;
                        break;
                    }
                } else {
                    token[j++] = linha[i++];
                }
            } else {
                if (linha[i] == ',') break; // Fim do campo sem aspas
                else token[j++] = linha[i++];
            }
        }
        token[j] = '\0';
        // Remove quebras de linha
        token[strcspn(token, "\n\r")] = '\0'; 
        strcpy(atributos[campo], token);
        campo++;
        if (i < len && linha[i] == ',') i++;
    }
}

// Logica de atribuir atributos a struct Game
void atribuirCamposJogo(Game *Jogo, char atributos[][MAX_STRING_LEN]){
    // 0: AppID
    Jogo->AppID = atoi(atributos[0]);
    
    // 1: Nome
    atributos[1][strcspn(atributos[1], "\n\r")] = '\0';
    strcpy(Jogo->nome, atributos[1]);
    
    // 2: Data de Lançamento
    parseDataJogo(atributos[2], Jogo->releaseDate);
    
    // 3: Estimated Owners
    char ownersLimpo[32];
    limparNaoNumericos(atributos[3], ownersLimpo);
    Jogo->estimatedOwners = atoi(ownersLimpo);
    
    // 4: Preço
    if (strstr(atributos[4], "Free") != NULL || strlen(atributos[4]) == 0) 
        Jogo->preco = 0.0f;
    else 
        Jogo->preco = (float) atof(atributos[4]);
        
    // 5: Supported Languages
    parseListaString(atributos[5], Jogo->supLanguages);
    
    // 6: Metacritic Score
    if (strlen(atributos[6]) == 0) Jogo->metacriticScore = -1;
    else Jogo->metacriticScore = atoi(atributos[6]);
    
    // 7: User Score
    if (strlen(atributos[7]) == 0 || strstr(atributos[7], "tbd") != NULL) 
        Jogo->userScore = -1.0f;
    else 
        Jogo->userScore = (float) atof(atributos[7]);
        
    // 8: Achievements
    if (strlen(atributos[8]) == 0) Jogo->achievements = 0;
    else Jogo->achievements = atoi(atributos[8]);
    
    // 9-13: Listas de strings
    parseListaString(atributos[9], Jogo->publishers);
    parseListaString(atributos[10], Jogo->developers);
    parseListaString(atributos[11], Jogo->categories);
    parseListaString(atributos[12], Jogo->genres);
    parseListaString(atributos[13], Jogo->tags);
}

// Logica de verificar se o ID está na lista
bool isIdNaLista(char *linhaCSV, char listaID[][20], int listaTam) {
    char idStr[20];
    int i = 0, j = 0;
    while (linhaCSV[i] != ',' && linhaCSV[i] != '\0' && j < 19) idStr[j++] = linhaCSV[i++];
    idStr[j] = '\0';
    int idLinha = atoi(idStr);
    for (int k = 0; k < listaTam; k++) {
        if (idLinha == atoi(listaID[k])) return true;
    }
    return false;
}

// ==============================================================================
// 2. FUNCOES AVL 
// ==============================================================================

// Funcao auxiliar para retornar o maior entre dois inteiros.
// Definida aqui para resolver o erro de linkagem 'undefined reference to maximo'.
int maximo(int a, int b) {
    return (a > b) ? a : b;
}

// Logica de pegar altura do no
int getAltura(No *no) {
    return (no == NULL) ? 0 : no->altura;
}

// Logica de pegar fator de balanceamento
int getFator(No *no) {
    return (no == NULL) ? 0 : (getAltura(no->dir) - getAltura(no->esq));
}

// Logica de criar um novo no
No* novoNo(Game elemento) {
    No* novo = (No*)malloc(sizeof(No));
    if (novo == NULL) {
        perror("Erro de alocacao de memoria para o no AVL");
        exit(EXIT_FAILURE);
    }
    novo->elemento = elemento;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->altura = 1; // No folha tem altura 1
    return novo;
}

// Logica de rotacao a direita
No* rotacaoDireita(No* no) {
    No* noEsq = no->esq;
    No* noEsqDir = noEsq->dir;

    // Rotacao
    noEsq->dir = no;
    no->esq = noEsqDir;

    // Atualiza alturas
    no->altura = maximo(getAltura(no->esq), getAltura(no->dir)) + 1;
    noEsq->altura = maximo(getAltura(noEsq->esq), getAltura(noEsq->dir)) + 1;

    return noEsq; // Nova raiz da subarvore
}

// Logica de rotacao a esquerda
No* rotacaoEsquerda(No* no) {
    No* noDir = no->dir;
    No* noDirEsq = noDir->esq;

    // Rotacao
    noDir->esq = no;
    no->dir = noDirEsq;

    // Atualiza alturas
    no->altura = maximo(getAltura(no->esq), getAltura(no->dir)) + 1;
    noDir->altura = maximo(getAltura(noDir->esq), getAltura(noDir->dir)) + 1;

    return noDir; // Nova raiz da subarvore
}

// Funcao de balanceamento e atualizacao de altura
No* balancear(No* no) {
    if (no == NULL) return no;

    // Atualiza a altura do no atual
    no->altura = maximo(getAltura(no->esq), getAltura(no->dir)) + 1;

    int fator = getFator(no);

    // Caso RR ou RL (Pesado a Direita: fator > 1)
    if (fator > 1) {
        if (getFator(no->dir) < 0) {
            no->dir = rotacaoDireita(no->dir); // RL Case (Dupla)
        }
        return rotacaoEsquerda(no); // RR Case (Simples) ou apos RL
    }
    
    // Caso LL ou LR (Pesado a Esquerda: fator < -1)
    if (fator < -1) {
        if (getFator(no->esq) > 0) {
            no->esq = rotacaoEsquerda(no->esq); // LR Case (Dupla)
        }
        return rotacaoDireita(no); // LL Case (Simples) ou apos LR
    }

    return no;
}

// Logica de inserir na AVL
No* inserirAVL(No* no, Game elemento) {
    if (no == NULL) {
        return novoNo(elemento);
    } 
    
    int comp = strcmp(elemento.nome, no->elemento.nome);
    
    if (comp < 0) {
        no->esq = inserirAVL(no->esq, elemento);
    } else if (comp > 0) {
        no->dir = inserirAVL(no->dir, elemento);
    } else {
        return no; // Elemento ja existe (nao insere repetido)
    }

    // Balanceia o no atual na volta da recursao
    return balancear(no); 
}

// Logica de pesquisar na AVL
bool pesquisarAVL(No* no, char* nome) {
    if (no == NULL) {
        printf(" NAO\n");
        return false;
    }

    int comp = strcmp(nome, no->elemento.nome);
    comparacoes++;

    if (comp == 0) {
        printf(" SIM\n");
        return true;
    } else if (comp < 0) {
        printf(" esq");
        return pesquisarAVL(no->esq, nome);
    } else {
        printf(" dir");
        return pesquisarAVL(no->dir, nome);
    }
}



int main() {
    setlocale(LC_ALL, "en_US.UTF-8");
    clock_t inicio = clock();

    //  Leitura dos IDs a serem carregados
    char entrada[100];
    char listaID[500][20];
    int listaTam = 0;

    if (scanf("%s", entrada) == 1) {
        while(strcmp(entrada, "FIM") != 0 && listaTam < 500) {
            strcpy(listaID[listaTam++], entrada);
            if (scanf("%s", entrada) != 1) break;
        }
    } else {
        printf("Erro ou entrada vazia na leitura dos IDs.\n");
        return 1;
    }

    //  Carga do CSV para o cache de jogos
    FILE *arquivo = fopen("/tmp/games.csv", "r");
    if (arquivo == NULL) { 
        printf("Erro ao abrir /tmp/games.csv\n"); 
        return 1; 
    }

    char linhaCSV[MAX_STRING_LEN + 100];
    fgets(linhaCSV, sizeof(linhaCSV), arquivo); // Pula o header

    char atributos[MAX_ATTRIBUTES][MAX_STRING_LEN]; // Array auxiliar para parsing
    while(fgets(linhaCSV, sizeof(linhaCSV), arquivo) != NULL && nJogos < CACHE_SIZE) {
        linhaCSV[strcspn(linhaCSV, "\n\r")] = '\0';
        if (isIdNaLista(linhaCSV, listaID, listaTam)) {
            parseCSVLinha(&JogosCache[nJogos], linhaCSV, atributos);
            atribuirCamposJogo(&JogosCache[nJogos], atributos);
            nJogos++;
        }
    }
    fclose(arquivo);

    //  Montar AVL a partir dos jogos no cache
    No* raiz = NULL;
    for(int i = 0; i < listaTam; i++) {
        int idProcurado = atoi(listaID[i]);
        for(int j = 0; j < nJogos; j++) {
            if(JogosCache[j].AppID == idProcurado) {
                raiz = inserirAVL(raiz, JogosCache[j]);
                break; // Jogo encontrado, passa para o proximo ID
            }
        }
    }

    //  Pesquisa e Saida
    // Limpa o restante da linha apos o ultimo ID lido, se houver
    if (listaTam > 0) {
        // Correcao: Limpa o buffer de entrada.
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    char nomePesquisa[200];
    while(fgets(nomePesquisa, sizeof(nomePesquisa), stdin) != NULL) {
        nomePesquisa[strcspn(nomePesquisa, "\n\r")] = '\0';
        if(strcmp(nomePesquisa, "FIM") == 0) break;

        printf("%s", nomePesquisa); 
        printf(": raiz"); 
        pesquisarAVL(raiz, nomePesquisa);
    }

    //  Log
    clock_t fim = clock();
    double tempoExecucao = (double)(fim - inicio) / CLOCKS_PER_SEC;
    
    char nomeArquivoLog[50];
    sprintf(nomeArquivoLog, "%s_avl.txt", MATRICULA);
    FILE *log = fopen(nomeArquivoLog, "w");
    if(log != NULL){
        fprintf(log, "%s\t%.4f\t%d", MATRICULA, tempoExecucao, comparacoes);
        fclose(log);
    }

    return 0;
}