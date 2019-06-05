#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <gmp.h>

unsigned int gseed = 40000;

typedef struct //Struct para guardar os valores de x e y
{
    double x;
    double y;
} aleatorio;

aleatorio geraAleatorio();        //Função que gera números aleatórios entre 0 e 1
void monteCarlo(int n, mpf_t pi); //Calcula o valor de pi através do método de Monte Carlos

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

    //Agora, vamos criar e inicializar a variável pi
    mpf_t pi;
    mpf_init(pi);

    //Por fim, vamos chamar a função que utiliza o método monteCarlo
    monteCarlo(pow(10, 8), pi);
    gmp_printf("\n\nPi: %.6Ff\n\n", pi);

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

void monteCarlo(int n, mpf_t pi)
{
    mpf_t dentro; //Pontos que caem dentro da circunferência
    mpf_t total;  //Pontos totais

    //Inicializando as variáveis
    mpf_init(dentro);
    mpf_init(total);

    mpf_set_ui(dentro, 0);
    mpf_set_ui(total, n); //Recebe o
    double z;

    for (int i = 0; i < n; i++) //Gera n pontos aleatórios
    {
        aleatorio nAleatorio = geraAleatorio();                        //Gera uma coordenada aleatória
        z = nAleatorio.x * nAleatorio.x + nAleatorio.y * nAleatorio.y; //Calcula o valor da área do círculo

        if (z <= 1) //Se estiver dentro do círculo
            mpf_add_ui(dentro, dentro, 1);

        mpf_mul_ui(pi, dentro, 4);
        mpf_div(pi, pi, total);
    }
}