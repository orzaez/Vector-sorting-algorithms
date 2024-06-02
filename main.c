#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

// Función de ordenación rápida (Quicksort)
void quicksort(int *array, int left, int right) {
    int i = left, j = right;
    int pivot = array[(left + right) / 2];
    int temp;

    while (i <= j) {
        while (array[i] < pivot) i++;
        while (array[j] > pivot) j--;
        if (i <= j) {
            temp = array[i];
            array[i] = array[j];
            array[j] = temp;
            i++;
            j--;
        }
    }

    if (left < j) quicksort(array, left, j);
    if (i < right) quicksort(array, i, right);
}

// Algoritmo PREZ
void PREZ(int *D1, int *D2, int *R, int s) {
    int c11 = 0, c21 = 0, c12 = s - 1, c22 = s - 1;

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            for (int k = 0; k < s; k++) {
                if (D1[c11] <= D2[c21]) {
                    R[k] = D1[c11++];
                } else {
                    R[k] = D2[c21++];
                }
            }
        }
        #pragma omp section
        {
            for (int k = 0; k < s; k++) {
                if (D2[c22] > D1[c12]) {
                    R[2 * s - 1 - k] = D2[c22--];
                } else {
                    R[2 * s - 1 - k] = D1[c12--];
                }
            }
        }
    }
}

// Algoritmo PCM
void PCM(int *D, int S, int N) {
    int sub_size = S / N;
    int **subarrays = (int **)malloc(N * sizeof(int *));
    
    // Dividir el array principal en subarrays
    for (int i = 0; i < N; i++) {
        subarrays[i] = &D[i * sub_size];
    }

    // Primera etapa: ordenación inicial
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        quicksort(subarrays[i], 0, sub_size - 1);
    }

    // Segunda etapa: mezcla y ordenación concurrente
    for (int size = sub_size; size < S; size *= 2) {
        #pragma omp parallel for
        for (int i = 0; i < N; i += 2 * (size / sub_size)) {
            if (i + size / sub_size < N) {
                int *R = (int *)malloc(2 * size * sizeof(int));
                PREZ(subarrays[i], subarrays[i + size / sub_size], R, size);
                for (int j = 0; j < 2 * size; j++) {
                    D[i * sub_size + j] = R[j];
                }
                free(R);
            }
        }
    }

    free(subarrays);
}

// Función principal para probar el algoritmo
int main() {
    int N = 4;  // Número de procesadores
    int S = 16; // Tamaño de la secuencia de datos
    int D[16] = {16, 14, 12, 10, 8, 6, 4, 2, 15, 13, 11, 9, 7, 5, 3, 1};

    // Inicializar OpenMP
    omp_set_num_threads(N);

    // Ejecutar el algoritmo PCM
    PCM(D, S, N);

    // Imprimir el array ordenado
    for (int i = 0; i < S; i++) {
        printf("%d ", D[i]);
    }
    printf("\n");

    return 0;
}
