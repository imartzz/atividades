#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Definições globais
#define TAM_NOME 256
#define TAM_LINHA 2048
#define TAM_TABELA 21 
#define MATRICULA "888678"
#define NOME_ARQUIVO_CSV "/tmp/games.csv"

// Variáveis globais para log
long comparacoes = 0;




// Estrutura Game 
typedef struct Game {
    int id;
    char name[TAM_NOME];
} Game;

// Nó da Lista Simples
typedef struct No {
    Game *elemento;
    struct No *prox;
} No;

// Tabela Hash Indireta 
typedef struct {
    No *tabela[TAM_TABELA];
} TabelaHash;

// FUNÇÕES AUXILIARES DE STRING E PARSING CSV

// Função para remover espaços em branco no início e fim de uma string
char *trim(char *s) {
    if (!s) return NULL;
    // Trim do fim
    int i = strlen(s) - 1;
    while (i >= 0 && isspace((unsigned char)s[i])) {
        s[i--] = '\0';
    }
    // Trim do início
    while (*s && isspace((unsigned char)*s)) {
        s++;
    }
    return s;
}

/**
 * Função robusta para extrair um campo de uma linha CSV, 
 * tratando aspas e vírgulas internas.
 * NOTA: Esta função duplica a linha de entrada, o caller deve liberar o resultado.
 */
char* get_field_by_index(const char *linha, int index) {
    char *linha_copy = strdup(linha);
    if (linha_copy == NULL) return NULL;

    char *result = NULL;
    char *token_start = linha_copy;
    int aspas = 0;
    int campo_atual = 0;
    char *cursor = linha_copy;
    
    for (; *cursor != '\0'; cursor++) {
        if (*cursor == '"') {
            aspas = !aspas;
        } else if (*cursor == ',' && !aspas) {
            if (campo_atual == index) {
                *cursor = '\0'; // Termina a string do campo
                result = strdup(trim(token_start));
                goto cleanup;
            }
            campo_atual++;
            token_start = cursor + 1;
        }
    }
    
    // Trata o último campo
    if (campo_atual == index) {
        result = strdup(trim(token_start));
    }
    
cleanup:
    free(linha_copy);
    return result;
}


// Função hash: (ASCII) mod tamTab
int hash(const char *nome) {
    long soma = 0;
    for (int i = 0; nome[i] != '\0'; i++) {
        soma += (long)nome[i];
    }
    return (int)(soma % TAM_TABELA);
}

// FUNÇÕES DA TABELA HASH

// Inicializa a tabela
void inicializar_hash(TabelaHash *hash) {
    for (int i = 0; i < TAM_TABELA; i++) {
        hash->tabela[i] = NULL;
    }
}

// Insere um novo Game na tabela (no início da lista)
void inserir_hash(TabelaHash *hash_table, Game *g) {
    if (g == NULL) return;

    int pos = hash(g->name);

    // Aloca e preenche o novo nó
    No *novo_no = (No *)malloc(sizeof(No));
    if (novo_no == NULL) {
        perror("Erro ao alocar novo no");
        return;
    }
    
    // O nó aponta para o objeto Game já alocado
    novo_no->elemento = g; 

    // Insere no início da lista (encadeamento separado)
    novo_no->prox = hash_table->tabela[pos];
    hash_table->tabela[pos] = novo_no;
}

/**
 * Pesquisa um nome na tabela Hash.
 * @param hash_table A tabela hash.
 * @param nome O nome a ser pesquisado.
 * @return A posição (bucket/índice 0-20) se encontrado, ou -1 se não encontrado.
 */
int pesquisar_hash(TabelaHash *hash_table, const char *nome) {
    int pos = hash(nome);
    No *atual = hash_table->tabela[pos];

    // Percorre a lista simples (bucket) na posição hash
    while (atual != NULL) {
        comparacoes++; // Conta a comparação da chave
        if (strcmp(atual->elemento->name, nome) == 0) {
            return pos; // Encontrou, retorna o índice do bucket
        }
        atual = atual->prox;
    }

    return -1; // Não encontrou
}

// Libera a memória da tabela hash
void liberar_hash(TabelaHash *hash_table) {
    for (int i = 0; i < TAM_TABELA; i++) {
        No *atual = hash_table->tabela[i];
        while (atual != NULL) {
            No *proximo = atual->prox;
            // Libera apenas os nós, os Game* serão liberados separadamente
            free(atual);
            atual = proximo;
        }
    }
}

