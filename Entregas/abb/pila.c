#include "pila.h"
#include <stdlib.h>
#include <stdio.h>

#define PILA_TAMANO_INICIAL 20
#define MULTIPLICADOR_TAMANO 2
#define PILA_VACIA 0

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t *pila_crear() {
    pila_t *pila = malloc(sizeof(pila_t));
    if (pila == NULL) {
        return NULL;
    }

    pila->datos = malloc(sizeof(void *) * PILA_TAMANO_INICIAL);
    if (pila->datos == NULL) {
        free(pila);
        return NULL;
    }

    pila->capacidad = PILA_TAMANO_INICIAL;
    pila->cantidad = PILA_VACIA;

    return pila;
}

void pila_destruir(pila_t *pila) {
    free(pila->datos);
    free(pila);
}

bool pila_redimensionar(pila_t *pila, size_t tamano) {
    void **datos_nuevos = realloc(pila->datos, tamano * sizeof(void *));

    if (datos_nuevos == NULL) {
        return false;
    }

    pila->capacidad = tamano;
    pila->datos = datos_nuevos;

    return true;
}

bool pila_esta_vacia(const pila_t *pila) {
    return pila->cantidad == PILA_VACIA;
}

bool pila_apilar(pila_t *pila, void *valor) {
    if (pila->capacidad == pila->cantidad) {
        bool redimensiono = pila_redimensionar(pila, pila->capacidad * MULTIPLICADOR_TAMANO);
        if (!redimensiono) {
            return false;
        }
    }

    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;

    return true;
}

void *pila_ver_tope(const pila_t *pila) {
    if (pila->cantidad == 0) {
        return NULL;
    }

    return pila->datos[pila->cantidad - 1];
}

void *pila_desapilar(pila_t *pila) {
    if (pila->capacidad / 4 == pila->cantidad && pila->capacidad > PILA_TAMANO_INICIAL) {
        pila_redimensionar(pila, pila->capacidad / MULTIPLICADOR_TAMANO);
    }
    if (pila->cantidad == 0) {
        return NULL;
    }
    return pila->datos[--pila->cantidad];
}
