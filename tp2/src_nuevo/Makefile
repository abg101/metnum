CPP = g++
FLAGS = -std=c++11 -O2 -g
DEPS = matriz_aux.hpp normas_aux.hpp metodos_reduc.hpp clasificador.hpp ppmloader/ppmloader.h IOUtils.hpp preprocesamiento.hpp
OBJ = tp2.o ppmloader/ppmloader.o

all: tp2

tp2: $(OBJ)
	$(CPP) $(FLAGS) -o $@ $^

%.o: %.cpp $(DEPS)
	$(CPP) $(FLAGS) -c -o $@ $<

clean:
	rm -f *.o
	rm -f tp2
