/*
*   TP2 -> Introdu��o a Intelig�ncia Art�ficial - 2020-2021
*   Trabalho realizado por:
*       Filipe Morgado.:  2019137625
*       Andr� Domingues.: 2019127839
*/
#pragma once
int** init_dados(char* nome, int* m, int* g);
void gera_sol_inicial(int* sol, int m, int g);
void escreve_sol(int* sol, int m, int g);

int obter_distancia(int** dist, int m, int a, int b);
void copia(int a[], int b[], int n);
void troca(int* sol, int a, int b);

//Gerar aleatoridade

void init_rand();
int random_l_h(int min, int max);
float rand_01();




// Computa��o Evolucinaria

pchrom init_pop(struct info d, int** dist);
pchrom init_parents(struct info d);
void evaluate(pchrom pop, struct info d, int** dist);
void get_best(pchrom pop, struct info d, pchrom best);

void atribui(pchrom a, chrom b, struct info d);
int flip();