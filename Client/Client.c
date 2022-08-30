#include "Client.h"

void connect_to_server(){
	if ((sockid = socket(AF_INET, SOCK_STREAM, 0)) < 0){ 
        write(1,"Socket creation error\n", 23); 
        return;
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){ 
        write(1,"Invalid address/ Address not supported\n", 40); 
        return; 
    } 
   
    if (connect(sockid, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){ 
        write(1,"Connection Failed\n", 19); 
        return; 
    } 
    int valread; 
    char buffer[1024] = {0};
    valread = read(sockid , buffer, 1024); 
    write(1, buffer, 1024);
}

void choose_group(){
	write(1, "Enter number of players\n", 25);
	char input[2];
	read(0,input, 2);
	group_num = atoi(input);
	send(sockid, input, strlen(input), 0);
	write(0, "Number of members sent\n", 24);
    group_num += 1;
}

void bind_gameport(int port){
    int broad = 1;
    if ((broad_id = socket(AF_INET, SOCK_DGRAM, 0)) < 0){ 
        write(1,"Socket creation error\n", 23); 
        return;
    } 
    if (setsockopt(broad_id, SOL_SOCKET, SO_BROADCAST, (char*) &broad, sizeof(broad)) < 0){
        write(1, "set socket",11 );
    }
    if (setsockopt(broad_id, SOL_SOCKET, SO_REUSEPORT, (char*) &broad, sizeof(broad)) < 0){
        write(1, "set socket",11 );
    }

    broad_addr.sin_family = AF_INET; 
    broad_addr.sin_port = htons(port);
    broad_addr.sin_addr.s_addr = inet_addr(ADDRESS);
    if(bind(broad_id, (struct sockaddr *) &broad_addr, sizeof(broad_addr)) < 0){
        write(1, "fail", 5);
    }


}
void get_id(){
    char id[1];
    read(sockid, id, 1);
    turn_id = atoi(id);
    write(1, "Your id is ",12);
    write(1, id, 1);
    write(1, "\n", 2);

}

void play_game(){
    int len = sizeof(broad_addr);
    int size = group_num * 2 - 1;
    int *scores;
    scores = (int *)malloc((group_num - 1) * sizeof(int));
    for(int i=0; i<group_num - 1; i ++)
        scores[i] = 0;
    char *map[size];
    for (int i=0; i<size; i++) 
         map[i] = (char *)malloc(size * sizeof(char)); 
    for (int i =0; i< size; i++){
        for (int j = 0; j< size; j++){
            map[i][j] = ' ';
        }
    }
    int* hline[group_num];
    
    for (int i = 0; i< group_num; i++)
        hline[i] = (int *) malloc((group_num - 1) * sizeof(int));
    for (int i = 0; i< group_num; i++)
        for (int j = 0; j< group_num - 1; j++)
            hline[i][j] = 0;
    int *vline[group_num - 1];
    for (int i = 0; i< group_num - 1; i++)
        vline[i] = (int *) malloc((group_num) * sizeof(int));
    for (int i = 0; i< group_num - 1; i++)
        for (int j = 0; j< group_num ; j++)
            vline[i][j] = 0;
    int again = 1;
    newmap(0, 0, hline, vline,map, scores);
    while(again){
        limit = '1';
        char temp[1];
        temp[0] = (turn % (group_num - 1)) + '0';
        write(1, "Turn : ", 8);
        write(1, temp, 1);
        write(1, "\n", 2);
        if ((turn % (group_num - 1)) == turn_id){
            char buffer[7];
            write(1, "choose\n", 8);
            siginterrupt(SIGALRM, 1);
            signal(SIGALRM, handler);
            alarm(20);
            read(0, buffer, 7);
            alarm(0);
            int type = buffer[0] - '0';
            int y = buffer[2] - '0';
            int x = buffer[4] - '0';
            buffer[6] = limit;
            correct_input(type, x, y, hline, vline);
            sendto(broad_id, buffer, 7, 0, (const struct sockaddr *) &broad_addr, len); 
        
        }
        
        char buffer[7];
        recvfrom(broad_id, buffer, 7, 0, NULL, 0);
        int type = buffer[0] - '0';
        int y = buffer[2] - '0';
        int x = buffer[4] - '0';
        if (buffer[6] == '0'){
            write(1, "player ", 8);
            write(1, temp, 1);
            write(1, " didn't choose\n", 16);
            turn ++;
            continue;
        }
        write(1, "input received from player ", 28);
        write(1, temp, 1);
        write(1, "\n", 2);
        int correct = add_line(type, x, y, hline, vline);
        int turn_change = 0;
        if (correct)
            turn_change = home(type, x, y, hline, vline, scores);
        
        newmap(x, y, hline, vline,map, scores);
        if(turn_change == 0)
            turn ++;
        else{
            write(1,"one score for player ", 22);
            write(1, temp, 1);
            write(1, "\n", 2);
        }


        again = end();
    }
        
    
    winner(scores);
    
    freeing(scores, hline, vline, map);
    

}


void freeing(int *scores, int **hline, int ** vline, char ** map){
    for (int i = 0; i< group_num; i++)
        free(hline[i]);

    for (int i = 0; i< group_num - 1; i++)
        free(vline[i]);
    
    for (int i = 0; i< group_num * 2 - 1; i++)
        free(map[i]);
   
    free(scores);

}
void handler(){
    limit = '0';
    write(1, "You lost your turn\n", 20);
}

int end(){
    int total = group_num * (group_num - 1) * 2;
    if(lines == total)
        return 0;
    else
        return 1;
}

void winner(int *scores){
    int count = 1;
    int max = scores[0];
    int winner = 0;
    for(int i = 1; i< group_num - 1; i ++){
        if(scores[i] > max){
            max = scores[i];
            count = 1;
            winner = i;
        }
        else if(scores[i] == max){
            count ++;
        }

    }
    if (count == 1){
        char temp[1];
        temp[0] = winner + '0';
        write(1,"winner is player ", 18);
        write(1, temp, 1);
        write(1, "\n", 2);
    }
    else{
        write(1, "winners are: \n", 15);
        for(int i = 0; i< group_num - 1; i ++){
            if(scores[i] == max){
                char temp[1];
                temp[0] = i + '0';
                write(1, temp, 1);
                write(1, "\n", 2);
            }
        }

        
    }
}

int add_line(int type, int y, int x, int** hline, int** vline) {
    int check = 0;  
    if (type == 0) {
        if (x <= group_num && y <= group_num - 1 && (hline[x - 1][y - 1] != 1)) {
            hline[x - 1][y - 1] = 1;
            lines ++;
            check = 1;
        }
    }
    else if (type == 1) {
        if (y <= group_num && x <= group_num - 1 && (vline[x - 1][y - 1] != 1)) {
            vline[x - 1][y - 1] = 1;
            check = 1;
            lines ++;
        }
    
    }
    
    return check;
}


void correct_input(int type, int y, int x, int** hline, int** vline) {
    int check = 0;  
    if (type == 0) {
        if (x <= group_num && y <= group_num - 1 && (hline[x - 1][y - 1] != 1)) {
            check = 1;
            
        }
    }
        
    else if (type == 1) {
        if (y <= group_num && x <= group_num - 1 && (vline[x - 1][y - 1] != 1)){
            check = 1;
           
        }
    }
           
    if (check == 0)
        write(1, "wrong input\n", 13);
        
}


int home(int type, int y, int x, int** hline, int **vline, int*scores) {
    int homecheck = 0;
    int index = turn % (group_num - 1);
    if(type == 0){
        if(x != 1 && hline[x - 2][y - 1] && hline[x - 2][y] && vline[x - 2][y - 1]){
            scores[index] ++;
            homecheck = 1;
        }
        if(x != group_num && hline[x - 1][y - 1] && hline[x - 1][y] && vline[x][y - 1]){
            scores[index] ++;
            homecheck = 1;
        }
    }
    else{
        if(y != 1 && vline[x - 1][y - 2] && vline[x][y - 2] && hline[x - 1][y - 2]){
            scores[index] ++;
            homecheck = 1;
        }
        if( y != group_num  && vline[x - 1][y - 1] && vline[x][y - 2] && hline[x-1 ][y]){
           scores[index] ++;
            homecheck = 1; 
        }
            
           
    }

    return homecheck;
        
        
}


void newmap(int x, int y, int** hline, int** vline, char** map, int* scores) {
    int i, j, homecheck;
    for (i = 0; i < group_num; i ++)
        for(j = 0; j < group_num -1; j ++)
            if (hline[i][j] == 1)
                map[2 * i][2* j + 1] = '-';
    for (i = 0; i < group_num - 1; i ++)
        for(j = 0; j < group_num; j ++)
            if (vline[i][j] == 1)
                map[2 * i + 1][2* j] = '|';
    int size = 2 * group_num - 1;
    for(i = 0; i< size; i ++){
        if (i % 2 == 0){
            for(j=0; j< size; j ++){
                if(j % 2 == 0)
                    write(1, "*", 2);
                else{
                    char temp[1];
                    temp[0] = map[i][j];
                    write(1, temp, 1);
                }

            }
        }
        else{
            for(j=0; j< size; j ++){
                if(j %2 == 0){
                    char temp[1];
                    temp[0] = map[i][j];
                    write(1, temp, 1);
                }
                else
                    write(1," ", 2);
            }
        }
        write(1, "\n", 2);
    }
    
}