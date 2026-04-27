#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Data parse_data(char *s) {
    Data d;
    sscanf(s, "%d-%d-%d", &d.ano, &d.mes, &d.dia);
    return d;
}

void formatar_data(Data *d, char *buffer) {
    sprintf(buffer, "%02d/%02d/%04d", d->dia, d->mes, d->ano);
}

Hora parse_hora(char *s) {
    Hora h;
    sscanf(s, "%d:%d", &h.hora, &h.minuto);
    return h;
}

void formatar_hora(Hora *h, char *buffer) {
    sprintf(buffer, "%02d:%02d", h->hora, h->minuto);
}

void trocar_char(char *s, char de, char para, char *dest) {
    int i;
    for (i = 0; s[i] != '\0'; i++) dest[i] = (s[i] == de) ? para : s[i];
    dest[i] = '\0';
}

Restaurante *parse_restaurante(char *s) {
    Restaurante *r = (Restaurante *) malloc(sizeof(Restaurante));
    char tipos_raw[200], faixa_raw[10], horario_raw[15], data_raw[15], aberto_raw[10];

    sscanf(s, "%d,%99[^,],%99[^,],%d,%lf,%199[^,],%9[^,],%14[^,],%14[^,],%9s",
           &r->id, r->nome, r->cidade, &r->capacidade, &r->avaliacao,
           tipos_raw, faixa_raw, horario_raw, data_raw, aberto_raw);

    trocar_char(tipos_raw, ';', ',', r->tipos_cozinha);
    r->faixa_preco = strlen(faixa_raw);

    char ab[6], fe[6];
    sscanf(horario_raw, "%5[^-]-%5s", ab, fe);
    r->horario_abertura   = parse_hora(ab);
    r->horario_fechamento = parse_hora(fe);
    r->data_abertura      = parse_data(data_raw);
    r->aberto             = (aberto_raw[0] == 't') ? 1 : 0;
    return r;
}

void formatar_restaurante(Restaurante *r, char *buffer) {
    char faixa[6]; int i;
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

void ler_csv_colecao(Colecao_Restaurantes *c, char *path) {
    FILE *f = fopen(path, "r");
    if (f == NULL) { printf("Erro ao abrir arquivo\n"); return; }
    char linha[500];
    fgets(linha, sizeof(linha), f);
    c->tamanho = 0;
    while (fgets(linha, sizeof(linha), f) != NULL) {
        int len = strlen(linha);
        if (linha[len-1] == '\n') linha[len-1] = '\0';
        Restaurante *r = parse_restaurante(linha);
        c->restaurantes[c->tamanho] = *r;
        free(r);
        c->tamanho++;
    }
    fclose(f);
}

Colecao_Restaurantes *ler_csv() {
    Colecao_Restaurantes *c = (Colecao_Restaurantes *) malloc(sizeof(Colecao_Restaurantes));
    ler_csv_colecao(c, "/tmp/restaurantes.csv");
    return c;
}

/* Busca restaurante pelo id e retorna ponteiro, ou NULL se nao encontrado */
Restaurante *buscar_por_id(Colecao_Restaurantes *c, int busca) {
    Restaurante *resp = NULL;
    int i = 0;
    while (i < c->tamanho && resp == NULL) {
        if (c->restaurantes[i].id == busca) resp = &c->restaurantes[i];
        i++;
    }
    return resp;
}

/* ==================== PILHA SEQUENCIAL ==================== */

Restaurante *pilha[1000];
int topo = -1;

/* Empilha restaurante no topo */
void empilhar(Restaurante *r) {
    pilha[++topo] = r;
}

/* desempilha e retorna restaurante do topo */
Restaurante *desempilhar() {
    return pilha[topo--];
}

/*
 *  le ids e empilha cada restaurante
 * Saida: nome de cada desempilhado e pilha final do topo a base.
 */
int main() {
    Colecao_Restaurantes *colecao = ler_csv();

    int busca, continuar = 1;

    /* parte 1: le ids ate -1 e empilha */
    while (continuar && scanf("%d", &busca) == 1) {
        if (busca == -1) {
            continuar = 0;
        } else {
            Restaurante *r = buscar_por_id(colecao, busca);
            if (r != NULL) empilhar(r);
        }
    }

    /*  le n comandos e executa */
    int n, i;
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        char cmd[5];
        scanf("%s", cmd);

        if (strcmp(cmd, "I") == 0) {
            /* empilhar le id e empilha */
            scanf("%d", &busca);
            Restaurante *r = buscar_por_id(colecao, busca);
            if (r != NULL) empilhar(r);

        } else if (strcmp(cmd, "R") == 0) {
            /* desempilhar imprime nome do removido */
            Restaurante *r = desempilhar();
            printf("(R)%s\n", r->nome);
        }
    }

    /* imprime pilha do topo ate a base */
    char buffer[600];
    for (i = topo; i >= 0; i--) {
        formatar_restaurante(pilha[i], buffer);
        printf("%s\n", buffer);
    }

    free(colecao);
    return 0;
}