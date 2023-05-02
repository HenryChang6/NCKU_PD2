all: main.o myfunctions.o
	g++ main.o myfunctions.o -o hw1

main.o: main.cpp
	g++ -c main.cpp

myfunctions.o: myfunctions.cpp myfunctions.h
	g++ -c myfunctions.cpp

clean:
	rm *.o hw1 

