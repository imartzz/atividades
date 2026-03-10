#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h> // Para isdigit e tolower

// -Constantes e Variáveis Globais 
#define MAX_NAME_LEN 256
#define MAX_FIELD_LEN 1024
#define MAX_LINE_LEN 4096 // Linhas grandes para campos de idiomas/tags
#define NUM_FIELDS 14    // Número fixo de colunas no CSV
#define MATRICULA "888678"
#define CSV_PATH "/tmp/games.csv"

// Variáveis globais para estatísticas
long long comparacoes = 0;
clock_t inicioTempo;


// Estrutura completa
typedef struct Game {
    int id;
    char name[MAX_NAME_LEN];
    char releaseDate[15];
    int estimatedOwners;
    float price;
    char supportedLanguages[MAX_FIELD_LEN]; // Armazenamos como string bruta
    int metacriticScore;
    float userScore;
    int achievements;
    char publishers[MAX_FIELD_LEN];
    char developers[MAX_FIELD_LEN];
    char categories[MAX_FIELD_LEN];
    char genres[MAX_FIELD_LEN];
    char tags[MAX_FIELD_LEN];
} Game;

// --- Estrutura do Nó AVL ---
typedef struct No {
    Game elemento;
    int altura; 
    struct No *esq, *dir;
} No;


// --- Protótipos das Funções ---
int max(int a, int b);
int getAltura(No *no);
void setAltura(No *no);
int getFatorBalanceamento(No *no);
No *novoNo(Game elemento);
void liberarArvore(No *no);
No *rotacaoSimplesDireita(No *y);
No *rotacaoSimplesEsquerda(No *x);
No *inserir(No *no, Game novoElemento);
int pesquisar(No *no, char *nome);
void pesquisarComSaida(No *raiz, char *nome);


char *trim(char *str);
int parseNumericField(const char *s);
float parsePrice(const char *s);
char **splitCsvAspasC(char *linha, int maxCampos, int *numCampos);
void parseGameFromFields(Game *g, char **campos);
Game *lerTodosOsJogos(int *numJogos);
Game *buscarJogoPorId(Game *todosOsJogos, int numJogos, int id);




int max(int a, int b) { return (a > b) ? a : b; }
int getAltura(No *no) { return (no == NULL) ? 0 : no->altura; }
void setAltura(No *no) {
    if (no != NULL) {
        no->altura = 1 + max(getAltura(no->esq), getAltura(no->dir));
    }
}
int getFatorBalanceamento(No *no) {
    if (no == NULL) return 0;
    return getAltura(no->dir) - getAltura(no->esq);
}
No *novoNo(Game elemento) {
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL) { exit(EXIT_FAILURE); }
    novo->elemento = elemento;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->altura = 1; 
    return novo;
}

void liberarArvore(No *no) {
    if (no != NULL) {
        liberarArvore(no->esq);
        liberarArvore(no->dir);
        free(no);
    }
}



No *rotacaoSimplesDireita(No *y) {
    No *x = y->esq;
    No *T2 = x->dir;
    x->dir = y;
    y->esq = T2;
    setAltura(y);
    setAltura(x);
    return x;
}
No *rotacaoSimplesEsquerda(No *x) {
    No *y = x->dir;
    No *T2 = y->esq;
    y->esq = x;
    x->dir = T2;
    setAltura(x);
    setAltura(y);
    return y;
}
No *rotacaoDuplaEsquerda(No *no) {
    no->esq = rotacaoSimplesEsquerda(no->esq);
    return rotacaoSimplesDireita(no);
}
No *rotacaoDuplaDireita(No *no) {
    no->dir = rotacaoSimplesDireita(no->dir);
    return rotacaoSimplesEsquerda(no);
}



