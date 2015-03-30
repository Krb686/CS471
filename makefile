run:		./bin/main.o ./bin/parser.o ./bin/process.o ./bin/mm.o
		gcc -g -o run ./bin/main.o ./bin/parser.o ./bin/process.o ./bin/mm.o
./bin/main.o:	./src/main.c ./src/main.h ./src/parser.h ./src/process.h ./src/mm.h
		gcc -g -c -o ./bin/main.o ./src/main.c
./bin/parser.o:	./src/parser.c ./src/parser.h ./src/process.h
		gcc -c -o ./bin/parser.o ./src/parser.c
./bin/process.o:./src/process.c ./src/process.h
		gcc -c -o ./bin/process.o ./src/process.c
./bin/mm.o:	./src/mm.c ./src/mm.h ./src/parser.h
		gcc -c -o ./bin/mm.o ./src/mm.c
