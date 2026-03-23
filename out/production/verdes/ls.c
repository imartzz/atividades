#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

bool ehVogal(char c) {
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

bool ehConsoante(char c) {
    c = tolower(c);
    return (c >= 'a' && c <= 'z') && !ehVogal(c);
}

bool isVogalRec(const char *s) {
    if (*s == '\0') {
        return true;
    }
    if (!ehVogal(*s)) {
        return false;
    }
    return isVogalRec(s + 1);
}

bool isVogal(const char *s) {
    if (s == NULL || *s == '\0') return false;
    return isVogalRec(s);
}

bool isConsoanteRec(const char *s) {
    if (*s == '\0') {
        return true;
    }
    if (!ehConsoante(*s)) {
        return false;
    }
    return isConsoanteRec(s + 1);
}

bool isConsoante(const char *s) {
    if (s == NULL || *s == '\0') return false;
    return isConsoanteRec(s);
}

bool isInteiroRec(const char *s, int pos) {
    if (s[pos] == '\0') {
        return pos > 0 && (pos > 1 || (s[0] != '-' && s[0] != '+'));
    }
    if (pos == 0 && (s[pos] == '-' || s[pos] == '+')) {
        return isInteiroRec(s, pos + 1);
    }
    if (!isdigit(s[pos])) {
        return false;
    }
    return isInteiroRec(s, pos + 1);
}

bool isInteiro(const char *s) {
    if (s == NULL || *s == '\0') return false;
    return isInteiroRec(s, 0);
}

bool isRealRec(const char *s, int pos, bool pontoEncontrado) {
    if (s[pos] == '\0') {
        return pos > 0 && (pos > 1 || (s[0] != '-' && s[0] != '+')) && s[pos-1] != '.' && s[pos-1] != ',';
    }
    if (pos == 0 && (s[pos] == '-' || s[pos] == '+')) {
        return isRealRec(s, pos + 1, pontoEncontrado);
    }
    if (s[pos] == '.' || s[pos] == ',') {
        if (pontoEncontrado) {
            return false;
        }
        return isRealRec(s, pos + 1, true);
    }
    if (!isdigit(s[pos])) {
        return false;
    }
    return isRealRec(s, pos + 1, pontoEncontrado);
}

bool isReal(const char *s) {
    if (s == NULL || *s == '\0') return false;
    return isRealRec(s, 0, false);
}

int main() {
    char entrada[1000];
    while (fgets(entrada, sizeof(entrada), stdin) != NULL) {
        entrada[strcspn(entrada, "\n")] = 0;
        if (strcmp(entrada, "FIM") == 0) {
            break;
        }
        printf("%s ", isVogal(entrada) ? "SIM" : "NAO");
        printf("%s ", isConsoante(entrada) ? "SIM" : "NAO");
        printf("%s ", isInteiro(entrada) ? "SIM" : "NAO");
        printf("%s\n", isReal(entrada) ? "SIM" : "NAO");
    }
    return 0;
}