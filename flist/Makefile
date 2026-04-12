.PHONY: default clean

default: main

main: main.o flist.o
	g++ main.o flist.o -o main
main.o: main.cpp
	g++ -c main.cpp
flist.o: flist.cpp
	g++ -c flist.cpp
clean:
	rm *.o main