No *inserir(No *no, Game novoElemento) {
    if (no == NULL) return novoNo(novoElemento);
    
    // Chave de pesquisa: 'name'
    int cmp = strcmp(novoElemento.name, no->elemento.name);
    
    if (cmp < 0) {
        no->esq = inserir(no->esq, novoElemento);
    } else if (cmp > 0) {
        no->dir = inserir(no->dir, novoElemento);
    } else {
        // Chave duplicada
        return no; 
    }
    
    setAltura(no);
    int fb = getFatorBalanceamento(no);

    // Caso LL (Left-Left)
    if (fb < -1 && getFatorBalanceamento(no->esq) <= 0) {
        return rotacaoSimplesDireita(no);
    }
    // Caso LR (Left-Right)
    if (fb < -1 && getFatorBalanceamento(no->esq) > 0) {
        return rotacaoDuplaEsquerda(no); 
    }
    // Caso RR (Right-Right)
    if (fb > 1 && getFatorBalanceamento(no->dir) >= 0) {
        return rotacaoSimplesEsquerda(no);
    }
    // Caso RL (Right-Left)
    if (fb > 1 && getFatorBalanceamento(no->dir) < 0) {
        return rotacaoDuplaDireita(no);
    }
    
    return no;
}

int pesquisar(No *no, char *nome) {
    if (no == NULL) return 0; 
    
    int cmp = strcmp(nome, no->elemento.name);
    comparacoes++;

    if (cmp == 0) return 1;
    else if (cmp < 0) {
        printf(" esq");
        return pesquisar(no->esq, nome);
    } else {
        printf(" dir");
        return pesquisar(no->dir, nome);
    }
}

void pesquisarComSaida(No *raiz, char *nome) {
    char *nomeTrim = strdup(nome); // Cria cópia para limpeza
    trim(nomeTrim);

    printf("%s: =>raiz", nomeTrim);
    int encontrado = pesquisar(raiz, nomeTrim);
    printf(encontrado ? " SIM\n" : " NAO\n");

    free(nomeTrim);
}


/**
 * Remove espaços em branco (leading/trailing) de uma string.
 */
char *trim(char *str) {
    size_t len = 0;
    char *front = str;
    char *end = NULL;

    if (str == NULL) { return NULL; }
    if (str[0] == '\0') { return str; }

    len = strlen(str);
    end = str + len;

    // Remove espaços do início
    while (isspace((unsigned char)*front)) { front++; }

    // Remove espaços do fim
    if (end != front) {
        while (isspace((unsigned char)*(--end)) && end != front);
    }

    if (front != str && end == front && *front == '\0') {
        str[0] = '\0';
    } else if (end != str + len - 1) {
        *(end + 1) = '\0';
    }

    len = (size_t)(end - front + 1);
    if (str != front) {
        memmove(str, front, len);
        str[len] = '\0';
    }
    return str;
}


int parseNumericField(const char *s) {
    if (s == NULL) return 0;
    char cleaned[MAX_FIELD_LEN];
    int j = 0;
    for (int i = 0; s[i] != '\0' && j < MAX_FIELD_LEN - 1; i++) {
        if (isdigit((unsigned char)s[i])) {
            cleaned[j++] = s[i];
        }
    }
    cleaned[j] = '\0';
    return atoi(cleaned);
}

/**
 * Converte campo de preço.
 */
float parsePrice(const char *s) {
    if (s == NULL) return 0.0f;
    char temp[MAX_FIELD_LEN];
    strcpy(temp, s);
    // Em alguns CSVs, a vírgula é separador decimal; substituímos por ponto.
    for (int i = 0; temp[i] != '\0'; i++) {
        if (temp[i] == ',') {
            temp[i] = '.';
            break; // Assumindo apenas um separador decimal
        }
    }
    return atof(temp);
}


