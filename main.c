
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
// primeira parte igual a antigamente pra ler o csv
#define MAX_JOGOS 20000
#define MAX_FIELDS 50
#define BUFFER_SIZE 8192
#define DATA_PADRAO "01/01/0000"
#define MATRICULA "888678" 

typedef struct Jogo {
    int id;
    char* nome;
    char* dataLancamento;
    int estimativaDonos;
    float preco;
    char** idiomasSuportados;
    int numIdiomas;
    int placarMetacritic;
    float placarUsuario;
    int conquistas;
    char** publicadoras;
    int numPublicadoras;
    char** desenvolvedoras;
    int numDesenvolvedoras;
    char** categorias;
    int numCategorias;
    char** generos;
    int numGeneros;
    char** tags;
    int numTags;
} Jogo;


void liberar_memoria(Jogo* g);
char* limpar_string(const char* s);
int tentar_parse_int(const char* s);
int parse_estimativa_donos(const char* s);
float tentar_parse_float_manual(const char* s, int is_price);
char* parse_data_lancamento(const char* s);
char** separar_csv(char* linha, int* num_campos);
char** parse_string_array(const char* campo, int* num_elementos);
char* formatar_float(float valor, int casas_decimais);
int encontrar_indice(char** cabecalhos, int num_cabecalhos, const char* palavras_chave[]);
void formatar_array(char** arr, int count);
void imprimir_jogo(const Jogo* g);


void liberar_memoria(Jogo* g) {
    if (!g) return;
    free(g->nome);
    free(g->dataLancamento);

    #define LIBERAR_ARRAY(arr, count) \
        if (arr) { \
            for (int i = 0; i < count; i++) free(arr[i]); \
            free(arr); \
        }

    LIBERAR_ARRAY(g->idiomasSuportados, g->numIdiomas);
    LIBERAR_ARRAY(g->publicadoras, g->numPublicadoras);
    LIBERAR_ARRAY(g->desenvolvedoras, g->numDesenvolvedoras);
    LIBERAR_ARRAY(g->categorias, g->numCategorias);
    LIBERAR_ARRAY(g->generos, g->numGeneros);
    LIBERAR_ARRAY(g->tags, g->numTags);
    #undef LIBERAR_ARRAY
}

char* limpar_string(const char* s) {
    if (!s) return strdup("");
    while (isspace((unsigned char)*s)) s++;
    int len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) len--;
    char* temp = (char*)malloc(len + 1);
    strncpy(temp, s, len);
    temp[len] = '\0';
    if (len >= 2 && ((temp[0] == '"' && temp[len - 1] == '"') || (temp[0] == '\'' && temp[len - 1] == '\''))) {
        char* resultado = (char*)malloc(len - 1);
        strncpy(resultado, temp + 1, len - 2);
        resultado[len - 2] = '\0';
        free(temp);
        return resultado;
    }
    return temp;
}

int tentar_parse_int(const char* s) {
    if (!s || *s == '\0') return 0;
    char *endptr;
    long val = strtol(s, &endptr, 10);
    if (endptr != s) return (int)val;
    char numStr[64]; int k = 0;
    for (int i = 0; s[i] != '\0' && k < (int)sizeof(numStr) - 1; i++) {
        char c = s[i];
        if (c >= '0' && c <= '9') numStr[k++] = c;
        else if (k > 0) break;
    }
    numStr[k] = '\0';
    if (k > 0) return (int)strtol(numStr, NULL, 10);
    return 0;
}

int parse_estimativa_donos(const char* s) {
    if (!s || *s == '\0') return 0;
    char numStr[64]; int k = 0;
    for (int i = 0; s[i] != '\0' && k < (int)sizeof(numStr) - 1; i++) {
        if (isdigit((unsigned char)s[i])) numStr[k++] = s[i];
    }
    numStr[k] = '\0';
    if (k == 0) return 0;
    return (int)strtol(numStr, NULL, 10);
}

