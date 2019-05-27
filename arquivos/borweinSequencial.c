#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

void borwein(int n, mpf_t pi);

int main()
{
    mpf_t pi;
    mpf_init(pi);
    mpf_set_ui(pi, 3);
    borwein(pow(10,5), pi);
    gmp_printf("\n\nPi piriripipi:  %.20Ff\n\n", pi);
    return 0;
}

void borwein(int n, mpf_t pi)
{
    mpf_set_default_prec(pow(10, 5)); //Reserva mais memória para as variáveis

    //Criando variáveis para o cálculo de pi
    mpf_t a;
    mpf_t b;
    mpf_t c;
    mpf_t d;
    mpf_t e;
    mpf_t pi_aux;

    //Inicializando as variáveis
    mpf_init(a);
    mpf_init(b);
    mpf_init(c);
    mpf_init(d);
    mpf_init(e);
    mpf_init (pi_aux);

    mpf_set_ui(pi, 0); //Inicia pi com 0
    for (int i = 0; i < n; i++) //Fazendo o somatório
    {
        //Atribuindo os valores iniciais
        //Para a:
        mpf_set_ui(a, 16);
        mpf_pow_ui(a, a, i);
        mpf_ui_div(a, 1, a);

        //Para b:
        mpf_set_ui(b, i);
        mpf_mul_ui(b, b, 8);
        mpf_add_ui(b, b, 1);
        mpf_ui_div(b, 4, b);

        //Para c:
        mpf_set_ui(c, i);
        mpf_mul_ui(c, c, 8);
        mpf_add_ui(c, c, 4);
        mpf_ui_div(c, 2, c);

        //Para d:
        mpf_set_ui(d, i);
        mpf_mul_ui(d, d, 8);
        mpf_add_ui(d, d, 5);
        mpf_ui_div(d, 1, d);

        //Para e:
        mpf_set_ui(e, i);
        mpf_mul_ui(e, e, 8);
        mpf_add_ui(e, e, 6);
        mpf_ui_div(e, 1, e);

        //Para pi
        mpf_set(pi_aux, b);
        mpf_sub(pi_aux, pi_aux, c);
        mpf_sub(pi_aux, pi_aux, d);
        mpf_sub(pi_aux, pi_aux, e);
        mpf_mul(pi_aux, a, pi_aux);
        mpf_add(pi, pi, pi_aux);
    }
}