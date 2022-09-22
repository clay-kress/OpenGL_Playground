# /***********************************************************************/
# /* Copyright (c) 2022 Clay Kress                                       */
# /*                                                                     */
# /* This file is part of VioletGE.                                      */
# /* VioletGE is free software: you can redistribute it and/or modify it */
# /* under the terms of the GNU General Public License as published by   */
# /* the Free Software Foundation, either version 3 of the License, or   */
# /* (at your option) any later version.                                 */
# /*                                                                     */
# /* VioletGE is distributed in the hope that it will be useful,         */
# /* but WITHOUT ANY WARRANTY; without even the implied warranty         */
# /* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.             */
# /*                                                                     */
# /* See the GNU General Public License for more details.                */
# /*                                                                     */
# /* You should have received a copy of the GNU General Public License   */
# /* along with VioletGE. If not, see <https://www.gnu.org/licenses/>.   */
# /*                                                                     */
# /***********************************************************************/

CC=gcc
CFLAGS=-std=c99 -O3 -Wfatal-errors -Wall -Wextra
MFLAGS=-std=c99 -O3 -Wfatal-errors

SRC_F=code
BIN_F=bin

SRC_C=$(wildcard $(SRC_F)/*.c)
DEP_C=$(wildcard $(SRC_F)/*.h)
OBJ_C=$(patsubst $(SRC_F)/%.c, $(BIN_F)/%.o, $(SRC_C))

LNK=-Llib

lGLFW=-lglfw3
lGLEW=-lglew32
lGL=-lopengl32

run: all
	@echo === Running Repository =================================================
	@$(BIN_F)/yMain.exe

build: all
	@echo === Building Repository ================================================

all: $(BIN_F)/yMain.exe

$(BIN_F)/yMain.exe: $(BIN_F)/main.o $(OBJ_C)
	$(CC) $(CFLAGS) -o $@ $^ $(LNK) $(lGLEW) $(lGLFW) $(lGL)

$(BIN_F)/main.o: main.c $(DEP_C)
	$(CC) $(MFLAGS) -c $< -o $@

$(BIN_F)/%.o: $(SRC_F)/%.c $(SRC_F)/%.h
	$(CC) $(CFLAGS) -c $< -o $@