CC=gcc
CFLAGS=-std=c99 -O3 -Wfatal-errors -Wall -Wextra
MFLAGS=-std=c99 -O3 -Wfatal-errors

SRC_F=src
OBJ_F=obj
BIN_F=bin

SRC_C=$(wildcard $(SRC_F)/*.c)
DEP_C=$(wildcard $(SRC_F)/*.h)
OBJ_C=$(patsubst $(SRC_F)/%.c, $(OBJ_F)/%.o, $(SRC_C))

LNK=-Llib

lGLFW=-lglfw3
lGLEW=-lglew32
lGL=-lopengl32

run: all
	@echo === Running Minecraft.exe ==============================================
	@$(BIN_F)/Minecraft.exe

all: $(BIN_F)/Minecraft.exe

$(BIN_F)/Minecraft.exe: $(OBJ_F)/main.o $(OBJ_C)
	$(CC) $(CFLAGS) -o $@ $^ $(LNK) $(lGLEW) $(lGLFW) $(lGL)

$(OBJ_F)/main.o: main.c $(DEP_C)
	$(CC) $(MFLAGS) -c $< -o $@

$(OBJ_F)/%.o: $(SRC_F)/%.c $(SRC_F)/%.h
	$(CC) $(CFLAGS) -c $< -o $@