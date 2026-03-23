#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>


#define MAX_JOGOS 20000
#define MAX_LINHA 4096
#define MATRICULA "888678"


int comparacoes_ordenacao = 0;
int movimentacoes_ordenacao = 0;

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

// Protótipos das funções auxiliares
char* duplicarString(const char* s);
char* removerAspas(char* s);
int tentarParseInt(char* s);
float tentarParseFloat(char* s);
char* tratarData(const char* dataCrua);
Data obterDataEstruturada(const char* dataStr);
char* formatarArray(char** array, int size, bool addSpace);
Game* carregarTodosOsJogos(const char* caminho, int* numTotal);
void imprimirGame(Game *g);

// Protótipos de ordenação e comparação
void swapGames(Game* g1, Game* g2);
void swapDatas(Data* d1, Data* d2);
bool gameCompare(Game *a, Game *b, Data *dataA, Data *dataB);
int partition(Game* games, Data* datas, int low, int high);
void quickSort(Game* games, Data* datas, int low, int high);

// Protótipo do log
void criarArquivoLog(char matricula[], double tempoExecucao, int comparacoes, int movimentacoes);



/** Aloca e copia uma string*/
char* duplicarString(const char* s) {
    if (!s) return NULL;
    char* result = (char*)malloc(strlen(s) + 1);
    if (result) strcpy(result, s);
    return result;
}

/** Remove aspas duplas ou simples das extremidades de uma string */
char* removerAspas(char* s) {
    if (!s || !*s) return s;
    size_t len = strlen(s);
    if (len >= 2 && ((s[0] == '"' && s[len-1] == '"') || (s[0] == '\'' && s[len-1] == '\''))) {
        s[len-1] = '\0';
        return s + 1;
    }
    return s;
}

/** Tenta converter uma string para inteiro, limpando caracteres não-numéricos */
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

