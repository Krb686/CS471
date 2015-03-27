run:		./bin/main.o ./bin/parser.o ./bin/process.o
		gcc -g -o run ./bin/main.o ./bin/parser.o ./bin/process.o
./bin/main.o:	./src/main.c ./src/main.h ./src/parser.h ./src/process.h
		gcc -g -c -o ./bin/main.o ./src/main.c
./bin/parser.o:	./src/parser.c ./src/parser.h ./src/process.h
		gcc -c -o ./bin/parser.o ./src/parser.c
./bin/process.o:./src/process.c ./src/process.h ./src/parser.h
		gcc -c -o ./bin/process.o ./src/process.c
