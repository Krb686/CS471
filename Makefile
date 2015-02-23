SRC = ./src/
BIN = ./bin/

scheduler: $(BIN)main.o $(BIN)parser.o $(BIN)process.o $(BIN)queue.o
	gcc -o scheduler $(BIN)main.o $(BIN)parser.o $(BIN)process.o $(BIN)queue.o

$(BIN)main.o: $(SRC)main.c $(SRC)parser.h
	gcc -c -o $(BIN)main.o $(SRC)main.c
	
$(BIN)parser.o: $(SRC)parser.c $(SRC)parser.h
	gcc -c -o $(BIN)parser.o $(SRC)parser.c

$(BIN)process.o: $(SRC)process.c $(SRC)process.h
	gcc -c -o $(BIN)process.o $(SRC)process.c

$(BIN)queue.o: $(SRC)queue.c $(SRC)queue.h
	gcc -c -o $(BIN)queue.o $(SRC)queue.c
