BUILDDIR = ./src

scheduler: $(BUILDDIR)/main.o $(BUILDIR)/parser.o
	gcc -o scheduler main.o parser.o
	
main.o: $(BUILDDIR)/main.c $(BUILDDIR)/parser.h
	gcc -c main.c
	
parser.o: $(BUILDDIR)/parser.c $(BUILDDIR)/parser.h
	gcc -c parser.c