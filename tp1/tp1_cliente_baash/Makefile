# La siguiente no es necesariamente requerida, se agrega para
# mostrar como funciona.

.SUFFIXES: .o .c
.c.o:
	$(CC) -c $(CFLAGS) $<

CC = gcc
SRC = tp1_cliente_baash.c 
OBJ = tp1_cliente_baash.o 
BIN = tp1_cliente_baash
CFLAGS = -Wall -pedantic
# Reglas explicitas

all:
	$(CC) $(CFLAGS) $(SRC) -o $(BIN) 

clean:
	$(RM) $(OBJ) $(BIN) 

# Reglas implicitas

 