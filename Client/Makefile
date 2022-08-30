all : Client

Client: Main.o Client.o 
	gcc Main.o Client.o -o client

Main.o: Main.c Client.h 
	gcc -c Main.c

Client.o: Client.c Client.h
	gcc -c Client.c



clean:
	rm *.o Client


