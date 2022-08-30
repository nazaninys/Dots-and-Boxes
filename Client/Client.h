#ifndef CLIENT
#define CLIENT
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
#define ADDRESS "255.255.255.255"
int sockid, broad_id;
int turn_id;
int turn;
int group_num;
int lines;
char limit;
int PORT;
struct sockaddr_in serv_addr, broad_addr; 
void connect_to_server();
void choose_group();
void bind_gameport(int);
void get_id();
void play_game();
int add_line(int, int , int , int **, int **);
void correct_input(int, int , int , int **, int **);
void newmap(int , int , int** , int** , char**, int*);
int home(int, int , int , int** , int **, int*);
void add_to_map(int, int , int , int **, int **);
int end();
void winner(int *);
void handler();
void freeing(int *, int**, int**, char**);

#endif