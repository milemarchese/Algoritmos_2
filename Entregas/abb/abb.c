//
// Created by Martin Feldsztejn on 10/9/17.
//

#include <stdlib.h>
#include <string.h>
#include "abb.h"
#include "pila.h"

typedef struct nodo nodo_t;
struct nodo {
    char *clave;
    void *valor;
    nodo_t *izq;
    nodo_t *der;
};

void nodo_clave_destruir(char *clave) {
    free(clave);
}

nodo_t *nodo_crear(const char *clave, void *valor) {
    nodo_t *nodo = malloc(sizeof(nodo_t));
    if (!nodo) {
        return NULL;
    }
    char *clave_aux = malloc(sizeof(char) * (strlen(clave) + 1));
    if (!clave_aux){
        free(nodo);
        return NULL;
    }
    strcpy(clave_aux, clave);
    nodo->clave = clave_aux;
    nodo->valor = valor;
    nodo->der = NULL;
    nodo->izq = NULL;
    return nodo;
}

void nodo_destruir(abb_destruir_dato_t destruir_dato, nodo_t *nodo) {
    if (destruir_dato) {
        destruir_dato(nodo->valor);
    }
    nodo_clave_destruir(nodo->clave);
    free(nodo);
}

struct abb {
    abb_comparar_clave_t comparar_clave;
    abb_destruir_dato_t destruir_dato;
    nodo_t *raiz;
    size_t cantidad;
};

int comparar_clave(const abb_t *abb, const char *clave_guardada, const char *clave_nueva) {
    return abb->comparar_clave(clave_guardada, clave_nueva);
}

abb_t *abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
    abb_t *abb = malloc(sizeof(abb_t));
    if (!abb) {
        return NULL;
    }
    abb->raiz = NULL;
    abb->comparar_clave = cmp;
    abb->destruir_dato = destruir_dato;
    abb->cantidad = 0;
    return abb;
}

bool abb_insertar_nodo(abb_t *abb, nodo_t *actual, nodo_t *nuevo_nodo) {
    int comparacion = comparar_clave(abb, actual->clave, nuevo_nodo->clave);
    if (comparacion == 0) {
        if (abb->destruir_dato)
            abb->destruir_dato(actual->valor);
        actual->valor = nuevo_nodo->valor;
        //Destruyo el nuevo nodo
        nodo_destruir(NULL, nuevo_nodo);
    } else if (comparacion < 0) {
        //La clave actual es mas chica, me voy a la derecha
        if (actual->der) {
            abb_insertar_nodo(abb, actual->der, nuevo_nodo);
        } else {
            abb->cantidad++;
            actual->der = nuevo_nodo;
        }
    } else {
        //La clave actual es mas grande, me voy a la izq
        if (actual->izq) {
            abb_insertar_nodo(abb, actual->izq, nuevo_nodo);
        } else {
            abb->cantidad++;
            actual->izq = nuevo_nodo;
        }
    }
    return true;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
    nodo_t *nodo = nodo_crear(clave, dato);
    if (!nodo) {
        return false;
    }
    if (!arbol->raiz) {
        arbol->cantidad++;
        arbol->raiz = nodo;
        return true;
    }

    return abb_insertar_nodo(arbol, arbol->raiz, nodo);
}

nodo_t *abb_obtener_nodo(const abb_t *abb, nodo_t *nodo, const char *clave) {
    if (!nodo) {
        return NULL;
    }
    int comparacion = comparar_clave(abb, nodo->clave, clave);

    if (comparacion == 0) {
        return nodo;
    }
    if (comparacion < 0) {
        return abb_obtener_nodo(abb, nodo->der, clave);
    }
    return abb_obtener_nodo(abb, nodo->izq, clave);
    
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
    nodo_t *nodo = abb_obtener_nodo(arbol, arbol->raiz, clave);
    if (!nodo) {
        return NULL;
    }
    return nodo->valor;
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
    nodo_t *nodo = abb_obtener_nodo(arbol, arbol->raiz, clave);
    return nodo != NULL;
}

nodo_t *abb_nodo_minimo(nodo_t *nodo) {
    nodo_t *act = nodo;
    while (act->izq) {
        act = act->izq;
    }
    return act;
}

nodo_t *abb_nodo_maximo(nodo_t *nodo) {
    nodo_t *act = nodo;
    while (act->der) {
        act = act->der;
    }
    return act;
}

nodo_t *abb_nodo_sucesor(nodo_t *nodo) {
    if (nodo->der) {
        return abb_nodo_minimo(nodo->der);
    }
    if (nodo->izq) {
        return abb_nodo_maximo(nodo->izq);
    }
    return NULL;
}

