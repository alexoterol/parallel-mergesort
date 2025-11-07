#include "sort_utils.h"
#include <string.h>
#include <limits.h>

#define MAX_INPUT_SIZE 100000 

// Leer números separados por comas
int* read_input(int* N, int source_type, const char* filename) {
    char input_buffer[MAX_INPUT_SIZE] = {0};
    int* arr = NULL;
    *N = 0;
    FILE *fp = NULL;

    if (source_type == 1) { // Leer desde Archivo CSV
        fp = fopen(filename, "r");
        if (fp == NULL) {
            perror("Error al abrir el archivo CSV");
            return NULL;
        }
        // Leer la primera línea del archivo
        if (fgets(input_buffer, MAX_INPUT_SIZE, fp) == NULL) {
            fclose(fp);
            printf("Error: Archivo CSV vacío o error de lectura.\n");
            return NULL;
        }
        fclose(fp);
        printf("-> Leyendo datos del archivo: %s\n", filename);

    } else { // Leer desde Teclado (por defecto)
        printf("Ingrese la lista de números separados por comas (ej: 45,12,78,3): \n");
        if (fgets(input_buffer, MAX_INPUT_SIZE, stdin) == NULL) {
            return NULL;
        }
    }

    // --- Lógica de Parseo (Común para ambas fuentes) ---

    // 1. Contar el número de elementos para asignar memoria
    char temp_buffer[MAX_INPUT_SIZE];
    strcpy(temp_buffer, input_buffer);
    
    // Usamos una copia del input_buffer para el strtok_r para evitar modificar el original
    char *saveptr_count;
    char* token = strtok_r(temp_buffer, ",\n", &saveptr_count);
    while (token != NULL) {
        // Asegurarse de que no contamos strings vacías (ej: 1,,2)
        if (strlen(token) > 0) {
            (*N)++;
        }
        token = strtok_r(NULL, ",\n", &saveptr_count);
    }

    if (*N == 0) {
        printf("Error: No se encontraron números válidos.\n");
        return NULL;
    }

    // 2. Asignar memoria y almacenar
    arr = (int*)malloc(*N * sizeof(int));
    if (arr == NULL) {
        perror("Error de asignación de memoria para el arreglo");
        return NULL;
    }

    int i = 0;
    char* saveptr_store;
    // Usamos el buffer original para el almacenamiento real
    token = strtok_r(input_buffer, ",\n", &saveptr_store);
    while (token != NULL && i < *N) {
        if (strlen(token) > 0) {
            // Usamos strtol en lugar de atoi para mejor manejo de errores
            arr[i] = (int)strtol(token, NULL, 10); 
            i++;
        }
        token = strtok_r(NULL, ",\n", &saveptr_store);
    }
    
    // Si la cuenta original difiere de los elementos realmente parseados (debido a errores/espacios), ajustamos.
    *N = i;

    return arr;
}

// Función para obtener el número de procesadores disponibles (estimación)
int get_num_processors() {
    // [CÓDIGO DE get_num_processors sigue siendo el mismo]
    long num_cpus = 1;
    #ifdef _WIN32
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        num_cpus = sysinfo.dwNumberOfProcessors;
    #elif defined(_SC_NPROCESSORS_ONLN)
        num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    #elif defined(_SC_NPROC_ONLN)
        num_cpus = sysconf(_SC_NPROC_ONLN);
    #endif
    
    if (num_cpus < 1) {
        num_cpus = 1;
    }
    return (int)num_cpus;
}


int main(int argc, char *argv[]) {
    int N; // Número de elementos
    int* arr = NULL;
    int source_type = 0; // 0: Teclado, 1: Archivo CSV
    const char* filename = "input.csv"; // Nombre de archivo por defecto

    // Verificar si se pasó un argumento de línea de comandos (nombre del archivo)
    if (argc > 1) {
        source_type = 1; // Modo archivo activado
        filename = argv[1]; // Usar el argumento como nombre de archivo
    }

    arr = read_input(&N, source_type, filename);

    if (arr == NULL || N <= 0) {
        printf("Programa finalizado.\n");
        return 1;
    }

    if (N == 1) {
        printf("\nLista ordenada (1 elemento): %d\n", arr[0]);
        free(arr);
        return 0;
    }

    // Obtener el número de procesadores disponibles para establecer P
    int P = get_num_processors();
    printf("-> Sistema detectó %d procesadores. ", P);
    
    // Asegurarse de que P no sea mayor que N/2 para que haya al menos un par de elementos por hilo
    if (P > N / 2) {
        P = (N / 2 > 0) ? N / 2 : 1;
    }
    printf("Usando P = %d hilos.\n", P);
    printf("Elementos a ordenar (N): %d\n", N);
    
    // Llamar al driver concurrente
    concurrent_mergesort_driver(arr, N, P);

    // Imprimir el resultado ordenado
    printf("\nResultado Final Ordenado: \n");
    for (int i = 0; i < N; i++) {
        printf("%d%s", arr[i], (i == N - 1) ? "" : ",");
    }
    printf("\n");

    free(arr);
    return 0;
}