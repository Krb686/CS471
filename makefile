all:		server seller buyer

server:		./bin/server.o
		gcc -g -o server ./bin/server.o
./bin/server.o:	./src/server.c ./src/server.h
		gcc -g -c -o ./bin/server.o ./src/server.c


seller:		./bin/seller.o
		gcc -g -o seller ./bin/seller.o
./bin/seller.o:	./src/seller.c ./src/seller.h
		gcc -c -o ./bin/seller.o ./src/seller.c


buyer:		./bin/buyer.o
		gcc -g -o buyer ./bin/buyer.o
./bin/buyer.o:  ./src/buyer.c ./src/buyer.h
		gcc -c -o ./bin/buyer.o ./src/buyer.c
