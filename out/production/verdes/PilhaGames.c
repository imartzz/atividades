#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Para isspace()




#define MAX_GAMES 5000         // Número máximo de jogos no CSV
#define MAX_STRING 200         // Tamanho máximo para strings simples (nome)
#define MAX_ARRAY_STRING 1000  // Tamanho para strings que representam arrays (tags, etc.)
#define MAX_LINHA_LEN 2000     // Tamanho máximo da linha do CSV

/**
 * Armazena os dados de um jogo.
 * Os campos "array" (ex: supportedLanguages) armazenam a string interna limpa.
 * (ex: "English, French" ao invés de "['English', 'French']")
 */
typedef struct Game {
    int id;
    char name[MAX_STRING];
    char releaseDate[11]; // "dd/MM/yyyy\0"
    int estimatedOwners;
    float price;
    char supportedLanguages[MAX_ARRAY_STRING];
    int metacriticScore;
    float userScore;
    int achievements;
    char publishers[MAX_ARRAY_STRING];
    char developers[MAX_ARRAY_STRING];
    char categories[MAX_ARRAY_STRING];
    char genres[MAX_ARRAY_STRING];
    char tags[MAX_ARRAY_STRING];
} Game;


typedef struct Celula {
    Game* elemento; // Ponteiro para o Game (no array 'todosOsGames')
    struct Celula* prox;
} Celula;

typedef struct PilhaGames {
    Celula* topo;
} PilhaGames;

// Array global para armazenar todos os jogos lidos do CSV
Game todosOsGames[MAX_GAMES];
int numTotalGames = 0;



/**
 * Remove espaços em branco do início e fim de uma string.
 */
char* trim(char* str) {
    char *end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    *(end+1) = 0;
    return str;
}

/**
 * Converte o nome de um mês (MMM) para seu número (MM).
 */
int converteMesParaNumero(char* mes) {
    if (strcmp(mes, "Jan") == 0) return 1;
    if (strcmp(mes, "Feb") == 0) return 2;
    if (strcmp(mes, "Mar") == 0) return 3;
    if (strcmp(mes, "Apr") == 0) return 4;
    if (strcmp(mes, "May") == 0) return 5;
    if (strcmp(mes, "Jun") == 0) return 6;
    if (strcmp(mes, "Jul") == 0) return 7;
    if (strcmp(mes, "Aug") == 0) return 8;
    if (strcmp(mes, "Sep") == 0) return 9;
    if (strcmp(mes, "Oct") == 0) return 10;
    if (strcmp(mes, "Nov") == 0) return 11;
    if (strcmp(mes, "Dec") == 0) return 12;
    return 0;
}

/**
 * Formata a data de "MMM dd, yyyy" ou "yyyy" para "dd/MM/yyyy".
 */
void formatarData(char* dataOriginal, char* dataDestino) {
    // Caso 1: "MMM dd, yyyy" (ex: "May 20, 2018")
    char mesStr[4];
    int dia, ano;
    if (sscanf(dataOriginal, "%s %d, %d", mesStr, &dia, &ano) == 3) {
        int mesNum = converteMesParaNumero(mesStr);
        sprintf(dataDestino, "%02d/%02d/%04d", dia, mesNum, ano);
    } 
    // Caso 2: "yyyy" (ex: "2018")
    else if (sscanf(dataOriginal, "%d", &ano) == 1 && strlen(dataOriginal) == 4) {
        sprintf(dataDestino, "01/01/%04d", ano);
    } 
    // Caso padrão ou falha
    else {
        strcpy(dataDestino, "01/01/0001");
    }
}

/**
 * Limpa as strings que representam arrays (remove '[', ']', '"', ''').
 */
void cleanArrayString(char* dest, const char* src) {
    char* d = dest;
    for (const char* s = src; *s != '\0'; s++) {
        if (*s != '[' && *s != ']' && *s != '\'' && *s != '"') {
            *d++ = *s;
        }
    }
    *d = '\0';
}

/**
 * Define o campo apropriado na struct Game com base no índice da coluna.
 */
void setGameField(Game* g, int campo, char* valor) {
    switch(campo) {
        case 0: g->id = atoi(valor); break;
        case 1: strcpy(g->name, valor); break;
        case 2: formatarData(valor, g->releaseDate); break;
        case 3: g->estimatedOwners = atoi(valor); break;
        case 4: g->price = (strcmp(valor, "Free to Play") == 0) ? 0.0f : atof(valor); break;
        case 5: cleanArrayString(g->supportedLanguages, valor); break;
        case 6: g->metacriticScore = atoi(valor); break;
        case 7: g->userScore = atof(valor); break;
        case 8: g->achievements = atoi(valor); break;
        case 9: cleanArrayString(g->publishers, valor); break;
        case 10: cleanArrayString(g->developers, valor); break;
        case 11: cleanArrayString(g->categories, valor); break;
        case 12: cleanArrayString(g->genres, valor); break;
        case 13: cleanArrayString(g->tags, valor); break;
    }
}

/**
 * Lê o arquivo CSV, faz o parsing de cada linha e preenche o array global 'todosOsGames'.
 */
