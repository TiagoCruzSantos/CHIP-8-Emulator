OBJ      := obj
SRC      := src
INC      := include
SDLFLAG  := -lSDL2
FLAGS    := -g -std=c++17
EXE 	 := main.out

main: $(OBJ)/CPU.o $(OBJ)/main.o
	g++ $(OBJ)/*.o -o $(EXE) $(FLAGS) $(SDLFLAG)

$(OBJ)/main.o: $(SRC)/main.cpp
	g++ -c "$(SRC)/main.cpp" -o "$(OBJ)/main.o" $(FLAGS)

$(OBJ)/CPU.o: $(SRC)/CPU.cpp $(INC)/CPU.hpp
	g++ -c "$(SRC)/CPU.cpp" -o "$(OBJ)/CPU.o" $(FLAGS)
clear:
	rm $(OBJ)/*.o
	rm $(EXE)