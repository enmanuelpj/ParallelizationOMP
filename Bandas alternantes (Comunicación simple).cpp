#include "mpi.h" 
#include <stdio.h>
#include <stdlib.h>
#include <cassert>

#define padre 0
#define dimension 100

int main(int argc, char* argv[])
{
    int	cantidadProcesos, rank, procesosActivos, origen, destino, etiquetaActividad;
    int cantFila, filaMandar, resto, numeroFilas;
    int i, j, k;
    double	mA[dimension][dimension], mB[dimension][dimension], mResultante[dimension][dimension];

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &cantidadProcesos);

    procesosActivos = cantidadProcesos - 1;

    if (rank == padre)
    {
        printf("Producto con un numero de %d procesos.\n", cantidadProcesos);

        //Rellenando las matrices
        for (int i = 0; i < dimension; i++)
            for (int j = 0; j < dimension; j++)
                mA[i][j] = rand() % 100;

        for (int i = 0; i < dimension; i++)
            for (int j = 0; j < dimension; j++)
                mB[i][j] = rand() % 100;


        double start = MPI_Wtime();

        //Repartiendo el trabajo a los demas procesos 
        //filaMandar = dimension / procesosActivos;
        resto = dimension % procesosActivos;
        numeroFilas = 0;

        etiquetaActividad = 1;
        for (int i = 0; i < dimension / cantidadProcesos; i++)
        {
            for (destino = 1; destino <= procesosActivos; destino++)
            {
                cantFila = 1;

                 //Enviamos el numero de fila, la cantidad de fila, la fila a la matriz A y la matriz B
                MPI_Send(&numeroFilas, 1, MPI_INT, destino, etiquetaActividad, MPI_COMM_WORLD);
                MPI_Send(&cantFila, 1, MPI_INT, destino, etiquetaActividad, MPI_COMM_WORLD);
                MPI_Send(&mA[numeroFilas][0], cantFila * dimension, MPI_DOUBLE, destino, etiquetaActividad, MPI_COMM_WORLD);
                MPI_Send(&mB, dimension * dimension, MPI_DOUBLE, destino, etiquetaActividad, MPI_COMM_WORLD);

                numeroFilas += cantFila;
            }
        }

        etiquetaActividad = 2;
        for (i = 1; i <= procesosActivos; i++)
        {
            origen = i;

            //Aqui recibimos los calculos resultantes de realizar el producto, donde se les seran entregados al proceso padre
            MPI_Recv(&numeroFilas, 1, MPI_INT, origen, etiquetaActividad, MPI_COMM_WORLD, &status);
            MPI_Recv(&cantFila, 1, MPI_INT, origen, etiquetaActividad, MPI_COMM_WORLD, &status);
            MPI_Recv(&mResultante[numeroFilas][0], cantFila * dimension, MPI_DOUBLE, origen, etiquetaActividad, MPI_COMM_WORLD, &status);
        }

        double finish = MPI_Wtime();

        printf("Tiempo transcurrido:  %f seconds.\n", finish - start);
    }


    if (rank > padre)
    {
        //Lo procesos reciben los datos enviados por el padre
        etiquetaActividad = 1;
        MPI_Recv(&numeroFilas, 1, MPI_INT, padre, etiquetaActividad, MPI_COMM_WORLD, &status);
        MPI_Recv(&cantFila, 1, MPI_INT, padre, etiquetaActividad, MPI_COMM_WORLD, &status);
        MPI_Recv(&mA, cantFila * dimension, MPI_DOUBLE, padre, etiquetaActividad, MPI_COMM_WORLD, &status);
        MPI_Recv(&mB, dimension * dimension, MPI_DOUBLE, padre, etiquetaActividad, MPI_COMM_WORLD, &status);

        for (k = 0; k < dimension; k++)
            for (i = 0; i < cantFila; i++)
            {
                mResultante[i][k] = 0.0;
                for (j = 0; j < dimension; j++)
                    mResultante[i][k] = mResultante[i][k] + mA[i][j] * mB[j][k];
            }

        //Los procesos mandan al padre el producto resultante
        etiquetaActividad = 2;
        MPI_Send(&numeroFilas, 1, MPI_INT, padre, etiquetaActividad, MPI_COMM_WORLD);
        MPI_Send(&cantFila, 1, MPI_INT, padre, etiquetaActividad, MPI_COMM_WORLD);
        MPI_Send(&mResultante, cantFila * dimension, MPI_DOUBLE, padre, etiquetaActividad, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}

