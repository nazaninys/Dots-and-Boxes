all : Server

Server: Main.o Server.o
	gcc Main.o Server.o -o server

Main.o: Main.c Server.h
	gcc -c Main.c

Server.o: Server.c Server.h
	gcc -c Server.c

clean:
	rm *.o Server


