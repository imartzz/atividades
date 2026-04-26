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

/* Recebe "YYYY-MM-DD" e retorna Data */
Data parse_data(char *s) {
    Data d;
    sscanf(s, "%d-%d-%d", &d.ano, &d.mes, &d.dia);
    return d;
}

/* Escreve data formatada como DD/MM/YYYY no buffer */
void formatar_data(Data *d, char *buffer) {
    sprintf(buffer, "%02d/%02d/%04d", d->dia, d->mes, d->ano);
}

/* Recebe "HH:mm" e retorna Hora */
Hora parse_hora(char *s) {
    Hora h;
    sscanf(s, "%d:%d", &h.hora, &h.minuto);
    return h;
}

/* Escreve hora formatada como HH:mm no buffer */
void formatar_hora(Hora *h, char *buffer) {
    sprintf(buffer, "%02d:%02d", h->hora, h->minuto);
}

/* Troca todas ocorrencias de 'de' por 'para' em s, resultado em dest */
void trocar_char(char *s, char de, char para, char *dest) {
    int i;
    for (i = 0; s[i] != '\0'; i++) dest[i] = (s[i] == de) ? para : s[i];
    dest[i] = '\0';
}
// limpa o /r e /n
void limpar_linha(char *s) {
    int len = strlen(s);
    if (len > 0 && s[len-1] == '\n') s[--len] = '\0';
    if (len > 0 && s[len-1] == '\r') s[--len] = '\0';
}

/* Recebe linha CSV e retorna ponteiro para Restaurante correspondente */
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

/* Escreve restaurante formatado no buffer */
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

/* Le CSV e preenche colecao */
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

/* Le dataset e retorna ponteiro para colecao */
Colecao_Restaurantes *ler_csv() {
    Colecao_Restaurantes *c = (Colecao_Restaurantes *) malloc(sizeof(Colecao_Restaurantes));
    ler_csv_colecao(c, "/tmp/restaurantes.csv");
    return c;
}

/* Ordena por nome via selecao - reutilizada da Q3 */
void selecao(Colecao_Restaurantes *c, long int *comp, long int *mov) {
    int i, j, min;
    *comp = 0; *mov = 0;
    for (i = 0; i < c->tamanho - 1; i++) {
        min = i;
        for (j = i + 1; j < c->tamanho; j++) {
            (*comp)++;
            if (strcmp(c->restaurantes[j].nome, c->restaurantes[min].nome) < 0) min = j;
        }
        if (min != i) {
            Restaurante temp     = c->restaurantes[i];
            c->restaurantes[i]   = c->restaurantes[min];
            c->restaurantes[min] = temp;
            (*mov) += 3;
        }
    }
}

/*
 * Busca binaria pelo nome na subcolecao ja ordenada
 * Retorna 1 se encontrado, 0 caso contrario
 * Conta cada strcmp como 1 comparacao
 */
int binaria(Colecao_Restaurantes *c, char *buscado, long int *comp) {
    int inicio = 0, fim = c->tamanho - 1, achou = 0;
    while (inicio <= fim && !achou) {
        int meio = (inicio + fim) / 2;
        (*comp)++;
        int cmp = strcmp(buscado, c->restaurantes[meio].nome);
        if (cmp == 0) {
            achou = 1;
        } else if (cmp < 0) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
    }
    return achou;
}

/*
 * Le ids (parte 1), monta e ordena subcolecao por nome via selecao
 * le nomes (parte 2) e busca cada um via binaria
 * Gera log com matricula, comparacoes e tempo
 */
int main() {
    Colecao_Restaurantes *colecao = ler_csv();

    int ids[500], n = 0, busca, continuar = 1;
    while (continuar && scanf("%d", &busca) == 1) {
        if (busca == -1) continuar = 0;
        else ids[n++] = busca;
    }

    Colecao_Restaurantes *sub = (Colecao_Restaurantes *) malloc(sizeof(Colecao_Restaurantes));
    sub->tamanho = 0;
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < colecao->tamanho; j++) {
            if (colecao->restaurantes[j].id == ids[i]) {
                sub->restaurantes[sub->tamanho++] = colecao->restaurantes[j];
                break;
            }
        }
    }

    long int comp_sel, mov_sel;
    selecao(sub, &comp_sel, &mov_sel);

    long int comp = 0;
    char nome_buscado[100];
    clock_t inicio = clock();

    scanf(" ");
    while (fgets(nome_buscado, sizeof(nome_buscado), stdin) != NULL) {
        limpar_linha(nome_buscado);
        if (strcmp(nome_buscado, "FIM") == 0) break;

        if (binaria(sub, nome_buscado, &comp)) printf("SIM\n");
        else printf("NAO\n");
    }

    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC * 1000.0;

    FILE *log = fopen("matricula_binaria.txt", "w");
    fprintf(log, "888678\t%ld\t%.2fms\n", comp, tempo);
    fclose(log);

    free(sub);
    free(colecao);
    return 0;
}