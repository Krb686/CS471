SRC = ./src/
BIN = ./bin/

scheduler: $(BIN)main.o $(BIN)parser.o
	gcc -o scheduler $(BIN)main.o $(BIN)parser.o

$(BIN)main.o: $(SRC)main.c $(SRC)parser.h
	gcc -c -o $(BIN)main.o $(SRC)main.c
	
$(BIN)parser.o: $(SRC)parser.c $(SRC)parser.h
	gcc -c -o $(BIN)parser.o $(SRC)parser.c
