CXX=g++
INC_DIR = ./include
CXXFLAGS=-std=c++11 -I$(INC_DIR) 
#CXXFLAGS += `pkg-config --cflags --libs tesseract opencv`
#CXXFLAGS += `pkg-config --cflags opencv` `pkg-config --libs opencv`
#CXXFLAGS += `pkg-config --cflags opencv` `pkg-config --libs opencv` `pkg-config --cflags tesseract` `pkg-config --libs tesseract` 
LIBS = `pkg-config --libs opencv`
DEPS = GlobalFunctions.h Sudoku.h 
OBJ = main.o Sudoku.o GlobalFunctions.o

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS) 

Sudoku_Solver_CV: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)
	rm -f *.o *~ core

.PHONY: clean

clean:
	rm -f *.o *~ core

