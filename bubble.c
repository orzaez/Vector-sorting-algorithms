#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Función para generar un array aleatorio
void generate_random_array(int *array, int size) {
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        array[i] = rand() % 100; // Números aleatorios entre 0 y 99
    }
}

// Implementación de Bubblesort con OpenMP
void parallel_bubblesort(int *array, int n) {
    int padding = 0;
    int arrayn[100096];
    int i, j, temp;
    int swapped;
    for (int a =0; a<8; a++)
    {
        for(int b = 0; b<12500; b++)
        {
            arrayn[a+b+padding] = array[a+b]; 
        }
        padding+= 12;
        
    }
    for (i = 0; i < n - 1; i++) {
        swapped = 0;

        #pragma omp parallel for private(j, temp) shared(swapped)
        for (j = 0; j < n - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                // Intercambiar los elementos
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;

                // Marcar que se ha realizado un intercambio
                swapped = 1;
            }
        }

        // Si no hubo intercambios, la lista ya está ordenada
        if (!swapped) {
            break;
        }
    }
}

int main() {
    int n;  // Tamaño del array
    int num_threads; // Número de hilos

    printf("Ingrese el tamaño del array: ");
    scanf("%d", &n);
    printf("Ingrese el número de hilos: ");
    scanf("%d", &num_threads);

    int *array = (int *)malloc(n * sizeof(int));

    // Generar el array aleatorio
    generate_random_array(array, n);

    // Mostrar el array original
    // printf("Array original:\n");
    // for (int i = 0; i < n; i++) {
    //     printf("%d ", array[i]);
    // }
    // printf("\n");

    // Inicializar OpenMP con el número de hilos especificado
    omp_set_num_threads(num_threads);

    // Medir el tiempo de ejecución
    double start_time = omp_get_wtime();

    // Ejecutar el algoritmo Bubblesort paralelo
    parallel_bubblesort(array, n);

    // Medir el tiempo de finalización
    double end_time = omp_get_wtime();
    double execution_time = end_time - start_time;

    // Mostrar el array ordenado
    // printf("Array ordenado:\n");
    // for (int i = 0; i < n; i++) {
    //     printf("%d ", array[i]);
    // }
    printf("\n");

    // Mostrar el tiempo de ejecución
    printf("Tiempo de ejecución: %f segundos\n", execution_time);

    free(array);

    return 0;
}
