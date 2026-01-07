EXE=final
CC=gcc
CFLAGS=-std=c99 -O3 -Wfatal-errors -Wall -Wextra
MAINFLAGS=-std=c99 -O3 -Wfatal-errors

#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-std=c99 -O3 -Wfatal-errors -Wall -DUSEGLEW -L.
LIBS=-lglew32 -lglfw3 -lopengl32
CLEAN=del *.exe bin\*.o  bin\data\*.o
else
#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations -DRES=2
LIBS=-framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS= -lGL -lm -lglfw
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f final bin/*.o bin/data/*.o
endif

SRC_F=src
BIN_F=bin
SRC_DATA_F=src/data
BIN_DATA_F=bin/data

SRC_C=$(wildcard $(SRC_F)/*.c)
SRC_DATA_C=$(wildcard $(SRC_DATA_F)/*.c)

DEP_C=$(wildcard $(SRC_F)/*.h)

OBJ_C=$(patsubst $(SRC_F)/%.c, $(BIN_F)/%.o, $(SRC_C))$(patsubst $(SRC_DATA_F)/%.c, $(BIN_DATA_F)/%.o, $(SRC_DATA_C))

executable: $(EXE)

run: $(EXE)
	$(EXE).exe

$(EXE): $(BIN_F)/main.o $(OBJ_C)
	$(CC) $(CFLG) -o $@ $^ $(LIBS)

$(BIN_F)/main.o: main.c $(DEP_C)
	$(CC) $(CFLG) -c $< -o $@

$(BIN_F)/%.o: $(SRC_F)/%.c $(SRC_F)/%.h
	$(CC) $(CFLG) -c $< -o $@

clean:
	$(CLEAN)
