#include "tp0.h"

/* swap() intercambia dos valores enteros.
 */
void swap (int* x, int* y) {
	int aux = *x;
	*x = *y;
	*y = aux;
}

/* maximo() busca el mayor elemento del arreglo y devuelve su posicion.
 * Si el vector es de largo 0, devuelve -1.
 */
int maximo(int vector[], int n) {
	if(!n){
		return -1;
	}

	int maximo = vector[0];
	int posicion = 0;

	for (int i = 0; i < n; ++i){
		if (vector[i] > maximo){
			maximo = vector[i];
			posicion = i;
		}
	}

	return posicion;
}

/* La función comparar recibe dos vectores y sus respectivas longitudes
 * y devuelve -1 si el primer vector es menor que el segundo; 0 si son
 * iguales; y 1 si el segundo es menor.
 *
 * Un vector es menor a otro cuando al compararlos elemento a elemento,
 * el primer elemento en el que difieren no existe o es menor.
 */
int comparar(int vector1[], int n1, int vector2[], int n2) {
	for (int i = 0; i < n1 && i < n2 ; ++i){
		if (vector1[i]<vector2[i]){
			return -1;
		}
		if (vector2[i]<vector1[i]){
			return 1;	
		}
	}

	if (n1<n2){
		return -1;
	}
	if (n2<n1){
		return 1;
	}

	return 0;
}

/* selection_sort() ordena el arreglo recibido mediante el algoritmo de
 * selección.
 */

void seleccion(int vector[], int n) {
	int largo = n-1;
	int posicion;
	while(largo > 0){
		posicion = maximo (vector, n);
		swap(&vector[posicion], &vector[largo]);
		largo -= 1;
		n -= 1;
	}
}









