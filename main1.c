#include <stdio.h>
#include <stdlib.h>

// --- 1. Definição da Estrutura Célula ---
typedef struct Celula {
    int elemento;          // Valor do elemento (a_{i,j})
    int linha;             // Índice da linha
    int coluna;            // Índice da coluna
    struct Celula *sup;    // Ponteiro para a célula superior
    struct Celula *inf;    // Ponteiro para a célula inferior
    struct Celula *esq;    // Ponteiro para a célula esquerda
    struct Celula *dir;    // Ponteiro para a célula direita
} Celula;

// --- 2. Definição da Estrutura Matriz (TAD) ---
typedef struct Matriz {
    int numLinhas;
    int numColunas;
    Celula *inicio;        // Ponteiro para o nó (cabeça) principal [0][0]
    Celula **cabecasLinhas; // Vetor de ponteiros para o início de cada linha
    Celula **cabecasColunas; // Vetor de ponteiros para o início de cada coluna
} Matriz;

// --- 3. Funções de Criação e Alocação ---

/**
 * @brief Cria uma nova Célula, inicializando todos os ponteiros como NULL.
 */
Celula* Nova_Celula(int valor, int i, int j) {
    Celula *nova = (Celula*)malloc(sizeof(Celula));
    if (nova == NULL) {
        perror("Erro ao alocar Celula");
        exit(EXIT_FAILURE);
    }
    nova->elemento = valor;
    nova->linha = i;
    nova->coluna = j;
    nova->sup = nova->inf = nova->esq = nova->dir = NULL;
    return nova;
}

/**
 * @brief Cria o TAD Matriz, incluindo as células-cabeça de cada linha e coluna.
 */
Matriz* Criar_Matriz_TAD(int linhas, int colunas) {
    if (linhas <= 0 || colunas <= 0) {
        fprintf(stderr, "Dimensões inválidas.\n");
        return NULL;
    }

    Matriz *m = (Matriz*)malloc(sizeof(Matriz));
    if (m == NULL) {
        perror("Erro ao alocar Matriz");
        exit(EXIT_FAILURE);
    }
    m->numLinhas = linhas;
    m->numColunas = colunas;

    // Aloca os vetores de cabeças
    m->cabecasLinhas = (Celula**)calloc(linhas, sizeof(Celula*));
    m->cabecasColunas = (Celula**)calloc(colunas, sizeof(Celula*));
    
    if (!m->cabecasLinhas || !m->cabecasColunas) {
        perror("Erro ao alocar cabeças");
        free(m);
        exit(EXIT_FAILURE);
    }

    // Cria as células cabeças de linha e coluna
    for (int i = 0; i < linhas; i++) {
        m->cabecasLinhas[i] = Nova_Celula(0, i, -1); // Coluna -1 para cabeças de linha
    }
    for (int j = 0; j < colunas; j++) {
        m->cabecasColunas[j] = Nova_Celula(0, -1, j); // Linha -1 para cabeças de coluna
    }

    // O início da matriz pode ser a cabeça da linha 0 e coluna 0 (ou um nó extra)
    // Para simplificar, usaremos o cabeçasLinhas[0] como 'inicio'
    m->inicio = m->cabecasLinhas[0]; 

    return m;
}

// --- 4. Função de Inserção (Lógica Complexa de Encadeamento) ---

/**
 * @brief Insere uma nova célula na matriz, conectando-a em 4 direções.
 */
void Inserir(Matriz *m, int valor, int i, int j) {
    if (i >= m->numLinhas || j >= m->numColunas || i < 0 || j < 0) {
        fprintf(stderr, "Tentativa de inserção fora dos limites.\n");
        return;
    }

    Celula *nova = Nova_Celula(valor, i, j);

    // --- CONEXÃO LATERAL (ESQ / DIR) ---
    // Começa na cabeça da linha 'i' e procura o ponto de inserção
    Celula *p = m->cabecasLinhas[i];
    while (p->dir != NULL && p->dir->coluna < j) {
        p = p->dir;
    }
    // Insere entre p e p->dir
    if (p->dir != NULL) p->dir->esq = nova;
    nova->dir = p->dir;
    nova->esq = p;
    p->dir = nova;


    // --- CONEXÃO VERTICAL (SUP / INF) ---
    // Começa na cabeça da coluna 'j' e procura o ponto de inserção
    p = m->cabecasColunas[j];
    while (p->inf != NULL && p->inf->linha < i) {
        p = p->inf;
    }
    // Insere entre p e p->inf
    if (p->inf != NULL) p->inf->sup = nova;
    nova->inf = p->inf;
    nova->sup = p;
    p->inf = nova;
}

