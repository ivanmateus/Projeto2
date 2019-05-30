#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
unsigned char typedef bool;
#define TRUE  1
#define FALSE 0

int* ler_inteiros(const char * arquivo, const int n)
{
    FILE* f = fopen(arquivo, "r");

    int * inteiros = (int *) malloc(sizeof(int) * n);

    for (int i = 0; !feof(f); i++)
        fscanf(f, "%d\n", &inteiros[i]);

    fclose(f);

    return inteiros;
}

void inicia_tempo()
{
    srand(time(NULL));
    _ini = clock();
}

double finaliza_tempo()
{
    _fim = clock();
    return ((double) (_fim - _ini)) / CLOCKS_PER_SEC;
}

int main(int argc, char const *argv[])
{
    const int N = 50000;
    unsigned encontrados = 0;
    int achou = 0; //flag para saber se o numero ja foi encontrado

    int* entradas = ler_inteiros("inteiros_entrada.txt", N);
    int* consultas = ler_inteiros("inteiros_busca.txt", N);

    // realiza busca sequencia com realocação
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // buscar o elemento consultas[i] na entrada
        achou = 0; //supoe-se que nao foi encontrado inicialmente
        for (int j = 0; j < N && !achou; ++j){ //enquanto nao chegar ao fim e nao achar
            if(consultas[i] == entradas[j]){    //se achou
                ++encontrados;  //incrementa os encontrados
                if(j != 0){ //se nao estiver na primeira posicao, manda o valor para a posicao anterior
                    int temp = entradas[j];
                    entradas[j] = entradas[j - 1];
                    entradas[j - 1] = temp;
                }
                achou = 1; //informa que achou
            }
        }
    }

    double tempo_busca = finaliza_tempo();

    printf("Tempo de busca    :\t%fs\n", tempo_busca);
    printf("Itens encontrados :\t%d\n", encontrados);

    return 0;
}
