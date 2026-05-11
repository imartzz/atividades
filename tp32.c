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

int sub_idx[500];
int sub_total = 0;

/* Busca id na colecao e adiciona indice na subcolecao */
void adicionar_por_id(Colecao_Restaurantes *c, int busca) {
    int i = 0, achou = 0;
    while (i < c->tamanho && !achou) {
        if (c->restaurantes[i].id == busca) {
            sub_idx[sub_total++] = i;
            achou = 1;
        }
        i++;
    }
}

/*
 * Insercao parcial com k=10: faz apenas k iteracoes do loop externo
 * Os k primeiros ficam ordenados por cidade, o restante permanece
 * na ordem original Imprime todos os sub_total elementos
 */
void insercao_parcial(Colecao_Restaurantes *c, int k, long int *comp, long int *mov) {
    int i, j;
    int limite = k < sub_total ? k : sub_total;
    *comp = 0;
    *mov  = 0;

    for (i = 1; i <= limite; i++) {
        int chave = sub_idx[i];
        j = i - 1;
        (*comp)++;
        while (j >= 0 && strcmp(c->restaurantes[sub_idx[j]].cidade,
                                c->restaurantes[chave].cidade) > 0) {
            sub_idx[j + 1] = sub_idx[j];
            (*mov)++;
            j--;
            if (j >= 0) (*comp)++;
        }
        sub_idx[j + 1] = chave;
    }
}

/*
 * Le ids da entrada, ordena parcialmente por cidade (k=10)
 * via insercao e imprime todos os elementos
 * Gera arquivo de log
 */
int main() {
    Colecao_Restaurantes *colecao = ler_csv();

    int busca, continuar = 1;
    while (continuar && scanf("%d", &busca) == 1) {
        if (busca == -1) continuar = 0;
        else adicionar_por_id(colecao, busca);
    }

    long int comp, mov;
    clock_t inicio = clock();
    insercao_parcial(colecao, 10, &comp, &mov);
    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC * 1000.0;

    char buffer[600];
    int i;
    for (i = 0; i < sub_total; i++) {
        formatar_restaurante(&colecao->restaurantes[sub_idx[i]], buffer);
        printf("%s\n", buffer);
    }

    FILE *log = fopen("matricula_insercao_parcial.txt", "w");
    fprintf(log, "888678\t%ld\t%ld\t%.2fms\n", comp, mov, tempo);
    fclose(log);

    free(colecao);
    return 0;
}