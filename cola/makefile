CC = gcc
CFLAGS = -std=c99 -Wall -Wconversion -pedantic 	-Wtype-limits -Wno-sign-conversion
EJECUTABLE = cola
ARCHIVOS_H = cola.h pila.h testing.h
ARCHIVOS_C = cola.c pila.c testing.c pruebas_alumno.c main.c


run: build
	./$(EJECUTABLE)

build: $(ARCHIVOS_C) $(ARCHIVOS_H)
	$(CC) $(CFLAGS) $(ARCHIVOS_C) -o $(EJECUTABLE)

valgrind: all
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./tp1