float tentar_parse_float_manual(const char* s, int is_price) {
    if (!s || *s == '\0') return (is_price ? 0.0f : -1.0f);
    if (strcasecmp(s, "tbd") == 0) return -1.0f;
    if (is_price && (strcasecmp(s, "Free to Play") == 0 || strcasecmp(s, "Free") == 0)) return 0.0f;
    char numStr[BUFFER_SIZE]; int k = 0; int encontrouPonto = 0;
    for (int i = 0; s[i] != '\0' && k < (int)sizeof(numStr) - 1; i++) {
        char c = s[i];
        if (c >= '0' && c <= '9') numStr[k++] = c;
        else if (c == ',' || c == '.') {
            if (!encontrouPonto) { numStr[k++] = '.'; encontrouPonto = 1; }
        }
    }
    numStr[k] = '\0';
    if (k == 0 || (k == 1 && numStr[0] == '.')) return (is_price ? 0.0f : -1.0f);
    return strtof(numStr, NULL);
}

char* parse_data_lancamento(const char* s) {
    if (!s || *s == '\0') return strdup(DATA_PADRAO);
    char cru[BUFFER_SIZE];
    strncpy(cru, s, BUFFER_SIZE - 1);
    cru[BUFFER_SIZE - 1] = '\0';
    char* meses[] = {"jan","feb","mar","apr","may","jun","jul","aug","sep","oct","nov","dec"};
    char* numeros_meses[] = {"01","02","03","04","05","06","07","08","09","10","11","12"};
    if (strchr(cru, '/') != NULL) {
        char* temp = strdup(cru);
        char* p[3] = {NULL,NULL,NULL}; int i = 0;
        char* token = strtok(temp, "/");
        while(token != NULL && i < 3) { p[i++] = strdup(token); token = strtok(NULL, "/"); }
        if (i >= 3) {
            char d[3], m[3], y[5];
            snprintf(d, 3, "%s%s", (strlen(p[0])==1 && isdigit((unsigned char)p[0][0])) ? "0" : "", p[0]);
            snprintf(m, 3, "%s%s", (strlen(p[1])==1 && isdigit((unsigned char)p[1][0])) ? "0" : "", p[1]);
            snprintf(y, 5, "%s", p[2]);
            if (strlen(y) == 2 && isdigit((unsigned char)y[0])) snprintf(y, 5, "19%s", p[2]);
            char resultado[11];
            snprintf(resultado, 11, "%s/%s/%s", d, m, y);
            for(int j=0;j<i;j++) free(p[j]); free(temp);
            return strdup(resultado);
        }
        for(int j=0;j<i;j++) free(p[j]); free(temp);
    }
    char* temp = strdup(cru);
    char* partes[5] = {NULL}; int n_partes = 0;
    char* saveptr;
    char* token = strtok_r(temp, " ,", &saveptr);
    while (token != NULL && n_partes < 5) {
        if (*token != '\0') partes[n_partes++] = strdup(token);
        token = strtok_r(NULL, " ,", &saveptr);
    }
    char dia[3] = "01", mes[3] = "01", ano[5] = "0000";
    if (n_partes >= 3) {
        if (strlen(partes[2]) == 4 && tentar_parse_int(partes[2]) > 0) {
            snprintf(ano,5,"%s",partes[2]);
            int idxMes = -1;
            for (int i = 0; i < 12; i++) {
                if (strncmp(partes[0], meses[i], 3) == 0) {
                    idxMes = i; snprintf(dia,3,"%s%s", (strlen(partes[1])==1 && isdigit((unsigned char)partes[1][0])) ? "0" : "", partes[1]); break;
                } else if (strncmp(partes[1], meses[i], 3) == 0) {
                    idxMes = i; snprintf(dia,3,"%s%s", (strlen(partes[0])==1 && isdigit((unsigned char)partes[0][0])) ? "0" : "", partes[0]); break;
                }
            }
            if (idxMes != -1) {
                snprintf(mes,3,"%s", numeros_meses[idxMes]);
                char resultado[11];
                snprintf(resultado, 11, "%s/%s/%s", dia, mes, ano);
                for(int j=0;j<n_partes;j++) free(partes[j]); free(temp);
                return strdup(resultado);
            }
        }
    }
    for(int i = 0; cru[i] != '\0'; i++) {
        if (isdigit((unsigned char)cru[i]) && isdigit((unsigned char)cru[i+1]) && isdigit((unsigned char)cru[i+2]) && isdigit((unsigned char)cru[i+3])) {
            char anoEncontrado[5]; strncpy(anoEncontrado, cru + i, 4); anoEncontrado[4] = '\0';
            if (strtol(anoEncontrado, NULL, 10) > 0) {
                char resultado[11];
                snprintf(resultado, 11, "01/01/%s", anoEncontrado);
                for(int j=0;j<n_partes;j++) free(partes[j]); free(temp);
                return strdup(resultado);
            }
        }
    }
    for(int j=0;j<n_partes;j++) free(partes[j]); free(temp);
    return strdup(DATA_PADRAO);
}

