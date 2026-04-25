#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Representa uma data no calendario */
typedef struct {
    int ano;
    int mes;
    int dia;
} Data;

/* Representa um horario do dia */
typedef struct {
    int hora;
    int minuto;
} Hora;

/* Representa um restaurante */
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
    int    aberto; /* 1 = true, 0 = false */
} Restaurante;

/* Colecao de restaurantes */
typedef struct {
    int         tamanho;
    Restaurante restaurantes[500];
} Colecao_Restaurantes;

/* Recebe string "YYYY-MM-DD" e retorna struct Data correspondente */
Data parse_data(char *s) {
    Data d;
    sscanf(s, "%d-%d-%d", &d.ano, &d.mes, &d.dia);
    return d;
}

/* Escreve no buffer a data formatada como DD/MM/YYYY */
void formatar_data(Data *d, char *buffer) {
    sprintf(buffer, "%02d/%02d/%04d", d->dia, d->mes, d->ano);
}

/* Recebe string "HH:mm" e retorna struct Hora correspondente */
Hora parse_hora(char *s) {
    Hora h;
    sscanf(s, "%d:%d", &h.hora, &h.minuto);
    return h;
}

/* Escreve no buffer a hora formatada como HH:mm */
void formatar_hora(Hora *h, char *buffer) {
    sprintf(buffer, "%02d:%02d", h->hora, h->minuto);
}

/* Troca todas as ocorrencias de 'de' por 'para' em s, resultado em dest */
void trocar_char(char *s, char de, char para, char *dest) {
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        dest[i] = (s[i] == de) ? para : s[i];
    }
    dest[i] = '\0';
}

/*
 * Recebe uma linha do CSV e retorna ponteiro para Restaurante correspondente
 */
Restaurante *parse_restaurante(char *s) {
    Restaurante *r = (Restaurante *) malloc(sizeof(Restaurante));

    char tipos_raw[200];
    char faixa_raw[10];
    char horario_raw[15];
    char data_raw[15];
    char aberto_raw[10];

    sscanf(s, "%d,%99[^,],%99[^,],%d,%lf,%199[^,],%9[^,],%14[^,],%14[^,],%9s",
           &r->id, r->nome, r->cidade, &r->capacidade, &r->avaliacao,
           tipos_raw, faixa_raw, horario_raw, data_raw, aberto_raw);

    /* tipos_cozinha: troca ';' por ',' */
    trocar_char(tipos_raw, ';', ',', r->tipos_cozinha);

    /* faixa_preco: conta os '$' */
    r->faixa_preco = strlen(faixa_raw);

    /* horario: divide "HH:mm-HH:mm" em abertura e fechamento */
    char ab[6], fe[6];
    sscanf(horario_raw, "%5[^-]-%5s", ab, fe);
    r->horario_abertura   = parse_hora(ab);
    r->horario_fechamento = parse_hora(fe);

    r->data_abertura = parse_data(data_raw);

    /* aberto: 1 se comecar com 't', 0 caso contrario */
    r->aberto = (aberto_raw[0] == 't') ? 1 : 0;

    return r;
}

/*
 * Escreve no buffer o restaurante formatado como:
 * [id ## nome ## cidade ## cap ## aval ## [tipos] ## faixa ## ab-fe ## data ## aberto]
 */
void formatar_restaurante(Restaurante *r, char *buffer) {
    char faixa[6];
    int i;
    for (i = 0; i < r->faixa_preco; i++) faixa[i] = '$';
    faixa[i] = '\0';

    char hab[6], hfe[6], data[12];
    formatar_hora(&r->horario_abertura, hab);
    formatar_hora(&r->horario_fechamento, hfe);
    formatar_data(&r->data_abertura, data);

    sprintf(buffer, "[%d ## %s ## %s ## %d ## %.1f ## [%s] ## %s ## %s-%s ## %s ## %s]",
            r->id, r->nome, r->cidade, r->capacidade, r->avaliacao,
            r->tipos_cozinha, faixa, hab, hfe, data,
            r->aberto ? "true" : "false");
}

/*
 * Le o arquivo CSV e preenche a colecao
 */
void ler_csv_colecao(Colecao_Restaurantes *c, char *path) {
    FILE *f = fopen(path, "r");
    if (f == NULL) { printf("Erro ao abrir arquivo\n"); return; }

    char linha[500];
    fgets(linha, sizeof(linha), f); /* descarta cabecalho */

    c->tamanho = 0;
    while (fgets(linha, sizeof(linha), f) != NULL) {
        int len = strlen(linha);
        if (linha[len - 1] == '\n') linha[len - 1] = '\0';

        Restaurante *r = parse_restaurante(linha);
        c->restaurantes[c->tamanho] = *r;
        free(r);
        c->tamanho++;
    }
    fclose(f);
}

/* Le o dataset do caminho padrao e retorna ponteiro para colecao */
Colecao_Restaurantes *ler_csv() {
    Colecao_Restaurantes *c = (Colecao_Restaurantes *) malloc(sizeof(Colecao_Restaurantes));
    ler_csv_colecao(c, "/tmp/restaurantes.csv");
    return c;
}

/* Busca por id e retorna ponteiro para o restaurante, ou NULL se nao encontrado */
Restaurante *buscar_por_id(Colecao_Restaurantes *c, int busca) {
    Restaurante *resp = NULL;
    int i;
    for (i = 0; i < c->tamanho && resp == NULL; i++) {
        if (c->restaurantes[i].id == busca) resp = &c->restaurantes[i];
    }
    return resp;
}

/*
 * Le ids da entrada padrao e imprime o restaurante correspondente
 * Encerra ao receber -1
 */
int main() {
    Colecao_Restaurantes *colecao = ler_csv();

    int busca, continuar = 1;
    while (continuar && scanf("%d", &busca) == 1) {
        if (busca == -1) {
            continuar = 0;
        } else {
            Restaurante *r = buscar_por_id(colecao, busca);
            if (r != NULL) {
                char buffer[600];
                formatar_restaurante(r, buffer);
                printf("%s\n", buffer);
            }
        }
    }

    free(colecao);
    return 0;
}