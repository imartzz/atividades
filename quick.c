#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>


#define MAX_LINHA 4096
#define MATRICULA "888678"


// metricas[0] = Comparações, metricas[1] = Movimentações
int metricas[2] = {0, 0}; 

typedef struct Game {
    int id;
    char *name;
    char *releaseDate;
    int estimatedOwners;
    float price;
    char **supportedLanguages;
    int numLanguages;
    int metacriticScore;
    float userScore;
    int achievements;
    char **publishers;
    int numPublishers;
    char **developers;
    int numDevelopers;
    char **categories;
    int numCategories;
    char **genres;
    int numGenres;
    char **tags;
    int numTags;
} Game;

typedef struct Data {
    int dia;
    int mes;
    int ano;
} Data;


char* duplicarString(const char*);
char* removerAspas(char*);
int tentarParseInt(char*);
float tentarParseFloat(char*);
char** splitCsvAspas(const char*, int*);
char** parseArray(char*, int*);
Game create_Game(int, char*, char*, int, float, char**, int, int, float, int, char**, int, char**, int, char**, int, char**, int, char**, int);
void liberarGame(Game*);
int obterTamanho(const char*);
char* tratarData(const char*);
int obterMes(const char*);
Game *preencherGames(const char*, int*);
Game* criarVetor (Game*, int, int*);
Game* criarVetorGames (Game*, int[], int, int);
void ordenarGames (Game*, int);
void quicksort (Game*, Data[], int, int);
void swapGames (Game*, int, int);
void swapDatas(Data[], int, int);
Data obterData (char*);
int dataCompare(Data, Data, int, int);
void imprimirResultado(Game*, int);
void imprimirVetor(Game*, int);
char* formatArray(char**, int, bool);
void criarArquivo(char*, double);




char* duplicarString(const char* s) {
    if (!s) return NULL;
    char* result = (char*)malloc(strlen(s) + 1);
    if (result) strcpy(result, s);
    return result;
}

char* removerAspas(char* s) {
    if (!s || !*s) return s;
    // Remove espaços iniciais e finais
    while(isspace((unsigned char)*s)) s++;
    size_t len = strlen(s);
    while(len > 0 && isspace((unsigned char)s[len-1])) s[--len] = '\0';
    
    // Remove aspas ou apóstrofos
    if (len >= 2 && ((s[0] == '"' && s[len-1] == '"') || (s[0] == '\'' && s[len-1] == '\''))) {
        s[len-1] = '\0';
        return s + 1;
    }
    return s;
}

int tentarParseInt(char* s) {
    if (!s || !*s) return 0;
    char* clean_s = (char*)malloc(strlen(s) + 1);
    if (!clean_s) return 0;

    int j = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        if (isdigit((unsigned char)s[i])) {
            clean_s[j++] = s[i];
        }
    }
    clean_s[j] = '\0';
    
    int result = 0;
    if (strlen(clean_s) > 0) {
        result = atoi(clean_s);
    }
    free(clean_s);
    return result;
}

float tentarParseFloat(char* s) {
    if (!s || !*s) return 0.0f;
    s = removerAspas(s);
    if (strcasecmp(s, "Free to Play") == 0 || strcasecmp(s, "Free") == 0) return 0.0f;
    
    char buffer[MAX_LINHA] = {0};
    int j = 0;
    bool decimal_found = false;

    for (int i = 0; s[i] != '\0' && j < MAX_LINHA - 1; i++) {
        if (isdigit((unsigned char)s[i])) {
            buffer[j++] = s[i];
        } else if ((s[i] == ',' || s[i] == '.') && !decimal_found) {
            buffer[j++] = '.';
            decimal_found = true;
        }
    }
    buffer[j] = '\0';
    
    if (strlen(buffer) == 0) return 0.0f;
    
    return strtof(buffer, NULL);
}

