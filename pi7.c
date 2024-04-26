#include <stdio.h>
#include <time.h>
#include <omp.h>

long long num_steps = 100000000;
double step;

int main(int argc, char *argv[])
{
	clock_t spstart, spstop, ppstart, ppstop;

	double sswtime, sewtime, pswtime, pewtime;
	// volatile
	double x, pi, sum = 0.0;
	int i;
	double tab[50];

	for (int j = 0; j < 48; j++)
	{
		// RÓWNOLEGLE
		pswtime = omp_get_wtime();
		ppstart = clock();
		sum = 0.0;
		step = 1. / (double)num_steps;

#pragma omp parallel num_threads(2)
		{
			int id = omp_get_thread_num();
			tab[id + j] = 0;
#pragma omp for
			for (i = 0; i < num_steps; i++)
			{
				double x = (i + .5) * step;
// zmienna jest wspódzielona, umieszczona w tym samym obszarze
// co 8 raz jest krótszy
#pragma omp flush(tab)
				tab[id + j] += 4.0 / (1. + x * x);
			}
#pragma omp atomic
			sum += tab[id + j];
		}
		pi = sum * step;

		ppstop = clock();
		pewtime = omp_get_wtime();

		// printf("%15.12f Wartosc liczby PI rownolegle \n", pi);
		// printf("Czas procesorów przetwarzania równoleglego  %f sekund \n", ((double)(ppstop - ppstart) / CLOCKS_PER_SEC));
		printf("Czas obliczen - wallclock %f sekund \n", pewtime - pswtime);
	}
	return 0;
}
