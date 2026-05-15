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
    Data d; sscanf(s, "%d-%d-%d", &d.ano, &d.mes, &d.dia); return d;
}
void formatar_data(Data *d, char *buffer) {
    sprintf(buffer, "%02d/%02d/%04d", d->dia, d->mes, d->ano);
}
Hora parse_hora(char *s) {
    Hora h; sscanf(s, "%d:%d", &h.hora, &h.minuto); return h;
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
        free(r); c->tamanho++;
    }
    fclose(f);
}

Colecao_Restaurantes *ler_csv() {
    Colecao_Restaurantes *c = (Colecao_Restaurantes *) malloc(sizeof(Colecao_Restaurantes));
    ler_csv_colecao(c, "/tmp/restaurantes.csv");
    return c;
}


int sub_idx[501];
int sub_n = 0;
long int comp_g = 0, mov_g = 0;

/*
 * compara data_abertura dos restaurantes sub_idx[i] e sub_idx[j].
 * Criterio: ano, mes, dia e  empate desempata pelo nome
 */
int comparar(Colecao_Restaurantes *c, int i, int j) {
    comp_g++;
    Restaurante *ri = &c->restaurantes[sub_idx[i]];
    Restaurante *rj = &c->restaurantes[sub_idx[j]];
    int resp;
    if      (ri->data_abertura.ano != rj->data_abertura.ano)
        resp = ri->data_abertura.ano - rj->data_abertura.ano;
    else if (ri->data_abertura.mes != rj->data_abertura.mes)
        resp = ri->data_abertura.mes - rj->data_abertura.mes;
    else if (ri->data_abertura.dia != rj->data_abertura.dia)
        resp = ri->data_abertura.dia - rj->data_abertura.dia;
    else { comp_g++; resp = strcmp(ri->nome, rj->nome); }
    return resp;
}

/* troca sub_idx[i] com sub_idx[j], conta 3 movimentacoes */
void trocar(int i, int j) {
    int temp = sub_idx[i]; sub_idx[i] = sub_idx[j]; sub_idx[j] = temp;
    mov_g += 3;
}

/* Retorna true se no i tem filho no heap de tamanho tam */
int has_filho(int i, int tam) { return (i <= (tam / 2)); }

/* retorna indice do maior filho do no i no heap de tamanho tam */
int get_maior_filho(Colecao_Restaurantes *c, int i, int tam) {
    int filho;
    if (2*i == tam || comparar(c, 2*i, 2*i+1) > 0) filho = 2*i;
    else filho = 2*i + 1;
    return filho;
}

/* Insere elemento na posicao tam no heap, sobe ate posicao correta */
void construir(Colecao_Restaurantes *c, int tam) {
    int i;
    for (i = tam; i > 1 && comparar(c, i, i/2) > 0; i /= 2) trocar(i, i/2);
}

/* reconstroi heap de tamanho tam, desce a raiz ate posicao correta */
void reconstruir(Colecao_Restaurantes *c, int tam) {
    int i = 1;
    while (has_filho(i, tam)) {
        int filho = get_maior_filho(c, i, tam);
        if (comparar(c, i, filho) < 0) { trocar(i, filho); i = filho; }
        else i = tam;
    }
}

/*
 * Heapsort parcial k=10
 *  constroi heap com todos os n elementos
 *  extrai apenas k elementos — os k menores ficam nas
 *         primeiras posicoes em ordem crescente
 */
void heapsort_parcial(Colecao_Restaurantes *c, int k) {
    int tam, i;
    for (tam = 2; tam <= sub_n; tam++) construir(c, tam);
    tam = sub_n; i = 0;
    while (tam > 1 && i < k) { trocar(1, tam--); reconstruir(c, tam); i++; }
}

/*
 * le ids da entrada, ordena parcialmente por data_abertura (k=10)
 * via heapsort e imprime todos os elementos Gera arquivo de log
 */
int main() {
    Colecao_Restaurantes *colecao = ler_csv();

    int busca, continuar = 1;
    while (continuar && scanf("%d", &busca) == 1) {
        if (busca == -1) { continuar = 0; }
        else {
            int i = 0, achou = 0;
            while (i < colecao->tamanho && !achou) {
                if (colecao->restaurantes[i].id == busca) {
                    sub_idx[++sub_n] = i; achou = 1;
                }
                i++;
            }
        }
    }

    clock_t inicio = clock();
    heapsort_parcial(colecao, 10);
    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC * 1000.0;

    char buffer[600]; int i;
    for (i = 1; i <= sub_n; i++) {
        formatar_restaurante(&colecao->restaurantes[sub_idx[i]], buffer);
        printf("%s\n", buffer);
    }

    FILE *log = fopen("matricula_heapsort_parcial.txt", "w");
    fprintf(log, "888678\t%ld\t%ld\t%.2fms\n", comp_g, mov_g, tempo);
    fclose(log);

    free(colecao);
    return 0;
}