all: main.o employee.o
	g++ main.o employee.o -o hw4

main.o: main.cpp
	g++ -c main.cpp

myfunctions.o: employee.cpp employee.h
	g++ -c employee.cpp

clean:
	rm *.o hw4 