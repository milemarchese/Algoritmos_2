//
// Created by Martin Feldsztejn on 11/2/17.
//

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "testing.h"
#include "heap.h"

#define VOLUMEN 1000

static void swap(void *array[], size_t p1, size_t p2) {
    void *temp = array[p1];
    array[p1] = array[p2];
    array[p2] = temp;
}

static void shuffle(int **claves, size_t largo) {
    srand((unsigned int) time(NULL));

    // Mezclamos el array: algoritmo de Fisher–Yates
    for (int i = (int) (largo / 2 - 1); i >= 0; i--) {
        size_t j = (size_t) rand() % (i + 1);
        swap((void **) claves, i, j);
    }
}

int comparar_ints(const int *a, const int *b) {
    if (*a == *b) {
        return 0;
    }
    if (*a < *b) {
        return -1;
    }
    return 1;
}

int comparar_void(const void *a, const void *b) {
    return comparar_ints((int *) a, (int *) b);
}

void pruebas_encolar() {
    heap_t *heap = heap_crear(comparar_void);
    int array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    heap_encolar(heap, &array[4]);
    heap_encolar(heap, &array[2]);
    heap_encolar(heap, &array[7]);
    heap_encolar(heap, &array[5]);
    heap_encolar(heap, &array[6]);
    heap_encolar(heap, &array[8]);
    heap_encolar(heap, &array[0]);
    print_test("La cantidad es 7", heap_cantidad(heap) == 7);
    print_test("El maximo es 8", heap_ver_tope(heap) == &array[8]);
    heap_destruir(heap, NULL);
}

void pruebas_desencolar() {
    heap_t *heap = heap_crear(comparar_void);
    int array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    int random_orden[] = {4, 2, 7, 5, 6, 8, 0};
    int ordered_indexes[] = {8, 7, 6, 5, 4, 2, 0};
    int largo = sizeof(random_orden) / sizeof(int);
    for (int i = 0; i < largo; i++) {
        heap_encolar(heap, &array[random_orden[i]]);
    }
    print_test("La cantidad es 7", heap_cantidad(heap) == 7);

    bool ok = true;
    for (int i = 0; i < largo; i++) {
        ok &= heap_desencolar(heap) == &array[ordered_indexes[i]];
    }

    print_test("El heap deberia estar vacio", heap_esta_vacio(heap));
    heap_destruir(heap, NULL);
}

void pruebas_heapify() {
    int array[] = {5, 2, 7, 3, 8, 1, 0, 9};
    int ordered_indexes[] = {7, 4, 2, 0, 3, 1, 5, 6};
    size_t largo = sizeof(array) / sizeof(int);
    void *auxArray[largo];
    for (int i = 0; i < largo; i++) {
        auxArray[i] = &array[i];
    }
    heap_t *heap = heap_crear_arr(auxArray, largo, comparar_void);

    print_test("El maximo es 9", heap_ver_tope(heap) == &array[7]);
    bool ok = true;
    for (int i = 0; i < largo; i++) {
        ok &= heap_desencolar(heap) == &array[ordered_indexes[i]];
    }
    print_test("Desencolo en orden de mayor a menor", ok);
    heap_destruir(heap, NULL);
}

void pruebas_volumen_heap(void) {
    heap_t *heap = heap_crear(comparar_void);
    int **arreglo = malloc(sizeof(int *) * VOLUMEN);
    bool OK = true;

    for (int i = 0; i < VOLUMEN; i++) {
        int *puntero = malloc(sizeof(int *));
        *puntero = i;
        arreglo[i] = puntero;
    }
    int **arreglo_desordenado = malloc(sizeof(int *) * VOLUMEN);
    memcpy(arreglo_desordenado, arreglo, sizeof(int *) * VOLUMEN);
    shuffle(arreglo_desordenado, VOLUMEN);
    for (int i = 0; i < VOLUMEN; i++) {
        OK &= heap_encolar(heap, &arreglo[i]);
    }

    print_test("Se apilaron todos los elementos", OK);

    print_test("El heap no esta vacio", !heap_esta_vacio(heap));
    print_test("El largo del heap es 500", heap_cantidad(heap) == VOLUMEN);

    OK = true;
    for (int i = 0; i < VOLUMEN; i++) {
        OK &= heap_desencolar(heap) == arreglo[i];
        free(arreglo_desordenado[i]);
    }

    print_test("El heap esta vacio", heap_esta_vacio(heap));

    print_test("Largo del heap es 0", !heap_cantidad(heap));
    heap_destruir(heap, NULL);
    print_test("El heap fue destruido", true);
    free(arreglo);
    free(arreglo_desordenado);
}

void pruebas_heapsort_menor_a_mayor() {
    int **arreglo = malloc(sizeof(int *) * VOLUMEN);

    for (int i = 0; i < VOLUMEN; i++) {
        int *puntero = malloc(sizeof(int *));
        *puntero = i;
        arreglo[i] = puntero;
    }
    int **arreglo_desordenado = malloc(sizeof(int *) * VOLUMEN);
    memcpy(arreglo_desordenado, arreglo, sizeof(int *) * VOLUMEN);
    shuffle(arreglo_desordenado, VOLUMEN);

    heap_sort((void **) arreglo_desordenado, VOLUMEN, comparar_void);
    bool OK = true;
    for (int i = 0; i < VOLUMEN; i++) {
        OK &= arreglo_desordenado[i] == arreglo[i];
        free(arreglo_desordenado[i]);
    }
    print_test("El arreglo esta ordenado", OK);
    free(arreglo_desordenado);
    free(arreglo);
}

int comparar_void2(const void *a, const void *b) {
    return comparar_ints((int *) a, (int *) b) * -1;
}

void pruebas_heapsort_mayor_a_menor() {
    int **arreglo = malloc(sizeof(int *) * VOLUMEN);

    for (int i = VOLUMEN - 1; i >= 0; i--) {
        int *puntero = malloc(sizeof(int *));
        *puntero = i;
        arreglo[i] = puntero;
    }
    int **arreglo_desordenado = malloc(sizeof(int *) * VOLUMEN);
    memcpy(arreglo_desordenado, arreglo, sizeof(int *) * VOLUMEN);
    shuffle(arreglo_desordenado, VOLUMEN);

    heap_sort((void **) arreglo_desordenado, VOLUMEN, comparar_void);
    bool OK = true;
    for (int i = 0; i < VOLUMEN; i++) {
        OK &= arreglo_desordenado[i] == arreglo[i];
        free(arreglo_desordenado[i]);
    }
    print_test("El arreglo esta ordenado", OK);
    free(arreglo_desordenado);
    free(arreglo);
}

void pruebas_destruyendo() {
    size_t largo = 10;
    heap_t *heap = heap_crear(comparar_void);
    for (int i = 0; i < largo; i++) {
        int *puntero = malloc(sizeof(int));
        *puntero = i;
        heap_encolar(heap, puntero);
    }
    print_test("El heap tiene 10 elementos", heap_cantidad(heap) == 10);
    heap_destruir(heap, free);
}

void pruebas_heap_alumno() {
    pruebas_encolar();
    pruebas_desencolar();
    pruebas_volumen_heap();
    pruebas_heapify();
    pruebas_heapsort_menor_a_mayor();
    pruebas_heapsort_mayor_a_menor();
    pruebas_destruyendo();
}

int main(){
    pruebas_heap_alumno();
    return 0;
}