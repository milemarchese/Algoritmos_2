#include "pila.h"
#include <stdlib.h>
#define CAP_INI 30
#define REDIM 2
#define COND 4

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

bool redimensionar(pila_t *pila, size_t cap_nueva ){
	void* datos_nuevo = realloc(pila->datos, cap_nueva * sizeof(void*));
	if (!datos_nuevo)
		return false;
	pila->datos = datos_nuevo;
	pila->capacidad = cap_nueva;
    	return true;
	}

pila_t* pila_crear(void){
	pila_t* pila = malloc(sizeof(pila_t));
	if (!pila) {
		return NULL;
	}

	pila->datos = malloc(CAP_INI*sizeof(void*));
	if (!pila->datos) {
		free(pila);
		return NULL;
	}

	pila->cantidad = 0;
	pila->capacidad = CAP_INI;
	return pila;
}

void pila_destruir(pila_t *pila){
	free(pila->datos);
	free(pila);
}

bool pila_apilar(pila_t *pila, void* valor){
	if (pila->cantidad+1 > pila->capacidad){
		size_t cap_nueva = pila->capacidad * REDIM;	
		if (!redimensionar(pila,cap_nueva)){
			return false; 
		}
	}

	pila->datos[pila->cantidad] = valor;
	pila->cantidad += 1;
	return true;
}

void* pila_ver_tope(const pila_t *pila){
	if (pila_esta_vacia(pila)){
		return NULL;
	}

	return pila->datos[pila->cantidad - 1];
}

void* pila_desapilar(pila_t *pila){
	if (pila_esta_vacia(pila))
		return NULL;
	pila->cantidad -= 1;
	void* aux = pila->datos[pila->cantidad];
	if (pila->cantidad == (pila->capacidad/COND) && pila->cantidad> CAP_INI){
		size_t cap_nueva = pila->capacidad / REDIM;
		redimensionar(pila,cap_nueva);
	}

	return aux;
}

bool pila_esta_vacia(const pila_t *pila){
	return !pila->cantidad;
}
