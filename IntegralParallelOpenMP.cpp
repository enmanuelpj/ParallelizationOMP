#include <iostream>
#include <omp.h>
#include <iomanip>
#include <math.h>

using namespace std;

#define TOTAL_THREADS 800 //numero de hilos

//funcion de resultado con OpenMP
double getIntResultOpenMP();

//variables constantes
const int TOTAL_STEPS = 1000000000;
const double DELTA = 0.5;
const double STEP_DELTA = (double)(1.0/TOTAL_STEPS);
const double INT_FACTOR = 1.0;

//variables para uso de hilos
int nthread, i;
double result, sum[TOTAL_THREADS];

int main()
{
    //Medicion del tiempo con OMP
	double inicialtime, finaltime, ms; //variables de tiempo
	inicialtime = omp_get_wtime();  //tiempo inicial

	double paralelo = getIntResultOpenMP(); //instancia de resultado

	cout << "El resultado de la integral paralela: " << setprecision(6) << paralelo << endl;

	finaltime = omp_get_wtime(); //tiempo final
	ms = (finaltime - inicialtime) * 1000; //tiempo total en milisegundos

	cout << "El tiempo de ejecuccion paralela es: " << ms << " milisegundos.";

	return 0;
}

//Operacion con OpenMP
double getIntResultOpenMP()
{
	omp_set_num_threads(TOTAL_THREADS); //número de subprocesos utilizados

	#pragma omp parallel
	{
		int i, num, nthrd;
		double var;

		num = omp_get_thread_num(); //rango del hilo
		nthrd = omp_get_num_threads(); // número de subprocesos utilizados

		if (num == 0) nthread = nthrd;

		for (i = num, sum[num] = 0.0; i < TOTAL_STEPS; i = i + nthrd) {
			var = (i + DELTA) * STEP_DELTA;
			sum[num] += INT_FACTOR  / (1.0 + var * var);
		}
	}

	#pragma omp critical
	for (i = 0, result = 0.0; i < nthread; i++)
		result += sum[i] * STEP_DELTA;

	return result;
}






