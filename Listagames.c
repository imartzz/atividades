#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>

#define MAX_LINE 1024
#define MAX_FIELD 256
#define MAX_ARRAY_STRING 512
#define MAX_ENTRADAS 1000



typedef struct Game {
    int id;
    char name[MAX_FIELD];
    char releaseDate[11]; // DD/MM/YYYY + \0
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



// Estrutura do nó (Célula)
typedef struct Celula {
    Game* elemento;
    struct Celula* prox;
} Celula;

// Construtor da Célula
Celula* novaCelula(Game* elemento) {
    Celula* nova = (Celula*)malloc(sizeof(Celula));
    if (nova == NULL) {
        perror("Erro ao alocar nova Celula");
        exit(1);
    }
    nova->elemento = elemento;
    nova->prox = NULL;
    return nova;
}

// Estrutura da Lista (Lista Simplesmente Encadeada)
typedef struct ListaGames {
    Celula* primeiro;
    Celula* ultimo;
    int tamanho;
} ListaGames;

// Construtor da Lista
void iniciarLista(ListaGames* lista) {
    lista->primeiro = novaCelula(NULL); // Célula cabeça (sentinela)
    lista->ultimo = lista->primeiro;
    lista->tamanho = 0;
}

// Destrutor da Lista (libera a memória de todas as células)
void destruirLista(ListaGames* lista) {
    Celula* i = lista->primeiro;
    while (i != NULL) {
        Celula* tmp = i;
        
        // A linha "free(tmp->elemento);" FOI REMOVIDA para evitar o double-free.
        
        i = i->prox;
        free(tmp); // Libera apenas a Celula
    }
    lista->primeiro = lista->ultimo = NULL;
}



/**
 * Funcao para tratar caracteres especiais nas strings de array
 */
void cleanArrayString(char *s) {
    int i, j = 0;
    char temp[MAX_ARRAY_STRING];
    bool inQuotes = false;

    // Remove aspas duplas, colchetes e aspas simples
    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] == '"') {
            inQuotes = !inQuotes;
        } else if (s[i] == '[' || s[i] == ']' || s[i] == '\'') {
            continue;
        } else {
            temp[j++] = s[i];
        }
    }
    temp[j] = '\0';
    strcpy(s, temp);
}

/**
 * Função de tratamento de data (simplificada)
 */
void tratarData(char *data) {
    if (strlen(data) == 0) {
        strcpy(data, "01/01/0001");
        return;
    }
    
    // Tenta encontrar o ano. Se só tiver o ano, assume 01/01/AAAA
    char *ano_ptr = data;
    while (*ano_ptr) {
        if (*ano_ptr >= '0' && *ano_ptr <= '9') {
            if (strlen(ano_ptr) >= 4 && ano_ptr[4] == '\0') {
                char temp[11];
                sprintf(temp, "01/01/%s", ano_ptr);
                strcpy(data, temp);
                return;
            }
        }
        ano_ptr++;
    }

    // Para o formato "MMM D, YYYY" (Inglês)
    char mesStr[4];
    int dia, ano;
    
    // Tenta parsear "MMM D, YYYY"
    if (sscanf(data, "%3s %d, %d", mesStr, &dia, &ano) == 3) {
        int mes = 0;
        if (strcmp(mesStr, "Jan") == 0) mes = 1; else if (strcmp(mesStr, "Feb") == 0) mes = 2;
        else if (strcmp(mesStr, "Mar") == 0) mes = 3; else if (strcmp(mesStr, "Apr") == 0) mes = 4;
        else if (strcmp(mesStr, "May") == 0) mes = 5; else if (strcmp(mesStr, "Jun") == 0) mes = 6;
        else if (strcmp(mesStr, "Jul") == 0) mes = 7; else if (strcmp(mesStr, "Aug") == 0) mes = 8;
        else if (strcmp(mesStr, "Sep") == 0) mes = 9; else if (strcmp(mesStr, "Oct") == 0) mes = 10;
        else if (strcmp(mesStr, "Nov") == 0) mes = 11; else if (strcmp(mesStr, "Dec") == 0) mes = 12;

        if (mes > 0) {
            char temp[11];
            sprintf(temp, "%02d/%02d/%d", dia, mes, ano);
            strcpy(data, temp);
            return;
        }
    }
}

/**
 * Função de parseamento de linha CSV que respeita aspas.
 */
