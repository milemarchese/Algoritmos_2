CC = gcc
CFLAGS = -std=c99 -Wall -Wconversion -Wtype-limits -pedantic

all: testing.c testing.h tp0.c tp0.h tp0_pruebas.c
	$(CC) $(CFLAGS) testing.c tp0.c tp0_pruebas.c -o pruebas

run: all
	./pruebas

clean:
	rm -f *.0 pruebas

#Tambien se pueden comandos de linux!
ejemplo_echo:
	echo "Texto super interesante"

ejemplo_ls:
	ls

ejemplo_date:
	date

