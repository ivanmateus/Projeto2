#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
unsigned char typedef bool;
#define TRUE  1
#define FALSE 0

void merge_sort(int *l, int *copia, int ini, int fim) {
    int l1, l2, lc;
    int meio;
    if (ini == fim) // criterio de parada
        return; // vetor com um soh elemento
    // ordena metades inferior e superior
    meio = ini + (fim - ini) / 2;
    merge_sort(l, copia, ini, meio); // metade inferior
    merge_sort(l, copia, meio + 1, fim); // metade superior
    // faz o merge das listas
    l1 = ini;
    lc = ini;
    l2 = meio + 1;
    while (l1 <= meio && l2 <= fim){
        copia[lc++] = (l[l1] < l[l2]) ? l[l1++] : l[l2++];
    }
    for (; l1 <= meio; l1++, lc++){
        copia[lc] = l[l1];
    }
    for (; l2 <= fim; l2++, lc++){
        copia[lc] = l[l2];
    }
    // passa a copia ordenada sobre o vetor original
    for (lc = ini; lc <= fim; lc++) {
        l[lc] = copia[lc];
    }
    return;
}

int* ler_inteiros(const char * arquivo, const int n)
{
    FILE* f = fopen(arquivo, "r");

    int* inteiros = (int *) malloc(sizeof(int) * n);

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

int busca_tabela(int *tabela, int *entradas, int valor, int N, int index_size){
    int j = 0;
    while(j < (N / index_size) && valor >= entradas[tabela[j]]){
        ++j;
    }
    return tabela[j - 1];
}

int main(int argc, char const *argv[])
{
    const int N = 50000;
    const int index_size = 10000;
    unsigned encontrados = 0;

    int* entradas = ler_inteiros("inteiros_entrada.txt", N);
    int* consultas = ler_inteiros("inteiros_busca.txt", N);
    int* copia = (int *) malloc(sizeof(int) * N);
    int* indexTable = (int *)malloc(sizeof(int) * (N / index_size));
    int achou = 0; //flag para saber se o numero ja foi encontrado

    // ordenar entrada
    merge_sort(entradas, copia, 0, N - 1);

    // criar tabela de indice
    for(int k = 0; k < (N / index_size); ++k){
        indexTable[k] = (k == 0 ? 0 : (index_size * k) - 1);
    }
    
    // realizar consultas na tabela de indices
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // buscar o elemento consultas[i] na entrada
        achou = 0; //supoe-se que nao foi encontrado inicialmente  
        int j = busca_tabela(indexTable, entradas, consultas[i], N, index_size); //procura o indice pelo qual a busca vai se iniciar
        for(; j < N && !achou; ++j){ //enquanto nao chegar ao fim e nao achar
            if(consultas[i] == entradas[j]){    //se achou
                ++encontrados;  //incrementa os encontrados
                achou = 1; //informa que achou
            }
        }
    }
    
    double tempo_busca = finaliza_tempo();

    printf("Tempo de busca    :\t%fs\n", tempo_busca);
    printf("Itens encontrados :\t%d\n", encontrados);

    return 0;
}
