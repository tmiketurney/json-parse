
pjson: main.cpp token.cpp token.hpp parser.cpp parser.hpp
	g++ -c main.cpp
	g++ -c token.cpp
	g++ -c parser.cpp
	g++ -o pjson main.o token.o parser.o

clean:
	rm -f pjson
	rm -f *.o

