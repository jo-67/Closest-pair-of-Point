#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> // Incluye la biblioteca para definir NULL
#include <time.h>
#include <math.h>
#include <string.h>
#include "source.c"

// Función para crear un arreglo aleatorio de tamaño n con números en el rango [0,1)
// Los elementos del arreglo son puntos Punto(x,y)
Punto* createRandomArray(int n) {
    Punto* arreglo = (Punto*)malloc(n * sizeof(Punto));

    for (int i = 0; i < n; i++) {
        arreglo[i].x = 1 + (double) rand() / RAND_MAX;
        arreglo[i].y = 1 + (double) rand() / RAND_MAX;
    }

    return arreglo;
}

int experiment_u(int n) {
    printf("Ejecutando para n=%d \n", n);

    // Semilla para la generación de números aleatorios
    srand((unsigned)time(NULL));

    int reps = 10;
    clock_t divide = 0;
    clock_t sweep = 0;
    clock_t random = 0;
    for (int i=0 ; i<reps ; i++) {
        Punto* randomArray = createRandomArray(n);
        Punto* randomArray2 = (Punto*)malloc(n * sizeof(Punto));
        Punto* randomArray3 = (Punto*)malloc(n * sizeof(Punto));
        memcpy(randomArray2, randomArray, n * sizeof(Punto));
        memcpy(randomArray3, randomArray, n * sizeof(Punto));

        clock_t inicio = clock();
        // Divide and conquer
        double divide_d = closestPairDivide(randomArray, n);
        //printf("divide %f\n", divide_d);
        clock_t fin = clock();
        divide = divide + (fin -inicio);

        clock_t inicio2 = clock();
        // sweep
        double sweep_d =closestPairSweep(randomArray, n);
        //printf("sweep %f\n", sweep_d);
        clock_t fin2 = clock();
        sweep = sweep + (fin2 -inicio2);

        
        clock_t inicio3 = clock();
        // random
        clock_t fin3 = clock();
        random = random + (fin3 -inicio3);

        free(randomArray);
        free(randomArray2);
        free(randomArray3);
    }

    double tiempo_divide = (double)(divide/reps) *1000/ CLOCKS_PER_SEC;
    //double tiempo_radix = (double)(fin - inicio);
    printf("Divide_and_Conquer tomó %f milisegundos en ejecutarse.\n", tiempo_divide);

    double tiempo_sweep = (double)(sweep/reps) *1000/ CLOCKS_PER_SEC;

    printf("Sweep_line tomó %f milisegundos en ejecutarse.\n", tiempo_sweep);

    double tiempo_random = (double)(random/reps) *1000/ CLOCKS_PER_SEC;

    printf("Aleatorizado tomó %f milisegundos en ejecutarse.\n", tiempo_random);

    printf("End\n");

    return 0;

}

int main() {
    //createRandomArray(100000000, u);
    for (int i = 5; i < 50; i+=5) {
        //experiment_u(i*1000000);
        experiment_u(i*10000);
    }
    printf("End");
    return 0;
}





