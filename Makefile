
pjson: main.cpp main.hpp tokenize.cpp tokenize.hpp parser.cpp parser.hpp
	g++ -c -std=c++11 main.cpp
	g++ -c -std=c++11 tokenize.cpp
	g++ -c -std=c++11 parser.cpp
	g++ -o pjson main.o tokenize.o parser.o

clean:
	rm -f pjson
	rm -f *.o

