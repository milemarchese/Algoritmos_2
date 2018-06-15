#include "lista.h"
#include "pila.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#define VOLUMEN 300

void pruebas_crear_lista(void){
//Pruebas en una lista recien creada.
	lista_t* lista= lista_crear();
	print_test("Una lista recien creada esta vacia",lista_esta_vacia(lista));
	print_test("Borrar primero es NULL",lista_borrar_primero(lista)==NULL);
	print_test("Ver primero es NULL",lista_ver_primero(lista)==NULL);
	print_test("Ver ultimo es NULL", lista_ver_ultimo(lista)==NULL);
	print_test("Largo de lista es 0", !lista_largo(lista));
	lista_destruir(lista, NULL);
	print_test("La lista fue destruida", true);
}

void pruebas_llenar_lista(void){
//Pruebas agregando elementos a la lista.
	lista_t* lista= lista_crear();	
    int aux1= 5;
	float aux2= 3.14159F;
	char cadena[]="hola";
	int valores[]={10,20,30};
	void* p= &aux1;
	void* q= &aux2;


	print_test("Agregamos un elemento 1 a la lista",lista_insertar_primero(lista,p));
	print_test("La lista no esta vacia",!lista_esta_vacia(lista));
	print_test("El largo de la lista es 1",lista_largo(lista)==1);
	print_test("Ver primero es el elemento 1",lista_ver_primero(lista)==p);
	print_test("Ver ultimo es el elemento 1",lista_ver_ultimo(lista)==p);
	print_test("Agregamos un elemento 2 al princiṕio de la lista",lista_insertar_primero(lista,q));
	print_test("La lista no esta vacia",!lista_esta_vacia(lista));
	print_test("El largo de la lista es 2",lista_largo(lista)==2);
	print_test("Ver primero es el elemento 2",lista_ver_primero(lista)==q);
	print_test("Ver ultimo es el elemento 1",lista_ver_ultimo(lista)==p);

	print_test("Agregamos un elemento 3 al final de la lista",lista_insertar_ultimo(lista,cadena));
	print_test("La lista no esta vacia",!lista_esta_vacia(lista));
	print_test("El largo de la lista es 3",lista_largo(lista)==3);
	print_test("Ver primero es el elemento 2",lista_ver_primero(lista)==q);
	print_test("Ver ultimo es el elemento 3",lista_ver_ultimo(lista)==cadena);

	print_test("Agregamos un elemento 4 al final de la lista",lista_insertar_ultimo(lista,valores));
	print_test("La lista no esta vacia",!lista_esta_vacia(lista));
	print_test("El largo de la lista es 4",lista_largo(lista)==4);
	print_test("Ver primero es el elemento 2",lista_ver_primero(lista)==q);
	print_test("Ver ultimo es el elemento 4",lista_ver_ultimo(lista)==valores);

	lista_destruir(lista, NULL);
	print_test("La lista fue destruida", true);

}

void pruebas_volumen_lista(void){
	lista_t* lista= lista_crear();
	int arreglo[VOLUMEN];
	bool OK=true;

//Aumentamos en cantidad el volumen de la lista
	for(int i=0; i<VOLUMEN; i++){
		arreglo[i]=i;
		OK&=lista_insertar_ultimo(lista,&arreglo[i]);
	}	

	print_test("Hace crecer la lista",OK);

	print_test("Agregar NULL a la lista es valido", lista_insertar_primero(lista,NULL));
	print_test("Borramos NULL",lista_borrar_primero(lista)==NULL);

	print_test("La lista no esta vacia",!lista_esta_vacia(lista));
	print_test("El largo de la lista es 300",lista_largo(lista)==VOLUMEN);
	print_test("Ver primero es el primer elemento de la lista", lista_ver_primero(lista)==&arreglo[0]);

//Vaciamos la lista

	for(int i=0; i<VOLUMEN; i++){
		lista_borrar_primero(lista);
	}

	print_test("La lista esta vacia",lista_esta_vacia(lista));

//La lista vaciada se comporta como recien creada

	print_test("Borrar primero es NULL",lista_borrar_primero(lista)==NULL);
	print_test("Ver primero es NULL",lista_ver_primero(lista)==NULL);
	print_test("Ver ultimo es NULL", lista_ver_ultimo(lista)==NULL);
	print_test("Largo de lista es 0", !lista_largo(lista));
	lista_destruir(lista, NULL);
	print_test("La lista fue destruida", true);
}

void destruir_pila(void* elemento){
	pila_t* _elemento= elemento;
	pila_destruir(_elemento);
}

void pruebas_destruir(void){
//Destruimos una lista de pilas.
	lista_t* lista= lista_crear();
	pila_t* pila_aux= pila_crear();
	print_test("Insertamos una pila",lista_insertar_primero(lista,pila_aux));	

	lista_destruir(lista,destruir_pila);
	print_test("La lista fue destruida", true);
} 

//Funcion auxiliar de visitar

bool sumar_elemento(void* a, void* b){
	int* int_a = a;
	int* int_b = b;
	*int_b+=*int_a;
	return true;
}

