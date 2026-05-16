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
    int          tamanho;
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
    r->aberto             = (aberto_raw[0] == 't' || aberto_raw[0] == 'T') ? 1 : 0;
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
    fgets(linha, sizeof(linha), f); // ignorar cabeçalho
    c->tamanho = 0;
    while (fgets(linha, sizeof(linha), f) != NULL) {
        int len = strlen(linha);
        if (linha[len-1] == '\n') linha[len-1] = '\0';
        if (linha[len-2] == '\r') linha[len-2] = '\0';
        Restaurante *r = parse_restaurante(linha);
        c->restaurantes[c->tamanho] = *r;
        free(r); c->tamanho++;
    }
    fclose(f);
}

Restaurante buscar_restaurante(Colecao_Restaurantes *c, int id) {
    for (int i = 0; i < c->tamanho; i++) {
        if (c->restaurantes[i].id == id) {
            return c->restaurantes[i];
        }
    }
    Restaurante vazio = {0};
    return vazio;
}

typedef struct Celula {
    Restaurante restaurante;
    struct Celula* prox;
} Celula;

typedef struct {
    Celula* primeiro;
    Celula* ultimo;
} Fila;

Fila fila;

void iniciarFila() {
    fila.primeiro = (Celula*) malloc(sizeof(Celula));
    fila.primeiro->prox = NULL;
    fila.ultimo = fila.primeiro;
}

// inserção no final da fila Enfileirar
void inserir(Restaurante r) {
    fila.ultimo->prox = (Celula*) malloc(sizeof(Celula));
    fila.ultimo = fila.ultimo->prox;
    fila.ultimo->restaurante = r;
    fila.ultimo->prox = NULL;
}

// Remoção no início da fila Desenfileirar
Restaurante remover() {
    if (fila.primeiro == fila.ultimo) {
        Restaurante vazio = {0}; return vazio;
    }
    Celula* tmp = fila.primeiro->prox;
    fila.primeiro->prox = tmp->prox;
    Restaurante resp = tmp->restaurante;
    if (fila.ultimo == tmp) {
        fila.ultimo = fila.primeiro;
    }
    free(tmp);
    return resp;
}

// mostra os elementos do primeiro ao último inserido sem os índices 
void mostrarFila() {
    char buffer[600];
    for (Celula* i = fila.primeiro->prox; i != NULL; i = i->prox) {
        formatar_restaurante(&i->restaurante, buffer);
        printf("%s\n", buffer);
    }
}


int main() {
    Colecao_Restaurantes colecao;
    ler_csv_colecao(&colecao, "/tmp/restaurantes.csv");

    iniciarFila();

    //  Carga inicial da fila até ler -1
    int busca;
    while (scanf("%d", &busca) == 1 && busca != -1) {
        Restaurante r = buscar_restaurante(&colecao, busca);
        if (r.id != 0) {
            inserir(r);
        }
    }

    // Processamento da quantidade N 
    int n;
    if (scanf("%d", &n) == 1) {
        for (int i = 0; i < n; i++) {
            char comando[5];
            scanf("%s", comando);

            if (strcmp(comando, "I") == 0) {
                int id; scanf("%d", &id);
                inserir(buscar_restaurante(&colecao, id));
            } else if (strcmp(comando, "R") == 0) {
                Restaurante r = remover();
                if (r.id != 0) {
                    printf("(R)%s\n", r.nome);
                }
            }
        }
    }

    //  Imprime o restante que sobrou na Fila
    mostrarFila();

    return 0;
}