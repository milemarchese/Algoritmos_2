//
// Created by Martin Feldsztejn (101320) and Milena Marchese (100962).
// Corrector: Matías Cano
//

#include "hash.h"
#include "lista.h"
#include <stdlib.h>
#include <string.h>

#define CAPACIDAD_INICIAL 10
#define MULTIPLICADOR 2
#define FACTOR_DE_CARGA 3

// ----------------- HASH ITEM -----------------
typedef struct hash_item {
    char *clave;
    void *valor;
} hash_item_t;

hash_item_t *crear_hash_item(const char *clave, void *dato) {
    hash_item_t *item = malloc(sizeof(hash_item_t));
    if (!item) {
        return NULL;
    }
    size_t largo = strlen(clave);
    char *clave_aux = malloc(sizeof(char) * (largo + 1));
    strcpy(clave_aux, clave);
    item->clave = clave_aux;
    item->valor = dato;
    return item;
}

void hash_item_destruir(hash_item_t *item, hash_destruir_dato_t destruir_dato) {
    if (destruir_dato) {
        destruir_dato(item->valor);
    }
    free(item->clave);
    free(item);
}
// ----------------- END HASH ITEM -----------------

// ----------------- HASH -----------------
struct hash {
    lista_t **tabla;
    size_t cantidad; // Cantidad
    size_t capacidad; // Capacidad
    hash_destruir_dato_t destruir_dato;
};

/**
 * Funcion de hashing para la clave
 * La robe de https://stackoverflow.com/a/7666577
 *
 * @param cp
 * @return
 */