char **splitCsvAspasC(char *linha, int maxCampos, int *numCampos) {
    *numCampos = 0;
    // Aloca espaço para os ponteiros dos campos
    char **campos = (char **)malloc(maxCampos * sizeof(char *));
    if (campos == NULL) return NULL;

    char *inicioCampo = linha;
    int aspas = 0;
    int i;
    
    for (i = 0; linha[i] != '\0' && *numCampos < maxCampos; i++) {
        char c = linha[i];

        if (c == '"') {
            aspas = !aspas;
        } else if (c == ',' && !aspas) {
            // Fim do campo
            linha[i] = '\0'; // Termina o campo atual
            
            // Aloca e copia o campo
            campos[*numCampos] = strdup(inicioCampo);
            (*numCampos)++;
            
            inicioCampo = &linha[i + 1]; // Início do próximo campo
        }
    }

    // Último campo (ou campo que sobrou)
    if (*numCampos < maxCampos) {
        campos[*numCampos] = strdup(inicioCampo);
        (*numCampos)++;
    }

    return campos;
}

/**
 * Atribui os campos parsed ao struct Game.
 */
void parseGameFromFields(Game *g, char **campos) {
    if (campos == NULL || g == NULL) return;

    //  id
    g->id = atoi(campos[0]);
    
    //  name (Chave da AVL)
    strncpy(g->name, trim(campos[1]), MAX_NAME_LEN - 1);
    g->name[MAX_NAME_LEN - 1] = '\0';
    
    //  releaseDate
    strncpy(g->releaseDate, trim(campos[2]), 14);
    g->releaseDate[14] = '\0';
    
    //  estimatedOwners 
    g->estimatedOwners = parseNumericField(campos[3]);
    
    // 5. price 
    g->price = parsePrice(campos[4]);
    
    // 6. supportedLanguages
    strncpy(g->supportedLanguages, campos[5], MAX_FIELD_LEN - 1);
    g->supportedLanguages[MAX_FIELD_LEN - 1] = '\0';
    
    // 7. metacriticScore
    g->metacriticScore = atoi(campos[6]);

    // 8. userScore (Requer limpeza)
    g->userScore = parsePrice(campos[7]);

    // 9. achievements
    g->achievements = atoi(campos[8]);

    // 10. publishers
    strncpy(g->publishers, campos[9], MAX_FIELD_LEN - 1);
    g->publishers[MAX_FIELD_LEN - 1] = '\0';

    // 11. developers
    strncpy(g->developers, campos[10], MAX_FIELD_LEN - 1);
    g->developers[MAX_FIELD_LEN - 1] = '\0';

    // 12. categories
    strncpy(g->categories, campos[11], MAX_FIELD_LEN - 1);
    g->categories[MAX_FIELD_LEN - 1] = '\0';

    // 13. genres
    strncpy(g->genres, campos[12], MAX_FIELD_LEN - 1);
    g->genres[MAX_FIELD_LEN - 1] = '\0';

    // 14. tags
    strncpy(g->tags, campos[13], MAX_FIELD_LEN - 1);
    g->tags[MAX_FIELD_LEN - 1] = '\0';
}

/**
 * Lê o CSV, aloca e retorna o array completo de Games.
 * O chamador é responsável por liberar o array.
 */
Game *lerTodosOsJogos(int *numJogos) {
    FILE *arquivo = fopen(CSV_PATH, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "ERRO: Nao foi possivel abrir o arquivo CSV em %s\n", CSV_PATH);
        *numJogos = 0;
        return NULL;
    }

    char linha[MAX_LINE_LEN];
    // 1. Passa o cabeçalho
    if (fgets(linha, MAX_LINE_LEN, arquivo) == NULL) {
        fclose(arquivo);
        *numJogos = 0;
        return NULL;
    }

    // 2. Conta as linhas restantes para alocar a memória
    int count = 0;
    while (fgets(linha, MAX_LINE_LEN, arquivo) != NULL) {
        count++;
    }
    
    // Volta para o início (após o cabeçalho)
    rewind(arquivo);
    fgets(linha, MAX_LINE_LEN, arquivo); // Pula o cabeçalho novamente

    *numJogos = count;
    Game *todosOsJogos = (Game *)malloc(count * sizeof(Game));
    if (todosOsJogos == NULL) {
        fclose(arquivo);
        *numJogos = 0;
        return NULL;
    }

    // 3. Lê e faz o parsing de cada linha
    int i = 0;
    while (i < count && fgets(linha, MAX_LINE_LEN, arquivo) != NULL) {
        // Aloca uma cópia da linha para que splitCsvAspasC possa modificá-la (null termination)
        char *linhaCopia = strdup(linha);
        if (linhaCopia == NULL) break; 
        
        int numCamposLidos = 0;
        char **campos = splitCsvAspasC(linhaCopia, NUM_FIELDS, &numCamposLidos);
        
        if (campos != NULL && numCamposLidos >= NUM_FIELDS) {
            parseGameFromFields(&todosOsJogos[i], campos);
            i++;
        }
        
        // Libera a memória alocada por strdup e splitCsvAspasC
        free(linhaCopia);
        for (int j = 0; j < numCamposLidos; j++) {
            if (campos[j] != NULL) free(campos[j]);
        }
        free(campos);
    }
    
    fclose(arquivo);
    return todosOsJogos;
}

