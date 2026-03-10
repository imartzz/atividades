#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* -------------------- Estruturas -------------------- */

typedef struct
{
    int id;
    char *name;
    char *releaseDate; // "DD/MM/YYYY"
    int estimatedOwners;
    float price;
    char *supportedLanguages; // "[A, B, C]"
    int metacriticScore;
    float userScore;
    int achievements;
    char *publishers;
    char *developers;
    char *categories;
    char *genres;
    char *tags;
} Game;

typedef struct Celula
{
    const Game *elemento;
    struct Celula *prox;
} Celula;

static Celula *primeiro;
static Celula *ultimo;

/* -------------------- Utilitários -------------------- */
// Funções auxiliares para manipulação de strings e memória, para ajustar a saida conforme o esperado ( que estava dando erro).

static void *xmalloc(size_t n)
{
    void *p = malloc(n);
    if (!p)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    return p;
}

static char *dupstr(const char *s)
{
    if (!s)
        return NULL;
    size_t L = strlen(s);
    char *r = (char *)xmalloc(L + 1);
    memcpy(r, s, L + 1);
    return r;
}

static void trim_inplace(char *s)
{
    if (!s)
        return;
    size_t i = 0, j = strlen(s);
    while (i < j && (s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\n'))
        i++;
    while (j > i && (s[j - 1] == ' ' || s[j - 1] == '\t' || s[j - 1] == '\r' || s[j - 1] == '\n'))
        j--;
    if (i > 0)
        memmove(s, s + i, j - i);
    s[j - i] = '\0';
}

static int ieq3(const char *a, const char *b)
{ // case-insensitive 3 letras
    return tolower((unsigned char)a[0]) == tolower((unsigned char)b[0]) &&
           tolower((unsigned char)a[1]) == tolower((unsigned char)b[1]) &&
           tolower((unsigned char)a[2]) == tolower((unsigned char)b[2]);
}

static const char *mesParaNumero(const char *m)
{ // usa ctype
    if (!m || !m[0])
        return "00";
    if (ieq3(m, "Jan"))
        return "01";
    if (ieq3(m, "Feb"))
        return "02";
    if (ieq3(m, "Mar"))
        return "03";
    if (ieq3(m, "Apr"))
        return "04";
    if (ieq3(m, "May"))
        return "05";
    if (ieq3(m, "Jun"))
        return "06";
    if (ieq3(m, "Jul"))
        return "07";
    if (ieq3(m, "Aug"))
        return "08";
    if (ieq3(m, "Sep"))
        return "09";
    if (ieq3(m, "Oct"))
        return "10";
    if (ieq3(m, "Nov"))
        return "11";
    if (ieq3(m, "Dec"))
        return "12";
    return "00";
}

/* Converte "Aug 13, 2018" OU "13 Aug 2018" em "13/08/2018". Limpa aspas, vírgula e CR/LF. */
static char *formatarData(const char *original)
{
    if (!original)
        return dupstr("");

    char buf[128];
    size_t k = 0;
    for (const char *p = original; *p && k < sizeof(buf) - 1; ++p)
    {
        if (*p == '"' || *p == '\'' || *p == '\r' || *p == '\n')
            continue;
        buf[k++] = (*p == ',') ? ' ' : *p;
    }
    buf[k] = '\0';
    trim_inplace(buf);

    char a[8] = {0}, b[8] = {0}, c[8] = {0};
    if (sscanf(buf, "%7s %7s %7s", a, b, c) == 3)
    {
        const char *mm = "00";
        char dd[3] = {0};

        if (isalpha((unsigned char)a[0]))
        { // "Mon DD YYYY"
            mm = mesParaNumero(a);
            if (strlen(b) == 1)
            {
                dd[0] = '0';
                dd[1] = b[0];
                dd[2] = '\0';
            }
            else
            {
                strncpy(dd, b, 2);
                dd[2] = '\0';
            }
            // c = ano
        }
        else
        { // "DD Mon YYYY"
            if (strlen(a) == 1)
            {
                dd[0] = '0';
                dd[1] = a[0];
                dd[2] = '\0';
            }
            else
            {
                strncpy(dd, a, 2);
                dd[2] = '\0';
            }
            mm = mesParaNumero(b);
            // c = ano
        }

        char out[16];
        snprintf(out, sizeof(out), "%s/%s/%s", dd, mm, c);
        return dupstr(out);
    }
    return dupstr(buf);
}

/* Normaliza listas: remove colchetes/aspas, divide por vírgula, faz trim e reconstrói em "[A, B, C]". */
static char *normalizarListaCampo(const char *s)
{
    if (!s)
        return dupstr("[]");

    // Copia removendo colchetes e aspas
    char tmp[4096];
    size_t w = 0;
    for (const char *p = s; *p && w < sizeof(tmp) - 1; ++p)
    {
        if (*p == '[' || *p == ']' || *p == '"' || *p == '\'')
            continue;
        tmp[w++] = *p;
    }
    tmp[w] = '\0';

    // Tokeniza por vírgula
    char *buf = dupstr(tmp);
    char *p = buf;
    char out[4096];
    out[0] = '\0';
    strcat(out, "[");

    int first = 1;
    while (p && *p)
    {
        char *comma = strchr(p, ',');
        if (comma)
            *comma = '\0';
        trim_inplace(p);
        if (*p)
        {
            if (!first)
                strcat(out, ", ");
            strcat(out, p);
            first = 0;
        }
        if (!comma)
            break;
        p = comma + 1;
    }
    strcat(out, "]");

    free(buf);
    return dupstr(out);
}

/* -------------------- Lista encadeada -------------------- */

static Celula *novaCelula(const Game *g)
{
    Celula *c = (Celula *)xmalloc(sizeof(Celula));
    c->elemento = g;
    c->prox = NULL;
    return c;
}

static void iniciarLista(void)
{
    primeiro = novaCelula(NULL);
    ultimo = primeiro;
}

/* Faltavam estas quatro funções usadas no main: */
static void inserirInicio(const Game *g)
{
    Celula *x = novaCelula(g);
    x->prox = primeiro->prox;
    primeiro->prox = x;
    if (ultimo == primeiro)
        ultimo = x;
}

static void inserirPosicao(const Game *g, int pos)
{
    Celula *i = primeiro;
    for (int j = 0; j < pos; ++j)
        i = i->prox; // assume pos válida conforme enunciado
    Celula *x = novaCelula(g);
    x->prox = i->prox;
    i->prox = x;
    if (i == ultimo)
        ultimo = x;
}

static const Game *removerInicio(void)
{
    if (primeiro == ultimo)
        return NULL;
    Celula *a = primeiro->prox;
    const Game *r = a->elemento;
    primeiro->prox = a->prox;
    if (a == ultimo)
        ultimo = primeiro;
    free(a);
    return r;
}

static const Game *removerPosicao(int pos)
{
    if (primeiro == ultimo)
        return NULL;
    Celula *i = primeiro;
    for (int j = 0; j < pos; ++j)
    {
        if (!i->prox)
            return NULL;
        i = i->prox;
    }
    Celula *a = i->prox;
    if (!a)
        return NULL;
    const Game *r = a->elemento;
    i->prox = a->prox;
    if (a == ultimo)
        ultimo = i;
    free(a);
    return r;
}

/* Já existiam no seu código e foram mantidos exatamente: */
static void inserirFim(const Game *g)
{
    ultimo->prox = novaCelula(g);
    ultimo = ultimo->prox;
}

static const Game *removerFim(void)
{
    if (primeiro == ultimo)
        return NULL;
    Celula *i = primeiro;
    while (i->prox != ultimo)
        i = i->prox;
    const Game *r = ultimo->elemento;
    free(ultimo);
    ultimo = i;
    ultimo->prox = NULL;
    return r;
}

/* -------------------- CSV / Parser -------------------- */

/* Divide uma linha CSV em 14 campos, respeitando aspas (sem tratar aspas duplas escapadas). */
static int dividirCsvComAspas(const char *line, char **fields, int max_fields)
{
    int count = 0, inq = 0;
    const char *p = line, *ini = line;

    while (*p && count < max_fields)
    {
        char c = *p;
        if (c == '"')
            inq = !inq;
        else if (c == ',' && !inq)
        {
            size_t n = (size_t)(p - ini);
            char *cel = (char *)xmalloc(n + 1);
            memcpy(cel, ini, n);
            cel[n] = '\0';
            trim_inplace(cel);
            // remove aspas externas se presentes
            if (cel[0] == '"' && cel[strlen(cel) - 1] == '"')
            {
                cel[strlen(cel) - 1] = '\0';
                memmove(cel, cel + 1, strlen(cel));
            }
            fields[count++] = cel;
            ini = p + 1;
        }
        p++;
    }
    if (count < max_fields)
    {
        size_t n = (size_t)(p - ini);
        char *cel = (char *)xmalloc(n + 1);
        memcpy(cel, ini, n);
        cel[n] = '\0';
        trim_inplace(cel);
        if (cel[0] == '"' && cel[strlen(cel) - 1] == '"')
        {
            cel[strlen(cel) - 1] = '\0';
            memmove(cel, cel + 1, strlen(cel));
        }
        fields[count++] = cel;
    }
    while (count < max_fields)
        fields[count++] = dupstr("");
    return count;
}

static void preencherGameDosCampos(Game *g, char **f)
{
    g->id = atoi(f[0]);
    g->name = dupstr(f[1]);
    g->releaseDate = formatarData(f[2]);

    // estimatedOwners: mantém apenas dígitos (pega a parte "baixa" ex.: "20000-50000" -> "20000")
    char dig[64];
    int k = 0;
    for (const char *p = f[3]; *p && k < 63; ++p)
        if (isdigit((unsigned char)*p))
            dig[k++] = *p;
    dig[k] = '\0';
    g->estimatedOwners = k ? atoi(dig) : 0;

    // price (vírgula -> ponto)
    char *preco = dupstr(f[4]);
    for (char *q = preco; *q; ++q)
        if (*q == ',')
            *q = '.';
    g->price = (*preco) ? (float)atof(preco) : 0.0f;
    free(preco);

    g->supportedLanguages = normalizarListaCampo(f[5]);
    g->metacriticScore = (*f[6]) ? atoi(f[6]) : 0;

    char *us = dupstr(f[7]);
    for (char *q = us; *q; ++q)
        if (*q == ',')
            *q = '.';
    g->userScore = (*us) ? (float)atof(us) : 0.0f;
    free(us);

    g->achievements = (*f[8]) ? atoi(f[8]) : 0;

    g->publishers = normalizarListaCampo(f[9]);
    g->developers = normalizarListaCampo(f[10]);
    g->categories = normalizarListaCampo(f[11]);
    g->genres = normalizarListaCampo(f[12]);
    g->tags = normalizarListaCampo(f[13]);
}

static Game *carregarCsv(size_t *out_n)
{
    *out_n = 0;
    FILE *f = fopen("/tmp/games.csv", "r");
    if (!f)
        return NULL;

    char line[20000];
    if (!fgets(line, sizeof(line), f))
    {
        fclose(f);
        return NULL;
    } // pula cabeçalho

    size_t cap = 1024, n = 0;
    Game *V = (Game *)xmalloc(sizeof(Game) * cap);

    while (fgets(line, sizeof(line), f))
    {
        char *fields[14] = {0};
        dividirCsvComAspas(line, fields, 14);

        if (n == cap)
        {
            cap *= 2;
            V = (Game *)realloc(V, sizeof(Game) * cap);
            if (!V)
            {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }
        preencherGameDosCampos(&V[n], fields);

        for (int i = 0; i < 14; i++)
            free(fields[i]);
        n++;
    }
    fclose(f);
    *out_n = n;
    return V;
}

static const Game *buscarPorId(const Game *V, size_t n, int id)
{
    for (size_t i = 0; i < n; i++)
        if (V[i].id == id)
            return &V[i];
    return NULL;
}

/* -------------------- Saída / Exibição -------------------- */

static void imprimir_jogo(const Game *j)
{
    printf("=> %d ## %s ## %s ## %d ## %.2f ## %s ## %d ## %.1f ## %d ## %s ## %s ## %s ## %s ## %s ##\n",
           j->id,
           j->name ? j->name : "",
           j->releaseDate ? j->releaseDate : "",
           j->estimatedOwners,
           j->price,
           j->supportedLanguages ? j->supportedLanguages : "[]",
           j->metacriticScore,
           j->userScore,
           j->achievements,
           j->publishers ? j->publishers : "[]",
           j->developers ? j->developers : "[]",
           j->categories ? j->categories : "[]",
           j->genres ? j->genres : "[]",
           j->tags ? j->tags : "[]");
}

static void mostrarLista(void)
{
    int idx = 0;
    for (Celula *i = primeiro->prox; i; i = i->prox)
    {
        printf("[%d] ", idx++);
        imprimir_jogo(i->elemento);
    }
}

/* -------------------- Main -------------------- */

int main(void)
{
    size_t n = 0;
    Game *jogos = carregarCsv(&n);

    iniciarLista();

    // Lê IDs até "FIM"
    char buf[256];
    while (fgets(buf, sizeof(buf), stdin))
    {
        trim_inplace(buf);
        if (strcasecmp(buf, "FIM") == 0)
            break;
        int id = atoi(buf);
        const Game *g = buscarPorId(jogos, n, id);
        if (g)
            inserirFim(g);
    }

    // Lê Q (apenas uma vez) e processa comandos: I <id> e R
    int q = 0;
    if (scanf("%d%*c", &q) != 1)
        q = 0;

    for (int i = 0; i < q; i++)
    {
        if (!fgets(buf, sizeof(buf), stdin))
            break;
        trim_inplace(buf);

        if (buf[0] == 'I')
        {
            int id = 0;
            if (sscanf(buf + 1, "%d", &id) == 1)
            {
                const Game *g = buscarPorId(jogos, n, id);
                if (g)
                    inserirFim(g);
            }
        }
        else if (buf[0] == 'R')
        {
            const Game *r = removerFim();
            if (r && r->name)
                printf("(R) %s\n", r->name);
        }
    }

    // separa as linhas de remoção da listagem final
    mostrarLista();

    // Libera memória
    for (size_t i = 0; i < n; i++)
    {
        Game *g = &jogos[i];
        free(g->name);
        free(g->releaseDate);
        free(g->supportedLanguages);
        free(g->publishers);
        free(g->developers);
        free(g->categories);
        free(g->genres);
        free(g->tags);
    }
    free(jogos);

    Celula *c = primeiro;
    while (c)
    {
        Celula *nx = c->prox;
        free(c);
        c = nx;
    }

    return 0;
}
