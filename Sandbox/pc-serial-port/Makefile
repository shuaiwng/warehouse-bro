CC = g++
EXE = serial.exe
OBJ = main.o
SRC = main.cpp


all: $(EXE)
$(EXE): $(OBJ)
	$(CC) $(OBJ) -std=c++11 -o $(EXE)
$(OBJ): $(SRC)
	$(CC) -Wall -c $(SRC)
	
clean:
	del $(OBJ) $(EXE)