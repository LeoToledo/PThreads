embedreportprint C 1.01 KB
 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
 
unsigned int gseed = 5913;
 
void* skellibone(void *args)
{
        double x = (((double)rand_r(&gseed))/((double)RAND_MAX));
        double y = (((double)rand_r(&gseed))/((double)RAND_MAX));
 
        //printf("%f, %f, %d\n", x, y, RAND_MAX);
        if(((x*x)+(y*y)) < 1)
        {
                return (void *)1;
        }
        else
        {
                return (void *)0;
        }
}
 
int main()
{
        gseed = time(NULL);
        srand(gseed); //seeded using decimal ascii values of (C+S+C+E)+3613+2015
        int tcount = rand()%1000;
        pthread_t threads[tcount];
        int results[tcount];
        double total = 0;
        double fresult = 0;
 
 
        int a = 0;
        for(a; a < tcount; a++)
        {
                pthread_create(&threads[a], NULL, skellibone, NULL);
        }
 
        int b = 0;
        for(b; b < tcount; b++)
        {
                pthread_join(threads[b], (void**)&results[b]);
        }
 
        int c = 0;
        for(c; c < tcount; c++)
        {
                //printf("%d ", results[c]);
                total += results[c];
        }
 
        fresult = ((4.0 * total)/((float)tcount));
 
        printf("Estimate of pi is %f\n", fresult);
}   