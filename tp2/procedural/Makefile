# La siguiente no es necesariamente requerida, se agrega para
# mostrar como funciona.

.SUFFIXES: .o .c
.c.o:
	$(CC) -c $(CFLAGS) $<

CC = gcc
SRC = tp2_procedural.c 
OBJ = tp2_procedural.o 
BIN = tp2_procedural
CFLAGS = -Wall -pedantic 
LDFLAGS =-lm
# Reglas explicitas

all:
	$(CC) $(CFLAGS) $(SRC) -o $(BIN) $(LDFLAGS) 

clean:
	$(RM) $(OBJ) $(BIN) 

# Reglas implicitas

 