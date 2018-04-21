#include <stdbool.h>
#include <stdlib.h>

#ifndef LISTA_H
#define LISTA_H
typedef struct lista lista_t;

// PRIMITIVAS LISTA
/**
Crea una lista vacia. Devuelve un puntero a la lista. El usuario debera encargarse de liberar la misma
*/
lista_t *lista_crear(void);

/**
Devuelve true si la lista esta vacia. Falso sino
*/
bool lista_esta_vacia(const lista_t *lista);

/**
Agrega un nuevo dato en la primera posicion de la lista
*/
bool lista_insertar_primero(lista_t *lista, void *dato);

/**
Agrega un nuevo dato en la ultima posicion de la lista
*/
bool lista_insertar_ultimo(lista_t *lista, void *dato);

/**
Borra el primer elemento de la lista
*/
void *lista_borrar_primero(lista_t *lista);

/**
Devuelve el primer elemento de la lista sin removerlo
*/
void *lista_ver_primero(const lista_t *lista);

/**
Devuelve el ultimo elemento de la lista sin removerlo
*/
void *lista_ver_ultimo(const lista_t *lista);

/**
Devuelve el largo de la lista
*/
size_t lista_largo(const lista_t *lista);

/**
Destruye la lista uqe se pasa por parametro. Si la funcion destruir_dato es != NULL se la llamara para cada dato almacenado en la lista.
*/
void lista_destruir(lista_t *lista, void destruir_dato(void *));

// PRIMITIVAS ITERADOR INTERNO
/**
Itera la lista. 
- La funcion visitar se llamara para cada elemento de la lista y debe ser necesariamente != NULL. Si la misma devuelve false
la lista dejara de iterarse.
- El parametro extra se le pasara a la funcion visitar
*/
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

#endif //LISTA_H


#ifndef LISTA_ITERADOR_H
#define LISTA_ITERADOR_H

typedef struct lista_iter lista_iter_t;

// PRIMITIVAS ITERADOR EXTERNO
/**
Crea un nuevo iterador que esta parado en la primera posicion de la lista o NULL si esta estuviera vacia
*/
lista_iter_t *lista_iter_crear(lista_t *lista);

/**
Se mueve a la siguiente posicion de la lista. Devuelve true si se puede, false si no
*/
bool lista_iter_avanzar(lista_iter_t *iter);

/**
Devuelve el valor del elemento actual
*/
void *lista_iter_ver_actual(const lista_iter_t *iter);

/**
Devuelve true si el iterador esta al final, false sino
*/
bool lista_iter_al_final(const lista_iter_t *iter);

/**
Destruye el iterador
*/
void lista_iter_destruir(lista_iter_t *iter);

/**
Inserta un nuevo elemento en la posicion actual. El actual pasara a ser el proximo del nuevo.
*/ 
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

/**
Borra el elemento actual de la lista
*/
void *lista_iter_borrar(lista_iter_t *iter);

#endif //LISTA_ITERADOR_H


// PRIMITIVAS PARA TESTING
void pruebas_lista_alumno(void);
