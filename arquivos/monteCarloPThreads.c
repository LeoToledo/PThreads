#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <gmp.h>
#include <pthread.h>

#define nThreads 5  //Representa o número de threads a ser usado
int n = pow(10, 8); //Número de iterações desejado

unsigned int gseed = 40000;

typedef struct //Struct para guardar os valores de x e y
{
        double x;
        double y;
} aleatorio;

aleatorio geraAleatorio();      //Função que gera números aleatórios entre 0 e 1
void *monteCarlo(void *arg);    //Calcula o valor de pi através do método de Monte Carlos
double calculaPi(int *vDentro); //Faz o calculo final de pi

int main(int argc, char const *argv[])
{

        if (argc != 3)
        {
                printf("Erro na passagem de parametros !");
                return -1;
        }

        FILE *arquivo;
        arquivo = fopen(argv[1], "r");

        //Para gerar numeros aleatórios
        gseed = time(NULL);
        srand(gseed);

        //Agora, vamos criar as threads
        pthread_t threads[nThreads]; //Cria um vetor com a quantidade de threads definida anteriormente
        int vDentro[nThreads];

        for (int a = 0; a < nThreads; a++) //Loop que cria nThreads
        {
                pthread_create(&threads[a], NULL, monteCarlo, NULL);
        }
        for (int b = 0; b < nThreads; b++) //Loop que garante que cada thread rode até o final
        {
                pthread_join(threads[b], (void *)&vDentro[b]);
                //printf("\nDENTRO MAIN: %d\n", vDentro[b]);
        }

        double pi = calculaPi(vDentro); //Calcula o valor final de pi

        //Por fim, vamos printar o valor calculado de pi
        printf("PI: %.6f", pi);
        FILE *arqSaida;
        arqSaida = fopen(argv[2], "w");

        fclose(arquivo);
        fclose(arqSaida);

        return 0;
}

aleatorio geraAleatorio()
{
        aleatorio nAleatorio; //Guarda as coordenadas do ponto aleatório geradox
        nAleatorio.x = (((double)rand_r(&gseed)) / ((double)RAND_MAX));
        nAleatorio.y = (((double)rand_r(&gseed)) / ((double)RAND_MAX));
        //printf("\nX: %f", nAleatorio.x);
        //  printf("\nY: %f", nAleatorio.y);
        return nAleatorio;
}

void *monteCarlo(void *arg)
{
        int dentro = 0; //Pontos que caem dentro da circunferência

        double z, count = 0;
        pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
        for (int i = 0; i < n / nThreads; i++) //Gera n pontos aleatórios
        {
                //pthread_mutex_lock(&lock);
                aleatorio nAleatorio = geraAleatorio();                        //Gera uma coordenada aleatória
                z = nAleatorio.x * nAleatorio.x + nAleatorio.y * nAleatorio.y; //Calcula o valor da área do círculo

                if (z <= 1) //Se estiver dentro do círculo
                        dentro++;
                //count++;
                //pthread_mutex_unlock(&lock);
        }
        //printf("\nCOUNT: %f\n", count);
        //printf("\nDENTRO: %d\n", dentro);
        return (void *)dentro;
}

double calculaPi(int *vDentro)
{
        double pi;
        int dentro = 0;
        for (int i = 0; i < nThreads; i++)
        {
                dentro += vDentro[i];
        }

        pi = (double)(dentro * 4) / n;
        return pi;
}