// MAIN

int main() {
    TabelaHash tabela;
    inicializar_hash(&tabela);

    // Array dinâmico para armazenar todos os Games lidos
    Game **todos_os_jogos = NULL;
    int num_jogos = 0;

    //  Leitura do arquivo 
    FILE *csv_file = fopen(NOME_ARQUIVO_CSV, "r");
    if (csv_file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo CSV: %s\n", NOME_ARQUIVO_CSV);
        return 1;
    }

    char linha[TAM_LINHA];
    if (fgets(linha, TAM_LINHA, csv_file) == NULL) { // Pula cabeçalho
        fclose(csv_file);
        return 1;
    }

    while (fgets(linha, TAM_LINHA, csv_file) != NULL) {
        char *id_str = get_field_by_index(linha, 0); // ID é o campo 0
        char *name_str = get_field_by_index(linha, 1); // NAME é o campo 1

        if (id_str != NULL && name_str != NULL) {
            Game *g = (Game *)malloc(sizeof(Game));
            if (g != NULL) {
                g->id = atoi(id_str);
                // Copia o nome, garantindo o fim de string
                strncpy(g->name, name_str, TAM_NOME - 1);
                g->name[TAM_NOME - 1] = '\0';

                // Adiciona ao array dinâmico
                todos_os_jogos = (Game **)realloc(todos_os_jogos, (num_jogos + 1) * sizeof(Game *));
                if (todos_os_jogos == NULL) {
                    perror("Erro ao realocar memoria para jogos");
                    free(g);
                    break;
                }
                todos_os_jogos[num_jogos++] = g;
            }
        }
        free(id_str);
        free(name_str);
    }
    fclose(csv_file);

    //  Leitura dos IDs para inserção
    char id_entrada[10];
    while (scanf("%s", id_entrada) == 1 && strcmp(id_entrada, "FIM") != 0) {
        int id_num = atoi(id_entrada);
        for (int i = 0; i < num_jogos; i++) {
            if (todos_os_jogos[i]->id == id_num) {
                inserir_hash(&tabela, todos_os_jogos[i]);
                break;
            }
        }
    }

    // Limpa o buffer de entrada
    while (getchar() != '\n' && !feof(stdin));
    
    // Inicia a contagem de tempo para a pesquisa
    clock_t start_time = clock();
    
    //  Leitura dos Nomes para pesquisa e Saída Formatada
    char nome_pesquisa[TAM_NOME];
    while (fgets(nome_pesquisa, TAM_NOME, stdin) != NULL) {
        // Remove quebra de linha do fgets
        nome_pesquisa[strcspn(nome_pesquisa, "\n")] = 0;
        
        char *nome_limpo = trim(nome_pesquisa);

        if (strcmp(nome_limpo, "FIM") == 0) break;
        if (strlen(nome_limpo) == 0) continue; 

        int pos_encontrada = pesquisar_hash(&tabela, nome_limpo);
        
        // A posição de saída é sempre o índice do bucket (hash)
        int pos_output = hash(nome_limpo); 
        const char *resultado = (pos_encontrada != -1) ? "SIM" : "NAO";

        // Saída formatada: Nome: (Posicao: X) Resultado
        printf("%s: (Posicao: %d) %s\n", nome_limpo, pos_output, resultado);
    }
    
    // Para capturar o tempo de execução da fase de pesquisa
    clock_t end_time = clock();
    double tempo_execucao = (double)(end_time - start_time) / CLOCKS_PER_SEC; 

    //  Gravação do Log
    char nome_arquivo_log[50];
    sprintf(nome_arquivo_log, "%s_hashIndireta.txt", MATRICULA);
    
    FILE *log_file = fopen(nome_arquivo_log, "w");
    if (log_file != NULL) {
        // Matrícula <tab> Tempo <tab> Comparacoes
        fprintf(log_file, "%s\t%.4f\t%ld\n", MATRICULA, tempo_execucao, comparacoes);
        fclose(log_file);
    } else {
        fprintf(stderr, "Erro ao criar arquivo de log: %s\n", nome_arquivo_log);
    }

    //  Liberação de memória
    for (int i = 0; i < num_jogos; i++) {
        free(todos_os_jogos[i]);
    }
    free(todos_os_jogos);
    liberar_hash(&tabela);

    return 0;
}