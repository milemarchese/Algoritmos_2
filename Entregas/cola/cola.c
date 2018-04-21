#include "cola.h"
#include <stdlib.h>

/*Definimos el struct cola_t y el struct auxiliar nodo*/
typedef struct nodo{
	void* dato;
	struct nodo* prox;	
} nodo_t;

struct cola{
	nodo_t* prim;
	nodo_t* ult;
} ;


/*Pimitivas de la cola*/

cola_t* cola_crear(void){
	cola_t* cola=malloc(sizeof(cola_t)); 
	if (cola==NULL){
		return NULL;
	}
	cola->prim=NULL;
	cola->ult=NULL;
	return cola;
}

void cola_destruir(cola_t *cola, void destruir_dato(void*)){
	while (!cola_esta_vacia(cola)){
		void* dato= cola_desencolar(cola);
		if(destruir_dato)
			destruir_dato(dato);
	}
	free(cola);
}

bool cola_esta_vacia(const cola_t *cola){
	return cola->prim == NULL;
}

bool cola_encolar(cola_t *cola, void* valor){
	nodo_t* nodo= malloc(sizeof(nodo_t));
	if(nodo==NULL)
		return false;
	nodo->dato= valor;
	nodo->prox= NULL;

	if (cola_esta_vacia(cola)){
		cola->prim=nodo;
	}
	else
		cola->ult->prox=nodo;
	cola->ult=nodo;
	return true;
} 

void* cola_ver_primero(const cola_t *cola){
	if(cola_esta_vacia(cola))
		return NULL;
	void* primero= cola->prim->dato;
	return primero;
}

void* cola_desencolar(cola_t *cola){
	if (cola_esta_vacia(cola))
			return NULL;
	void* dato= cola->prim->dato;
	nodo_t* aux= cola->prim;
	cola->prim=aux->prox;
	free(aux);
	if (!cola->prim)
		cola->ult=NULL;
	return dato;
}