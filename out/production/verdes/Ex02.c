#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_LINE_SIZE 4096
#define MAX_FIELDS 14


typedef struct {
    int id;
    char *name;
    char *release_date;
    int estimated_owners;
    float price;
    char *supported_languages;
    int metacritic_score;
    float user_score;
    int achievements;
    char *publishers;
    char *developers;
    char *categories;
    char *genres;
    char *tags;
} Game;

typedef struct Celula {
    Game *jogo;
    struct Celula *prox;
} Celula;

typedef struct {
    Celula *primeiro;
    Celula *ultimo;
} Lista;



// Remove espaços em branco do início e fim (trim)
char *trim(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = 0;
    return str;
}

// Réplica da função 'separar' do Java: remove [, ], ", ' e formata a lista
char *clean_list(char *raw) {
    if (!raw || strlen(raw) == 0) return strdup("[]");
    
    // Remove caracteres indesejados como no Java .replace
    char buffer[MAX_LINE_SIZE];
    int idx = 0;
    for (int i = 0; raw[i]; i++) {
        if (raw[i] != '[' && raw[i] != ']' && raw[i] != '"' && raw[i] != '\'') {
            buffer[idx++] = raw[i];
        }
    }
    buffer[idx] = '\0';

    // Se ficou vazio
    if (strlen(buffer) == 0) return strdup("[]");

    // Reconstrói no formato de saída esperado [A, B, C]
    char final_str[MAX_LINE_SIZE] = "[";
    char *token = strtok(buffer, ",");
    bool first = true;
    
    while (token != NULL) {
        if (!first) strcat(final_str, ", ");
        strcat(final_str, trim(token));
        first = false;
        token = strtok(NULL, ",");
    }
    strcat(final_str, "]");
    return strdup(final_str);
}

// Converte Mês texto para número 
const char* get_month_num(char *month) {
    if (strncasecmp(month, "Jan", 3) == 0) return "01";
    if (strncasecmp(month, "Feb", 3) == 0) return "02";
    if (strncasecmp(month, "Mar", 3) == 0) return "03";
    if (strncasecmp(month, "Apr", 3) == 0) return "04";
    if (strncasecmp(month, "May", 3) == 0) return "05";
    if (strncasecmp(month, "Jun", 3) == 0) return "06";
    if (strncasecmp(month, "Jul", 3) == 0) return "07";
    if (strncasecmp(month, "Aug", 3) == 0) return "08";
    if (strncasecmp(month, "Sep", 3) == 0) return "09";
    if (strncasecmp(month, "Oct", 3) == 0) return "10";
    if (strncasecmp(month, "Nov", 3) == 0) return "11";
    if (strncasecmp(month, "Dec", 3) == 0) return "12";
    return "00";
}


char *format_date(char *raw) {
    if (!raw || strlen(raw) < 4) return strdup("01/01/0001"); 
    
    char aux[100];
    strcpy(aux, raw);
    
    // Se já for aspas duplas (acontece no split as vezes), limpa
    if(aux[0] == '"') memmove(aux, aux+1, strlen(aux));
    int len = strlen(aux);
    if(aux[len-1] == '"') aux[len-1] = '\0';

    // Formato "MMM dd, yyyy" -> "dd/MM/yyyy"
    char month_str[10], day_str[10], year_str[10];
    
    // Tenta extrair MMM dd, yyyy
    if (strstr(aux, ",") != NULL) {
        // Separa por espaço e remove vírgula
        char *p = strtok(aux, " ,");
        if(p) strcpy(month_str, p);
        
        p = strtok(NULL, " ,");
        if(p) strcpy(day_str, p);
        
        p = strtok(NULL, " ,");
        if(p) strcpy(year_str, p);
        
        char buffer[20];
        sprintf(buffer, "%02d/%s/%s", atoi(day_str), get_month_num(month_str), year_str);
        return strdup(buffer);
    } 
    else if (strlen(aux) == 4) {
         // Apenas ano
         char buffer[20];
         sprintf(buffer, "01/01/%s", aux);
         return strdup(buffer);
    }
    
    return strdup("01/01/0001"); // Fallback
}




