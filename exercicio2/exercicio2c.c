#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
typedef unsigned char bool;
#define TRUE 1
#define FALSE 0

// Definição do tipo string
typedef char* string;

#define MAX_STRING_LEN 20

typedef struct list {
    string data;
    struct list* next;
} List;

unsigned converter(string s) {
    unsigned h = 0;
    for (int i = 0; s[i] != '\0'; i++) h = h * 256 + s[i];
    return h;
}

string* ler_strings(const char* arquivo, const int n) {
    FILE* f = fopen(arquivo, "r");

    string* strings = (string*)malloc(sizeof(string) * n);

    for (int i = 0; !feof(f); i++) {
        strings[i] = (string)malloc(sizeof(char) * MAX_STRING_LEN);
        fscanf(f, "%s\n", strings[i]);
    }

    fclose(f);

    return strings;
}

void inicia_tempo() {
    srand(time(NULL));
    _ini = clock();
}

double finaliza_tempo() {
    _fim = clock();
    return ((double)(_fim - _ini)) / CLOCKS_PER_SEC;
}

unsigned h_div(unsigned x, unsigned B) { return x % B; }

unsigned h_mul(unsigned x, unsigned B) {
    const double A = 0.6180;
    return fmod(x * A, 1) * B;
}

List* insert(List* head, string data) {
    List* curr = (List*)malloc(sizeof(List));
    if (curr == NULL) {
        exit(0);  // no memory available
    }
    curr->data = data;
    curr->next = head;
    head = curr;
    return head;
}

List* free_list(List* head) {
    List* prev = head;
    List* curr;
    while (prev != NULL) {
        curr = prev;
        prev = prev->next;
        free(curr);
    }
    return NULL;
}

int main(int argc, char const* argv[]) {
    const int N = 50000;
    const int M = 70000;
    const int B = 50021;

    unsigned colisoes_h_div = 0;
    unsigned colisoes_h_mul = 0;

    unsigned encontrados_h_div = 0;
    unsigned encontrados_h_mul = 0;

    string* insercoes = ler_strings("strings_entrada.txt", N);
    string* consultas = ler_strings("strings_busca.txt", M);

    // cria tabela hash com hash por divisão
    List** tabelaDiv = (List**)malloc(sizeof(List*) * B);
    for (int i = 0; i < B; ++i) {
        tabelaDiv[i] = NULL;
    }

    // inserção dos dados na tabela hash com hash por divisão
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
        int pos = h_div(converter(insercoes[i]), B);
        if (tabelaDiv[pos] != NULL) ++colisoes_h_div;
        tabelaDiv[pos] = insert(tabelaDiv[pos], insercoes[i]);
    }
    double tempo_insercao_h_div = finaliza_tempo();

    // busca dos dados na tabela hash com hash por divisão
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
        List* curr;
        if (consultas[i] != NULL) {
            int pos = h_div(converter(consultas[i]), B);
            curr = tabelaDiv[pos];
            while (curr != NULL) {
                if (strcmp(curr->data, consultas[i]) == 0) {
                    ++encontrados_h_div;
                    break;
                };
                curr = curr->next;
            }
        }
    }
    double tempo_busca_h_div = finaliza_tempo();

    // destroi tabela hash com hash por divisão
    for (int i = 0; i < B; i++) {
        tabelaDiv[i] = free_list(tabelaDiv[i]);
    }
    free(tabelaDiv);

    // cria tabela hash com hash por multiplicação
    List** tabelaMul = (List**)malloc(sizeof(List*) * B);
    for (int i = 0; i < B; ++i) {
        tabelaMul[i] = NULL;
    }

    // inserção dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
        int pos = h_mul(converter(insercoes[i]), B);
        if (tabelaMul[pos] != NULL) ++colisoes_h_mul;
        tabelaMul[pos] = insert(tabelaMul[pos], insercoes[i]);
    }
    double tempo_insercao_h_mul = finaliza_tempo();

    // busca dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
        List* curr;
        if (consultas[i] != NULL) {
            int pos = h_mul(converter(consultas[i]), B);
            curr = tabelaMul[pos];
            while (curr != NULL) {
                if (strcmp(curr->data, consultas[i]) == 0) {
                    ++encontrados_h_mul;
                    break;
                };
                curr = curr->next;
            }
        }
    }
    double tempo_busca_h_mul = finaliza_tempo();

    // destroi tabela hash com hash por multiplicação
    for (int i = 0; i < B; i++) {
        tabelaMul[i] = free_list(tabelaMul[i]);
    }
    free(tabelaMul);

    printf("Hash por Divisão\n");
    printf("Colisões na inserção: %d\n", colisoes_h_div);
    printf("Tempo de inserção   : %fs\n", tempo_insercao_h_div);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_div);
    printf("Itens encontrados   : %d\n", encontrados_h_div);
    printf("\n");
    printf("Hash por Multiplicação\n");
    printf("Colisões na inserção: %d\n", colisoes_h_mul);
    printf("Tempo de inserção   : %fs\n", tempo_insercao_h_mul);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_mul);
    printf("Itens encontrados   : %d\n", encontrados_h_mul);

    return 0;
}
