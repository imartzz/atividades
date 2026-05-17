#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_RESTAURANTES 500
#define MAX_STRING 200

// Contadores globais para o arquivo de log
int comparacoes = 0;
int movimentacoes = 0;

// Estrutura para representar um Restaurante
typedef struct {
    int id;
    char nome[MAX_STRING];
    char cidade[MAX_STRING];
    int capacidade;
    double avaliacao;
    char tiposCozinha[MAX_STRING];
    int faixaPreco;
    int horaAb, minAb, horaFe, minFe;
    int dataAno, dataMes, dataDia;
    bool aberto;
} Restaurante;

// Array global para armazenar a base de dados lida do CSV
Restaurante todosRestaurantes[MAX_RESTAURANTES];
int totalRestaurantes = 0;

// estrutura da Célula para a Lista 
typedef struct Celula {
    Restaurante elemento;
    struct Celula* prox;
} Celula;

// Ponteiros para controle da lista flexível
Celula* primeiro;
Celula* ultimo;

// cria uma nova célula na memória
Celula* novaCelula(Restaurante r) {
    Celula* c = (Celula*)malloc(sizeof(Celula));
    c->elemento = r;
    c->prox = NULL;
    return c;
}

// Inicializa a lista com uma célula cabeça
void startLista() {
    Restaurante tmp;
    memset(&tmp, 0, sizeof(Restaurante));
    primeiro = novaCelula(tmp);
    ultimo = primeiro;
}

// insere um elemento no final da lista encadeada
void inserirFim(Restaurante r) {
    ultimo->prox = novaCelula(r);
    ultimo = ultimo->prox;
}

/* Remove espaços em branco extras antes e depois da string */
void trim(char* dest, const char* src) {
    while (*src == ' ' || *src == '\t') {
        src++;
    }
    int len = strlen(src);
    while (len > 0 && (src[len - 1] == ' ' || src[len - 1] == '\t' || src[len - 1] == '\r' || src[len - 1] == '\n')) {
        len--;
    }
    strncpy(dest, src, len);
    dest[len] = '\0';
}

/* Parseia e extrai as informações de uma linha do arquivo CSV */
void parseLinha(char* linha) {
    char tokens[10][MAX_STRING];
    int t = 0;
    char* start = linha;
    char* p = linha;
    
    while (*p) {
        if (*p == ',') {
            int len = p - start;
            strncpy(tokens[t], start, len);
            tokens[t][len] = '\0';
            t++;
            start = p + 1;
        }
        p++;
    }
    strcpy(tokens[t], start);
    int len = strlen(tokens[t]);
    if (len > 0 && tokens[t][len-1] == '\n') tokens[t][len-1] = '\0';
    if (len > 1 && tokens[t][len-2] == '\r') tokens[t][len-2] = '\0';
    t++;

    Restaurante* r = &todosRestaurantes[totalRestaurantes];

    r->id = atoi(tokens[0]);
    trim(r->nome, tokens[1]);
    trim(r->cidade, tokens[2]);
    r->capacidade = atoi(tokens[3]);
    r->avaliacao = atof(tokens[4]);
    
    char cozinhaRaw[MAX_STRING];
    trim(cozinhaRaw, tokens[5]);
    int idxC = 0;
    for (int i = 0; cozinhaRaw[i] != '\0'; i++) {
        if (cozinhaRaw[i] != '"') {
            if (cozinhaRaw[i] == ';') {
                r->tiposCozinha[idxC++] = ',';
            } else {
                r->tiposCozinha[idxC++] = cozinhaRaw[i];
            }
        }
    }
    r->tiposCozinha[idxC] = '\0';
    
    char faixaRaw[MAX_STRING];
    trim(faixaRaw, tokens[6]);
    r->faixaPreco = strlen(faixaRaw);
    
    sscanf(tokens[7], "%d:%d-%d:%d", &r->horaAb, &r->minAb, &r->horaFe, &r->minFe);
    sscanf(tokens[8], "%d-%d-%d", &r->dataAno, &r->dataMes, &r->dataDia);
    
    char abertoRaw[MAX_STRING];
    trim(abertoRaw, tokens[9]);
    r->aberto = (abertoRaw[0] == 't');
    
    totalRestaurantes++;
}

/* Lê o arquivo dataset CSV */
void lerCsv() {
    FILE* file = fopen("/tmp/restaurantes.csv", "r");
    if (file == NULL) {
        return;
    }
    char linha[1000];
    if (fgets(linha, sizeof(linha), file) == NULL) {
        fclose(file);
        return;
    }
    while (fgets(linha, sizeof(linha), file) != NULL) {
        parseLinha(linha);
    }
    fclose(file);
}

/* formata a saida textual de um restaurante */
void formatarRestaurante(Restaurante r, char* resultado) {
    char faixa[10] = "";
    for (int j = 0; j < r.faixaPreco; j++) {
        strcat(faixa, "$");
    }
    
    char strAberto[6];
    if (r.aberto) {
        strcpy(strAberto, "true");
    } else {
        strcpy(strAberto, "false");
    }
    
    sprintf(resultado, "[%d ## %s ## %s ## %d ## %.1f ## [%s] ## %s ## %02d:%02d-%02d:%02d ## %02d/%02d/%04d ## %s]",
            r.id, r.nome, r.cidade, r.capacidade, r.avaliacao,
            r.tiposCozinha, faixa,
            r.horaAb, r.minAb, r.horaFe, r.minFe,
            r.dataDia, r.dataMes, r.dataAno, strAberto);
}

/* Imprime todos os elementos válidos da lista encadeada */
void mostrarLista() {
    char buffer[1000];
    for (Celula* i = primeiro->prox; i != NULL; i = i->prox) {
        formatarRestaurante(i->elemento, buffer);
        printf("%s\n", buffer);
    }
}

/* algoritmo de Ordenação por Seleção*/
void selectionSort() {
    for (Celula* i = primeiro->prox; i != NULL && i->prox != NULL; i = i->prox) {
        Celula* menor = i;
        for (Celula* j = i->prox; j != NULL; j = j->prox) {
            comparacoes++;
            if (strcmp(j->elemento.nome, menor->elemento.nome) < 0) {
                menor = j;
            }
        }
        if (menor != i) {
            // permuta os dados internos das células
            Restaurante temp = i->elemento;
            i->elemento = menor->elemento;
            menor->elemento = temp;
            movimentacoes += 3;
        }
    }
}

int main() {
    lerCsv();
    startLista();
    
    int idBusca;
    // preenche a lista com base nos IDs fornecidos na entrada até ler -1
    while (scanf("%d", &idBusca) == 1) {
        if (idBusca == -1) {
            break;
        }
        for (int i = 0; i < totalRestaurantes; i++) {
            if (todosRestaurantes[i].id == idBusca) {
                inserirFim(todosRestaurantes[i]);
                break;
            }
        }
    }
    
    // Medição do tempo de execução do algoritmo
    clock_t inicio = clock();
    selectionSort();
    clock_t fim = clock();
    
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000.0;
    
    // Imprime o resultado final ordenado
    mostrarLista();
    
    // geração do arquivo de log 
    FILE* log = fopen("matricula.selecao_flexivel.txt", "w");
    if (log != NULL) {
        fprintf(log, "888678\t%d\t%d\t%dms", comparacoes, movimentacoes, (int)tempo);
        fclose(log);
    }
    
    // liberação da memória 
    Celula* atual = primeiro;
    while (atual != NULL) {
        Celula* prox = atual->prox;
        free(atual);
        atual = prox;
    }
    
    return 0;
}