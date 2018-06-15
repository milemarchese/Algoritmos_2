#include "pila.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#define VOLUMEN 300

void pruebas_crear_destruir(){
/*Pruebas de crear y destruir una pila*/    
	pila_t* pila1= pila_crear();
	print_test("Crea una pila", pila1 != NULL);
	pila_destruir(pila1);
	print_test("La pila fue destruida", true);
}



void pruebas_invariante(){
/*Pruebas que se comprueba la invariante de la pila*/
	pila_t* pila2= pila_crear();
	int aux1= 5;
	float aux2= 3.14159F;
	char aux3= 'm';
	char cadena[]="hola";
	int valores[]={10,20,30};
	void* p= &aux1;
	void* q= &aux2;
	void* r= &aux3;
	void* s= cadena;
	void* t= valores;
	
	print_test("Apila un elemento 1 del tipo int", pila_apilar(pila2, p));
	print_test("Apila un elemento 2 del tipo float", pila_apilar(pila2, q));
	print_test("Apila un elemento 3 del tipo char", pila_apilar(pila2, r));
	print_test("Apila un elemento 4 cadena de caracteres", pila_apilar(pila2, s));
	print_test("Apila un elemento 5 vector de int", pila_apilar(pila2, t));
	print_test("La pila no esta vacia", !pila_esta_vacia(pila2));
	print_test("Desapilo un elemento 5", pila_desapilar(pila2) == t);
	print_test("Desapilo un elemento 4", pila_desapilar(pila2) == s);
	print_test("Desapilo un elemento 3", pila_desapilar(pila2) == r);
	print_test("Desapilo un elemento 2", pila_desapilar(pila2) == q);
	print_test("Desapilo un elemento 1", pila_desapilar(pila2) == p);
	pila_destruir(pila2);
}

void pruebas_apilar_desapilar(){
/*Pruebas con una pila recien creada, apilar mas de su capacidad incial y luego desapilarla hasta que este vacia*/
	int arreglo[VOLUMEN];
	pila_t* pila3= pila_crear();
	bool OK=true;
	
	for(int i=0; i<VOLUMEN; i++){
		arreglo[i]=i;
		OK&=pila_apilar(pila3,&arreglo[i]);
	}
	print_test("Hace crecer la pila hasta que tiene un valor mayor al inicial",OK);

	print_test("Apilamos NULL",pila_apilar(pila3, NULL));

	print_test("Desapilamos NULL", pila_desapilar(pila3)==NULL);
	
	for(int j=VOLUMEN-1; j>=0; j--){
		OK&=(&arreglo[j]==pila_desapilar(pila3));
	}

	print_test("Desapila hasta que este vacia", pila_esta_vacia(pila3));

/*Pruebas con una pila desapilada hasta que esta vacia,se comporta como recien creada*/

	print_test("Esta vacia en un pila que se le apilo y desapilo hasta estar vacia es valido", pila_esta_vacia(pila3));
	print_test("Desapilar en una pila que se le apilo y desapilo hasta estar vacio es invalido",pila_desapilar(pila3)==NULL);
	print_test("Ver tope en una pila que se le apilo y desapilo hasta estar vacio es invalido",pila_desapilar(pila3)==NULL);
	pila_destruir(pila3);
}

void pruebas_recien_creada(){
/*Una pila recien creada*/

	pila_t* pila4=pila_crear();
	print_test("Desapilar en una pila recien creada es invalido", pila_desapilar(pila4) == NULL);
	print_test("Ver tope en una pila recien creada es invalido", pila_ver_tope(pila4) == NULL);
	print_test("Una pila recien creada esta vacia", pila_esta_vacia(pila4));
	pila_destruir(pila4);

}

void pruebas_pila_alumno() {
	pruebas_crear_destruir();
	pruebas_invariante();	
	pruebas_apilar_desapilar();
	pruebas_recien_creada();

}


