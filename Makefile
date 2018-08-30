GCC=gcc
all: Lab3
proj1: Lab3.o
	${GCC} Lab3.o -o Lab3
proj1.o: Lab3.c 
	${GCC} Lab3.c -c

clean:
	rm -f *.o *.exe
	