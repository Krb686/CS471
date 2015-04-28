all:		server seller buyer clean

server:		server.o
		gcc -g server.o -lpthread -o server

server.o:	./src/server.c ./src/server.h
		gcc -c ./src/server.c -lpthread 


seller:		seller.o
		gcc -g seller.o -o seller
seller.o:	./src/seller.c ./src/seller.h
		gcc -c ./src/seller.c 


buyer:		buyer.o
		gcc -g buyer.o -o buyer
buyer.o:  	./src/buyer.c ./src/buyer.h
		gcc -c ./src/buyer.c 

clean:
		-rm -f *.o