void *xborrar(abb_t *abb, nodo_t *actual, nodo_t *padre, const char *clave) {
    int comparacion = comparar_clave(abb, actual->clave, clave);
    if (comparacion == 0) {
        if (actual->izq && actual->der) {
            // Caso tiene dos hijos
            nodo_t *sucesor = abb_nodo_sucesor(actual);
            xborrar(abb, actual->der, actual, sucesor->clave);
            sucesor->der = actual->der;
            sucesor->izq = actual->izq;
            if (padre) {
                if (actual == padre->der) {
                    padre->der = sucesor;
                } else {
                    padre->izq = sucesor;
                }
            } else {
                abb->raiz = sucesor;
            }
        } else if (actual->izq) {
            // Caso solo tengo el hijo izq
            if (padre) {
                if (actual == padre->izq) {
                    padre->izq = actual->izq;
                } else {
                    padre->der = actual->izq;
                }
            } else {
                abb->raiz = actual->izq;
            }
        } else if (actual->der) {
            // Caso solo tengo el hijo der
            if (padre) {
                if (actual == padre->izq)
                    padre->izq = actual->der;
                else
                    padre->der = actual->der;
            } else {
                abb->raiz = actual->der;
            }
        } else {
            //Caso hoja
            if (padre) {
                if (padre->der == actual) {
                    padre->der = NULL;
                } else {
                    padre->izq = NULL;
                }
            } else {
                // Si soy una hoja y no tengo padre soy el ultimo nodo
                abb->raiz = NULL;
            }
        }
        return actual;
    } else if (comparacion < 0) {
        return xborrar(abb, actual->der, actual, clave);
    } else {
        return xborrar(abb, actual->izq, actual, clave);
    }
}


void *abb_borrar(abb_t *arbol, const char *clave) {
    if (!abb_pertenece(arbol, clave)) {
        return NULL;
    }
    nodo_t *borrar = xborrar(arbol, arbol->raiz, NULL, clave);
    void *valor = borrar->valor;
    arbol->cantidad--;
    nodo_destruir(NULL, borrar);
    return valor;
}

size_t abb_cantidad(abb_t *arbol) {
    return arbol->cantidad;
}

void abb_nodo_destruir(abb_t *abb, nodo_t *nodo) {
    if (!nodo) {
        return;
    }
    abb_nodo_destruir(abb, nodo->izq);
    abb_nodo_destruir(abb, nodo->der);
    nodo_destruir(abb->destruir_dato, nodo);
}

void abb_destruir(abb_t *arbol) {
    // Destruyo todos los nodos
    abb_nodo_destruir(arbol, arbol->raiz);
    free(arbol);
}

// ----------- ITERADOR INTERNO -----------
void apilar_izquierdos(pila_t *pila, nodo_t *inicio) {
    nodo_t *actual = inicio;
    while (actual) {
        pila_apilar(pila, actual);
        actual = actual->izq;
    }
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
    pila_t *pila = pila_crear();
    if (!pila) {
        return;
    }

    apilar_izquierdos(pila, arbol->raiz);
    while (!pila_esta_vacia(pila)) {
        nodo_t *actual = pila_desapilar(pila);
        apilar_izquierdos(pila, actual->der);
        if (!visitar(actual->clave, actual->valor, extra)) {
            break;
        }
    }
    pila_destruir(pila);
}
// ----------- END ITERADOR INTERNO -----------

// ----------- ITERADOR -----------
struct abb_iter {
    pila_t *pila;
};

bool abb_iter_in_al_final(const abb_iter_t *iter) {
    return pila_esta_vacia(iter->pila);
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
    abb_iter_t *iter = malloc(sizeof(abb_iter_t));
    if (!iter) {
        return NULL;
    }

    pila_t *pila = pila_crear();
    if (!pila) {
        return NULL;
    }

    iter->pila = pila;
    apilar_izquierdos(iter->pila, arbol->raiz);
    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
    if (abb_iter_in_al_final(iter)) {
        return false;
    }
    nodo_t *actual = pila_desapilar(iter->pila);
    apilar_izquierdos(iter->pila, actual->der);
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
    if (abb_iter_in_al_final(iter)) {
        return NULL;
    }
    nodo_t *actual = pila_ver_tope(iter->pila);
    return actual->clave;
}

void abb_iter_in_destruir(abb_iter_t *iter) {
    pila_destruir(iter->pila);
    free(iter);
}
// ----------- END ITERADOR -----------