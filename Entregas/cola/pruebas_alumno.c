#include "pila.h"
#include "cola.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#define VOLUMEN 300
/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación del alumno.
//
// Las pruebas deben emplazarse en el archivo ‘pruebas_alumno.c’, y
// solamente pueden emplear la interfaz pública tal y como aparece en cola.h
// (esto es, las pruebas no pueden acceder a los miembros del struct cola).
//
// Para la implementación de las pruebas se debe emplear la función
// print_test(), como se ha visto en TPs anteriores.

void pruebas_crear_destruir(){
/*Pruebas de crear y destruir una cola*/
	cola_t* cola1= cola_crear();
	print_test("Crea una cola", cola1 != NULL);
	print_test("El primero es NULL", cola_ver_primero(cola1)==NULL);
	cola_destruir(cola1, NULL);
	print_test("La cola fue destruida", true);

}

void pruebas_invariante(){
/*Pruebas que verifican la invariante de la cola*/
	cola_t* cola2= cola_crear(); 
    int aux1= 5;
	float aux2= 3.14159F;
	char aux3= 'm';
	char cadena[]="hola";
	int valores[]={10,20,30};
	void* p= &aux1;
	void* q= &aux2;
	void* r= &aux3;

	print_test("Encolamos un elemento 1", cola_encolar(cola2,p));
	print_test("Encolamos un elemento 2", cola_encolar(cola2,q));
	print_test("Encolamos un elemento 3", cola_encolar(cola2,r));
	print_test("Encolamos un elemento 4", cola_encolar(cola2,cadena));
	print_test("Encolamos un elemento 5", cola_encolar(cola2,valores));
	print_test("El primero es el elemento 1", cola_ver_primero(cola2)==p);
	print_test("Desencolamos un elemento 1", cola_desencolar(cola2)==p);
	print_test("El primero es el elemento 2", cola_ver_primero(cola2)==q);
	print_test("Desencolamos un elemento 2", cola_desencolar(cola2)==q);
	print_test("El primero es el elemento 3", cola_ver_primero(cola2)==r);
	print_test("Desencolamos un elemento 3", cola_desencolar(cola2)==r);
	print_test("El primero es el elemento 4", cola_ver_primero(cola2)==cadena);
	print_test("Desencolamos un elemento 4", cola_desencolar(cola2)==cadena);
	print_test("El primero es el elemento 5", cola_ver_primero(cola2)==valores);
	print_test("Desencolamos un elemento 5", cola_desencolar(cola2)==valores);
	print_test("El primer elemento es NULL", cola_ver_primero(cola2)==NULL);


	print_test("Desencolamos hasta que este vacia", cola_esta_vacia(cola2));
	cola_destruir(cola2, NULL);
	print_test("La cola la fue destruida", true);	
}

void pruebas_encolar_desencolar(){
/*Encolamos volumen de elementos*/
	cola_t* cola3= cola_crear();
	int arreglo[VOLUMEN];
	bool OK=true;
	
	for(int i=0; i<VOLUMEN; i++){
		arreglo[i]=i;
		OK&=cola_encolar(cola3,&arreglo[i]);
	}
	print_test("Hace crecer la cola ",OK);

	print_test("Encolamos NULL",cola_encolar(cola3, NULL));
	
	print_test("El primero es el primer elemento del arreglo", cola_ver_primero(cola3)==&arreglo[0]);	

	for(int j=0; j<VOLUMEN; j++){
		OK&=(&arreglo[j]==cola_desencolar(cola3));
	}

	print_test("Verficamos que se mantuvo la invariante en volumen", OK);

	print_test("Desencolamos NULL", cola_desencolar(cola3)==NULL);
	
	print_test("Desencolamos hasta que este vacia", cola_esta_vacia(cola3));

	cola_destruir(cola3, NULL);
	print_test("La cola la fue destruida", true);

}


void pruebas_recien_creada(){
/*Pruebas en una cola recien creada*/
	cola_t* cola4= cola_crear();
   	print_test("Esta vacia en un cola recien creada es valido", cola_esta_vacia(cola4));
	print_test("Desencolar en una cola recien creada es invalido",cola_desencolar(cola4)==NULL);
	print_test("Ver primero en una cola recien creada invalido",cola_ver_primero(cola4)==NULL);
	cola_destruir(cola4, NULL);
}

void destruir_pila(void* elemento){
	pila_t* _elemento= elemento;
	pila_destruir(_elemento);
}

void pruebas_destruir(){
/*Pruebas encolando un elemento que tiene una funcion destruir*/
	pila_t* pila_aux= pila_crear();
	cola_t* cola5= cola_crear();
	print_test("Encolamos una pila", cola_encolar(cola5,pila_aux));
	cola_destruir(cola5,destruir_pila);
	print_test("La cola fue destruida", true);
} 


void pruebas_cola_alumno(void){
	pruebas_crear_destruir();
	pruebas_invariante();
	pruebas_encolar_desencolar();
	pruebas_destruir();
	pruebas_recien_creada();
}

