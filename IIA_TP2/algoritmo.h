int trepa_colinas(int sol[], int** mat, int m, int g, int num_iter);
int trepa_colinasv2(int sol[], int** mat, int m, int g, int num_iter);
int tc_prob(int sol[], int** mat, int m, int g, int num_iter);



struct info {
    int m;       // numero de elementos
    int g;       // numero de subconjuntos
    int popsize; //tam populacao inicial
    float pm_swap; //prob de mutacao
    float pr;       //prob de cruzamento
    int t_size;        //tam do torneio
    int numGenerations;    //numero de gera��es
    int numTabuDescidas;
};

/* Revolucionario */
// Individuo (solucao)
typedef struct individual chrom, * pchrom;

struct individual {
    int* sol; //solucao
    int fitness; // distancia (qualidade)
};

//void binary_tournament(pchrom pop, struct info d, pchrom parents);
void sized_tournament(pchrom pop, struct info d, pchrom parents);
void genetic_operators(pchrom parents, struct info d, pchrom offspring, int** dist);
void recombination(pchrom parents, struct info d, pchrom offspring, int** dist);
void mutation(struct info d, pchrom offspring);
void mutation_swap(struct info d, int a[]);
void cx_order(int p1[], int p2[], int d1[], int d2[], struct info d);
//void mutation_ins(struct info d, int a[]);