size_t djb2_hash(const char *cp) {
    unsigned int hash = 5381;
    int c;

    while ((c = *cp++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

/**
 * Esta es la funcion que se deberia usar para hashear la clave. LLama a la funcion de hashing y le aplica el modulo del
 * tamaño del hash
 * @param hash
 * @param clave
 * @return
 */
size_t hash_value(const size_t capacidad, const char *clave) {
    return djb2_hash(clave) % capacidad;
}

/**
 * Busca un elemento en particular en la lista y devuelve un iterador en la posicion de dicho elemento. Si el elemento
 * no esta en la lista devuelve un iterador al final
 * @param lista La lista en la cual buscar
 * @param clave La clave para buscar
 * @return  Un iterador parado en la posicion del elemento o al final si este no esta.
 */
lista_iter_t *buscar(lista_t *lista, const char *clave) {
    lista_iter_t *iter = lista_iter_crear(lista);
    while (!lista_iter_al_final(iter)
           && strcmp(((hash_item_t *) lista_iter_ver_actual(iter))->clave, clave) != 0) {
        lista_iter_avanzar(iter);
    }
    return iter;
}

bool hash_redimensionar(hash_t *hash, size_t tamano) {
    lista_t **tabla_nueva = malloc(sizeof(hash_item_t) * tamano);
    if (!tabla_nueva) {
        return false;
    }
    for (int i = 0; i < tamano; i++) {
        tabla_nueva[i] = lista_crear();
    }
    for (int i = 0; i < hash->capacidad; i++) {
        while (!lista_esta_vacia(hash->tabla[i])) {
            hash_item_t *item = lista_borrar_primero(hash->tabla[i]);
            size_t posicion = hash_value(tamano, item->clave);
            lista_insertar_ultimo(tabla_nueva[posicion], item);
        }
        lista_destruir(hash->tabla[i], NULL);
    }
    free(hash->tabla);
    hash->capacidad = tamano;
    hash->tabla = tabla_nueva;
    return true;
}

/**
 * Crea un nuevo hash
 * @param destruir_dato la funcion que se usara a la hora de destruir un valor
 * @return un hash vacio o NULL si hubo problemas de memoria
 */
hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
    hash_t *hash = malloc(sizeof(hash_t));
    if (!hash) {
        return NULL;
    }

    hash->tabla = malloc(sizeof(hash_item_t) * CAPACIDAD_INICIAL);
    for (int i = 0; i < CAPACIDAD_INICIAL; i++) {
        hash->tabla[i] = lista_crear();
    }
    hash->cantidad = 0;
    hash->capacidad = CAPACIDAD_INICIAL;
    hash->destruir_dato = destruir_dato;
    return hash;
}

/**
 * Busca un elemento en el hash.
 * @param hash  El hash en el cual buscar
 * @param clave La clave que hay que buscar
 * @return El elemento si estuviera o NULL si no hay nada
 */
void *hash_obtener(const hash_t *hash, const char *clave) {
    size_t hash_key = hash_value(hash->capacidad, clave);
    lista_t *lista = hash->tabla[hash_key];

    lista_iter_t *iter = buscar(lista, clave);
    void *valor;
    if (lista_iter_al_final(iter)) {
        valor = NULL;
    } else {
        valor = ((hash_item_t *) lista_iter_ver_actual(iter))->valor;
    }
    lista_iter_destruir(iter);
    return valor;
}

/**
 * Borra un elemento del hash
 * @param hash  El hash
 * @param clave La clave a borrar
 * @return El elemento borrado
 */
void *hash_borrar(hash_t *hash, const char *clave) {
    size_t hash_key = hash_value(hash->capacidad, clave);
    lista_t *lista = hash->tabla[hash_key];
    if (hash->cantidad / hash->capacidad == FACTOR_DE_CARGA) {
        hash_redimensionar(hash, hash->capacidad / MULTIPLICADOR);
    }

    lista_iter_t *iter = buscar(lista, clave);
    void *valor;
    if (lista_iter_al_final(iter)) {
        valor = NULL;
    } else {
        hash_item_t *item = lista_iter_borrar(iter);
        valor = item->valor;
        hash_item_destruir(item, NULL);
        hash->cantidad--;
    }
    lista_iter_destruir(iter);
    return valor;
}

/**
 * Devuelve la cantidad de elementos en el hash
 * @param hash
 * @return
 */
size_t hash_cantidad(const hash_t *hash) {
    return hash->cantidad;
}

/**
 * Guarda un nuevo dato con la clave en el hash
 * @param hash
 * @param clave
 * @param dato
 * @return
 */
bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
    if (hash->cantidad / hash->capacidad >= FACTOR_DE_CARGA) {
        hash_redimensionar(hash, hash->capacidad * MULTIPLICADOR);
    }

    size_t hash_key = hash_value(hash->capacidad, clave);
    lista_t *lista = hash->tabla[hash_key];
    lista_iter_t *iter = buscar(lista, clave);
    if (!lista_iter_al_final(iter)) {
        hash_item_t *item_viejo = lista_iter_ver_actual(iter);
        if(hash->destruir_dato)
            hash->destruir_dato(item_viejo->valor);
        item_viejo->valor = dato;
    } else {
        hash_item_t *item = crear_hash_item(clave, dato);
        if (!item) {
            return false;
        }
        hash->cantidad++;
        lista_iter_insertar(iter, item);
    }
    lista_iter_destruir(iter);
    return true;

}

/**
 * Valida si la clave pertenece al hash
 * @param hash
 * @param clave
 * @return
 */
bool hash_pertenece(const hash_t *hash, const char *clave) {
    size_t hash_key = hash_value(hash->capacidad, clave);
    lista_t *lista = hash->tabla[hash_key];
    lista_iter_t *iter = buscar(lista, clave);
    bool pertenece = !lista_iter_al_final(iter);
    lista_iter_destruir(iter);
    return pertenece;
}

/**
 * Destruye un hash
 * @param hash
 */
void hash_destruir(hash_t *hash) {
    for (int i = 0; i < hash->capacidad; i++) {
        lista_t *lista = hash->tabla[i];
        while (lista_ver_primero(lista)) {
            hash_item_destruir((hash_item_t *) lista_borrar_primero(lista), hash->destruir_dato);
        }
        lista_destruir(lista, NULL);
    }
    free(hash->tabla);
    free(hash);
}
// ----------------- END HASH -----------------

// ----------------- HASH ITERATOR -----------------
struct hash_iter {
    const hash_t *hash;
    size_t iterados;
    size_t indice;
    lista_iter_t *iter_lista;

};

/**
 * Busca la siguiente lista no vacia
 * @param iter
 * @param indice el indice desde donde empezar
 * @return la lista
 */
lista_t *buscar_proximo(hash_iter_t *iter) {
    while (!hash_iter_al_final(iter) && lista_esta_vacia(iter->hash->tabla[iter->indice])) {
        iter->indice++;
    }

    if (hash_iter_al_final(iter)) {
        return NULL;
    }
    return iter->hash->tabla[iter->indice];
}

/**
 * Crea un nuevo iterador para un hash
 * @param hash
 * @return
 */
hash_iter_t *hash_iter_crear(const hash_t *hash) {
    hash_iter_t *iter = malloc(sizeof(hash_iter_t));
    if (!iter) {
        return NULL;
    }
    iter->hash = hash;
    iter->iterados = 0;
    iter->indice = 0;
    lista_t *lista = buscar_proximo(iter);
    lista_iter_t *iter_lista = NULL;
    if (lista) {
        iter_lista = lista_iter_crear(lista);
    }
    iter->iter_lista = iter_lista;
    return iter;
}

/**
 * Valida que el iterador no este al final del hash
 * @param iter
 * @return
 */
bool hash_iter_al_final(const hash_iter_t *iter) {
    return iter->iterados == iter->hash->cantidad || iter->indice == iter->hash->capacidad;
}

/**
 * Avanza al proximo elemento del hash
 * @param iter
 * @return
 */
bool hash_iter_avanzar(hash_iter_t *iter) {
    if (hash_iter_al_final(iter)) {
        return false;
    }
    iter->iterados++;
    lista_iter_avanzar(iter->iter_lista);
    if (lista_iter_al_final(iter->iter_lista)) {
        lista_iter_destruir(iter->iter_lista);
        iter->indice++;
        lista_t *lista = buscar_proximo(iter);
        if (!lista) { // Si la lista es NULL el iterador esta al final
            iter->iter_lista = NULL;
            return false;
        }
        lista_iter_t *iter_lista = lista_iter_crear(lista);
        if (!iter_lista) {
            return false;
        }
        iter->iter_lista = iter_lista;
    }
    return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
    // Devuelvo esto directo
    if (hash_iter_al_final(iter)) {
        return NULL;
    }
    return ((hash_item_t *) lista_iter_ver_actual(iter->iter_lista))->clave;
}

void hash_iter_destruir(hash_iter_t *iter) {
    if (iter->iter_lista) {
        lista_iter_destruir(iter->iter_lista);
    }
    free(iter);
}
// ----------------- END HASH ITERATOR -----------------