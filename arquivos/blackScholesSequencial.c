#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>

unsigned int gseed = 40000; //Para gerar números aleatórios

//Declaração das funções utilizadas
void blackScholes();                                                 //Utiliza o método de blackScholes
double media(double trials[], unsigned int m);                       //Calcula a média de um vetor de double
double desvio_padrao(double trials[], unsigned int m, double media); //Calcula o desvio padrão de um vetor de double
double intervalo_confianca(double trials[], double desv, int M);     //Calcula o intervalo de confiança dados a média e desvio padrão anteriores

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Erro na passagem de parametros !");
        return -1;
    }
    //Para gerar numeros aleatórios
    gseed = time(NULL);
    srand(gseed);

    blackScholes(argv);

    return 0;
}

void blackScholes(char const *argv[])
{
    //Primeiramente, vamos ler o txt
    FILE *arquivo = fopen(argv[1], "r");

    if (arquivo == NULL) //Caso não seja possível abrir o arquivo
    {
        printf("ERRO AO ABRIR O ARQUIVO");
        return;
    }

    double valores[6]; //Array que armazena os valores lidos
    for (int i = 0; i < 6; i++)
    {
        fscanf(arquivo, "%lf\n", &valores[i]); //Lê os valores do arquivo
    }

    double S = valores[0];     //Valor da ação
    double E = valores[1];     //Preço de exercício da opção
    double r = valores[2];     //Taxa de juros livre de riscos(SELIC)
    double sigma = valores[3]; //Volatilidade da ação
    double T = valores[4];     //Tempo de validade da opção
    int M = (int)valores[5];   //Número de iterações

    double trials[M];

    double aux1, aux2, t;

    double x;
    for (int i = 0; i < M; i++)
    {
        x = (((double)rand_r(&gseed)) / ((double)RAND_MAX)); //X será um número randomico entre 0 e 1

        //Primeiro termo dentro do expoente ( (r - 0.5*sigma²)*T )
        aux1 = ((r - 0.5 * sigma * sigma) * T);

        //Segundo termo do expoente ( sigma*sqrt(T)*randomNumber )
        aux2 = (sigma * sqrt(T) * x);

        aux1 = (aux1 + aux2);

        //Elevando o expoente
        t = (exp(aux1) * S);

        if ((t - E) < 0) //Checa max{T-E, 0}
        {
            trials[i] = 0;
            //printf("\nt: %f\n", t);
        }
        else
        {
            trials[i] = exp(((-1) * (r)) * T) * (t - E);
        }
        //printf("TRIALS: %lf", trials[i]);
    }

    double med = media(trials, M);                                         //Calcula a média dos valores
    double desv_pad = desvio_padrao(trials, M, med);                       //Calcula o desvio padrão dos valores
    double confidence_interval = intervalo_confianca(trials, desv_pad, M); //Calcula o intervalo de confiança dos valores
    double confMin = (med - confidence_interval);                          //Confiança mínima
    double confMax = (med + confidence_interval);

    printf("\nMedia: %f\n", med);
    printf("\nDesvio Padrao: %f\n", desv_pad);
    printf("\nIntervalo de Confiança: %f\n", confidence_interval);
    printf("\nConfianca minima: %f\n", confMin);
    printf("\nConfianca maxima: %f\n", confMax);

    //Por fim, vamos escrever no arquivo de saída
    FILE *arquivoSaida = fopen(argv[2], "w");
    if (arquivoSaida == NULL) //Checa se foi possível abrir o arquivo
    {
        printf("ERRO AO ESCREVER NO ARQUIVO");
        return;
    }

    fprintf(arquivoSaida, "%lf \n%lf \n%lf \n%lf \n%lf \n%d \n%lf \n%lf", S, E, r, sigma, T, M, confMin, confMax);

    fclose(arquivo);
    fclose(arquivoSaida);
}

double media(double trials[], unsigned int m)
{
    double total = 0;
    double media;
    for (int i = 0; i < m; i++)
    {
        total += trials[i];
    }
    //printf("Média é = %lf", total/m);
    media = total / m;
    return media;
}

double desvio_padrao(double trials[], unsigned int m, double media)
{
    double p, dp;

    for (int i = 0; i < m; i++)
    {
        p = p + pow(trials[i] - media, 2);
    }
    dp = sqrt(p / (m - 1));
    return dp;
}

double intervalo_confianca(double trials[], double desv, int M)
{
    double confwidth = ((1.96 * desv) / sqrt(M));
    return confwidth;
}