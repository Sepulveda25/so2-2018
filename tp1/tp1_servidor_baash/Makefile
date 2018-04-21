# La siguiente no es necesariamente requerida, se agrega para
# mostrar como funciona.

.SUFFIXES: .o .c
.c.o:
	$(CC) -c $(CFLAGS) $<

CC = gcc
SRC = tp1_servidor_baash.c parsear_entrada.c parsear_entrada.h buscar_en_PATH.c buscar_en_PATH.h cantidad_de_retrocesos.c cantidad_de_retrocesos.h armar_path_relativo.c armar_path_relativo.h pipe_argumentos.c pipe_argumentos.h baash.c baash.h
OBJ = tp1_servidor_baash.o parsear_entrada.o buscar_en_PATH.o cantidad_de_retrocesos.o armar_path_relativo.o pipe_argumentos.o baash.o
BIN = tp1_servidor_baash
CFLAGS = -Wall -pedantic
# Reglas explicitas

all:
	$(CC) $(CFLAGS) $(SRC) -o $(BIN) 

clean:
	$(RM) $(OBJ) $(BIN) 

# Reglas implicitas
parsear_entrada.o: parsear_entrada.c parsear_entrada.h
buscar_en_PATH.o: buscar_en_PATH.c buscar_en_PATH.h
cantidad_de_retrocesos.o: cantidad_de_retrocesos.c cantidad_de_retrocesos.h
armar_path_relativo.o: armar_path_relativo.c armar_path_relativo.h
pipe_argumentos.o: pipe_argumentos.c pipe_argumentos.h
baash.o: baash.c baash.h
 