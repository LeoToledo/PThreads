#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <pthread.h>

#define N 100000     //Número de iterações a serem realizadas
int control_a = 0, control_b = 0, control_c = 0, control_d = 0;

//Mutex utilizada para garantir que o compartilhamento de memória vai ocorrer corretamente
pthread_mutex_t mutex;

//Primeiramente, vamos criar uma struct com as variáveis a serem utilizadas
typedef struct BORWEIN
{
    mpf_t a;
    mpf_t b;
    mpf_t c;
    mpf_t d;
    mpf_t e;
    mpf_t pi_aux;
    mpf_t pi;
} BORWEIN;

void *termoA(void *arg); //Calcula ( 4 / (8k + 1) )
void *termoB(void *arg); //Calcula (2 / (8k + 4) )
void *termoC(void *arg); //Calcula ( 1/(8k + 5) )
void *termoD(void *arg); //Calcula ( 1/(8k + 6) )
void *termoE(void *arg); //Calcula ( 1/16k ) * (A - B - C - D)

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Erro na passagem de parametros !");
        return -1;
    }

    FILE *arquivo;
    arquivo = fopen(argv[1], "r");

    BORWEIN argumento;

    mpf_init(argumento.a);
    mpf_init(argumento.b);
    mpf_init(argumento.c);
    mpf_init(argumento.d);
    mpf_init(argumento.e);
    mpf_init(argumento.pi_aux);
    mpf_init(argumento.pi);

    pthread_mutex_init(&mutex, NULL);

    pthread_t t[5];

    pthread_create(&t[0], NULL, termoA, &argumento);
    pthread_create(&t[1], NULL, termoB, &argumento);
    pthread_create(&t[2], NULL, termoC, &argumento);
    pthread_create(&t[3], NULL, termoD, &argumento);
    pthread_create(&t[4], NULL, termoE, &argumento);

    pthread_join(t[0], NULL);
    pthread_join(t[1], NULL);
    pthread_join(t[2], NULL);
    pthread_join(t[3], NULL);
    pthread_join(t[4], NULL);

    gmp_printf("\n\nPi: %.6Ff\n\n", argumento.pi);

    FILE *arqSaida;
    arqSaida = fopen(argv[2], "w");

    fclose(arquivo);
    fclose(arqSaida);
    return 0;
}

void *termoA(void *arg)
{
    BORWEIN *borwein = (BORWEIN *)arg;

    int i = 0;
    while (i < N)
    {
        if (control_a == 0)
        {
            pthread_mutex_lock(&mutex);
            mpf_set_ui(borwein->a, i);
            mpf_mul_ui(borwein->a, borwein->a, 8);
            mpf_add_ui(borwein->a, borwein->a, 1);
            mpf_ui_div(borwein->a, 4, borwein->a);

            control_a = 1;
            i++;
            pthread_mutex_unlock(&mutex);
        }
    }
    return (void *)borwein->a;
}

void *termoB(void *arg)
{
    BORWEIN *borwein = (BORWEIN *)arg;

    int i = 0;
    
    while (i < N)
    {
        if (control_b == 0)
        {
            pthread_mutex_lock(&mutex);
            mpf_set_ui(borwein->b, i);
            mpf_mul_ui(borwein->b, borwein->b, 8);
            mpf_add_ui(borwein->b, borwein->b, 4);
            mpf_ui_div(borwein->b, 2, borwein->b);

            control_b = 1;
            i++;
            pthread_mutex_unlock(&mutex);
        }
    }
    return (void *)borwein->b;
}

void *termoC(void *arg)
{
    BORWEIN *borwein = (BORWEIN *)arg;

    int i = 0;
    while (i < N)
    {
        if (control_c == 0)
        {
            pthread_mutex_lock(&mutex);
            mpf_set_ui(borwein->c, i);
            mpf_mul_ui(borwein->c, borwein->c, 8);
            mpf_add_ui(borwein->c, borwein->c, 5);
            mpf_ui_div(borwein->c, 1, borwein->c);

            control_c = 1;
            i++;
            pthread_mutex_unlock(&mutex);
        }
    }
    return (void *)borwein->c;
}

void *termoD(void *arg)
{
    BORWEIN *borwein = (BORWEIN *)arg;

    int i = 0;
    while (i < N)
    {
        if (control_d == 0)
        {
            pthread_mutex_lock(&mutex);
            mpf_set_ui(borwein->d, i);
            mpf_mul_ui(borwein->d, borwein->d, 8);
            mpf_add_ui(borwein->d, borwein->d, 6);
            mpf_ui_div(borwein->d, 1, borwein->d);

            control_d = 1;
            i++;
            pthread_mutex_unlock(&mutex);
        }
    }
    return (void *)borwein->d;
}

void *termoE(void *arg)
{
    BORWEIN *borwein = (BORWEIN *)arg;

    int i = 0;
    while (i < N)
    {
        if (control_a == 1 && control_b == 1 && control_c == 1 && control_d == 1)
        {
            pthread_mutex_lock(&mutex);
            mpf_set_ui(borwein->e, 16);
            mpf_pow_ui(borwein->e, borwein->e, i);
            mpf_ui_div(borwein->e, 1, borwein->e);

            mpf_sub(borwein->a, borwein->a, borwein->b);
            mpf_sub(borwein->a, borwein->a, borwein->c);
            mpf_sub(borwein->a, borwein->a, borwein->d);

            mpf_mul(borwein->a, borwein->a, borwein->e);
            mpf_set(borwein->pi_aux, borwein->a);
            mpf_add(borwein->pi, borwein->pi, borwein->pi_aux);

            control_a = 0;
            control_b = 0;
            control_c = 0;
            control_d = 0;

            i++;
            pthread_mutex_unlock(&mutex);
        }
    }
    
    return (void *)borwein->pi_aux;
}