void splitCsvAspas(char *linha, char campos[][MAX_ARRAY_STRING]) {
    int i = 0, j = 0, k = 0;
    bool inQuotes = false;
    
    while (linha[i] != '\0' && j < 14) {
        k = 0;
        
        while (linha[i] != '\0') {
            if (linha[i] == '"') {
                inQuotes = !inQuotes;
                i++;
            } else if (linha[i] == ',' && !inQuotes) {
                i++; // Pula a vírgula
                break;
            } else {
                if (k < MAX_ARRAY_STRING - 1) {
                    campos[j][k++] = linha[i];
                }
                i++;
            }
        }
        campos[j][k] = '\0';
        // Limpar espaços e chars especiais após o parse 
        cleanArrayString(campos[j]);
        j++;
    }
}

/**
 * Função que substitui ',' por " ## " na string, para formatação da saída.
 */
void replaceCharWithStr(char* str, char target, const char* replacement) {
    char temp[MAX_ARRAY_STRING * 2]; // Buffer temporário
    int i = 0, j = 0;
    
    while (str[i] != '\0' && j < MAX_ARRAY_STRING * 2 - 1) {
        if (str[i] == target) {
            // Se encontrar a vírgula, insere a substituição " ## "
            int k = 0;
            while (replacement[k] != '\0' && j < MAX_ARRAY_STRING * 2 - 1) {
                temp[j++] = replacement[k++];
            }
            i++;
        } else {
            temp[j++] = str[i++];
        }
    }
    temp[j] = '\0';
    strcpy(str, temp);
}

/**
 * Imprime um Game
 */
void printGame(Game* game, int index) {
    // Buffers temporários para formatar as strings de lista
    char lang[MAX_ARRAY_STRING * 2], pub[MAX_ARRAY_STRING * 2], dev[MAX_ARRAY_STRING * 2];
    char cat[MAX_ARRAY_STRING * 2], gen[MAX_ARRAY_STRING * 2], tag[MAX_ARRAY_STRING * 2];

    // Copia e formata as strings de array: substituir ',' por " ## "
    strcpy(lang, game->supportedLanguages);
    replaceCharWithStr(lang, ',', " ## ");
    
    strcpy(pub, game->publishers);
    replaceCharWithStr(pub, ',', " ## ");

    strcpy(dev, game->developers);
    replaceCharWithStr(dev, ',', " ## ");
    
    strcpy(cat, game->categories);
    replaceCharWithStr(cat, ',', " ## ");
    
    strcpy(gen, game->genres);
    replaceCharWithStr(gen, ',', " ## ");
    
    strcpy(tag, game->tags);
    replaceCharWithStr(tag, ',', " ## ");

    // Imprimir usando as strings temporárias formatadas
    printf("[%d] => %d ## %s ## %s ## %d ## %.2f ## [%s] ## %d ## %.1f ## %d ## [%s] ## [%s] ## [%s] ## [%s] ## [%s] ##\n",
        index,
        game->id,
        game->name,
        game->releaseDate,
        game->estimatedOwners,
        game->price,
        lang, 
        game->metacriticScore,
        game->userScore,
        game->achievements,
        pub,  
        dev,  
        cat,  
        gen,  
        tag   
    );
}

/**
 * Aloca memória para um novo Game e preenche seus campos.
 */
Game* criarGame(char campos[][MAX_ARRAY_STRING]) {
    Game* novo = (Game*)malloc(sizeof(Game));
    if (novo == NULL) {
        perror("Erro ao alocar novo Game");
        return NULL;
    }

    // Copia e converte os dados
    novo->id = atoi(campos[0]);
    strcpy(novo->name, campos[1]);
    tratarData(campos[2]);
    strcpy(novo->releaseDate, campos[2]);
    novo->estimatedOwners = atoi(campos[3]);
    // Trata "Free to Play"
    if (strcmp(campos[4], "Free to Play") == 0) {
        novo->price = 0.0f;
    } else {
        novo->price = atof(campos[4]);
    }
    strcpy(novo->supportedLanguages, campos[5]);
    novo->metacriticScore = atoi(campos[6]);
    // Trata userScore (pode vir como string vazia ou float)
    novo->userScore = (strlen(campos[7]) > 0) ? atof(campos[7]) : 0.0f;
    novo->achievements = atoi(campos[8]);
    strcpy(novo->publishers, campos[9]);
    strcpy(novo->developers, campos[10]);
    strcpy(novo->categories, campos[11]);
    strcpy(novo->genres, campos[12]);
    strcpy(novo->tags, campos[13]);

    return novo;
}

/**
 * Lê o arquivo CSV e retorna um array de ponteiros para Game
 */
