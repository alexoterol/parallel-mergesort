import csv
import random
import time

def generar_csv_aleatorio(nombre_archivo, cantidad_numeros):
    """Genera un archivo CSV con la cantidad especificada de números enteros aleatorios."""
    print(f"-> Generando {cantidad_numeros:,} números en {nombre_archivo}...")
    
    # Rango de números aleatorios (ajustable)
    rango_min = 1
    rango_max = 1000000 
    
    datos = []
    
    # Generar la lista de números aleatorios
    for _ in range(cantidad_numeros):
        datos.append(random.randint(rango_min, rango_max))
        
    # Escribir la lista en el archivo CSV en una sola línea
    with open(nombre_archivo, 'w', newline='') as archivo_csv:
        # csv.writer es la forma estándar, pero para una sola línea
        # separada por comas, un simple join es más directo.
        archivo_csv.write(",".join(map(str, datos)))
        
    print(f"-> ¡Éxito! Archivo {nombre_archivo} creado.")


if __name__ == "__main__":
    
    start_time = time.time()
    
    # 1. Generar archivo de 10,000 números
    generar_csv_aleatorio("datos_10000.csv", 10000)
    
    # 2. Generar archivo de 1,000,000 de números
    generar_csv_aleatorio("datos_1000000.csv", 1000000)
    
    end_time = time.time()
    print(f"\nProceso completado en {end_time - start_time:.2f} segundos.")s