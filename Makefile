scheduler: main.o parser.o
	gcc -o scheduler main.o parser.o
	
main.o: main.c parser.h
	gcc -c main.c
	
parser.o: parser.c parser.h
	gcc -c parser.c