Game** preencherGames(const char *caminho, int *totalGames) {
    FILE *file = fopen(caminho, "r");
    if (file == NULL) {
        perror("Erro ao abrir arquivo CSV");
        return NULL;
    }

    Game** todosOsGames = (Game**)malloc(sizeof(Game*) * MAX_ENTRADAS);
    if (todosOsGames == NULL) {
        perror("Erro ao alocar array de Games");
        fclose(file);
        return NULL;
    }

    char linha[MAX_LINE];
    char campos[14][MAX_ARRAY_STRING];
    *totalGames = 0;

    // Pula o cabeçalho
    if (fgets(linha, MAX_LINE, file) == NULL) {
        fclose(file);
        return NULL;
    }

    while (fgets(linha, MAX_LINE, file) != NULL && *totalGames < MAX_ENTRADAS) {
        if (linha[0] == '\n' || linha[0] == '\r') continue;
        
        // Limpa a quebra de linha
        linha[strcspn(linha, "\n\r")] = 0;

        splitCsvAspas(linha, campos);
        Game* novoGame = criarGame(campos);
        if (novoGame != NULL) {
            todosOsGames[*totalGames] = novoGame;
            (*totalGames)++;
        }
    }

    fclose(file);
    return todosOsGames;
}

/**
 * Procura um Game no array de Games lidos pelo ID
 */
Game* ProcurarPorCodigo(Game** todosOsGames, int totalGames, int id) {
    for (int i = 0; i < totalGames; i++) {
        if (todosOsGames[i] != NULL && todosOsGames[i]->id == id) {
            return todosOsGames[i];
        }
    }
    return NULL;
}


/**
 * Insere um elemento no início da lista.
 */
void inserirInicio(ListaGames* lista, Game* x) {
    Celula* nova = novaCelula(x);
    nova->prox = lista->primeiro->prox;
    lista->primeiro->prox = nova;
    if (lista->primeiro == lista->ultimo) {
        lista->ultimo = nova;
    }
    lista->tamanho++;
}

/**
 * Insere um elemento no fim da lista.
 */
void inserirFim(ListaGames* lista, Game* x) {
    lista->ultimo->prox = novaCelula(x);
    lista->ultimo = lista->ultimo->prox;
    lista->tamanho++;
}

/**
 * Insere um elemento em uma posição específica (0 <= pos <= tamanho).
 */
void inserir(ListaGames* lista, Game* x, int pos) {
    if (pos < 0 || pos > lista->tamanho) {
        printf("Erro ao inserir: Posicao invalida!\n");
    } else if (pos == 0) {
        inserirInicio(lista, x);
    } else if (pos == lista->tamanho) {
        inserirFim(lista, x);
    } else {
        Celula* i = lista->primeiro;
        for (int j = 0; j < pos; j++) {
            i = i->prox;
        }
        Celula* nova = novaCelula(x);
        nova->prox = i->prox;
        i->prox = nova;
        lista->tamanho++;
    }
}

/**
 * Remove o primeiro elemento da lista.
 * Retorna o Game removido.
 */
Game* removerInicio(ListaGames* lista) {
    if (lista->primeiro == lista->ultimo) {
        printf("Erro ao remover: Lista vazia!\n");
        return NULL;
    }
    
    Celula* tmp = lista->primeiro->prox;
    lista->primeiro->prox = tmp->prox;
    
    if (tmp == lista->ultimo) {
        lista->ultimo = lista->primeiro;
    }
    
    Game* resp = tmp->elemento;
    free(tmp);
    lista->tamanho--;
    return resp;
}

/**
 * Remove o último elemento da lista.
 * Retorna o Game removido.
 */
Game* removerFim(ListaGames* lista) {
    if (lista->primeiro == lista->ultimo) {
        printf("Erro ao remover: Lista vazia!\n");
        return NULL;
    }
    
    // Encontra o penúltimo
    Celula* i = lista->primeiro;
    while (i->prox != lista->ultimo) {
        i = i->prox;
    }
    
    Celula* tmp = lista->ultimo;
    Game* resp = tmp->elemento;
    lista->ultimo = i;
    lista->ultimo->prox = NULL;
    
    free(tmp);
    lista->tamanho--;
    return resp;
}

/**
 * Remove um elemento em uma posição específica (0 <= pos < tamanho).
 * Retorna o Game removido.
 */
Game* remover(ListaGames* lista, int pos) {
    Game* resp = NULL;

    if (lista->primeiro == lista->ultimo || pos < 0 || pos >= lista->tamanho) {
        printf("Erro ao remover: Posicao invalida ou Lista vazia!\n");
    } else if (pos == 0) {
        resp = removerInicio(lista);
    } else if (pos == lista->tamanho - 1) {
        resp = removerFim(lista);
    } else {
        Celula* i = lista->primeiro;
        for (int j = 0; j < pos; j++) {
            i = i->prox;
        }
        Celula* tmp = i->prox;
        i->prox = tmp->prox;
        resp = tmp->elemento;
        free(tmp);
        lista->tamanho--;
    }
    return resp;
}

