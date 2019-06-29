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

unsigned h_div(unsigned x, unsigned i, unsigned B) { return ((x % B) + i) % B; }

unsigned h_mul(unsigned x, unsigned i, unsigned B) {
    const double A = 0.6180;
    return ((int)((fmod(x * A, 1) * B) + i)) % B;
}

unsigned h_second(unsigned x, unsigned i, unsigned B) {
    return (h_div(x, i, B) + i * h_mul(x, i, B)) % B;
}

int main(int argc, char const* argv[]) {
    const int N = 50000;
    const int M = 70000;
    const int B = 50021;

    unsigned colisoes = 0;
    unsigned encontrados = 0;

    string* insercoes = ler_strings("strings_entrada.txt", N);
    string* consultas = ler_strings("strings_busca.txt", M);

    int inseriu = 0;
    int nao_achou = 0;

    // cria tabela hash com hash por hash duplo
    string* tabela = (string*)malloc(sizeof(string) * B);
    for (int i = 0; i < B; ++i) {
        tabela[i] = "";
    }

    // inserção dos dados na tabela hash
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
        inseriu = 0;
        int pos = h_mul(converter(insercoes[i]), 0, B);
        if (strcmp(tabela[pos], "") == 0) {
            tabela[pos] = insercoes[i];
        } else {
            for (int j = 1; j < B && !inseriu; ++j) {
                pos = h_second(converter(insercoes[i]), j, B);
                if (strcmp(tabela[pos], "") == 0) {
                    tabela[pos] = insercoes[i];
                    ++colisoes;
                    inseriu = 1;
                } else if (strcmp(tabela[pos], insercoes[i]) == 0) {
                    inseriu = 1;
                }
            }
        }
    }
    double tempo_insercao = finaliza_tempo();

    // busca dos dados na tabela hash
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
        nao_achou = 0;
        if (consultas[i] != NULL) {
            int pos = h_mul(converter(consultas[i]), 0, B);
            if (strcmp(tabela[pos], consultas[i]) == 0) {
                ++encontrados;
            } else {
                for (int j = 1; j < B && !nao_achou; ++j) {
                    pos = h_second(converter(consultas[i]), j, B);
                    if (strcmp(tabela[pos], consultas[i]) == 0) {
                        ++encontrados;
                    }
                    if (strcmp(tabela[pos], "") == 0) {
                        nao_achou = 1;
                    }
                }
            }
        }
    }
    double tempo_busca = finaliza_tempo();

    free(tabela);

    printf("Colisões na inserção: %d\n", colisoes);
    printf("Tempo de inserção   : %fs\n", tempo_insercao);
    printf("Tempo de busca      : %fs\n", tempo_busca);
    printf("Itens encontrados   : %d\n", encontrados);

    return 0;
}
