SRC = ./src/
BIN = ./bin/

scheduler: $(BIN)main.o $(BIN)parser.o $(BIN)process.o $(BIN)queue.o $(BIN)device.o
	gcc -g -o scheduler $(BIN)main.o $(BIN)parser.o $(BIN)process.o $(BIN)queue.o $(BIN)device.o

$(BIN)main.o: $(SRC)main.c $(SRC)main.h $(SRC)parser.h $(SRC)queue.h $(SRC)process.h
	gcc -g -c -o $(BIN)main.o $(SRC)main.c
	
$(BIN)parser.o: $(SRC)parser.c $(SRC)parser.h $(SRC)process.h
	gcc -c -o $(BIN)parser.o $(SRC)parser.c

$(BIN)process.o: $(SRC)process.c $(SRC)process.h $(SRC)parser.h
	gcc -c -o $(BIN)process.o $(SRC)process.c

$(BIN)queue.o: $(SRC)queue.c $(SRC)queue.h $(SRC)process.h
	gcc -c -o $(BIN)queue.o $(SRC)queue.c

$(BIN)device.o: $(SRC)device.c $(SRC)device.h
	gcc -c -o $(BIN)device.o $(SRC)device.c