char** splitCsvAspas(const char* linha, int* numPartes) {
    char** tempArray = (char**)calloc(50, sizeof(char*));
    int count = 0;
    int inicio = 0;
    bool emAspas = false;
    
    char* tempLinha = duplicarString(linha);
    if (!tempLinha) { *numPartes = 0; return NULL; }

    for (int i = 0; tempLinha[i] != '\0'; i++) {
        char c = tempLinha[i];

        if (c == '"') {
            emAspas = !emAspas;
        } else if (c == ',' && !emAspas) {
            if (count < 50) {
                tempLinha[i] = '\0'; 
                tempArray[count++] = duplicarString(tempLinha + inicio);
            }
            inicio = i + 1;
        }
    }
    
    if (count < 50) {
        tempArray[count++] = duplicarString(tempLinha + inicio);
    }
    
    free(tempLinha);

    char** resultado = (char**)malloc(count * sizeof(char*));
    for (int i = 0; i < count; i++) {
        resultado[i] = tempArray[i];
    }
    free(tempArray);
    
    *numPartes = count;
    return resultado;
}

char** parseArray(char* campo, int* size) {
    if (!campo || !*campo) { *size = 0; return NULL; }
    
    char *s = removerAspas(campo);
    if (s[0] == '[') s++;
    if (s[strlen(s) - 1] == ']') s[strlen(s) - 1] = '\0';

    char** tempArray = (char**)malloc(50 * sizeof(char*));
    int count = 0;
    char* token;
    
    char* s_copy = duplicarString(s);
    if (!s_copy) { *size = 0; return NULL; }
    
   
    int aux;
    token = strtok(s_copy, ",");
    while (token != NULL) {
        if (count < 50) {
            char* trimmed_token = removerAspas(token);
            tempArray[count++] = duplicarString(trimmed_token);
        }
        token = strtok(NULL, ",");
    }
    free(s_copy);

    char** resultado = (char**)malloc(count * sizeof(char*));
    for (int i = 0; i < count; i++) {
        resultado[i] = tempArray[i];
    }
    free(tempArray);

    *size = count;
    return resultado;
}

Game create_Game(
    int id, char *name, char *releaseDate, int estimatedOwners, float price,
    char **supportedLanguages, int numLanguages,
    int metacriticScore, float userScore, int achievements,
    char **publishers, int numPublishers,
    char **developers, int numDevelopers,
    char **categories, int numCategories,
    char **genres, int numGenres,
    char **tags, int numTags
) {
    Game newGame;
    newGame.id = id;
    newGame.name = name;
    newGame.releaseDate = releaseDate;
    newGame.estimatedOwners = estimatedOwners;
    newGame.price = price;
    newGame.supportedLanguages = supportedLanguages;
    newGame.numLanguages = numLanguages;
    newGame.metacriticScore = metacriticScore;
    newGame.userScore = userScore;
    newGame.achievements = achievements;
    newGame.publishers = publishers;
    newGame.numPublishers = numPublishers;
    newGame.developers = developers;
    newGame.numDevelopers = numDevelopers;
    newGame.categories = categories;
    newGame.numCategories = numCategories;
    newGame.genres = genres;
    newGame.numGenres = numGenres;
    newGame.tags = tags;
    newGame.numTags = numTags;
    return newGame;
}

/** Libera toda a memória alocada dinamicamente para um único Game */
void liberarGame(Game* g) {
    if (!g) return;

    free(g->name);
    free(g->releaseDate);

    for(int k = 0; k < g->numLanguages; k++) free(g->supportedLanguages[k]);
    free(g->supportedLanguages);
    
    for(int k = 0; k < g->numPublishers; k++) free(g->publishers[k]);
    free(g->publishers);

    for(int k = 0; k < g->numDevelopers; k++) free(g->developers[k]);
    free(g->developers);

    for(int k = 0; k < g->numCategories; k++) free(g->categories[k]);
    free(g->categories);

    for(int k = 0; k < g->numGenres; k++) free(g->genres[k]);
    free(g->genres);

    for(int k = 0; k < g->numTags; k++) free(g->tags[k]);
    free(g->tags);
    
    // Nota: O struct Game em si é liberado no main com free(games)
}


