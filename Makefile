OBJ      := obj
SRC      := src
INC      := include
FLAGS := -g -std=c++17
EXE 	 := main.out

main: $(OBJ)/CPU.o $(OBJ)/main.o
	g++ $(OBJ)/*.o -o $(EXE) $(FLAGS)

$(OBJ)/main.o: $(SRC)/main.cpp
	g++ -c $(FLAGS) "$(SRC)/main.cpp" -o "$(OBJ)/main.o"

$(OBJ)/CPU.o: $(SRC)/CPU.cpp $(INC)/CPU.hpp
	g++ -c $(FLAGS) "$(SRC)/CPU.cpp" -o "$(OBJ)/CPU.o"
clear:
	rm $(OBJ)/*.o
	rm $(EXE)