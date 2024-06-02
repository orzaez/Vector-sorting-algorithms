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

// Implementación de Selection Sort con OpenMP
void parallel_selection_sort(int *array, int n) {
    int i, j, min_idx, temp;

    for (i = 0; i < n - 1; i++) {
        min_idx = i;

        // Paralelizar la búsqueda del mínimo
        #pragma omp parallel for shared(array, min_idx)
        for (j = i + 1; j < n; j++) {
            #pragma omp critical
            {
                if (array[j] < array[min_idx]) {
                    min_idx = j;
                }
            }
        }

        // Intercambiar el elemento mínimo con el primer elemento desordenado
        temp = array[min_idx];
        array[min_idx] = array[i];
        array[i] = temp;
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
    printf("Array original:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    // Inicializar OpenMP con el número de hilos especificado
    omp_set_num_threads(num_threads);

    // Medir el tiempo de ejecución
    double start_time = omp_get_wtime();

    // Ejecutar el algoritmo Selection Sort paralelo
    parallel_selection_sort(array, n);

    // Medir el tiempo de finalización
    double end_time = omp_get_wtime();
    double execution_time = end_time - start_time;

    // Mostrar el array ordenado
    printf("Array ordenado:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    // Mostrar el tiempo de ejecución
    printf("Tiempo de ejecución: %f segundos\n", execution_time);

    free(array);

    return 0;
}
