#include "heap.h"
#include <stdlib.h>
#include <string.h>

#define LARGO_INICIAL 15
#define REDIMENSION 2

#define PADRE(hijo) (((hijo) - 1) / 2)
#define HIJO_IZQ(padre) (((padre) * 2) + 1)
#define HIJO_DER(padre) (((padre) * 2) + 2)
// Si devuelve -1 el primero es menor
// Si devuelve 0 son iguales
// Si devuelve 1 el primer es mayor
#define COMPARAR(cmp, padre, hijo) ((cmp)((padre), (hijo)))

struct heap {
    cmp_func_t cmp;
    void **arreglo;
    size_t largo;
    size_t cantidad;
};

// ----------- UTILS ----------
void swap(void **x, void **y) {
    void *aux = *x;
    *x = *y;
    *y = aux;
}

void upheap(void *arreglo[], cmp_func_t cmp, size_t posicion) {
    size_t padre = PADRE(posicion);
    if (posicion == 0 || COMPARAR(cmp, arreglo[padre], arreglo[posicion]) >= 0) {
        return;
    }
    swap(&arreglo[posicion], &arreglo[padre]);
    upheap(arreglo, cmp, padre);
}

size_t obtener_hijo_mayor(void* arreglo[], cmp_func_t cmp, size_t padre, size_t largo){
    size_t hijo_izq = HIJO_IZQ(padre);
    size_t hijo_der = HIJO_DER(padre);
    size_t pos_mayor;
    bool izq_mayor = COMPARAR(cmp, arreglo[padre], arreglo[hijo_izq]) < 0;
    bool der_mayor = hijo_der < largo && COMPARAR(cmp, arreglo[padre], arreglo[hijo_der]) < 0;
    if (izq_mayor && der_mayor) {
        bool izq_mayor_hijos = hijo_der >= largo || COMPARAR(cmp, arreglo[hijo_izq], arreglo[hijo_der]) > 0;
        pos_mayor = izq_mayor_hijos ? hijo_izq : hijo_der;
    } else {
        pos_mayor = izq_mayor ? hijo_izq : hijo_der;
    }
    return pos_mayor;
}

bool es_mayor_a_ambos_hijos(void* arreglo[], cmp_func_t cmp, size_t padre, size_t largo){
    size_t hijo_izq = HIJO_IZQ(padre);
    size_t hijo_der = HIJO_DER(padre);
    return COMPARAR(cmp, arreglo[padre], arreglo[hijo_izq]) >= 0 && // Es mas grande al izq
    (hijo_der >= largo || COMPARAR(cmp, arreglo[padre], arreglo[hijo_der]) >= 0); // Existe el derecho y es mas chico
}

void downheap(void *arreglo[], cmp_func_t cmp, size_t posicion, size_t largo) {
    if (HIJO_IZQ(posicion) >= largo) {
        //soy una hoja
        return;
    }
    if (es_mayor_a_ambos_hijos(arreglo, cmp, posicion, largo)) {
        // El padre es mas grande que los dos hijos, todo cool
        return;
    }
    // Uno de los dos tiene que ser mas chico

    size_t hijo_mayor = obtener_hijo_mayor(arreglo, cmp, posicion, largo);
    swap(&arreglo[posicion], &arreglo[hijo_mayor]);

    downheap(arreglo, cmp, hijo_mayor, largo);
}
// ------------ END UTILS ---------

heap_t *_crear_heap(cmp_func_t cmp, size_t cantidad, size_t largo) {
    heap_t *heap = malloc(sizeof(heap_t));
    if (!heap) {
        return NULL;
    }
    heap->largo = largo;
    heap->cantidad = cantidad;
    heap->cmp = cmp;
    return heap;
}

heap_t *heap_crear(cmp_func_t cmp) {
    heap_t *heap = _crear_heap(cmp, 0, LARGO_INICIAL);
    if (!heap) return NULL;
    void *arreglo = malloc(sizeof(void *) * LARGO_INICIAL);
    if (!arreglo) {
        free(heap);
        return NULL;
    }
    heap->arreglo = arreglo;
    return heap;
}

void heapify(void *arreglo[], cmp_func_t cmp, size_t n) {
    size_t k = n/2 + 1;
    do {
        downheap(arreglo, cmp, k - 1, n);
        k--;
    } while (k > 0); // Para fixear que el size_t no puede ser negativo
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
    heap_t *heap = _crear_heap(cmp, n, n);
    if (!heap) return NULL;
    void** arreglo_copia = malloc(sizeof(void*) * n);
    if (!arreglo_copia){
        free(heap);
        return NULL;
    }
    memcpy(arreglo_copia, arreglo, sizeof(void*) * n);
    heapify(arreglo_copia, cmp, n);
    heap->arreglo = arreglo_copia;
    return heap;
}


bool redimensionar(heap_t *heap, size_t largo_nuevo) {
    void **arreglo = heap->arreglo;
    void **arreglo_nuevo = realloc(arreglo, sizeof(void *) * largo_nuevo);
    if (!arreglo_nuevo) {
        return false;
    }
    heap->arreglo = arreglo_nuevo;
    heap->largo = largo_nuevo;
    return true;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)) {
    if (destruir_elemento) {
        for (int i = 0; i < heap->cantidad; i++) {
            destruir_elemento(heap->arreglo[i]);
        }
    }
    free(heap->arreglo);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap) {
    return heap->cantidad;

}

bool heap_esta_vacio(const heap_t *heap) {
    return heap->cantidad == 0;
}

bool heap_encolar(heap_t *heap, void *elem) {
    if (heap->cantidad + 1 == heap->largo) {
        size_t largo_nuevo = heap->largo * REDIMENSION;
        if (!redimensionar(heap, largo_nuevo)) {
            return false;
        }
    }
    heap->arreglo[heap->cantidad] = elem;
    upheap(heap->arreglo, heap->cmp, heap->cantidad);
    heap->cantidad++;
    return true;
}

void *heap_ver_tope(const heap_t *heap) {
    if(heap_esta_vacio(heap)){
        return NULL;
    }
    return heap->arreglo[0];
}

void *heap_desencolar(heap_t *heap) {
    if (heap_esta_vacio(heap)) {
        return NULL;
    }
    if(heap->cantidad / 4 == heap->largo){
        redimensionar(heap, heap->largo / REDIMENSION);
    }
    void *desencolado = heap->arreglo[0];
    heap->cantidad--;
    heap->arreglo[0] = heap->arreglo[heap->cantidad];
    downheap(heap->arreglo, heap->cmp, 0, heap->cantidad);
    return desencolado;

}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {
    heapify(elementos, cmp, cant);
    size_t k = cant;
    do {
        swap(&elementos[0], &elementos[k - 1]);
        k--;
        downheap(elementos, cmp, 0, k);
    } while (k > 0);
}
