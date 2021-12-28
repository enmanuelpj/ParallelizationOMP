#include <iostream>
#include <omp.h>
#include <iomanip>
#include <math.h>

using namespace std;

//variables constantes
const int TOTAL_STEPS = 1000000000;
const double DELTA = 0.5;
const double STEP_DELTA = (double)(1.0/TOTAL_STEPS);
const double INT_FACTOR = 1.0;

//Funcion de forma secuencial
double getIntResult();

int main(int argc, char** argv)
{
    //Medicion de tiempo
    int inicialtime, finaltime, ms; //variables de tiempo
	inicialtime = omp_get_wtime(); //tiempo inicial

	double intResult = getIntResult(); //instancia de valor

	cout << "El resultado de la integral es: " << setprecision(6) << intResult << endl;

    finaltime = omp_get_wtime(); // tiempo final
	ms = (finaltime - inicialtime) * 1000; // tiempo total en milisegundos

	cout << "El tiempo de ejecuccion secuencial es: " << ms << " milisegundos.";

	return 0;
}

double getIntResult()
{
	double x, result;
	double  totalSum = 0.0;

	for(int i = 0; i < TOTAL_STEPS; i++)
	{
		x = (i + DELTA)*STEP_DELTA;
		totalSum += INT_FACTOR/(1.0 + x*x);
	}
	result = STEP_DELTA * totalSum;

	return result;
}