char** separar_csv(char* linha, int* num_campos) {
    char** campos = (char**)malloc(MAX_FIELDS * sizeof(char*));
    if (!campos) { perror("malloc"); exit(EXIT_FAILURE); }
    int len_linha = strlen(linha);
    while (len_linha > 0 && (linha[len_linha-1] == '\n' || linha[len_linha-1] == '\r')) linha[--len_linha] = '\0';
    *num_campos = 0;
    int inicio = 0;
    int em_aspas = 0;
    for (int i = 0; linha[i] != '\0'; i++) {
        char c = linha[i];
        if (c == '"') em_aspas = !em_aspas;
        else if (c == ',' && !em_aspas) {
            if (*num_campos < MAX_FIELDS) {
                int len = i - inicio;
                campos[*num_campos] = (char*)malloc(len + 1);
                strncpy(campos[*num_campos], linha + inicio, len);
                campos[*num_campos][len] = '\0';
                (*num_campos)++;
            }
            inicio = i + 1;
        }
    }
    if (*num_campos < MAX_FIELDS) {
        int len = len_linha - inicio;
        campos[*num_campos] = (char*)malloc(len + 1);
        strncpy(campos[*num_campos], linha + inicio, len);
        campos[*num_campos][len] = '\0';
        (*num_campos)++;
    }
    return campos;
}

char** parse_string_array(const char* campo, int* num_elementos) {
    char* limpo = limpar_string(campo);
    if (limpo == NULL || strlen(limpo) < 1) { if (limpo) free(limpo); *num_elementos = 0; return NULL; }
    char* inicio_dados = limpo;
    int len_limpo = strlen(limpo);
    if (len_limpo >= 2 && limpo[0] == '[' && limpo[len_limpo - 1] == ']') {
        
    }
    if (len_limpo >= 2 && limpo[0] == '[' && limpo[len_limpo - 1] == ']') {
        inicio_dados = limpo + 1;
        limpo[len_limpo - 1] = '\0';
    }
    if (*inicio_dados == '\0') { free(limpo); *num_elementos = 0; return NULL; }
    char* token_string = strdup(inicio_dados);
    int capacidade = 10;
    char** elementos = (char**)malloc(capacidade * sizeof(char*));
    *num_elementos = 0;
    char* saveptr;
    char* token = strtok_r(token_string, ",", &saveptr);
    while (token != NULL) {
        char* elemento_limpo = limpar_string(token);
        if (*elemento_limpo != '\0') {
            if (*num_elementos >= capacidade) {
                capacidade *= 2;
                elementos = (char**)realloc(elementos, capacidade * sizeof(char*));
            }
            elementos[*num_elementos] = elemento_limpo;
            (*num_elementos)++;
        } else {
            free(elemento_limpo);
        }
        token = strtok_r(NULL, ",", &saveptr);
    }
    free(token_string);
    free(limpo);
    if (*num_elementos == 0) { free(elementos); *num_elementos = 0; return NULL; }
    return elementos;
}