int obterMes(const char *mes_str) {
    static const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    for (int i = 0; i < 12; i++) {
        if (strncasecmp(mes_str, months[i], 3) == 0) {
            return i + 1;
        }
    }
    return 1;
}

char *tratarData(const char *data) {
    char *resultado = (char*)malloc(11); // dd/mm/yyyy\0

    if (data == NULL || strlen(data) == 0) {
        strcpy(resultado, "01/01/0001");
        return resultado;
    }

    if (strlen(data) == 4 && strspn(data, "0123456789") == 4) {
        sprintf(resultado, "01/01/%s", data);
        return resultado;
    }

    char mes_str[10];
    int dia = 1, ano = 1;
    int mes_num;

    // Formato: Mon DD, YYYY
    if (sscanf(data, "%s %d, %d", mes_str, &dia, &ano) == 3) {
        mes_num = obterMes(mes_str);
        sprintf(resultado, "%02d/%02d/%d", dia, mes_num, ano);
        return resultado;
    }

    // Formato: Mon YYYY
    if (sscanf(data, "%s %d", mes_str, &ano) == 2) {
        mes_num = obterMes(mes_str);
        sprintf(resultado, "01/%02d/%d", mes_num, ano);
        return resultado;
    }
    
    // Se a data já for dd/mm/yyyy (ou similar)
    if (strchr(data, '/') != NULL) {
        int d, m, y;
        if (sscanf(data, "%d/%d/%d", &d, &m, &y) == 3) {
            sprintf(resultado, "%02d/%02d/%04d", d, m, y);
            return resultado;
        }
    }

    // Caso fallback
    strcpy(resultado, "01/01/0001");
    return resultado;
}

Data obterData (char* dataGame) {
    Data data = {1, 1, 1}; // Default
    int aux = 3;
    // Assume que a string já está tratada no formato dd/mm/yyyy
    char** releaseDate = NULL;
    
    // Usa strdup para evitar modificar dataGame diretamente
    char* dataCopy = duplicarString(dataGame);
    if (dataCopy) {
        releaseDate = (char**)malloc(3 * sizeof(char*));
        char* token = strtok(dataCopy, "/");
        int count = 0;
        
        while(token != NULL && count < 3) {
            releaseDate[count++] = duplicarString(token);
            token = strtok(NULL, "/");
        }
        aux = count;
    }
    
    if (aux == 3 && releaseDate != NULL) {
        data.dia = atoi(releaseDate[0]);
        data.mes = atoi(releaseDate[1]);
        data.ano = atoi(releaseDate[2]);
    }

    // Liberação de memória
    for (int i = 0; i < aux; i++) free(releaseDate[i]);
    free(releaseDate);
    free(dataCopy);

    return data;
}



int obterTamanho(const char *caminho) {
    int linhas = 0;
    FILE *file = fopen(caminho, "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir arquivo: %s\n", caminho);
        return 0;
    }
    char buffer[4096];
    fgets(buffer, sizeof(buffer), file); // Pula o cabeçalho
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        linhas++;
    }
    fclose(file);
    return linhas;
}

