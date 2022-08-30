#ifndef SERVER
#define SERVER  
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#define TRUE 1
#define FALSE 0
#define MAX_CLI 40
#define ADDRESS "127.0.0.1"
int PORT;
int sockid;
struct sockaddr_in address;
int client_sockets[MAX_CLI];
fd_set readfds;
int group2[2];
int group3[3];
int group4[4];
int index2, index3, index4;
int new_port;

void add_client();
void set_group(int, int);
void get_inf();
void create_game_port(int);
void send_port(int, char*);
void int_to_char(char *, int);
void send_id(int);

#endif
