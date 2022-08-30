#include "Client.h"




int main(int argc, char *argv[]){ 
	turn = 0;
	lines = 0;
	limit = '1';
	PORT = atoi(argv[1]);
	connect_to_server();
	choose_group();
	char port[4];
	read(sockid, port, 4);
	get_id();
	write(1, "port is ", 8);
	write(1, port, 4);
	write(1, "\n", 2);
	bind_gameport(atoi(port));
	play_game();
	close(sockid);
    close(broad_id);


    return 0; 
} 