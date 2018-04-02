CPP = g++
FLAGS = -std=c++11 -O2 -g
DEPS = Matriz.h IOUtils.h PageRank.h
OBJ = tp1.o Matriz.o IOUtils.o PageRank.o

all: tp1

tp1: $(OBJ)
	$(CPP) $(FLAGS) -o $@ $^

%.o: %.cpp $(DEPS)
	$(CPP) $(FLAGS) -c -o $@ $<

#IOUtils.o: IOUtils.cpp IOUtils.h
#	$(CPP) $FLAGS -c -o $^	

clean:
	rm -f *.o
	rm -f tp1