int split_csv_java_style(char *linha, char **campos) {
    int field_count = 0;
    char current_field[MAX_LINE_SIZE];
    int cursor = 0;
    bool aspas = false;
    
    for (int i = 0; linha[i] != '\0' && linha[i] != '\n' && linha[i] != '\r'; i++) {
        char c = linha[i];
        
        if (c == '"') {
            aspas = !aspas; // Alterna estado das aspas
        } else if (c == ',' && !aspas) {
            // Separador encontrado fora de aspas: finaliza campo atual
            current_field[cursor] = '\0';
            campos[field_count++] = strdup(current_field);
            cursor = 0;
        } else {
            current_field[cursor++] = c;
        }
    }
    // Adiciona o último campo
    current_field[cursor] = '\0';
    campos[field_count++] = strdup(current_field);
    
    return field_count;
}

Game criar_game(char **dados) {
    Game g;
    
    // Conversões com lógica similar ao try-catch do Java (atoi/atof retornam 0 se falhar)
    g.id = atoi(dados[0]);
    g.name = strdup(trim(dados[1]));
    g.release_date = format_date(dados[2]);
    g.estimated_owners = atoi(dados[3]);
    g.price = atof(dados[4]);
    g.supported_languages = clean_list(dados[5]);
    g.metacritic_score = atoi(dados[6]);
    g.user_score = atof(dados[7]);
    g.achievements = atoi(dados[8]);
    g.publishers = clean_list(dados[9]);
    g.developers = clean_list(dados[10]);
    g.categories = clean_list(dados[11]);
    g.genres = clean_list(dados[12]);
    g.tags = clean_list(dados[13]);
    
    return g;
}



Lista* iniciar_lista() {
    Lista *l = (Lista*)malloc(sizeof(Lista));
    // Cabeça sentinela para facilitar inserção/remoção 
    l->primeiro = (Celula*)malloc(sizeof(Celula));
    l->primeiro->prox = NULL;
    l->ultimo = l->primeiro;
    return l;
}

void inserir_fim(Lista *l, Game *g) {
    l->ultimo->prox = (Celula*)malloc(sizeof(Celula));
    l->ultimo = l->ultimo->prox;
    l->ultimo->jogo = g;
    l->ultimo->prox = NULL;
}

void inserir_inicio(Lista *l, Game *g) {
    Celula *tmp = (Celula*)malloc(sizeof(Celula));
    tmp->jogo = g;
    tmp->prox = l->primeiro->prox;
    l->primeiro->prox = tmp;
    if (l->primeiro == l->ultimo) {
        l->ultimo = tmp;
    }
}

void inserir_posicao(Lista *l, Game *g, int pos) {
    Celula *i = l->primeiro;
    for(int j = 0; j < pos && i->prox != NULL; j++) {
        i = i->prox;
    }
    Celula *tmp = (Celula*)malloc(sizeof(Celula));
    tmp->jogo = g;
    tmp->prox = i->prox;
    i->prox = tmp;
    if (i == l->ultimo) {
        l->ultimo = tmp;
    }
}

Game* remover_inicio(Lista *l) {
    if (l->primeiro == l->ultimo) return NULL;
    Celula *tmp = l->primeiro->prox;
    l->primeiro->prox = tmp->prox;
    Game *resp = tmp->jogo;
    if (tmp == l->ultimo) l->ultimo = l->primeiro;
    free(tmp);
    return resp;
}

Game* remover_fim(Lista *l) {
    if (l->primeiro == l->ultimo) return NULL;
    Celula *i;
    for(i = l->primeiro; i->prox != l->ultimo; i = i->prox);
    Game *resp = l->ultimo->jogo;
    l->ultimo = i;
    free(i->prox);
    l->ultimo->prox = NULL;
    return resp;
}

