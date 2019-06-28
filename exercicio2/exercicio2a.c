#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // funções strcmp e strcpy
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

int main(int argc, char const* argv[]) {
    unsigned N = 50000;
    unsigned M = 70000;
    unsigned B = 50021;

    unsigned colisoes_h_div = 0;
    unsigned colisoes_h_mul = 0;

    unsigned encontrados_h_div = 0;
    unsigned encontrados_h_mul = 0;

    string* insercoes = ler_strings("strings_entrada.txt", N);
    string* consultas = ler_strings("strings_busca.txt", M);

    int inseriu = 0;
    int nao_achou = 0;

    // cria tabela hash com hash por divisão
    string* tabelaDiv = (string*)malloc(sizeof(string) * B);
    for (int i = 0; i < B; ++i) {
        tabelaDiv[i] = "";
    }

    // inserção dos dados na tabela hash usando hash por divisão
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
        inseriu = 0;
        for (int j = 0; j < B && !inseriu; ++j) {
            int pos = h_div(converter(insercoes[i]), j, B);
            if (strcmp(tabelaDiv[pos], "") == 0) {
                tabelaDiv[pos] = insercoes[i];
                if (j != 0) {
                    ++colisoes_h_div;
                }
                inseriu = 1;
            } else if (strcmp(tabelaDiv[pos], insercoes[i]) == 0) {
                inseriu = 1;
            }
        }
    }
    double tempo_insercao_h_div = finaliza_tempo();

    // consulta dos dados na tabela hash usando hash por divisão
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
        nao_achou = 0;
        for (int j = 0; j < B && !nao_achou; ++j) {
            if (consultas[i] != NULL) {
                int pos = h_div(converter(consultas[i]), j, B);
                if (strcmp(tabelaDiv[pos], consultas[i]) == 0) {
                    ++encontrados_h_div;
                }
                if (strcmp(tabelaDiv[pos], "") == 0) {
                    nao_achou = 1;
                }
            }
        }
    }

    double tempo_busca_h_div = finaliza_tempo();

    // limpa a tabela hash com hash por divisão
    for (int i = 0; i < B; ++i) {
        tabelaDiv[i] = "";
    }
    free(tabelaDiv);

    // cria tabela hash com hash por multiplicação
    string* tabelaMul = (string*)malloc(sizeof(string) * B);
    for (int i = 0; i < B; ++i) {
        tabelaMul[i] = "";
    }

    // inserção dos dados na tabela hash usando hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
        inseriu = 0;
        for (int j = 0; j < B && !inseriu; ++j) {
            int pos = h_mul(converter(insercoes[i]), j, B);
            if (strcmp(tabelaMul[pos], "") == 0) {
                tabelaMul[pos] = insercoes[i];
                if (j != 0) {
                    ++colisoes_h_mul;
                }
                inseriu = 1;
            } else if (strcmp(tabelaMul[pos], insercoes[i]) == 0) {
                inseriu = 1;
            }
        }
    }
    double tempo_insercao_h_mul = finaliza_tempo();

    // busca dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
        nao_achou = 0;
        for (int j = 0; j < B && !nao_achou; ++j) {
            if (consultas[i] != NULL) {
                int pos = h_mul(converter(consultas[i]), j, B);
                if (strcmp(tabelaMul[pos], consultas[i]) == 0) {
                    ++encontrados_h_mul;
                }
                if (strcmp(tabelaMul[pos], "") == 0) {
                    nao_achou = 1;
                }
            }
        }
    }
    double tempo_busca_h_mul = finaliza_tempo();

    // limpa a tabela hash com hash por multiplicação
    for (int i = 0; i < B; ++i) {
        tabelaMul[i] = "";
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