/** Tenta converter uma string para float, tratando vírgulas/pontos */
float tentarParseFloat(char* s) {
    if (!s || !*s) return 0.0f;
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

/** Converte string de data (dd/mm/yyyy) para a struct Data */
Data obterDataEstruturada(const char* dataStr) {
    Data data = {1, 1, 0};
    if (strlen(dataStr) == 10) { // Espera dd/mm/yyyy
        sscanf(dataStr, "%d/%d/%d", &data.dia, &data.mes, &data.ano);
    }
    return data;
}

/** Trata diversos formatos de data para o padrão dd/mm/yyyy */
char* tratarData(const char* dataCrua) {
    if (!dataCrua || !*dataCrua) return duplicarString("01/01/0000");

    // Tentar formato dd/mm/yyyy
    int d, m, y;
    if (sscanf(dataCrua, "%d/%d/%d", &d, &m, &y) == 3) {
        char *buffer = (char*)malloc(11);
        if (buffer) {
            sprintf(buffer, "%02d/%02d/%04d", d, m, y);
            return buffer;
        }
    }

    // Tenta extrair ano e mês para criar 01/mm/yyyy
    int ano = 0;
    for(int i = 0; dataCrua[i] != '\0'; i++) {
        if (i + 3 < strlen(dataCrua) && isdigit((unsigned char)dataCrua[i]) && isdigit((unsigned char)dataCrua[i+1]) && isdigit((unsigned char)dataCrua[i+2]) && isdigit((unsigned char)dataCrua[i+3])) {
            char temp[5];
            strncpy(temp, &dataCrua[i], 4);
            temp[4] = '\0';
            ano = atoi(temp);
            if (ano > 1900) break;
        }
    }
    int mes = 1;
    char meses[12][4] = {"jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec"};
    for(int i = 0; i < 12; i++) {
        if (strstr(dataCrua, meses[i]) != NULL) {
            mes = i + 1;
            break;
        }
    }

    char *buffer = (char*)malloc(11);
    sprintf(buffer, "01/%02d/%04d", mes, ano);
    return buffer;
}


/** Divide uma linha CSV em colunas, respeitando aspas */
char** splitCsvAspas(const char* linha, int* numPartes) {
    // ... Implementação de parsing com aspas
    char** tempArray = (char**)malloc(50 * sizeof(char*));
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

/** Faz o parsing de arrays como [lang1, lang2] ou listas separadas por vírgula */
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


/** Encontra o índice da coluna no cabeçalho */
int obterIndice(char** cabecalhos, int numCabecalhos, char* palavraChave) {
    for (int i = 0; i < numCabecalhos; i++) {
        // Assume que as palavras-chave já estão em minúsculo
        if (strstr(cabecalhos[i], palavraChave) != NULL) {
            return i;
        }
    }
    return -1;
}

/** Formata um array de strings para a saída padrão (ex: [elem1, elem2]) */
char* formatarArray(char** array, int size, bool addSpace) {
    if (!array || size == 0) {
        return duplicarString("[]");
    }

    size_t required_size = 3; // Estimativa
    for (int i = 0; i < size; i++) {
        required_size += strlen(array[i]);
        if (i < size - 1) required_size += 2; 
    }

    char* result = (char*)malloc(required_size * 2); // Aloca o dobro por segurança
    if (!result) return duplicarString("[]");

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

/** Imprime o registro Game no formato da saída padrão */
void imprimirGame(Game *g) {
    char *langs = formatarArray(g->supportedLanguages, g->numLanguages, true);
    char *pubs = formatarArray(g->publishers, g->numPublishers, true);
    char *devs = formatarArray(g->developers, g->numDevelopers, true);
    char *cats = formatarArray(g->categories, g->numCategories, true);
    char *genres = formatarArray(g->genres, g->numGenres, true);
    char *tags = formatarArray(g->tags, g->numTags, true);

    // Nota: O formato %.2f e %.1f é para garantir a precisão
    printf("=> %d ## %s ## %s ## %d ## %.2f ## %s ## %d ## %.1f ## %d ## %s ## %s ## %s ## %s ## %s ##\n",
        g->id,
        g->name,
        g->releaseDate,
        g->estimatedOwners,
        g->price,
        langs,
        g->metacriticScore,
        g->userScore,
        g->achievements,
        pubs,
        devs,
        cats,
        genres,
        tags
    );
    
    // Libera a memória temporária das strings formatadas
    free(langs);
    free(pubs);
    free(devs);
    free(cats);
    free(genres);
    free(tags);
}

/** Troca dois elementos Game e incrementa as movimentações */
void swapGames(Game* g1, Game* g2) {
    Game temp = *g1;
    *g1 = *g2;
    *g2 = temp;
    movimentacoes_ordenacao += 3; // 3 atribuições por swap
}

/** Troca dois elementos Data (auxiliar, não conta movimentação) */
void swapDatas(Data* d1, Data* d2) {
    Data temp = *d1;
    *d1 = *d2;
    *d2 = temp;
}


/**
 * Compara dois jogos. 
 * Chave principal: Data (Ascendente). 
 * Chave secundária: ID (Decrescente).
 * Retorna true se 'a' deve vir antes de 'b'.
 */
bool gameCompare(Game *a, Game *b, Data *dataA, Data *dataB) {
    comparacoes_ordenacao++; 
    
    // Chave Principal: Ano (Ascendente)
    if (dataA->ano != dataB->ano) {
        return dataA->ano < dataB->ano;
    }
    
    //  Chave Principal: Mês (Ascendente)
    if (dataA->mes != dataB->mes) {
        return dataA->mes < dataB->mes;
    }
    
    //  Chave Principal: Dia (Ascendente)
    if (dataA->dia != dataB->dia) {
        return dataA->dia < dataB->dia;
    }
    
    // Chave Secundária (Desempate): ID (Decrescente)
    return a->id > b->id;
}


/** Particionamento do Quicksort*/
int partition(Game* games, Data* datas, int low, int high) {
    Data pivotData = datas[high];
    Game pivotGame = games[high];
    
    int i = (low - 1); 

    for (int j = low; j < high; j++) {
        // Se games[j] deve vir antes do pivot (gameCompare retorna true)
        if (gameCompare(&games[j], &pivotGame, &datas[j], &pivotData)) {
            i++;
            swapGames(&games[i], &games[j]);
            swapDatas(&datas[i], &datas[j]);
        }
    }

    // Coloca o pivô na posição correta (i + 1)
    swapGames(&games[i + 1], &games[high]);
    swapDatas(&datas[i + 1], &datas[high]);

    return i + 1;
}

/** Função principal do Quicksort */
void quickSort(Game* games, Data* datas, int low, int high) {
    if (low < high) {
        int pi = partition(games, datas, low, high);
        quickSort(games, datas, low, pi - 1);
        quickSort(games, datas, pi + 1, high);
    }
}

/** Cria o arquivo de log no formato exigido */
void criarArquivoLog(char matricula[], double tempoExecucao, int comparacoes, int movimentacoes) {
    char nomeArquivo[50];
    sprintf(nomeArquivo, "%s_quicksort.txt", matricula);
    FILE *arquivo = fopen(nomeArquivo, "w");

    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir/criar o arquivo de log!\n");
        return;
    }

    // Formato: MATRICULA\tCOMPARAÇÕES\tMOVIMENTAÇÕES\tTEMPO
    fprintf(arquivo, "%s\t%d\t%d\t%.3f", matricula, comparacoes, movimentacoes, tempoExecucao);
    fclose(arquivo);
}


/** Carrega todos os jogos do arquivo CSV (Pré-leitura) */
Game* carregarTodosOsJogos(const char* caminho, int* numTotal) {
    FILE *arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir arquivo CSV: %s\n", caminho);
        *numTotal = 0;
        return NULL;
    }

    // Alocação inicial do array principal
    char linha[MAX_LINHA];
    Game* todosOsJogos = (Game*)malloc(MAX_JOGOS * sizeof(Game));
    if (!todosOsJogos) {
        fprintf(stderr, "Erro de alocação de memória!\n");
        fclose(arquivo);
        *numTotal = 0;
        return NULL;
    }

    // Pula o cabeçalho e faz o parsing dos índices
    if (fgets(linha, MAX_LINHA, arquivo) == NULL) {
        fclose(arquivo);
        *numTotal = 0;
        return todosOsJogos;
    }
    int numCabecalhos;
    char** cabecalhos = splitCsvAspas(linha, &numCabecalhos);
    
    int idxId = obterIndice(cabecalhos, numCabecalhos, "appid");
    int idxNome = obterIndice(cabecalhos, numCabecalhos, "name");
    int idxLancamento = obterIndice(cabecalhos, numCabecalhos, "release");
    int idxDonos = obterIndice(cabecalhos, numCabecalhos, "owners");
    int idxPreco = obterIndice(cabecalhos, numCabecalhos, "price");
    int idxLinguas = obterIndice(cabecalhos, numCabecalhos, "languages");
    int idxMetacritic = obterIndice(cabecalhos, numCabecalhos, "metacritic");
    int idxPlacarUsuario = obterIndice(cabecalhos, numCabecalhos, "userscore");
    int idxConquistas = obterIndice(cabecalhos, numCabecalhos, "achievements");
    int idxPublicadoras = obterIndice(cabecalhos, numCabecalhos, "publisher");
    int idxDesenvolvedoras = obterIndice(cabecalhos, numCabecalhos, "developer");
    int idxCategorias = obterIndice(cabecalhos, numCabecalhos, "categories");
    int idxGeneros = obterIndice(cabecalhos, numCabecalhos, "genres");
    int idxTags = obterIndice(cabecalhos, numCabecalhos, "tags");
    
    // Libera a memória do cabeçalho
    for(int i = 0; i < numCabecalhos; i++) free(cabecalhos[i]);
    free(cabecalhos);

    *numTotal = 0;
    while (fgets(linha, MAX_LINHA, arquivo) != NULL && *numTotal < MAX_JOGOS) {
        if (linha[0] == '\n' || linha[0] == '\r' || linha[0] == '\0') continue;
        
        int numColunas;
        char** colunas = splitCsvAspas(linha, &numColunas);
        
        if (numColunas > 0) {
            Game g = {0};
            
            if (idxId >= 0 && idxId < numColunas) g.id = tentarParseInt(removerAspas(colunas[idxId]));
            if (g.id == 0) goto cleanup_game; 
            
            // Atribuição com duplicação de string (alocação)
            if (idxNome >= 0 && idxNome < numColunas) g.name = duplicarString(removerAspas(colunas[idxNome]));
            if (idxLancamento >= 0 && idxLancamento < numColunas) g.releaseDate = tratarData(removerAspas(colunas[idxLancamento]));
            
            if (idxDonos >= 0 && idxDonos < numColunas) g.estimatedOwners = tentarParseInt(removerAspas(colunas[idxDonos]));
            if (idxPreco >= 0 && idxPreco < numColunas) g.price = tentarParseFloat(removerAspas(colunas[idxPreco]));
            if (idxLinguas >= 0 && idxLinguas < numColunas) g.supportedLanguages = parseArray(colunas[idxLinguas], &g.numLanguages);
            if (idxMetacritic >= 0 && idxMetacritic < numColunas) g.metacriticScore = tentarParseInt(removerAspas(colunas[idxMetacritic]));
            if (idxPlacarUsuario >= 0 && idxPlacarUsuario < numColunas) g.userScore = tentarParseFloat(removerAspas(colunas[idxPlacarUsuario]));
            if (idxConquistas >= 0 && idxConquistas < numColunas) g.achievements = tentarParseInt(removerAspas(colunas[idxConquistas]));
            if (idxPublicadoras >= 0 && idxPublicadoras < numColunas) g.publishers = parseArray(colunas[idxPublicadoras], &g.numPublishers);
            if (idxDesenvolvedoras >= 0 && idxDesenvolvedoras < numColunas) g.developers = parseArray(colunas[idxDesenvolvedoras], &g.numDevelopers);
            if (idxCategorias >= 0 && idxCategorias < numColunas) g.categories = parseArray(colunas[idxCategorias], &g.numCategories);
            if (idxGeneros >= 0 && idxGeneros < numColunas) g.genres = parseArray(colunas[idxGeneros], &g.numGenres);
            if (idxTags >= 0 && idxTags < numColunas) g.tags = parseArray(colunas[idxTags], &g.numTags);

            todosOsJogos[(*numTotal)++] = g;
            
            cleanup_game:
        
            for(int i = 0; i < numColunas; i++) free(colunas[i]);
            free(colunas);
        }
    }

    fclose(arquivo);
    return todosOsJogos;
}


int main(int argc, char *argv[]) {
    // Configura a localidade para garantir que o separador decimal seja um ponto na saída
    setlocale(LC_ALL, "C"); 

    char* caminho = "/tmp/games.csv";
    if (argc > 1 && argv[1] != NULL && strlen(argv[1]) > 0) {
        caminho = argv[1];
    }

    int numTotal = 0;
    Game *todosOsJogos = carregarTodosOsJogos(caminho, &numTotal);
    
    if (numTotal == 0) {
        fprintf(stderr, "Nenhum jogo carregado. Encerrando.\n");
        return 1;
    }

    // Leitura da Primeira Parte da Entrada (IDs)
    int *idsParaIncluir = (int*)malloc(numTotal * sizeof(int));
    int numIds = 0;
    
    char linhaEntrada[MAX_LINHA];
    while (fgets(linhaEntrada, MAX_LINHA, stdin) != NULL) {
        size_t len = strlen(linhaEntrada);
        while (len > 0 && isspace((unsigned char)linhaEntrada[len - 1])) {
            linhaEntrada[--len] = '\0';
        }

        if (strcmp(linhaEntrada, "FIM") == 0) break;

        int id = tentarParseInt(linhaEntrada);
        if (id > 0 && numIds < numTotal) {
            idsParaIncluir[numIds++] = id;
        }
    }

    // Seleção dos Jogos para o vetor final
    Game *jogosInclusos = (Game*)malloc(numIds * sizeof(Game));
    Data *datasInclusas = (Data*)malloc(numIds * sizeof(Data));
    int numJogosInclusos = 0;

    for (int i = 0; i < numIds; i++) {
        for (int j = 0; j < numTotal; j++) {
            if (todosOsJogos[j].id == idsParaIncluir[i]) {
                if (numJogosInclusos < numIds) {
                    // Cópia rasa dos dados
                    jogosInclusos[numJogosInclusos] = todosOsJogos[j];
                    // Conversão e armazenamento da data para ordenação
                    datasInclusas[numJogosInclusos] = obterDataEstruturada(jogosInclusos[numJogosInclusos].releaseDate);
                    numJogosInclusos++;
                }
                break; // Encontrado
            }
        }
    }
    
    free(idsParaIncluir);
    
    //Ordenação
    clock_t inicioOrdenacao = clock();
    
    quickSort(jogosInclusos, datasInclusas, 0, numJogosInclusos > 0 ? numJogosInclusos - 1 : -1);

    clock_t fimOrdenacao = clock();
    double tempoOrdenacao = (double)(fimOrdenacao - inicioOrdenacao) / CLOCKS_PER_SEC;


    //Impressão dos Resultados Ordenados
    for (int i = 0; i < numJogosInclusos; i++) {
        imprimirGame(&jogosInclusos[i]);
    }
    

    //Escrita do Arquivo de Log
    criarArquivoLog(MATRICULA, tempoOrdenacao, comparacoes_ordenacao, movimentacoes_ordenacao);


    
    for (int i = 0; i < numTotal; i++) {
        free(todosOsJogos[i].name);
        free(todosOsJogos[i].releaseDate);

        for(int k = 0; k < todosOsJogos[i].numLanguages; k++) free(todosOsJogos[i].supportedLanguages[k]);
        free(todosOsJogos[i].supportedLanguages);
        
        for(int k = 0; k < todosOsJogos[i].numPublishers; k++) free(todosOsJogos[i].publishers[k]);
        free(todosOsJogos[i].publishers);

        for(int k = 0; k < todosOsJogos[i].numDevelopers; k++) free(todosOsJogos[i].developers[k]);
        free(todosOsJogos[i].developers);

        for(int k = 0; k < todosOsJogos[i].numCategories; k++) free(todosOsJogos[i].categories[k]);
        free(todosOsJogos[i].categories);

        for(int k = 0; k < todosOsJogos[i].numGenres; k++) free(todosOsJogos[i].genres[k]);
        free(todosOsJogos[i].genres);

        for(int k = 0; k < todosOsJogos[i].numTags; k++) free(todosOsJogos[i].tags[k]);
        free(todosOsJogos[i].tags);
    }
    free(todosOsJogos);
    free(jogosInclusos);
    free(datasInclusas);

    return 0;
}