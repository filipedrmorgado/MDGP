/*
*   TP2 -> Introdu��o a Intelig�ncia Art�ficial - 2020-2021
*   Trabalho realizado por:
*       Filipe Morgado.:  2019137625
*       Andr� Domingues.: 2019127839
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "algoritmo.h"
#include "funcao.h"
#include "utils.h"

#define DEFAULT_RUNS 30

enum TipoDeAlgoritmo
{
    //Local
	algorTrepaColinas,
    algorTrepaColinas2viz,
    algorTrepaColinasProb,
    algorRecristalizacaoSimulada,
    algorTabu,
    //Evolucionario
    algorEvolucionario,
    //Hibrido
    algorHibridoTrepaSimples,
    algorHibridoTrepa2viz,
    algorHibridoTabu
};


int main(int argc, char* argv[])
{
	char    nome_fich[100];
	int     m, g, num_iteracoes, j, runs, custo = 0, best_custo = 0, invalidos = 0;;
	int  * sol, * best, **distancia, i;
	float   mbf = 0.0;
    char nome_algor[100];

    //Local
	enum TipoDeAlgoritmo algoritmo;
	struct info parameters;

    /*Revolucionario*/
    int gen_actual;
    chrom best_run, best_ever;
    pchrom pop = NULL, parents = NULL;

	// L� os argumentos de entrada
    while (1) {
        mbf = 0.0;
        if (argc == 3)
        {
            runs = atoi(argv[2]);
            strcpy(nome_fich, argv[1]);
        }
        else
            // Se o n�mero de execu��es do processo n�o for colocado nos argumentos de entrada, define-o com um valor por defeito
            if (argc == 2)
            {
                runs = DEFAULT_RUNS;
                strcpy(nome_fich, argv[1]);
            }
             // Se o nome do ficheiro de informa��es n�o for colocado nos argumentos de entrada, pede-o novamente
            else
            {
                runs = DEFAULT_RUNS;
                printf("Nome do Ficheiro: ");
                if (scanf("%s", &nome_fich) != 1)
                    return 0;;
                //fgets(nome_fich, sizeof(nome_fich), stdin);

            }

        // Se o n�mero de execu��es do processo for menor ou igual a 0, termina o programa
        if (runs <= 0)
            return 0;

        init_rand();
        //Algoritmo a ser usado no teste
        algoritmo = algorEvolucionario;
        //Numero de itera��es a serem feitas
        num_iteracoes = 100;
        if (argc == 4)
            parameters.numTabuDescidas = atoi(argv[3]);
        else
            parameters.numTabuDescidas = 15;

        /* Evolutivo */
        //Parametros
        parameters.numGenerations = 2500;
        parameters.popsize = 100;
        parameters.pm_swap = 0.01f;
        parameters.pr = 0.3f;
        parameters.t_size = 5;


        //Leitura de Ficheiro e obtemcao de distancia
        distancia = init_dados(nome_fich, &m, &g);
        parameters.m = m; //Nr Elementos
        parameters.g = g; //Nr Sub-conjuntos

        printf("Elementos: %d\n", m);
        printf("Sub-conjuntos: %d\n", g);

        switch (algoritmo)
        {
        case algorTrepaColinas:
        case algorTrepaColinas2viz:
        case algorTrepaColinasProb:
        case algorRecristalizacaoSimulada:
        case algorTabu:
            /*case algTrepaColinasProb:*/
            sol = malloc(sizeof(int) * m);
            best = malloc(sizeof(int) * m);

            if (sol == NULL || best == NULL)
            {
                printf("Erro na alocacao de memoria");
                exit(1);
            }
            for (j = 0; j < runs; j++)
            {
                // Gera solucao inicial
                gera_sol_inicial(sol, m, g);

                switch (algoritmo)
                {
                case algorTrepaColinas:
                    // Trepa colinas simples
                    strcpy(nome_algor, "Trepa Colinas");
                    custo = trepa_colinas(sol, distancia, parameters, num_iteracoes);
                    break;
                case algorTrepaColinas2viz:
                    // Trepa colinas 2 vizinhos
                    strcpy(nome_algor, "Trepa Colinas 2 Vizinhos");
                    custo = trepa_colinas2viz(sol, distancia, parameters, num_iteracoes);
                    break;
                case  algorTrepaColinasProb: 
                    // Trepa Colinas Probabilistico
                    strcpy(nome_algor, "Trepa Colinas Probabilistico");
                    custo = trepaColinas_probabilistico(sol, distancia, parameters, num_iteracoes);
                    break;
                case algorRecristalizacaoSimulada:
                    // Trepa colinas Recristaliza��o Simulada
                    strcpy(nome_algor, "Recristalizacao simulada");
                    custo = recristalizacao_simulada(sol, distancia, parameters, num_iteracoes);
                    break;
                case algorTabu:
                    // Tabu
                    strcpy(nome_algor, "Tabu");
                    custo = pesquisa_tabu(sol, distancia, parameters, num_iteracoes, 1);
                    break;
                default:
                    exit(0);
                }
                // Escreve resultados da repeticao k
                printf("\nRepeticao numero %d:", j);
                escreve_sol(sol, m, g);
                printf("Custo final da repeticao: %2d\n", custo);

                mbf += custo;
                if (j == 0 || best_custo < custo)
                {
                    best_custo = custo;
                    copia(best, sol, m);
                }
            }
            // Escreve resultados globais
            printf("\n\nMBF: %f\n", mbf / j);
            printf("\nMelhor solucao encontrada foi:\n");
            escreve_sol(best, m, g);
            printf("Melhor custo final: %2d\n\n", best_custo);
            // Libertar memoria
            free(sol);
            free(best);
            break;
            
        //Evolucionario
        case algorEvolucionario:
        //Hibridos
        case algorHibridoTrepaSimples:
        case algorHibridoTrepa2viz:
        case algorHibridoTabu:
            strcpy(nome_algor, "Evolucionario");

            best_run.sol = malloc(sizeof(int) * m);
            best_ever.sol = malloc(sizeof(int) * m);
        

            for (j = 0; j < runs; j++)
            {
                pop = init_pop(parameters, distancia);
                // Avalia a qualidade da populacao
                evaluate(pop, parameters, distancia);
                // Encontra-se a melhor solu��o dentro de toda a popula��o
                atribui(&best_run, pop[0], parameters);
                // Inicializar a melhor solucao encontrada
                get_best(pop, parameters, &best_run);
                // Reservar espaco para os pais
                parents = init_parents(parameters);
                gen_actual = 1;
                // Loop evolucionario
                while (gen_actual <= parameters.numGenerations)
                {
                    // Torneio binario para encontrar os progenitores (ficam armazenados no vector parents)
                    tournament_geral(pop, parameters, parents);
                    // Aplicar operadores geneticos aos pais (os descendentes ficam armazenados no vector pop)
                    genetic_operators(parents, parameters, pop, distancia);
                    // Reavaliar a qualidade da populacao
                    evaluate(pop, parameters, distancia);
                    // Actualizar a melhor solucao encontrada
                    get_best(pop, parameters, &best_run);
                    gen_actual++;
                }
                // HIBRIDO: tentar melhorar solu��o com pesquisa local
                if (algoritmo == algorHibridoTrepaSimples)
                {
                    strcpy(nome_algor, "Genetico por torneio + trepa colinas");
                    // Trepa colinas simples
                    best_run.fitness = trepa_colinas(best_run.sol, distancia, parameters, 1000); /*Utilizando 1000 iteracoes*/
                }
                if (algoritmo == algorHibridoTrepa2viz)
                {
                    strcpy(nome_algor, "Genetico por torneio + trepa colinas 2 vizinhos");
                    // Trepa colinas Melhorado
                    best_run.fitness = trepa_colinas2viz(best_run.sol, distancia, parameters, 1000); /*Utilizando 1000 iteracoes*/
                }
                if (algoritmo == algorHibridoTabu)
                {
                    strcpy(nome_algor, "Genetico por torneio + Tabu");
                    // Trepa colinas Probabilistico
                    best_run.fitness = pesquisa_tabu(best_run.sol, distancia, parameters, num_iteracoes, 1); /*Utilizando 1000 iteracoes*/
                }
              /*  for (invalidos = 0, i = 0; i < parameters.popsize; i++)
                    if (pop[i].valido == 0)
                        invalidos++;*/

                // Escreve resultados da repeticao que terminou
                printf("\nRepeticao numero %d:", j);
                escreve_sol(best_run.sol, m, g);
                printf("Custo final da repeticao: %2d\n", best_run.fitness);

                /*printf("\nINVALIDOS percentagem: %f", 100 * (float)invalidos / parameters.popsize);
                printf("\tQUANTIDADE DE INVALIDOS: %d\n", invalidos);*/

                mbf += best_run.fitness;
                if (j == 0 || best_ever.fitness < best_run.fitness)
                {
                    atribui(&best_ever, best_run, parameters);
                }
                // Liberta��o de memoria da popula��o
                for (i = 0; i < parameters.popsize; i++)
                    free(pop[i].sol);
                free(pop);
                // Liberta��o de memoria dos pais
                for (i = 0; i < parameters.popsize; i++)
                    free(parents[i].sol);
                free(parents);
                 //invalidos = 0;
            }

            // Escreve resultados globais
            printf("\n\nMBF: %f\n", mbf / j);
            printf("\nMelhor solucao encontrada foi:\n");
            escreve_sol(best_ever.sol, m, g);
            printf("Melhor custo final: %2d\n", best_ever.fitness);
            free(best_run.sol);
            free(best_ever.sol);
            break;
        }

        for (i = 0; i < (m - 1); i++)
            free(distancia[i]);
        free(distancia);
    }
    return 0;
}