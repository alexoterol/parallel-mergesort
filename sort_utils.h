#ifndef SORT_UTILS_H
#define SORT_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

// Estructura para pasar argumentos a los hilos de ordenación inicial
typedef struct {
    int* arr;
    int start;
    int end;
} SortArgs;

// Estructura para pasar argumentos a los hilos de fusión jerárquica
typedef struct {
    int* arr;
    int start;
    int mid;
    int end;
} MergeArgs;

// Funciones de utilidad y secuenciales (implementadas en sort_utils.c)
void sequential_merge(int arr[], int start, int mid, int end);
void sequential_mergesort(int arr[], int start, int end);

// Función que ejecuta cada hilo para la fase de ordenación inicial
void* thread_sort_task(void* args);

// Función driver principal que coordina la concurrencia
void concurrent_mergesort_driver(int arr[], int N, int P);

#endif // SORT_UTILS_H