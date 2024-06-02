#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Función para fusionar dos subarrays
void merge(int *array, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++)
        L[i] = array[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = array[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            array[k] = L[i];
            i++;
        } else {
            array[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        array[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        array[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

// Función recursiva de Merge Sort
void merge_sort(int *array, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                merge_sort(array, left, mid);
            }
            #pragma omp section
            {
                merge_sort(array, mid + 1, right);
            }
        }

        merge(array, left, mid, right);
    }
}

// Función para generar un array aleatorio
void generate_random_array(int *array, int size) {
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        array[i] = rand() % 100; // Números aleatorios entre 0 y 99
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

    // Inicializar OpenMP con el número de hilos especificado
    omp_set_num_threads(num_threads);

    // Medir el tiempo de ejecución
    double start_time = omp_get_wtime();

    // Ejecutar el algoritmo Merge Sort paralelo
    merge_sort(array, 0, n - 1);

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
