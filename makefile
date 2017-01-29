
FLAGS = -g  -O3 -Wl,--no-as-needed -ldl -Wall -pedantic -Wextra -std=c++11

all: clean test

test: test.o
	g++ $(FLAGS)  test.o -o test

test.o: test.cpp Deque.hpp
	g++ $(FLAGS) -c test.cpp -o test.o

clean:
	rm -f *.o test