char* formatar_float(float valor, int casas_decimais) {
    char buffer[BUFFER_SIZE];
    if (casas_decimais == 1) snprintf(buffer, BUFFER_SIZE, "%.1f", valor);
    else snprintf(buffer, BUFFER_SIZE, "%.2f", valor);
    return strdup(buffer);
}

int encontrar_indice(char** cabecalhos, int num_cabecalhos, const char* palavras_chave[]) {
    for (int i = 0; i < num_cabecalhos; i++) {
        char* h = strdup(cabecalhos[i]);
        for(size_t k = 0; h[k] != '\0'; k++) h[k] = tolower((unsigned char)h[k]);
        for (int j = 0; palavras_chave[j] != NULL; j++) {
            if (strstr(h, palavras_chave[j]) != NULL) { free(h); return i; }
        }
        free(h);
    }
    return -1;
}

void formatar_array(char** arr, int count) {
    if (!arr || count <= 0) { printf("[]"); return; }
    printf("[");
    for (int i = 0; i < count; i++) {
        printf("%s", arr[i]);
        if (i < count - 1) printf(", ");
    }
    printf("]");
}

void imprimir_jogo(const Jogo* g) {
    char* preco_str = formatar_float(g->preco, 2);
    char* placar_usuario_str;
    if (g->placarUsuario < 0) placar_usuario_str = strdup("-1.0");
    else placar_usuario_str = formatar_float(g->placarUsuario, 1);

    printf("=> %d ## %s ## %s ## %d ## %s ## ",
           g->id, g->nome, g->dataLancamento, g->estimativaDonos, preco_str);

    formatar_array(g->idiomasSuportados, g->numIdiomas);
    printf(" ## %d ## %s ## %d ## ", g->placarMetacritic, placar_usuario_str, g->conquistas);
    formatar_array(g->publicadoras, g->numPublicadoras);
    printf(" ## ");
    formatar_array(g->desenvolvedoras, g->numDesenvolvedoras);
    printf(" ## ");
    formatar_array(g->categorias, g->numCategorias);
    printf(" ## ");
    formatar_array(g->generos, g->numGeneros);
    printf(" ## ");
    formatar_array(g->tags, g->numTags);
    printf(" ##\n");

    free(preco_str);
    free(placar_usuario_str);
}

