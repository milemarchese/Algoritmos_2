//
// Created by Martin Feldsztejn (101320) on 9/18/17.
//
#include "lista.h"
#include <stdlib.h>

// --------- NODO ---------
typedef struct nodo nodo_t;
struct nodo {
    void *dato;
    nodo_t *proximo;
};

nodo_t *nodo_crear(void *dato, nodo_t *proximo) {
    nodo_t *nodo = malloc(sizeof(nodo_t));
    if (!nodo) return NULL;

    nodo->dato = dato;
    nodo->proximo = proximo;
    return nodo;
}

void nodo_destruir(nodo_t *nodo) {
    free(nodo);
}
// --------- END NODO ---------

// --------- LISTA ---------
struct lista {
    size_t cantidad;
    nodo_t *primero;
    nodo_t *ultimo;
};

lista_t *lista_crear() {
    lista_t *lista = malloc(sizeof(lista_t));
    if (!lista) {
        return NULL;
    }
    lista->cantidad = 0;
    lista->primero = NULL;
    lista->ultimo = NULL;
    return lista;
}

size_t lista_largo(const lista_t *lista) {
    return lista->cantidad;
}

void lista_destruir(lista_t *lista, void destruir_dato(void *)) {
    while (!lista_esta_vacia(lista)) {
        void *dato = lista_borrar_primero(lista);
        if (destruir_dato) {
            destruir_dato(dato);
        }
    }
    free(lista);
}

void *lista_ver_primero(const lista_t *lista) {
    if (!lista->primero) return NULL;
    return lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t *lista) {
    if (!lista->ultimo) return NULL;
    return lista->ultimo->dato;
}

bool lista_insertar_primero(lista_t *lista, void *dato) {
    nodo_t *nodo = nodo_crear(dato, lista->primero);
    if (!nodo) {
        return false;
    }

    if (lista_esta_vacia(lista)) {
        lista->ultimo = nodo;
    }
    lista->primero = nodo;
    lista->cantidad++;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
    nodo_t *nodo = nodo_crear(dato, NULL);
    if (!nodo) {
        return false;
    }

    if (lista_esta_vacia(lista)) {
        lista->primero = nodo;
    } else {
        lista->ultimo->proximo = nodo;
    }
    lista->ultimo = nodo;
    lista->cantidad++;
    return true;
}

void *lista_borrar_primero(lista_t *lista) {
    if (lista_esta_vacia(lista)) return NULL;

    nodo_t *primero = lista->primero;
    void *dato = primero->dato;
    lista->primero = primero->proximo;
    nodo_destruir(primero);
    lista->cantidad--;

    if (lista->cantidad == 0) {
        lista->ultimo = NULL;
    }
    return dato;
}

bool lista_esta_vacia(const lista_t *lista) {
    return lista->cantidad == 0 || !lista->primero || !lista->ultimo;
}

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
    if (visitar == NULL) return;

    nodo_t *actual = lista->primero;
    while (actual) {
        if (!visitar(actual->dato, extra)) {
            break;
        }
        actual = actual->proximo;
    }
}
// --------- END LISTA ---------

// --------- ITERADOR ---------
struct lista_iter {
    lista_t *lista;
    nodo_t *anterior;
    nodo_t *actual;
};

lista_iter_t *lista_iter_crear(lista_t *lista) {
    lista_iter_t *iterador = malloc(sizeof(lista_iter_t));
    if (!iterador) return NULL;

    iterador->lista = lista;
    iterador->actual = lista->primero;
    iterador->anterior = NULL;
    return iterador;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
    if (lista_iter_al_final(iter)) {
        return false;
    }
    iter->anterior = iter->actual;
    iter->actual = iter->actual->proximo;
    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter) {
    if (!iter->actual) return NULL;
    return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter) {
    return !iter->actual;
}

void lista_iter_destruir(lista_iter_t *iter) {
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
    nodo_t *nodo = nodo_crear(dato, iter->actual);
    if (!nodo) return false;

    if (lista_iter_al_final(iter)) {
        iter->lista->ultimo = nodo;
    }

    if (iter->anterior) {
        iter->anterior->proximo = nodo;
    } else {
        iter->lista->primero = nodo; // Si no tengo anterior es el principio de la lista
    }
    iter->actual = nodo;
    iter->lista->cantidad++;
    return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
    if (lista_iter_al_final(iter)) return NULL;

    nodo_t *destruir = iter->actual;
    iter->actual = destruir->proximo;

    if (destruir == iter->lista->primero) {
        iter->lista->primero = destruir->proximo;
    } else {
        iter->anterior->proximo = iter->actual; //No era el primero, entonces cambio el anterior
    }

    if (destruir == iter->lista->ultimo) {
        iter->lista->ultimo = iter->anterior;
    }

    void *valor = destruir->dato;
    nodo_destruir(destruir);
    iter->lista->cantidad--;
    return valor;
}
// --------- END ITERADOR ---------