Game *preencherGames(const char *caminho, int *numGames) {
    *numGames = obterTamanho(caminho);
    if (*numGames == 0) return NULL; 

    // O tamanho do vetor é o número de linhas de dados
    Game *games = (Game*)malloc(sizeof(Game) * (*numGames)); 
    if (!games) { *numGames = 0; return NULL; }

    FILE *file = fopen(caminho, "r");
    if (!file) {
        *numGames = 0;
        free(games);
        return NULL;
    }

    char linha[MAX_LINHA];
    fgets(linha, sizeof(linha), file); // Pula o cabeçalho

    int i = 0;
    int linhasLidas = 0;
    while (fgets(linha, sizeof(linha), file) != NULL && linhasLidas < *numGames) {
        linha[strcspn(linha, "\r\n")] = 0;

        int numCampos;
        char **campos = splitCsvAspas(linha, &numCampos); 
        
        if (numCampos >= 14) { 
            int id = tentarParseInt(campos[0]); 
            
            if (id > 0) { 
                // Alocação de memória para cada campo
                char *name = duplicarString(removerAspas(campos[1]));
                char *releaseDate = tratarData(removerAspas(campos[2]));
                int estimatedOwners = tentarParseInt(removerAspas(campos[3]));
                float price = tentarParseFloat(removerAspas(campos[4]));

                int numLanguages;
                char **supportedLanguages = parseArray(campos[5], &numLanguages);

                int metacriticScore = tentarParseInt(removerAspas(campos[6]));
                float userScore = tentarParseFloat(removerAspas(campos[7]));
                int achievements = tentarParseInt(removerAspas(campos[8]));

                int numPublishers;
                char **publishers = parseArray(campos[9], &numPublishers);

                int numDevelopers;
                char **developers = parseArray(campos[10], &numDevelopers);

                int numCategories;
                char **categories = parseArray(campos[11], &numCategories);

                int numGenres;
                char **genres = parseArray(campos[12], &numGenres);

                int numTags;
                char **tags = parseArray(campos[13], &numTags);

                // Preenche o vetor principal
                games[i] = create_Game(id, name, releaseDate, estimatedOwners, price, 
                                        supportedLanguages, numLanguages, metacriticScore, userScore, 
                                        achievements, publishers, numPublishers, developers, numDevelopers, 
                                        categories, numCategories, genres, numGenres, tags, numTags);
                i++;
            }
        }

        // Liberação de memória dos campos temporários do splitCsvAspas
        for (int j = 0; j < numCampos; j++) free(campos[j]);
        free(campos);
        linhasLidas++;
    }
    
    *numGames = i; // Número final de jogos lidos com sucesso
    
    fclose(file);
    return games;
}



Game* criarVetorGames (Game* games, int idGames[], int max, int tam) {
    Game *vetorGames = (Game*)malloc(max * sizeof(Game));
    
    // Cria um vetor de ponteiros para evitar cópias profundas desnecessárias
    Game *tempPointers[max];
    int foundCount = 0;
    
    for (int i = 0; i < max; i++) {
        for (int j = 0; j < tam; j++) {
            if (games[j].id == idGames[i]) {
                tempPointers[foundCount++] = &games[j];
                break; 
            }
        }
    }
    
    
    for (int i = 0; i < foundCount; i++) {
        Game *src = tempPointers[i];
        
        // Copia os dados
        vetorGames[i].id = src->id;
        vetorGames[i].estimatedOwners = src->estimatedOwners;
        vetorGames[i].price = src->price;
        vetorGames[i].metacriticScore = src->metacriticScore;
        vetorGames[i].userScore = src->userScore;
        vetorGames[i].achievements = src->achievements;
        vetorGames[i].numLanguages = src->numLanguages;
        vetorGames[i].numPublishers = src->numPublishers;
        vetorGames[i].numDevelopers = src->numDevelopers;
        vetorGames[i].numCategories = src->numCategories;
        vetorGames[i].numGenres = src->numGenres;
        vetorGames[i].numTags = src->numTags;

        // Faz a cópia profunda 
        vetorGames[i].name = duplicarString(src->name);
        vetorGames[i].releaseDate = duplicarString(src->releaseDate);
        
        // Copia os arrays de strings
        vetorGames[i].supportedLanguages = (char**)malloc(src->numLanguages * sizeof(char*));
        for(int k=0; k < src->numLanguages; k++) vetorGames[i].supportedLanguages[k] = duplicarString(src->supportedLanguages[k]);

        vetorGames[i].publishers = (char**)malloc(src->numPublishers * sizeof(char*));
        for(int k=0; k < src->numPublishers; k++) vetorGames[i].publishers[k] = duplicarString(src->publishers[k]);
        
        vetorGames[i].developers = (char**)malloc(src->numDevelopers * sizeof(char*));
        for(int k=0; k < src->numDevelopers; k++) vetorGames[i].developers[k] = duplicarString(src->developers[k]);

        vetorGames[i].categories = (char**)malloc(src->numCategories * sizeof(char*));
        for(int k=0; k < src->numCategories; k++) vetorGames[i].categories[k] = duplicarString(src->categories[k]);
        
        vetorGames[i].genres = (char**)malloc(src->numGenres * sizeof(char*));
        for(int k=0; k < src->numGenres; k++) vetorGames[i].genres[k] = duplicarString(src->genres[k]);
        
        vetorGames[i].tags = (char**)malloc(src->numTags * sizeof(char*));
        for(int k=0; k < src->numTags; k++) vetorGames[i].tags[k] = duplicarString(src->tags[k]);
    }
    
    // Ajusta o tamanho final do vetor
    Game *finalVetor = (Game*)realloc(vetorGames, foundCount * sizeof(Game));
    if (finalVetor == NULL) finalVetor = vetorGames; // Em caso de realloc falhar
    
    return finalVetor;
}

