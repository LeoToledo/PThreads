#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>


void gaussLegendre(int n, mpf_t pi);


void gaussLegendre(int n, mpf_t pi)
{
      mpf_set_default_prec(pow(10,5));

      //Declarando as variáveis
      mpf_t aValue;
      mpf_t bValue;
      mpf_t tValue;
      mpf_t pValue;
      mpf_t piValue;
      mpf_t aAux;
      mpf_t bAux;
      mpf_t tAux;
      mpf_t pAux;
      mpf_t tTemp;
      
      //Inicializando as variáveis
      mpf_init (aValue);
      mpf_init (bValue);
      mpf_init (tValue);
      mpf_init (pValue);
      mpf_init (piValue);
      mpf_init (aAux);
      mpf_init (bAux);
      mpf_init (tAux);
      mpf_init (pAux);
      mpf_init (tTemp);
      mpf_init (tTemp);

      //Atribuindo os valores iniciais
      mpf_set_ui(aValue, 1);
      mpf_set_d(bValue, 0.5);
      mpf_sqrt(bValue, bValue);
      mpf_set_d(tValue, 0.25);
      mpf_set_ui(pValue, 1);
      mpf_set_ui(tTemp, 0);

      //Atribuição de variáveis auxiliares
      mpf_set(aAux, aValue);
      mpf_set(bAux, bValue);
      mpf_set(tAux, tValue);
      mpf_set(pAux, pValue);
      
      //Loop que faz o cálculo do método de Gauss
      for(int i = 0; i < n; i++)
      {

            //Cálculo do valor de "a"
            mpf_add(aValue, aAux, bAux);
            mpf_div_ui(aValue, aValue, 2);
            
            //Cálculo do valor de "b"
            mpf_mul(bValue, aAux, bAux);
            mpf_sqrt(bValue, bValue);
            

            //Cálculo de t **********88(PROBLEMA NO VALOR FINAL DE TVALUE)**********
            mpf_sub(tTemp, aAux, aValue);
            mpf_mul(tTemp, tTemp, tTemp);
            mpf_mul(tTemp, pAux, tTemp);
            mpf_sub(tValue, tAux, tTemp);
            

            //Cálculo de p
            mpf_mul_ui(pValue, pAux, 2);

            //Atualizando os valores de aux
            mpf_set(aAux, aValue);
            mpf_set(bAux, bValue);
            mpf_set(tAux, tValue);
            mpf_set(pAux, pValue);

      }
      
      //Por fim, vamos calcular o valor de PI
      mpf_t piAux;
      mpf_init(piAux);
      
      mpf_add(piAux, aValue, bValue);
      mpf_mul(piAux, piAux, piAux);
      mpf_div(pi, piAux, tValue);
      mpf_div_ui(pi, pi, 4);

}

int main()
{
      /*
     * Basta alterar o valor de 10 por outro
  * valor maior, para obter valores mais precisos.
  * CUIDADO!!! Se você colocar um valor muito alto,
  * pode gerar extrema lentidão no seu pc. 
  */
      mpf_t pi;
      mpf_init (pi);
      
      gaussLegendre(pow(10,9), pi);

      gmp_printf("\n\nPi piriripipi:  %Ff\n\n", pi);
      return 0;
}