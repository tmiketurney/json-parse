
pjson: main.cpp tokenize.cpp tokenize.hpp parser.cpp parser.hpp
	g++ -c main.cpp
	g++ -c tokenize.cpp
	g++ -c parser.cpp
	g++ -o pjson main.o tokenize.o parser.o

clean:
	rm -f pjson
	rm -f *.o

