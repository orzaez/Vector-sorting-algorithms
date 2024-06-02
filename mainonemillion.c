#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void quicksort(int *array, int left, int right)
{
    int i = left, j = right;
    int pivot = array[(left + right) / 2];
    int temp;

    while (i <= j)
    {
        while (array[i] < pivot)
            i++;
        while (array[j] > pivot)
            j--;
        if (i <= j)
        {
            temp = array[i];
            array[i] = array[j];
            array[j] = temp;
            i++;
            j--;
        }
    }

    if (left < j)
        quicksort(array, left, j);
    if (i < right)
        quicksort(array, i, right);
}

void PREZ(int *D1, int *D2, int *R, int s)
{
    int c11 = 0, c21 = 0, c12 = s - 1, c22 = s - 1;

#pragma omp parallel sections
    {
#pragma omp section
        {
            for (int k = 0; k < s; k++)
            {
                if (D1[c11] <= D2[c21])
                {
                    R[k] = D1[c11++];
                }
                else
                {
                    R[k] = D2[c21++];
                }
            }
        }
#pragma omp section
        {
            for (int k = 0; k < s; k++)
            {
                if (D2[c22] > D1[c12])
                {
                    R[2 * s - 1 - k] = D2[c22--];
                }
                else
                {
                    R[2 * s - 1 - k] = D1[c12--];
                }
            }
        }
    }
}

void PCM(int *D, int S, int N)
{
    int sub_size = S / N;
    int **subarrays = (int **)malloc(N * sizeof(int *));

    for (int i = 0; i < N; i++)
    {
        subarrays[i] = &D[i * sub_size];
    }

    printf("Etapa 1: Ordenación inicial de los subarrays con quicksort\n");
#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        quicksort(subarrays[i], 0, sub_size - 1);
    }

    int stage = 2;
    for (int size = sub_size; size < S; size *= 2)
    {
        printf("Etapa %d: Mezcla y ordenación\n", stage);
#pragma omp parallel for
        for (int i = 0; i < N; i += 2 * (size / sub_size))
        {
            if (i + size / sub_size < N)
            {
                int *R = (int *)malloc(2 * size * sizeof(int));
                PREZ(subarrays[i], subarrays[i + size / sub_size], R, size);
                for (int j = 0; j < 2 * size; j++)
                {
                    D[i * sub_size + j] = R[j];
                }
                free(R);
            }
        }
        stage++;
    }

    free(subarrays);
}

void generate_random_array(int *array, int size)
{
    srand(time(NULL));
    for (int i = 0; i < size; i++)
    {
        array[i] = rand() % 100;
    }
}

int main()
{
    int N;
    int S;

    printf("Ingrese el número de procesadores (N): ");
    scanf("%d", &N);
    printf("Ingrese el tamaño de la secuencia de datos (S): ");
    scanf("%d", &S);

    int *D = (int *)malloc(S * sizeof(int));

    generate_random_array(D, S);

    omp_set_num_threads(N);

    double start_time = omp_get_wtime();

    PCM(D, S, N);

    double end_time = omp_get_wtime();
    double execution_time = end_time - start_time;

    printf("Tiempo de ejecución: %f segundos\n\n", execution_time);

    free(D);

    return 0;
}
