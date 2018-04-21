#ifndef LISTA_H
#define LISTA_H
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/* La lista en sí está definida en el .c.  */

struct lista;
typedef struct lista lista_t;

struct  lista_iter;
typedef struct lista_iter lista_iter_t;


// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t* lista_crear(void);

// Destruye la lista.
// Pre: la lista fue creada.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t* lista, void destruir_dato(void *));

// Devuelve verdadero o falso, según si la lista tiene o no elementos.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t* lista);

// Agrega un nuevo elemento a la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor ahora es el primero.
bool lista_insertar_primero(lista_t* lista, void* dato);

// Agrega un nuevo elemento a la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor ahora es el ultimo.
bool lista_insertar_ultimo(lista_t* lista, void* dato);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primer elemento de la lista, y se devuelve ese valor. Si la lista está vacía, devuelve
// NULL.
// Pre: la lista fue creada.
// Post: si la lista no estaba vacía, se devuelve el valor del primer elemento
// y la lista contiene un elemento menos.
void* lista_borrar_primero(lista_t* lista);

// Obtiene el primer valor de la lista. Si la lista tiene elementos,
// se devuelve el primer valor. Si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer valor de la lista, cuando la lista no está
// vacía, NULL en caso contrario.
void* lista_ver_primero(const lista_t* lista);

// Obtiene el ultimo valor de la lista. Si la lista tiene elementos,
// se devuelve el ultimo valor. Si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el ultimo valor de la lista, cuando la lista no está
// vacía, NULL en caso contrario.
void* lista_ver_ultimo(const lista_t* lista);


// Devuelve el largo de la lista. Si la lista contiene elementos
// devuelve el largo, si esta vacia es 0.
// Pre: la lista fue creada.
// Post: se devolvio el largo y la lista queda intacta.
size_t lista_largo(const lista_t* lista);

//Recorre toda la lista y aplica a cada elemento la funcion visitar,
//devuelve el resultado del total de la funcion.
//Pre: la lista fue creada.
//Post: devuelve la sumatoria del resultado de aplicar la funcion a cada elemento.
void lista_iterar(lista_t* lista, bool visitar(void* dato, void* extra), void* extra);

//Crea un iterador.
//Pre:la lista fue creada.
//Post: devuelve un iterador de una lista.
lista_iter_t* lista_iter_crear(lista_t* lista);

//Avanza una posicion en la lista.
//Pre: el iterador fue creado.
//Post: el iterador se encuentra una posicion mas adelante.
bool lista_iter_avanzar(lista_iter_t* iter);

//Devuelve el valor de la posicion donde se encuentra el iterador.
//Pre: el iterador fue creado.
//Post: devuleve el valor al actal, el iterador no se mueve.
void* lista_iter_ver_actual(const lista_iter_t* iter);

//Devuelve verdadero si el iterador se encuentra en el ultimo
//elemento de la lista, en caso contrario devuelve falso.
//Pre: el iterador fue creado.
//Post: devuelve verdadero si esta a final, falso si no. 
//El iterador no se mueve.
bool lista_iter_al_final(const lista_iter_t* iter);

//Destruye un iterador.
//Pre: el iterador fue creado.
//Post: el iterador fue destruido.
void lista_iter_destruir(lista_iter_t* iter);

//Inserta un nuevo elemento a la lista en la posicion 
//donde esta el iterador.
//Pre: el iterador fue creado.
//Post: se inserto un nuevo elemento y el iterador apunta a ese elemento.
bool lista_iter_insertar(lista_iter_t* iter, void* dato);

//Elimina el elemento donde se encuentra el iterador.
//Pre: el iterador fue creado.
//Post: se elimino el elemento donde apuntaba el iterador y ahora
//apunta hacia el siguiente.
void* lista_iter_borrar(lista_iter_t* iter);


// Realiza pruebas sobre la implementación de la lista.
void pruebas_lista_alumno(void);

#endif // LISTA_H
