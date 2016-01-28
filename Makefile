CXX=g++
INC_DIR = ./include
CXXFLAGS=-std=c++11 -I$(INC_DIR)
DEPS = GlobalFunctions.h Sudoku.h 
OBJ = main.o Sudoku.o GlobalFunctions.o

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

Sudoku_Solver: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)
	rm -f *.o *~ core

.PHONY: clean

clean:
	rm -f *.o *~ core