void pruebas_lista_iterar(void){
//Pruebas con el iterador externo.
	bool OK=true;	
	int arreglo[VOLUMEN];
	lista_t* lista= lista_crear();

	for(int i=0; i<VOLUMEN; i++){
		arreglo[i]=1;
		OK&=lista_insertar_ultimo(lista,&arreglo[i]);
	}	

	print_test("Hace crecer la lista",OK);

	int suma=0;
	lista_iterar(lista,sumar_elemento,&suma);	
	print_test("La suma de toda la lista", suma==300);
	lista_destruir(lista,NULL);	

}

void pruebas_iterador(void){
// Prebas con un iterador.   
	int arreglo[3];
	lista_t* lista= lista_crear();
	bool OK=true;

//Aumentamos en cantidad el volumen de la lista
	for(int i=0; i<3; i++){
		arreglo[i]=i;
		OK&=lista_insertar_ultimo(lista,&arreglo[i]);
	}	

	print_test("Hace crecer la lista a 3",OK);
	print_test("El largo de la lista es 3",lista_largo(lista)==3);

	lista_iter_t* iter=lista_iter_crear(lista);

	print_test("El iterador no esta al final",!lista_iter_al_final(iter));
	print_test("Actual es igual que el primero de la lista",lista_iter_ver_actual(iter)==&arreglo[0]);
//Borramos el primer elemento
	lista_iter_borrar(iter);
	print_test("El largo es 2",lista_largo(lista)==2);
	print_test("Insertamos el primer elemento",lista_iter_insertar(iter,&arreglo[0]));
	print_test("EL largo es 3",lista_largo(lista)==3);
	lista_iter_avanzar(iter);

//Borramos en el medio.
	lista_iter_borrar(iter);
	print_test("El largo es 2",lista_largo(lista)==2);
	print_test("Insertamos en el medio",lista_iter_insertar(iter,&arreglo[1]));
	print_test("Actual es el medio de la lista",lista_iter_ver_actual(iter)==&arreglo[1]);	
	print_test("EL largo es 3",lista_largo(lista)==3);	
	lista_iter_avanzar(iter);

	print_test("Actual es el ultimo de la lista",lista_iter_ver_actual(iter)==&arreglo[2]);
//Borramos el ultimo
	lista_iter_borrar(iter);
	print_test("El largo es 2",lista_largo(lista)==2);
	print_test("Agregamos al final",lista_iter_insertar(iter,&arreglo[2]));
	print_test("EL largo es 3",lista_largo(lista)==3);	
 	lista_destruir(lista,NULL);
	lista_iter_destruir(iter);


	print_test("La lista y el iterador fueron destruidos", true);
}

void pruebas_iterador_volumen(void){
//Pruebas insertando volumen a una lista con el iterador.
	lista_t* lista= lista_crear();
	lista_iter_t* iter=lista_iter_crear(lista);
	int arreglo[VOLUMEN];
	bool OK=true;

	for(int i=0; i<VOLUMEN; i++){
		arreglo[i]=i;
		OK&=lista_iter_insertar(iter,&arreglo[i]);
	}
	print_test("Se inserto correctamente volumen",OK);		
	print_test("La lista no esta vacia",!lista_esta_vacia(lista));
	print_test("El largo de la lista es 300",lista_largo(lista)==VOLUMEN);
	print_test("El iterador esta al princiṕio",!lista_iter_al_final(iter));
	for(int i=0; i<VOLUMEN; i++){
		lista_iter_borrar(iter);
	}


	print_test("La lista esta vacia",lista_esta_vacia(lista));
	lista_iter_destruir(iter);
	lista_destruir(lista,destruir_pila);

	print_test("La lista y el iterador fueron destruidos", true);
}

void pruebas_iterador_avanzar(void){
	lista_t* lista= lista_crear();
	int aux1= 5;
	char cadena[]="hola";
	int valores[]={10,20,30};
	void* p= &aux1;

	print_test("Prueba insertar primero valor1",lista_insertar_primero(lista,p));
	print_test("Prueba insertar primero valor2",lista_insertar_primero(lista,cadena));
	print_test("Prueba insertar primero valor3",lista_insertar_primero(lista,valores));

	lista_iter_t* iter=lista_iter_crear(lista);	
	
	print_test("Prueba iter no esta al final",!lista_iter_al_final(iter));		
	print_test("Prueba iter ver actual es valor3",lista_iter_ver_actual(iter)==valores);
	print_test("Prueba iter avanzar es true",lista_iter_avanzar(iter));
	print_test("Prueba iter ver actual es valor2",lista_iter_ver_actual(iter)==cadena);
	print_test("Prueba iter avanzar es true",lista_iter_avanzar(iter));
	print_test("Prueba iter ver actual es valor1",lista_iter_ver_actual(iter)==p);
	print_test("Prueba iter avanzar es true",lista_iter_avanzar(iter));
	print_test("Prueba iter esta al final",lista_iter_al_final(iter));
	print_test("Prueba iter ver actual es NULL",lista_iter_ver_actual(iter)==NULL);
	print_test("Prueba iter avanzar es false",!lista_iter_avanzar(iter));
	lista_iter_destruir(iter);
	lista_destruir(lista,NULL);

	print_test("La lista y el iterador fueron destruidos", true);
}

void pruebas_lista_alumno(void){
	pruebas_crear_lista();
	pruebas_llenar_lista();
	pruebas_volumen_lista();
	pruebas_destruir();
	pruebas_lista_iterar();
	pruebas_iterador();
	pruebas_iterador_volumen();
	pruebas_iterador_avanzar();
}