Game* criarVetor (Game* games, int tam, int* tamVetor) {
    int *idGames = (int*)malloc(tam * sizeof(int));
    int max = 0;
    int aux = 0;
    
    // Leitura dos IDs do stdin
    char linhaEntrada[MAX_LINHA];
    while (fgets(linhaEntrada, MAX_LINHA, stdin) != NULL) {
        size_t len = strlen(linhaEntrada);
        while (len > 0 && isspace((unsigned char)linhaEntrada[len - 1])) {
            linhaEntrada[--len] = '\0';
        }

        if (strcmp(linhaEntrada, "FIM") == 0) break;

        int id = tentarParseInt(linhaEntrada); 
        if (id > 0 && max < tam) {
            idGames[max++] = id;
        }
    }
    
    *tamVetor = max;
    Game* resultado = criarVetorGames(games, idGames, max, tam);
    
    free(idGames);
    return resultado;
}



void swapGames (Game* games, int i, int j) {
    Game tempGame = games[i];
    games[i] = games[j];
    games[j] = tempGame;
}

void swapDatas(Data datas[], int i, int j) {
    Data tempData = datas[i];
    datas[i] = datas[j];
    datas[j] = tempData;
}

int dataCompare(Data a, Data b, int idA, int idB) {
    metricas[0]++; // Conta a comparação
    
    // Chave Principal: Ano (Ascendente)
    if (a.ano != b.ano) {
        return a.ano < b.ano;
    }

    // Chave Principal: Mês (Ascendente)
    if (a.mes != b.mes) {
        return a.mes < b.mes;
    }

    // Chave Principal: Dia (Ascendente)
    if (a.dia != b.dia) {
        return a.dia < b.dia;
    }

    // Chave Secundária (Desempate): ID (Decrescente)
    return idA > idB;
}

void quicksort (Game* games, Data datas[], int esq, int dir) {
    int i = esq, j = dir;
    Data pivo = datas[(dir+esq)/2];
    int pivoID = games[(dir+esq)/2].id;

    while (i <= j) {
        // dataCompare já incrementa metricas[0]
        while (dataCompare(datas[i], pivo, games[i].id, pivoID)) i++;
        while (dataCompare(pivo, datas[j], pivoID, games[j].id)) j--;
        
        if (i <= j) {
            swapGames(games, i, j);
            swapDatas(datas, i, j);
            metricas[1] += 3; // 3 movimentações 
            i++;
            j--;
        }
    }
    if (esq < j) quicksort(games, datas, esq, j);
    if (i < dir) quicksort(games, datas, i, dir);
}

void ordenarGames (Game* games, int tam) {
    Data datas[tam];
    for(int i = 0; i < tam; i++) {
        datas[i] = obterData(games[i].releaseDate);
    }
    quickSort(games, datas, 0, tam-1);
}



