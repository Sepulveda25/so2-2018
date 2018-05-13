# La siguiente no es necesariamente requerida, se agrega para
# mostrar como funciona.

.SUFFIXES: .o .c
.c.o:
	$(CC) -c $(CFLAGS) $<

CC = gcc
SRC = tp2_paralelo.c 
OBJ = tp2_paralelo.o 
BIN = tp2_paralelo
CFLAGS = -Wall -pedantic
GPROF= -pg 
# Reglas explicitas

all:
	$(CC) $(GPROF) $(CFLAGS) $(SRC) -o $(BIN) -fopenmp

clean:
	$(RM) $(OBJ) $(BIN) 

# Reglas implicitas

 