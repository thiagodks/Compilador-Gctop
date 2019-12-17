PROJ_NAME=main
PROJ_NAME_O=main.o
EXEC=gctop

C_SOURCE=$(wildcard *.c)
OBJ=$(subst .c,.o,$(subst source,objects,$(C_SOURCE)))

all:	$(PROJ_NAME)	objFolder

$(PROJ_NAME):	$(OBJ)
	gcc	$(OBJ)	-o	$(EXEC)

$(PROJ_NAME_O):	$(C_SOURCE)
	gcc	-c	-Wall	$(C_SOURCE)

objFolder:
	@ mkdir -p ../objects
	@ mv $(OBJ) ../objects

clean:
	@ rm ../objects/*.o
	@ rm $(EXEC)
	@ rmdir ../objects