/**
 * Busca o jogo no array pelo ID (para inserção).
 */
Game *buscarJogoPorId(Game *todosOsJogos, int numJogos, int id) {
    for (int i = 0; i < numJogos; i++) {
        if (todosOsJogos[i].id == id) {
            return &todosOsJogos[i];
        }
    }
    return NULL;
}


// =========================================================================
// MAIN
// =========================================================================
int main() {
    No *raiz = NULL;
    int numJogosCSV = 0;
    
    // --- PARTE 0: CARREGAMENTO DE DADOS ---
    Game *todosOsJogos = lerTodosOsJogos(&numJogosCSV); 
    if (todosOsJogos == NULL) {
        fprintf(stderr, "Falha no carregamento do CSV. Saindo.\n");
        return 1;
    }

    char linha[MAX_LINE_LEN];
    
    // --- PARTE 1: INSERÇÃO (Lendo IDs da entrada padrão) ---
    while (fgets(linha, MAX_LINE_LEN, stdin) != NULL) {
        linha[strcspn(linha, "\n")] = 0; 
        if (strcmp(linha, "FIM") == 0) break;

        int idLido = atoi(linha);
        if (idLido == 0 && linha[0] != '0') continue;

        Game *jogo = buscarJogoPorId(todosOsJogos, numJogosCSV, idLido);
        
        if (jogo != NULL) { 
            Game novo = *jogo; 
            raiz = inserir(raiz, novo); 
        }
    }

    // --- PARTE 2: PESQUISA (Lendo Nomes da entrada padrão) ---
    inicioTempo = clock(); 
    
    while (fgets(linha, MAX_LINE_LEN, stdin) != NULL) {
        linha[strcspn(linha, "\n")] = 0; 
        if (strcmp(linha, "FIM") == 0) break;

        // A pesquisa requer o nome exato. A limpeza (trim) é feita dentro da função.
        char nomeLimp[MAX_NAME_LEN];
        strncpy(nomeLimp, linha, MAX_NAME_LEN - 1);
        nomeLimp[MAX_NAME_LEN - 1] = '\0';
        
        pesquisarComSaida(raiz, nomeLimp);
    }
    
    clock_t fimTempo = clock();
    double tempoExecucao = (double)(fimTempo - inicioTempo) / CLOCKS_PER_SEC * 1000; 

    // --- PARTE 3: LOG ---
    char nomeArquivoLog[50];
    sprintf(nomeArquivoLog, "%s_avl.txt", MATRICULA);
    
    FILE *logFile = fopen(nomeArquivoLog, "w");
    if (logFile != NULL) {
        fprintf(logFile, "%s\t%.4f\t%lld", MATRICULA, tempoExecucao, comparacoes);
        fclose(logFile);
    }
    
    // --- PARTE 4: LIBERAÇÃO DE MEMÓRIA ---
    liberarArvore(raiz);
    if (todosOsJogos != NULL) {
        free(todosOsJogos);
    }
    
    return 0;
}