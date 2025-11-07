#include "sort_utils.h"
#include <math.h>


// Función auxiliar para la mezcla secuencial (Merge)
void sequential_merge(int arr[], int start, int mid, int end) {
    int i, j, k;
    int n1 = mid - start + 1;
    int n2 = end - mid;

    // Arreglos temporales
    int L[n1], R[n2];

    // Copiar datos a arreglos temporales L[] y R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[start + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Fusionar los arreglos temporales de vuelta en arr[start..end]
    i = 0; // Índice inicial del primer subarreglo
    j = 0; // Índice inicial del segundo subarreglo
    k = start; // Índice inicial del arreglo fusionado
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copiar los elementos restantes de L[], si hay alguno
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copiar los elementos restantes de R[], si hay alguno
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Implementación de MergeSort secuencial (usado en la Fase 1)
void sequential_mergesort(int arr[], int start, int end) {
    if (start < end) {
        int mid = start + (end - start) / 2;

        // Llamadas recursivas (secuenciales)
        sequential_mergesort(arr, start, mid);
        sequential_mergesort(arr, mid + 1, end);

        // Mezcla
        sequential_merge(arr, start, mid, end);
    }
}

// Tarea del hilo para la Fase 1: Ordenación inicial de un bloque
void* thread_sort_task(void* args) {
    SortArgs* s_args = (SortArgs*)args;
    
    // Cada hilo ejecuta la versión secuencial de MergeSort en su bloque
    sequential_mergesort(s_args->arr, s_args->start, s_args->end);
    
    // Liberar la memoria de los argumentos después de usarlos
    free(args); 
    
    pthread_exit(NULL);
}

// Tarea del hilo para la Fase 2: Fusión de dos bloques
void* thread_merge_task(void* args) {
    MergeArgs* m_args = (MergeArgs*)args;
    
    // El hilo ejecuta la mezcla de los dos bloques ordenados
    sequential_merge(m_args->arr, m_args->start, m_args->mid, m_args->end);
    
    // Liberar la memoria de los argumentos
    free(args);
    
    pthread_exit(NULL);
}


// Función principal driver para la concurrencia
void concurrent_mergesort_driver(int arr[], int N, int P) {
    
    // ===================================
    // FASE 1: Ordenación Inicial Paralela
    // ===================================
    
    pthread_t threads[P];
    int i;
    int chunk_size = N / P;
    
    printf("-> Fase 1: Creando %d hilos para ordenar bloques de ~%d elementos.\n", P, chunk_size);

    for (i = 0; i < P; i++) {
        // Asignar dinámicamente argumentos para cada hilo
        SortArgs* s_args = (SortArgs*)malloc(sizeof(SortArgs));
        s_args->arr = arr;
        s_args->start = i * chunk_size;
        
        // El último hilo toma todos los elementos restantes si N no es divisible por P
        s_args->end = (i == P - 1) ? N - 1 : (s_args->start + chunk_size) - 1;

        // Crear y lanzar el hilo
        if (pthread_create(&threads[i], NULL, thread_sort_task, s_args) != 0) {
            perror("Error al crear hilo de ordenación inicial");
            exit(EXIT_FAILURE);
        }
    }

    // Esperar a que todos los hilos de ordenación inicial terminen (JOIN)
    for (i = 0; i < P; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("-> Fase 1 completada: %d subarreglos ordenados.\n", P);

    // ===================================
    // FASE 2: Fusión Jerárquica (Merge Tree)
    // ===================================
    
    int current_size = chunk_size;
    int thread_count = 0; // Contador auxiliar para el pool de hilos de fusión
    
    printf("-> Fase 2: Comenzando fusión jerárquica.\n");
    
    // La fusión continúa mientras el tamaño del bloque ordenado no sea el tamaño total (N)
    while (current_size < N) {
        
        int num_merges = 0;
        
        // Número de fusiones que se realizarán en paralelo en este nivel
        for (i = 0; i * 2 * current_size < N; i++) {
            num_merges++;
        }
        
        // Creamos un arreglo de hilos para el nivel actual
        pthread_t merge_threads[num_merges];
        thread_count = 0;
        
        for (i = 0; i < N; i += 2 * current_size) {
            // Calcular índices para la fusión:
            int merge_start = i;
            int merge_mid = merge_start + current_size - 1;
            int merge_end = merge_start + 2 * current_size - 1;
            
            // Asegurar que merge_end no se salga de los límites del arreglo
            if (merge_end >= N) {
                merge_end = N - 1;
            }

            // Si hay un bloque derecho para fusionar
            if (merge_mid < merge_end) {
                
                // Asignar dinámicamente argumentos para el hilo de fusión
                MergeArgs* m_args = (MergeArgs*)malloc(sizeof(MergeArgs));
                m_args->arr = arr;
                m_args->start = merge_start;
                m_args->mid = merge_mid;
                m_args->end = merge_end;

                // Crear y lanzar el hilo de fusión
                if (pthread_create(&merge_threads[thread_count], NULL, thread_merge_task, m_args) != 0) {
                    perror("Error al crear hilo de fusión");
                    exit(EXIT_FAILURE);
                }
                thread_count++;
            }
        }
        
        // Esperar a que todos los hilos de fusión de este nivel terminen
        for (i = 0; i < thread_count; i++) {
            pthread_join(merge_threads[i], NULL);
        }
        
        printf("   - Nivel completado. Tamaño de bloque ordenado: %d\n", current_size * 2);

        // Duplicar el tamaño de los bloques ordenados para el siguiente nivel
        current_size *= 2;
    }
    printf("-> Fase 2 completada: Arreglo totalmente ordenado.\n");
}