char *formatArray(char **array, int size, bool addSpace) {
    if (array == NULL || size == 0) return duplicarString("[]");

    int total_len = 3; 
    for (int i = 0; i < size; i++) {
        total_len += strlen(array[i]);
        if (i < size - 1) {
            total_len++;
            if (addSpace) total_len++;
        }
    }

    char *result = (char*)malloc(total_len * 2); // Aloca o dobro por segurança
    if (!result) return NULL;

    strcpy(result, "[");
    for (int i = 0; i < size; i++) {
        strcat(result, array[i]);
        if (i < size - 1) {
            strcat(result, ",");
            if (addSpace) strcat(result, " ");
        }
    }
    strcat(result, "]");
    return result;
}

void imprimirVetor(Game *game, int i) {
    char *languages_str = formatArray(game[i].supportedLanguages, game[i].numLanguages, true);
    char *publishers_str = formatArray(game[i].publishers, game[i].numPublishers, true);
    char *developers_str = formatArray(game[i].developers, game[i].numDevelopers, true);
    char *categories_str = formatArray(game[i].categories, game[i].numCategories, true);
    char *genres_str = formatArray(game[i].genres, game[i].numGenres, true);
    char *tags_str = formatArray(game[i].tags, game[i].numTags, true);

    
    printf("=> %d ## %s ## %s ## %d ## %.2f ## %s ## %d ## %.1f ## %d ## %s ## %s ## %s ## %s ## %s ##\n",
        game[i].id, game[i].name, game[i].releaseDate, game[i].estimatedOwners, game[i].price, languages_str,
        game[i].metacriticScore, game[i].userScore, game[i].achievements, publishers_str, developers_str,
        categories_str, genres_str, tags_str
    );

    free(languages_str);
    free(publishers_str);
    free(developers_str);
    free(categories_str);
    free(genres_str);
    free(tags_str);
}

void imprimirResultado(Game* games, int tam) {
    for(int i = 0; i < tam; i++) {
        imprimirVetor(games, i);
    }
}

void criarArquivo(char matricula[], double tempoExecucao) {
    char nomeArquivo[50];
    sprintf(nomeArquivo, "%s_quicksort.txt", matricula);
    FILE *arquivo = fopen(nomeArquivo, "w");

    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir/criar o arquivo de log!\n");
        return;
    }

    // Formato: MATRICULA\tCOMPARAÇÕES\tMOVIMENTAÇÕES\tTEMPO
    fprintf(arquivo, "%s\t%d\t%d\t%.3f", matricula, metricas[0], metricas[1], tempoExecucao);
    fclose(arquivo);
}




int main() {
    setlocale(LC_ALL, "C");

   
    char* caminho = "/tmp/games.csv"; 
    char matricula[50] = MATRICULA;
    
    metricas[0] = 0; 
    metricas[1] = 0;
    
    int numGames = 0;

    clock_t tempoInicial = clock(); 
    
    Game *games = preencherGames(caminho, &numGames);
    
    if (numGames == 0 || games == NULL) {
        
        return 1;
    }

    int tamVetor = 0;
    Game *vetorGames = criarVetor(games, numGames, &tamVetor);
    
    if (tamVetor > 0) {
        ordenarGames(vetorGames, tamVetor);
        imprimirResultado(vetorGames, tamVetor);
    } 

    clock_t tempoFinal = clock(); 
    double tempoExecucao = (double)(tempoFinal - tempoInicial) / CLOCKS_PER_SEC; 
    
    criarArquivo(matricula, tempoExecucao);

    
    // Libera a memória alocada dinamicamente dentro de cada Game do vetor original
    for (int i = 0; i < numGames; i++) {
        liberarGame(&games[i]);
    }
    free(games);
    
    // Libera a memória alocada dinamicamente dentro de cada Game do vetor a ordenar
    for (int i = 0; i < tamVetor; i++) {
        liberarGame(&vetorGames[i]);
    }
    free(vetorGames);

    return 0;
}