void preencherTodosGames(const char* caminho) {
    FILE* f = fopen(caminho, "r");
    if (f == NULL) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    char linha[MAX_LINHA_LEN];
    fgets(linha, MAX_LINHA_LEN, f); // Pula o cabeçalho

    numTotalGames = 0;
    while (fgets(linha, MAX_LINHA_LEN, f) != NULL && numTotalGames < MAX_GAMES) {
        Game* g = &todosOsGames[numTotalGames];
        char buffer[MAX_ARRAY_STRING];
        char* b_ptr = buffer;
        int in_quotes = 0;
        int campo = 0;
        
        linha[strcspn(linha, "\n")] = 0; // Remove newline

        for (char* c = linha; *c != '\0'; c++) {
            if (*c == '"') {
                in_quotes = !in_quotes;
            } else if (*c == ',' && !in_quotes) {
                *b_ptr = '\0';
                setGameField(g, campo, trim(buffer));
                campo++;
                b_ptr = buffer;
                memset(buffer, 0, MAX_ARRAY_STRING); // Limpa o buffer
            } else {
                *b_ptr++ = *c;
            }
        }
        *b_ptr = '\0';
        setGameField(g, campo, trim(buffer)); // Define o último campo

        numTotalGames++;
    }

    fclose(f);
}

/**
 * Procura um Game no array global 'todosOsGames' pelo ID.
 * Retorna um ponteiro para o Game se encontrado, ou NULL.
 */
Game* procurarPorCodigo(int id) {
    for (int i = 0; i < numTotalGames; i++) {
        if (todosOsGames[i].id == id) {
            return &todosOsGames[i];
        }
    }
    return NULL;
}




PilhaGames* novaPilha() {
    PilhaGames* p = (PilhaGames*) malloc(sizeof(PilhaGames));
    p->topo = NULL;
    return p;
}

/**
 * Insere (Push) um elemento (Game) no topo da pilha.
 */
void inserir(PilhaGames* p, Game* x) {
    Celula* tmp = (Celula*) malloc(sizeof(Celula));
    tmp->elemento = x; // Armazena o ponteiro para o Game
    tmp->prox = p->topo;
    p->topo = tmp;
}

/**
 * Remove (Pop) um elemento do topo da pilha.
 * Retorna o ponteiro para o Game removido ou NULL se a pilha estiver vazia.
 */
Game* remover(PilhaGames* p) {
    if (p->topo == NULL) {
        printf("Erro ao remover!\n");
        return NULL;
    }

    Celula* tmp = p->topo;
    Game* resp = tmp->elemento;
    p->topo = tmp->prox;
    
    tmp->prox = NULL;
    free(tmp);
    
    return resp;
}


/**
 * Formata e imprime os dados de um único Game.
 */
void printGame(Game* game, int index) {
    
    printf("[%d] => %d ## %s ## %s ## %d ## %.2f ## [%s] ## %d ## %.1f ## %d ## [%s] ## [%s] ## [%s] ## [%s] ## [%s] ##\n",
        index,
        game->id,
        game->name,
        game->releaseDate,
        game->estimatedOwners,
        game->price,
        game->supportedLanguages, 
        game->metacriticScore,
        game->userScore,
        game->achievements,
        game->publishers,
        game->developers,
        game->categories,
        game->genres,
        game->tags
    );
}

void mostrarRecursivo(Celula* i, int* index) {
    if (i != NULL) {
        //  Vai até o fundo da pilha
        mostrarRecursivo(i->prox, index);
        
        //  Imprime na volta (bottom-up)
        printGame(i->elemento, *index);
        (*index)++; // Incrementa o índice
    }
}


void mostrar(PilhaGames* p) {
    int index = 0;
    mostrarRecursivo(p->topo, &index);
}

/**
 * Libera a memória alocada para a pilha (células e a própria pilha).
 * Não libera os elementos (Game*), pois eles pertencem ao array 'todosOsGames'.
 */
void freePilha(PilhaGames* p) {
    Celula* atual = p->topo;
    while (atual != NULL) {
        Celula* tmp = atual;
        atual = atual->prox;
        free(tmp);
    }
    free(p);
}


int main() {
    const char* caminho = "/tmp/games.csv";
    
    // Pré-leitura de todos os jogos
    preencherTodosGames(caminho);

    PilhaGames* pilhaGames = novaPilha();
    char linhaEntrada[100];

    //  Leitura da Primeira Parte (IDs até "FIM")
    while (fgets(linhaEntrada, 100, stdin) != NULL) {
        linhaEntrada[strcspn(linhaEntrada, "\n")] = 0; // Remove newline
        
        if (strcmp(linhaEntrada, "FIM") == 0) break;

        int idNum = atoi(linhaEntrada);
        if (idNum > 0) {
            Game* jogoParaInserir = procurarPorCodigo(idNum);
            if (jogoParaInserir != NULL) {
                inserir(pilhaGames, jogoParaInserir);
            }
        }
    }

    // Lendo o número de operações
    int numOperacoes = 0;
    scanf("%d", &numOperacoes);
    getchar(); // Consome o newline após o número

    // 4. Processamento das operações
    for(int i = 0; i < numOperacoes; i++){
        char linhaOperacao[50];
        fgets(linhaOperacao, 50, stdin);
        
        char operacao[3];
        int idOp;
        
        // Faz o parse da linha de operação
        sscanf(linhaOperacao, "%s %d", operacao, &idOp);

        if (strcmp(operacao, "I") == 0) {
            Game* jogo = procurarPorCodigo(idOp);
            if (jogo != NULL) {
                inserir(pilhaGames, jogo);
            }
        } else if (strcmp(operacao, "R") == 0) {
            Game* removido = remover(pilhaGames);
            if (removido != NULL) {
                printf("(R) %s\n", removido->name);
            }
        }
    }
    
    //  Impressão final
    mostrar(pilhaGames);

    //  Limpeza
    freePilha(pilhaGames);

    return 0;
}