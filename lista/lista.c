#include "lista.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

//Definimos el struct lista_t y el struct auxiliar nodo*/

typedef struct nodo{
	void* dato;
	struct nodo* prox;	
} nodo_t;

struct lista{
	nodo_t* prim;
	size_t largo;
	nodo_t* ult;
} ;


lista_t* lista_crear(void){
	lista_t* lista=malloc(sizeof(lista_t));
	if (lista==NULL)
		return NULL;	
	lista->prim=NULL;
	lista->largo= 0;
	lista->ult=NULL;
	return lista;
}

//Funcion auxiliar para crear nodos.
//Post: el proximo del nodo apunta a NULL.
nodo_t* nodo_crear(void* dato){
	nodo_t* nodo=malloc(sizeof(nodo_t));
	if (!nodo)
		return NULL;
	nodo->dato= dato;
	nodo->prox= NULL;
	return nodo;
}

bool lista_esta_vacia(const lista_t* lista){
	return lista->largo==0;
}

bool lista_insertar_primero(lista_t* lista, void* dato){
	nodo_t* nodo= nodo_crear(dato);
	if (nodo==NULL){
		return false;
	}
	if (lista_esta_vacia(lista)){		
		lista->ult=nodo;
	}
	else{
		nodo_t* aux=lista->prim;
		nodo->prox=aux;
	}
	lista->prim=nodo;	
	lista->largo++;
	return true;
}
 
bool lista_insertar_ultimo(lista_t* lista, void* dato){
	nodo_t* nodo= nodo_crear(dato);
	if (nodo==NULL){
		return false;
	}
	if (lista_esta_vacia(lista)){
		lista->prim=nodo;
	}
	else{
		lista->ult->prox=nodo;
	}

	lista->ult=nodo;
	lista->largo++;
	return true;
}

void* lista_borrar_primero(lista_t* lista){
	if(lista_esta_vacia(lista))
		return NULL;
	void* dato= lista->prim->dato;
	nodo_t* aux= lista->prim;
	lista->prim=aux->prox;
	free(aux);
	lista->largo--;
	if (!lista->largo)
		lista->ult=NULL;
	return dato;
}

void* lista_ver_primero(const lista_t* lista){
	if(lista_esta_vacia(lista))
		return NULL;
	void* dato=lista->prim->dato;
	return dato;
}

void* lista_ver_ultimo(const lista_t* lista){
	if (lista_esta_vacia(lista))
		return NULL;
	void* dato=lista->ult->dato;
	return dato;
}

size_t lista_largo(const lista_t* lista){
	return lista->largo;
}


void lista_destruir(lista_t* lista, void destruir_dato(void*)){
	while (!lista_esta_vacia(lista)){
		void* dato= lista_borrar_primero(lista);
		if(destruir_dato)
			destruir_dato(dato);
	}
	free(lista);
}

void lista_iterar(lista_t* lista, bool visitar(void* dato, void* extra), void* extra){
	nodo_t* actual=lista->prim;
		while(actual){
			if(!visitar(actual->dato, extra))
				return;
			actual=actual->prox;
		}	
}

struct lista_iter{
	nodo_t* n_actual;
	nodo_t* n_anterior;
	lista_t* lista;
};

lista_iter_t* lista_iter_crear(lista_t* lista){
	lista_iter_t* iter=malloc(sizeof(lista_iter_t));
	if (!iter)
		return NULL;
	iter->n_actual= lista->prim;
	iter->n_anterior=  NULL;
	iter->lista= lista;
	return iter;
}

bool lista_iter_avanzar(lista_iter_t* iter){
	if(!iter->n_actual)
		return false;
	nodo_t* aux= iter->n_actual;
	iter->n_anterior=iter->n_actual;
	iter->n_actual=aux->prox;
	return true;
}

void* lista_iter_ver_actual(const lista_iter_t* iter){
	if (!iter->n_actual)
		return NULL;
	void* dato= lista_esta_vacia(iter->lista)?NULL:iter->n_actual->dato;
	return dato;
}

bool lista_iter_al_final(const lista_iter_t* iter){
	return iter->n_actual==NULL;
}

void lista_iter_destruir(lista_iter_t* iter){
	free(iter);
}


bool lista_iter_insertar(lista_iter_t* iter, void* dato){
	nodo_t* nodo= nodo_crear(dato);
	if (nodo==NULL){
		return false;
	}
	if(iter->n_anterior==NULL){
		nodo_t* aux= iter->lista->prim;
		nodo->prox=aux;
		iter->lista->prim=nodo;
		if(!nodo->prox)
			iter->lista->ult=nodo;
	}
	else{
		iter->n_anterior->prox=nodo;
		nodo->prox=iter->n_actual;
		if(!iter->n_actual)
			iter->lista->ult=nodo;	
	}
	iter->n_actual=nodo;
	iter->lista->largo++;
	return true;
}


		
void* lista_iter_borrar(lista_iter_t* iter){
	if(lista_esta_vacia(iter->lista)){
		return NULL;
	}
	if(!iter->n_actual){
		return NULL;
	}
	
	void* dato;
	nodo_t* aux= iter->n_actual;
	dato= aux->dato;
	iter->n_actual=aux->prox;	
	if(!iter->n_anterior){		
		iter->lista->prim=iter->n_actual;		
	}
	else{
		iter->n_anterior->prox=iter->n_actual;		
		if(!iter->n_actual)	
			iter->lista->ult=iter->n_anterior;	
	}
	free(aux);
	iter->lista->largo--;
	return dato;
}