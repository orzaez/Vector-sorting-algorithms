#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Función para generar un array aleatorio
void generate_random_array(int *array, int size)
{
    srand(time(NULL));
    for (int i = 0; i < size; i++)
    {
        array[i] = rand() % 100; // Números aleatorios entre 0 y 99
    }
}

// Función para intercambiar dos elementos
void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

// Función para realizar la partición del arreglo
int partition(int arr[], int low, int high)
{
    int pivot = arr[high]; // Pivote
    int i = (low - 1);     // Índice del elemento más pequeño

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Función principal de Quick Sort
void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);

// Paralelizar los llamados recursivos
#pragma omp parallel sections
        {
#pragma omp section
            {
                quickSort(arr, low, pi - 1);
            }
#pragma omp section
            {
                quickSort(arr, pi + 1, high);
            }
        }
    }
}

int main()
{
    int n;           // Tamaño del array
    int num_threads; // Número de hilos

    printf("Ingrese el tamaño del array: ");
    scanf("%d", &n);
    printf("Ingrese el número de hilos: ");
    scanf("%d", &num_threads);

    int *array = (int *)malloc(n * sizeof(int));
    if (array == NULL)
    {
        printf("Error al asignar memoria\n");
        return -1;
    }

    // Generar el array aleatorio
    generate_random_array(array, n);

    // Mostrar el array original
    printf("Array original:\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");

    // Inicializar OpenMP con el número de hilos especificado
    omp_set_num_threads(num_threads);

    // Medir el tiempo de ejecución
    double start_time = omp_get_wtime();

    // Ejecutar el algoritmo Quick Sort paralelo
    quickSort(array, 0, n - 1);

    // Medir el tiempo de finalización
    double end_time = omp_get_wtime();
    double execution_time = end_time - start_time;

    // Mostrar el array ordenado
    printf("Array ordenado:\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");

    // Mostrar el tiempo de ejecución
    printf("Tiempo de ejecución: %f segundos\n", execution_time);

    free(array);

    return 0;
}
