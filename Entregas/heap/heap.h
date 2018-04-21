#include <stddef.h>
#include <stdbool.h>
#ifndef HEAP_H
#define HEAP_H

struct heap;
typedef struct heap heap_t;

typedef int (*cmp_func_t) (const void *a, const void *b);

// Crea un heap vacio
heap_t *heap_crear(cmp_func_t cmp);

// Crea un heap utilizando el arreglo provisto
heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp);

// Destruye el heap y todos los elementos que aun esten en el
void heap_destruir(heap_t *heap, void destruir_elemento(void *e));

// Devuelve la cantidad de elementos en el heap
size_t heap_cantidad(const heap_t *heap);

// True si el heap esta vacio, false sino
bool heap_esta_vacio(const heap_t *heap);

// Agrega un nuevo elemento al heap
bool heap_encolar(heap_t *heap, void *elem);

// Devuelve el valor maximo del heap
void *heap_ver_tope(const heap_t *heap);

// Devuelve y remueve el valor maximo del heap
void *heap_desencolar(heap_t *heap);

// Ordena el array utilizando heapsort
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp);

#endif // HEAP_H
