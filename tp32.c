#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct { int ano, mes, dia; } Data;
typedef struct { int hora, minuto; } Hora;

typedef struct {
    int    id;
    char   nome[100];
    char   cidade[100];
    int    capacidade;
    double avaliacao;
    char   tipos_cozinha[200];
    int    faixa_preco;
    Hora   horario_abertura;
    Hora   horario_fechamento;
    Data   data_abertura;
    int    aberto;
} Restaurante;

typedef struct {
    int         tamanho;
    Restaurante restaurantes[500];
} Colecao_Restaurantes;

int sub_idx[500];
int sub_total = 0;

/* Limpa espaços e quebras de linha no início e fim da string */
void trim(char *dest, const char *src) {
    while (*src == ' ' || *src == '\t') src++;
    int len = strlen(src);
    while (len > 0 && (src[len - 1] == ' ' || src[len - 1] == '\t' || src[len - 1] == '\r' || src[len - 1] == '\n')) {
        len--;
    }
    strncpy(dest, src, len);
    dest[len] = '\0';
}

/* Separa de forma robusta as colunas da linha do CSV */
void parse_linha(char *linha, Restaurante *r) {
    char tokens[10][200];
    int t = 0;
    char *start = linha;
    char *p = linha;
    
    while (*p) {
        if (*p == ',') {
            int len = p - start;
            if (len >= 200) len = 199;
            strncpy(tokens[t], start, len);
            tokens[t][len] = '\0';
            t++;
            start = p + 1;
            if (t >= 10) break;
        }
        p++;
    }
    if (t < 10) {
        strcpy(tokens[t], start);
        int len = strlen(tokens[t]);
        if (len > 0 && tokens[t][len-1] == '\n') tokens[t][len-1] = '\0';
        if (len > 1 && tokens[t][len-2] == '\r') tokens[t][len-2] = '\0';
        t++;
    }

    r->id = atoi(tokens[0]);
    trim(r->nome, tokens[1]);
    trim(r->cidade, tokens[2]);
    r->capacidade = atoi(tokens[3]);
    r->avaliacao = atof(tokens[4]);
    // Trata aspas e ponto e vírgula nos tipos de cozinha
    char cozinha_raw[200];
    trim(cozinha_raw, tokens[5]);
    int idx = 0;
    for (int i = 0; cozinha_raw[i] != '\0'; i++) {
        if (cozinha_raw[i] != '"') {
            if (cozinha_raw[i] == ';') {
                r->tipos_cozinha[idx++] = ',';
            } else {
                r->tipos_cozinha[idx++] = cozinha_raw[i];
            }
        }
    }
    r->tipos_cozinha[idx] = '\0';

    char faixa_raw[200];
    trim(faixa_raw, tokens[6]);
    r->faixa_preco = strlen(faixa_raw);

    int h_ab, m_ab, h_fe, m_fe;
    sscanf(tokens[7], "%d:%d-%d:%d", &h_ab, &m_ab, &h_fe, &m_fe);
    r->horario_abertura.hora = h_ab;
    r->horario_abertura.minuto = m_ab;
    r->horario_fechamento.hora = h_fe;
    r->horario_fechamento.minuto = m_fe;

    int ano, mes, dia;
    sscanf(tokens[8], "%d-%d-%d", &ano, &mes, &dia);
    r->data_abertura.ano = ano;
    r->data_abertura.mes = mes;
    r->data_abertura.dia = dia;

    char aberto_raw[200];
    trim(aberto_raw, tokens[9]);
    r->aberto = (aberto_raw[0] == 't') ? 1 : 0;
}

/* Formata a saída do restaurante no padrão esperado */
void formatar_restaurante(Restaurante *r, char *buffer) {
    char faixa[10] = "";
    for (int i = 0; i < r->faixa_preco; i++) {
        strcat(faixa, "$");
    }
    sprintf(buffer, "[%d ## %s ## %s ## %d ## %.1f ## [%s] ## %s ## %02d:%02d-%02d:%02d ## %02d/%02d/%04d ## %s]",
            r->id, r->nome, r->cidade, r->capacidade, r->avaliacao,
            r->tipos_cozinha, faixa,
            r->horario_abertura.hora, r->horario_abertura.minuto,
            r->horario_fechamento.hora, r->horario_fechamento.minuto,
            r->data_abertura.dia, r->data_abertura.mes, r->data_abertura.ano,
            r->aberto ? "true" : "false");
}