/**
 * Imprime todos os elementos da lista.
 */
void mostrarLista(ListaGames* lista) {
    int index = 0;
    for (Celula* i = lista->primeiro->prox; i != NULL; i = i->prox) {
        printGame(i->elemento, index++);
    }
}



int main() {
    setlocale(LC_ALL, "Portuguese");
    
    //  Leitura Inicial dos Games do CSV
    const char *caminho = "/tmp/games.csv";
    int totalGames = 0;
    Game** todosOsGames = preencherGames(caminho, &totalGames);

    if (todosOsGames == NULL || totalGames == 0) {
        return 1;
    }

    // Leitura dos IDs a serem incluídos na lista
    ListaGames lista;
    iniciarLista(&lista);
    int id;

    // Lendo IDs até o EOF ou "FIM" (se o "FIM" for lido como um int inválido)
    while (scanf("%d", &id) == 1) {
        if (id > 0) {
            Game* jogoParaInserir = ProcurarPorCodigo(todosOsGames, totalGames, id);
            if (jogoParaInserir != NULL) {
                // A lista armazena a referência (ponteiro)
                inserirFim(&lista, jogoParaInserir); 
            }
        }
    }
    
    // Consome o restante da linha e o "FIM"
    while (getchar() != '\n' && getchar() != EOF); 

    //  Processamento das Operações
    int numOperacoes;
    if (scanf("%d", &numOperacoes) != 1) {
        numOperacoes = 0;
    }
    
    // Consome o restante da linha
    while (getchar() != '\n' && getchar() != EOF); 

    char linhaOperacao[MAX_FIELD];
    char operacao[5];
    int idOp, posOp;
    Game* jogoOp;
    Game* removido;

    for (int i = 0; i < numOperacoes; i++) {
        if (fgets(linhaOperacao, MAX_FIELD, stdin) == NULL) break;
        
        // Remove a quebra de linha
        linhaOperacao[strcspn(linhaOperacao, "\n\r")] = 0;

        // Tenta ler a operação e os parâmetros
        idOp = -1; posOp = -1; // Reset
        if (sscanf(linhaOperacao, "%4s %d %d", operacao, &posOp, &idOp) == 3) {
            // Operação I* POS ID
            // Variaveis já lidas
        } else if (sscanf(linhaOperacao, "%4s %d", operacao, &idOp) == 2) {
            // Operação II ID, IF ID, ou R* POS
            // O 'idOp' é o segundo parametro lido (ID ou POS)
        } else if (sscanf(linhaOperacao, "%4s", operacao) == 1) {
            // Operação RI, RF
            // Nenhuma variável lida
        }

        if (strcmp(operacao, "II") == 0) {
            id = idOp;
            jogoOp = ProcurarPorCodigo(todosOsGames, totalGames, id);
            if (jogoOp) inserirInicio(&lista, jogoOp);
        } else if (strcmp(operacao, "I*") == 0) {
            // Re-parse para garantir ordem: I* POS ID
            if (sscanf(linhaOperacao, "%4s %d %d", operacao, &posOp, &idOp) == 3) {
                jogoOp = ProcurarPorCodigo(todosOsGames, totalGames, idOp);
                if (jogoOp) inserir(&lista, jogoOp, posOp);
            }
        } else if (strcmp(operacao, "IF") == 0) {
            id = idOp;
            jogoOp = ProcurarPorCodigo(todosOsGames, totalGames, id);
            if (jogoOp) inserirFim(&lista, jogoOp);
        } else if (strcmp(operacao, "RI") == 0) {
            removido = removerInicio(&lista);
            if (removido) printf("(R) %s\n", removido->name);
        } else if (strcmp(operacao, "R*") == 0) {
            posOp = idOp; // O único parâmetro lido é a posição
            removido = remover(&lista, posOp);
            if (removido) printf("(R) %s\n", removido->name);
        } else if (strcmp(operacao, "RF") == 0) {
            removido = removerFim(&lista);
            if (removido) printf("(R) %s\n", removido->name);
        }
    }

    //  Impressão da Lista Final
    mostrarLista(&lista);

    //  Limpeza de memória (Libera os Games e depois as células)
    for(int i = 0; i < totalGames; i++) {
        free(todosOsGames[i]);
    }
    free(todosOsGames);
    
    // Libera as células.
    destruirLista(&lista);

    return 0;
}