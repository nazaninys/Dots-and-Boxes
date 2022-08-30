#include "Server.h"
void add_client(){
    int newSocket;
    int addrlen = sizeof(address);
     
    if (FD_ISSET(sockid, &readfds)){   
        if ((newSocket= accept(sockid,  
                (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)   
        {   
            write(1, "accept", 7);   
            exit(EXIT_FAILURE);   
        }   
            
        write(1, "New Connection\n", 15); 
                
        char* message = "You connected to Server\n";
        if( send(newSocket, message, strlen(message), 0) != strlen(message) ){   
            write(1, "send welcome\n", 13);   
        }       
    
        for (int i = 0; i < MAX_CLI; i++){ 
            if( client_sockets[i] == 0 ){   
                client_sockets[i] = newSocket;   
                write(1, "Adding to list of sockets\n", 27);
                break;   
            }   
        }   
    } 
}

void get_inf(){
    char buffer[1025];
    int valread;
    for (int i = 0; i < MAX_CLI; i++){   
        int cur = client_sockets[i];   
                 
        if (FD_ISSET( cur , &readfds)){   
            if ((valread = recv( cur , buffer, 1024, 0)) <= 0){
                write(1, "Host disconnected\n", 19);  
                close(cur);   
                client_sockets[i] = 0;   
            }   
     
            else{
                set_group(atoi(buffer), cur);
                 
            }   
        }   
    }   
}

void set_group(int num, int id){
    write(1, "You joined to group\n", 21);
    switch(num){
        case 2:
            group2[index2] = id;
            if (index2 != 1){
                
                
                index2 ++;

            }
            else{
                index2 = 0;
                create_game_port(2);
            }
            break;
        case 3:
            group3[index3] = id;
            if (index3 != 2){
                
                
                index3 ++;

            }
            else{
                index3 = 0;
                create_game_port(3);
            }
            break;
        case 4:
            group4[index4] = id;
            if (index4 != 3){
                
                
                index4 ++;

            }
            else{
                index4 = 0;
                create_game_port(4);
            }
            break;






    }


}  
void int_to_char(char* portnum, int digit){
    int temp = digit;
    for(int i=3; i>=0; i--){
        portnum[i] = temp % 10 + '0';
        temp = temp / 10;
    }

}

void create_game_port(int size){
    char portnum[4];
    send_port(size, portnum);
    send_id(size);
    write(1, "A game started on port ", 24);
    write(1, portnum, 5);
    write(1,"\n", 2);
    new_port += 1;
}

void send_port(int size, char* portnum){
    
    int_to_char(portnum, new_port);
    
    
    switch(size){
        case 2:
            for(int i = 0; i < size; i ++)
                
                send(group2[i], portnum, 4, 0);
            

            
            break;

        case 3:
            for(int i = 0; i < size; i ++)
                send(group3[i], portnum, 4, 0);
            
            break;

        case 4:
            for(int i = 0; i < size; i ++)
                send(group4[i], portnum, 4, 0);
            
            break;


    }

    

}


void send_id(int size){
    switch(size){
        case 2:
    
            for(int i = 0; i < size; i ++){
                char id[1];
                id[0] = i + '0';
                send(group2[i], id, 1, 0);
            }
        break;
        case 3:
    
            for(int i = 0; i < size; i ++){
                char id[1];
                id[0] = i + '0';
                send(group3[i], id, 1, 0);
            }
        break;
        case 4:
    
            for(int i = 0; i < size; i ++){
                char id[1];
                id[0] = i + '0';
                send(group4[i], id, 1, 0);
            }
        break;
    
    }
            
}