Game* remover_posicao(Lista *l, int pos) {
    if (l->primeiro == l->ultimo) return NULL;
    Celula *i = l->primeiro;
    for(int j = 0; j < pos && i->prox != NULL; j++) i = i->prox;
    if(i->prox == NULL) return NULL;
    
    Celula *tmp = i->prox;
    Game *resp = tmp->jogo;
    i->prox = tmp->prox;
    if (tmp == l->ultimo) l->ultimo = i;
    free(tmp);
    return resp;
}

void imprimir_game(Game *g) {
    printf("%d ## %s ## %s ## %d ## %.2f ## %s ## %d ## %.1f ## %d ## %s ## %s ## %s ## %s ## %s ##\n",
        g->id, g->name, g->release_date, g->estimated_owners, g->price,
        g->supported_languages, g->metacritic_score, g->user_score, g->achievements,
        g->publishers, g->developers, g->categories, g->genres, g->tags);
}

void mostrar_lista(Lista *l) {
    Celula *i = l->primeiro->prox;
    int index = 0;
    while (i != NULL) {
        printf("[%d] => ", index++);
        imprimir_game(i->jogo);
        i = i->prox;
    }
}



int main() {
    FILE *fp = fopen("/tmp/games.csv", "r");
    if (!fp) { printf("Erro ao abrir arquivo.\n"); return 1; }

    //  Pré-leitura do CSV para memória (ArrayList do Java -> Vetor em C)
    Game *todos_jogos = NULL;
    int total_jogos = 0;
    char linha[MAX_LINE_SIZE];

    // Pula cabeçalho
    fgets(linha, MAX_LINE_SIZE, fp);

    while (fgets(linha, MAX_LINE_SIZE, fp)) {
        char *campos[MAX_FIELDS];
        // Usa a lógica do splitCsvAspas traduzida
        if (split_csv_java_style(linha, campos) >= MAX_FIELDS) {
            todos_jogos = realloc(todos_jogos, (total_jogos + 1) * sizeof(Game));
            todos_jogos[total_jogos] = criar_game(campos);
            total_jogos++;
            
            // Limpa memória dos campos temporários
            for(int k=0; k<MAX_FIELDS; k++) free(campos[k]);
        }
    }
    fclose(fp);

    Lista *lista = iniciar_lista();
    char input[100];

    //  Leitura inicial de IDs até "FIM"
    while (scanf("%s", input) && strcmp(input, "FIM") != 0) {
        int id = atoi(input);
        for (int i = 0; i < total_jogos; i++) {
            if (todos_jogos[i].id == id) {
                inserir_fim(lista, &todos_jogos[i]);
                break;
            }
        }
    }

    //  Processamento de Operações 
    int qtd_ops;
    scanf("%d", &qtd_ops);
    
    for (int k = 0; k < qtd_ops; k++) {
        char comando[5];
        scanf("%s", comando);
        
        if (strcmp(comando, "II") == 0) {
            int id; scanf("%d", &id);
            for (int i=0; i<total_jogos; i++) 
                if (todos_jogos[i].id == id) inserir_inicio(lista, &todos_jogos[i]);
        }
        else if (strcmp(comando, "IF") == 0) {
            int id; scanf("%d", &id);
            for (int i=0; i<total_jogos; i++) 
                if (todos_jogos[i].id == id) inserir_fim(lista, &todos_jogos[i]);
        }
        else if (strcmp(comando, "I*") == 0) {
            int pos, id; scanf("%d %d", &pos, &id);
            for (int i=0; i<total_jogos; i++) 
                if (todos_jogos[i].id == id) inserir_posicao(lista, &todos_jogos[i], pos);
        }
        else if (strcmp(comando, "RI") == 0) {
            Game *g = remover_inicio(lista);
            if(g) printf("(R) %s\n", g->name);
        }
        else if (strcmp(comando, "RF") == 0) {
            Game *g = remover_fim(lista);
            if(g) printf("(R) %s\n", g->name);
        }
        else if (strcmp(comando, "R*") == 0) {
            int pos; scanf("%d", &pos);
            Game *g = remover_posicao(lista, pos);
            if(g) printf("(R) %s\n", g->name);
        }
    }

    mostrar_lista(lista);

    
    free(todos_jogos); // (idealmente dar free nas strings internas também)
    return 0;
}