// --- 5. Funções de Cálculo e Liberação ---

/**
 * @brief Calcula a soma dos elementos da diagonal principal ($i = j$).
 */
int getDiagonalPri(Matriz *m) { // Método sugerido no material [cite: 52]
    if (m == NULL || m->numLinhas != m->numColunas) return 0;
    
    int soma = 0;
    
    // Percorre cada linha
    for (int i = 0; i < m->numLinhas; i++) {
        // Inicia na cabeça da linha 'i'
        Celula *atual = m->cabecasLinhas[i];
        
        // Percorre a lista horizontalmente até encontrar a coluna 'i'
        while (atual != NULL && atual->coluna < i) {
            atual = atual->dir;
        }
        
        // Se a célula existe e é o elemento da diagonal (coluna == linha)
        if (atual != NULL && atual->coluna == i) {
            printf("Elemento: matriz[%d][%d] = %d\n", i, i, atual->elemento);
            soma += atual->elemento;
        }
    }
    return soma;
}

/**
 * @brief Libera a memória de todas as células e da estrutura Matriz.
 */
void Liberar_Matriz_TAD(Matriz *m) {
    if (m == NULL) return;
    
    // Libera as células de dados (percorrendo linha a linha)
    for (int i = 0; i < m->numLinhas; i++) {
        Celula *celulaAtual = m->cabecasLinhas[i]->dir; // Começa após a cabeça da linha
        while (celulaAtual != NULL) {
            Celula *proximaCelula = celulaAtual->dir;
            free(celulaAtual);
            celulaAtual = proximaCelula;
        }
    }
    
    // Libera as células-cabeça e os vetores de ponteiros
    for (int i = 0; i < m->numLinhas; i++) free(m->cabecasLinhas[i]);
    for (int j = 0; j < m->numColunas; j++) free(m->cabecasColunas[j]);
    
    free(m->cabecasLinhas);
    free(m->cabecasColunas);
    free(m);
}

// --- 6. Main para Teste ---
int main() {
    int n; 
    
    printf("--- TAD Matriz Ortogonal Completa (Células Encadeadas) ---\n");
    printf("Digite a dimensão N (para uma matriz N x N): ");
    
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Dimensão inválida. Encerrando.\n");
        return 1;
    }

    // 1. Cria o TAD Matriz (com as cabeças de linha e coluna)
    Matriz *matrizTAD = Criar_Matriz_TAD(n, n);
    if (matrizTAD == NULL) return 1;

    // 2. Preenchimento (com chamadas à função Inserir)
    printf("\nDigite os elementos da matriz %d x %d:\n", n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int valor;
            printf("Elemento [%d][%d]: ", i, j);
            if (scanf("%d", &valor) != 1) {
                printf("Entrada inválida. Encerrando.\n");
                Liberar_Matriz_TAD(matrizTAD);
                return 1;
            }
            // Inserimos a célula e ela é automaticamente conectada em 4 direções
            Inserir(matrizTAD, valor, i, j);
        }
    }

    // 3. Calcula a soma da diagonal principal
    printf("\nCálculo da Soma da Diagonal Principal:\n");
    int soma_diagonal = getDiagonalPri(matrizTAD);
    
    // Exibe o resultado
    printf("\nA soma dos elementos da diagonal principal é: **%d**\n", soma_diagonal);

    // 4. Libera a memória alocada
    Liberar_Matriz_TAD(matrizTAD);

    return 0;
}