/* --- main: carrega CSV, lê IDs, seleciona jogos, ordena por Name com SelectionSort e gera log --- */
int main(int argc, char *argv[]) {
    char *caminho = (argc > 1) ? argv[1] : "/tmp/games.csv";
    FILE *arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo: %s\n", caminho);
        return 1;
    }

    Jogo* jogos[MAX_JOGOS];
    int num_jogos_carregados = 0;
    char linha[BUFFER_SIZE];
    int num_cabecalhos = 0;

    if (fgets(linha, BUFFER_SIZE, arquivo) != NULL) {
        char* linha_copy = strdup(linha);
        char** headers_parsed = separar_csv(linha_copy, &num_cabecalhos);
        free(linha_copy);

        const char* k_id[] = {"appid","id", NULL};
        const char* k_nome[] = {"name","nome", NULL};
        const char* k_lancamento[] = {"release","release_date","released","lancamento", NULL};
        const char* k_donos[] = {"owners","estimated","donos", NULL};
        const char* k_preco[] = {"price","initialprice","preco", NULL};
        const char* k_linguas[] = {"supported_languages","languages","idiomas", NULL};
        const char* k_metacritic[] = {"metacritic", NULL};
        const char* k_usuario[] = {"score","userscore","steamspy_userscore","placarusuario", NULL};
        const char* k_conquistas[] = {"achievements","conquistas", NULL};
        const char* k_publicadoras[] = {"publisher","publishers","publicadoras", NULL};
        const char* k_desenvolvedoras[] = {"developer","developers","desenvolvedoras", NULL};
        const char* k_categorias[] = {"category","categories","categorias", NULL};
        const char* k_generos[] = {"genre","genres","generos", NULL};
        const char* k_tags[] = {"tags","steamspy_tags", NULL};

        int idxId = encontrar_indice(headers_parsed, num_cabecalhos, k_id);
        int idxNome = encontrar_indice(headers_parsed, num_cabecalhos, k_nome);
        int idxLancamento = encontrar_indice(headers_parsed, num_cabecalhos, k_lancamento);
        int idxDonos = encontrar_indice(headers_parsed, num_cabecalhos, k_donos);
        int idxPreco = encontrar_indice(headers_parsed, num_cabecalhos, k_preco);
        int idxLinguas = encontrar_indice(headers_parsed, num_cabecalhos, k_linguas);
        int idxMetacritic = encontrar_indice(headers_parsed, num_cabecalhos, k_metacritic);
        int idxPlacarUsuario = encontrar_indice(headers_parsed, num_cabecalhos, k_usuario);
        int idxConquistas = encontrar_indice(headers_parsed, num_cabecalhos, k_conquistas);
        int idxPublicadoras = encontrar_indice(headers_parsed, num_cabecalhos, k_publicadoras);
        int idxDesenvolvedoras = encontrar_indice(headers_parsed, num_cabecalhos, k_desenvolvedoras);
        int idxCategorias = encontrar_indice(headers_parsed, num_cabecalhos, k_categorias);
        int idxGeneros = encontrar_indice(headers_parsed, num_cabecalhos, k_generos);
        int idxTags = encontrar_indice(headers_parsed, num_cabecalhos, k_tags);

        for (int i = 0; i < num_cabecalhos; i++) free(headers_parsed[i]);
        free(headers_parsed);

        while (fgets(linha, BUFFER_SIZE, arquivo) != NULL && num_jogos_carregados < MAX_JOGOS) {
            char* linha_dados = strdup(linha);
            int num_colunas_parsed = 0;
            char** colunas = separar_csv(linha_dados, &num_colunas_parsed);

            Jogo* g = (Jogo*)malloc(sizeof(Jogo));
            *g = (Jogo){0, strdup(""), strdup(DATA_PADRAO), 0, 0.0f, NULL, 0, -1, -1.0f, 0, NULL, 0, NULL, 0, NULL, 0, NULL, 0, NULL, 0};

            if (num_colunas_parsed > 0) {
                if (idxId >= 0 && idxId < num_colunas_parsed) g->id = tentar_parse_int(limpar_string(colunas[idxId]));
                if (g->id > 0) {
                    if (idxNome >= 0 && idxNome < num_colunas_parsed) { free(g->nome); g->nome = limpar_string(colunas[idxNome]); }
                    if (idxLancamento >= 0 && idxLancamento < num_colunas_parsed) { free(g->dataLancamento); g->dataLancamento = parse_data_lancamento(limpar_string(colunas[idxLancamento])); }
                    if (idxDonos >= 0 && idxDonos < num_colunas_parsed) g->estimativaDonos = parse_estimativa_donos(limpar_string(colunas[idxDonos]));
                    if (idxPreco >= 0 && idxPreco < num_colunas_parsed) g->preco = tentar_parse_float_manual(limpar_string(colunas[idxPreco]), 1);
                    if (idxLinguas >= 0 && idxLinguas < num_colunas_parsed) g->idiomasSuportados = parse_string_array(colunas[idxLinguas], &(g->numIdiomas));
                    if (idxMetacritic >= 0 && idxMetacritic < num_colunas_parsed) g->placarMetacritic = tentar_parse_int(limpar_string(colunas[idxMetacritic]));
                    if (idxPlacarUsuario >= 0 && idxPlacarUsuario < num_colunas_parsed) g->placarUsuario = tentar_parse_float_manual(limpar_string(colunas[idxPlacarUsuario]), 0);
                    if (idxConquistas >= 0 && idxConquistas < num_colunas_parsed) g->conquistas = tentar_parse_int(limpar_string(colunas[idxConquistas]));
                    if (idxPublicadoras >= 0 && idxPublicadoras < num_colunas_parsed) g->publicadoras = parse_string_array(colunas[idxPublicadoras], &(g->numPublicadoras));
                    if (idxDesenvolvedoras >= 0 && idxDesenvolvedoras < num_colunas_parsed) g->desenvolvedoras = parse_string_array(colunas[idxDesenvolvedoras], &(g->numDesenvolvedoras));
                    if (idxCategorias >= 0 && idxCategorias < num_colunas_parsed) g->categorias = parse_string_array(colunas[idxCategorias], &(g->numCategorias));
                    if (idxGeneros >= 0 && idxGeneros < num_colunas_parsed) g->generos = parse_string_array(colunas[idxGeneros], &(g->numGeneros));
                    if (idxTags >= 0 && idxTags < num_colunas_parsed) g->tags = parse_string_array(colunas[idxTags], &(g->numTags));

                    jogos[num_jogos_carregados++] = g;
                } else {
                    liberar_memoria(g);
                    free(g);
                }
                for (int i = 0; i < num_colunas_parsed; i++) free(colunas[i]);
                free(colunas);
            }
            free(linha_dados);
        }
    }
    fclose(arquivo);

    /* Leitura de IDs da entrada padrão: adiciona jogos correspondentes a 'selecionados' */
    Jogo* selecionados[MAX_JOGOS];
    int sel_count = 0;
    while (fgets(linha, BUFFER_SIZE, stdin) != NULL) {
        linha[strcspn(linha, "\n")] = 0;
        linha[strcspn(linha, "\r")] = 0;
        if (strcmp(linha, "FIM") == 0) break;
        int id_para_buscar = tentar_parse_int(linha);
        if (id_para_buscar > 0) {
            for (int i = 0; i < num_jogos_carregados; i++) {
                if (jogos[i]->id == id_para_buscar) {
                    selecionados[sel_count++] = jogos[i];
                    break;
                }
            }
        }
    }

    /* Se não houver selecionados, nada para ordenar/imprimir */
    if (sel_count == 0) {
        /* libera memoria e sai */
        for (int i = 0; i < num_jogos_carregados; i++) {
            liberar_memoria(jogos[i]);
            free(jogos[i]);
        }
        return 0;
    }

    /* Selection Sort sobre selecionados usando nome como chave */
    long long comparacoes = 0;
    long long movimentacoes = 0;
    clock_t inicio = clock();

    for (int i = 0; i < sel_count - 1; ++i) {
        int min_idx = i;
        for (int j = i + 1; j < sel_count; ++j) {
            comparacoes++;
            if (strcmp(selecionados[j]->nome, selecionados[min_idx]->nome) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            Jogo* tmp = selecionados[i];
            selecionados[i] = selecionados[min_idx];
            selecionados[min_idx] = tmp;
            movimentacoes += 3; /* contar 3 movimentações por swap (como pedido) */
        }
        }

    clock_t fim = clock();
    double tempo_exec = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    /* Imprime os selecionados ordenados */
    for (int i = 0; i < sel_count; i++) {
        imprimir_jogo(selecionados[i]);
    }

    /* Gera arquivo de log: MATRICULA_selecao.txt */
    char nome_log[256];
    snprintf(nome_log, sizeof(nome_log), "%s_selecao.txt", MATRICULA);
    FILE *log = fopen(nome_log, "w");
    if (log) {
        fprintf(log, "%s\t%lld\t%lld\t%.6f\n", MATRICULA, comparacoes, movimentacoes, tempo_exec);
        fclose(log);
    } else {
        fprintf(stderr, "Aviso: não foi possível criar o arquivo de log '%s'\n", nome_log);
    }

    /* Libera memória dos jogos carregados */
    for (int i = 0; i < num_jogos_carregados; i++) {
        liberar_memoria(jogos[i]);
        free(jogos[i]);
    }

    return 0;
}