/* Carrega o arquivo CSV completo */
void ler_csv_colecao(Colecao_Restaurantes *c, char *path) {
    FILE *f = fopen(path, "r");
    if (f == NULL) { return; }
    char linha[1000];
    if (fgets(linha, sizeof(linha), f) == NULL) { fclose(f); return; }
    c->tamanho = 0;
    while (fgets(linha, sizeof(linha), f) != NULL) {
        parse_linha(linha, &c->restaurantes[c->tamanho]);
        c->tamanho++;
    }
    fclose(f);
}

Colecao_Restaurantes *ler_csv() {
    Colecao_Restaurantes *c = (Colecao_Restaurantes *) malloc(sizeof(Colecao_Restaurantes));
    ler_csv_colecao(c, "/tmp/restaurantes.csv");
    return c;
}

/* Adiciona o índice do restaurante encontrado na subcoleção */
void adicionar_por_id(Colecao_Restaurantes *c, int busca) {
    for (int i = 0; i < c->tamanho; i++) {
        if (c->restaurantes[i].id == busca) {
            sub_idx[sub_total++] = i;
            break;
        }
    }
}

/* Algoritmo de Inserção Parcial com desempate por Nome */
void insercao_parcial(Colecao_Restaurantes *c, int k, long int *comp, long int *mov) {
    *comp = 0;
    *mov  = 0;
    int i, j;

    for (i = 1; i < sub_total; i++) {
        int chave = sub_idx[i];
        
        if (i < k) {
            //  Ordena normalmente os primeiros k elementos
            j = i - 1;
            while (j >= 0) {
                (*comp)++;
                int comp_cidade = strcmp(c->restaurantes[sub_idx[j]].cidade, c->restaurantes[chave].cidade);
                if (comp_cidade > 0 || (comp_cidade == 0 && strcmp(c->restaurantes[sub_idx[j]].nome, c->restaurantes[chave].nome) > 0)) {
                    sub_idx[j + 1] = sub_idx[j];
                    (*mov)++;
                    j--;
                } else {
                    break;
                }
            }
            sub_idx[j + 1] = chave;
        } else {
            //  Avalia se os elementos após k devem entrar no Top-K (posição k-1)
            (*comp)++;
            int comp_cidade = strcmp(c->restaurantes[sub_idx[k - 1]].cidade, c->restaurantes[chave].cidade);
            if (comp_cidade > 0 || (comp_cidade == 0 && strcmp(c->restaurantes[sub_idx[k - 1]].nome, c->restaurantes[chave].nome) > 0)) {
                
                sub_idx[i] = sub_idx[k - 1];
                (*mov)++;
                
                // Desloca elementos internos para abrir espaço para a nova chave menor
                j = k - 2;
                while (j >= 0) {
                    (*comp)++;
                    int comp_cidade_j = strcmp(c->restaurantes[sub_idx[j]].cidade, c->restaurantes[chave].cidade);
                    if (comp_cidade_j > 0 || (comp_cidade_j == 0 && strcmp(c->restaurantes[sub_idx[j]].nome, c->restaurantes[chave].nome) > 0)) {
                        sub_idx[j + 1] = sub_idx[j];
                        (*mov)++;
                        j--;
                    } else {
                        break;
                    }
                }
                sub_idx[j + 1] = chave;
                (*mov)++;
            }
        }
    }
}

int main() {
    Colecao_Restaurantes *colecao = ler_csv();

    int busca;
    while (scanf("%d", &busca) == 1) {
        if (busca == -1) break;
        adicionar_por_id(colecao, busca);
    }

    long int comp, mov;
    clock_t inicio = clock();
    insercao_parcial(colecao, 10, &comp, &mov);
    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC * 1000.0;

    char buffer[600];
    for (int i = 0; i < sub_total; i++) {
        formatar_restaurante(&colecao->restaurantes[sub_idx[i]], buffer);
        printf("%s\n", buffer);
    }
// Gravação  do Log
    FILE *log = fopen("matricula_insercao_parcial.txt", "w");
    if (log != NULL) {
        fprintf(log, "888678\t%ld\t%ld\t%dms\n", comp, mov, (int)tempo);
        fclose(log);
    }

    free(colecao);
    return 0;
}