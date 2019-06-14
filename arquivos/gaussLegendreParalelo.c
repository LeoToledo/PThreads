#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <pthread.h>
#define N 100000 //Número de iterações

//Definição de variáveis de controle. Quando elas tem o mesmo valor, significa que elas estão na mesma iteração
int control_ab = 0, control_bt = 0, control_at = 0; //São inicializadas com 0 para fazer a primeira operação com os valores iniciais
int init_a = 0, init_b = 0, init_t = 0, init_p = 0; //Variáveis para fazer a primeira iteração

//Mutex utilizada para garantir que o compartilhamento de memória vai ocorrer corretamente
pthread_mutex_t mutex;

//Primeiramente, vamos criar uma struct com as variáveis a serem utilizadas
typedef struct GAUSS
{
    mpf_t aValue, bValue, tValue, pValue;
    mpf_t aAux, bAux, tAux, pAux;
    mpf_t tTemp;
    mpf_t a0, b0, t0, p0;
} GAUSS;

//Definindo as funções
void gaussLegendre(mpf_t pi); //Calcula pi
void *gaussA(void *arg);      //Calcula o termo A
void *gaussB(void *arg);      //Calcula o termo B
void *gaussTP(void *arg);     //Calcula os termos T e P

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Erro na passagem de parametros !");
        return -1;
    }

    FILE *arquivo;
    arquivo = fopen(argv[1], "r");
    mpf_t pi;
    mpf_init(pi);

    gaussLegendre(pi);

    gmp_printf("\n\nPi: %.6Ff\n\n", pi);

    FILE *arqSaida;
    arqSaida = fopen(argv[2], "w");

    fclose(arquivo);
    fclose(arqSaida);
    return 0;
}

void *gaussA(void *arg) //Calcula o termo A
{
    GAUSS *gauss = (GAUSS *)arg;

    int i = 0;

    //Faz o cálculo do primeiro termo
    mpf_add(gauss->aAux, gauss->a0, gauss->b0);
    mpf_div_ui(gauss->aAux, gauss->aAux, 2);
    i++;

    while (i < N)
    {
        if (control_ab == 0 && control_at == 0) //Se ambas as variáveis das quais A depende estiverem iguais, elas estão na mesma iteração e pode-se fazer o cálculo
        {
            pthread_mutex_lock(&mutex);

            mpf_add(gauss->aValue, gauss->aAux, gauss->bAux);
            mpf_div_ui(gauss->aValue, gauss->aValue, 2);

            control_ab = 1;
            control_at = 1;
            i++;
            pthread_mutex_unlock(&mutex);
        }
    }
    return (void *)gauss->aValue;
}

void *gaussB(void *arg) //Calcula o termo B
{
    GAUSS *gauss = (GAUSS *)arg;

    int i = 0;

    //Faz o cálculo da primeira iteração
    mpf_mul(gauss->bAux, gauss->a0, gauss->b0);
    mpf_sqrt(gauss->bAux, gauss->bAux);
    i++;

    while (i < N)
    {
        //printf("\nTESTE B\n");
        if (control_ab == 1 && control_bt == 0) //Se ambas as variáveis das quais A depende estiverem iguais, elas estão na mesma iteração e pode-se fazer o cálculo
        {
            pthread_mutex_lock(&mutex);

            mpf_mul(gauss->bValue, gauss->aAux, gauss->bAux);
            mpf_sqrt(gauss->bValue, gauss->bValue);

            control_ab = 0;
            control_bt = 1;
            i++;
            pthread_mutex_unlock(&mutex);
        }
    }
    return (void *)gauss->bValue;
}

void *gaussTP(void *arg) //Calcula os termos T e P
{
    GAUSS *gauss = (GAUSS *)arg;

    int i = 0;

    //Faz o cálculo dos primeiros termos
    //Calcula T
    mpf_sub(gauss->tTemp, gauss->a0, gauss->aAux);
    mpf_mul(gauss->tTemp, gauss->tTemp, gauss->tTemp);
    mpf_mul(gauss->tTemp, gauss->p0, gauss->tTemp);
    mpf_sub(gauss->tAux, gauss->t0, gauss->tTemp);

    //Calcula P
    mpf_mul_ui(gauss->pAux, gauss->p0, 2);
    i++;

    while (i < N)
    {
        if (control_at == 1 && control_bt == 1) //Se ambas as variáveis das quais A depende estiverem iguais, elas estão na mesma iteração e pode-se fazer o cálculo
        {
            pthread_mutex_lock(&mutex);

            //Calcula T
            mpf_sub(gauss->tTemp, gauss->aAux, gauss->aValue);
            mpf_mul(gauss->tTemp, gauss->tTemp, gauss->tTemp);
            mpf_mul(gauss->tTemp, gauss->pAux, gauss->tTemp);
            mpf_sub(gauss->tValue, gauss->tAux, gauss->tTemp);

            //Calcula P
            mpf_mul_ui(gauss->pValue, gauss->pAux, 2);

            control_at = 0;
            control_bt = 0;
            i++;
            pthread_mutex_unlock(&mutex);
        }
        if (i == N - 2)
            i++;
    }

    return (void *)gauss;
}

void gaussLegendre(mpf_t pi)
{
    GAUSS arg;

    pthread_mutex_init(&mutex, NULL);
    mpf_set_default_prec(pow(10, 5));

    //Valores inicias do método (n=0) e dos termos n=1 para conseguir paralelizar o t
    mpf_init(pi);
    mpf_init_set_ui(arg.a0, 1);
    mpf_init_set_ui(arg.p0, 1);
    mpf_init_set_d(arg.b0, 1 / sqrt(2));
    mpf_init_set_d(arg.t0, 0.25);

    mpf_init(arg.aAux);
    mpf_init(arg.bAux);
    mpf_init(arg.tAux);
    mpf_init(arg.pAux);
    mpf_init(arg.tTemp);

    mpf_init(arg.aValue);
    mpf_init(arg.bValue);
    mpf_init(arg.tValue);
    mpf_init(arg.pValue);
    mpf_init(pi);

    pthread_t t[3];

    pthread_create(&t[0], NULL, gaussA, &arg);

    pthread_create(&t[1], NULL, gaussB, &arg);

    pthread_create(&t[2], NULL, gaussTP, &arg);

    pthread_join(t[0], NULL);
    pthread_join(t[1], NULL);
    pthread_join(t[2], NULL);

    mpf_add(pi, arg.aValue, arg.bValue);
    mpf_pow_ui(pi, pi, 2);
    mpf_div_ui(pi, pi, 4);
    mpf_div(pi, pi, arg.tValue);
}
