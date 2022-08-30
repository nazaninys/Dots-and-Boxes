#include "Server.h"
     
int main(int argc , char *argv[])   
{   
    int PORT = atoi(argv[1]);
    int opt = TRUE;     
    index2 = index3 = index4 = 0;  
    new_port = PORT + 1; 
        
        
    char *message = "ECHO Daemon v1.0 \r\n";   
      
    for (int i = 0; i < MAX_CLI; i++){   
        client_sockets[i] = 0;   
    }   
         
    if ((sockid = socket(AF_INET , SOCK_STREAM , 0)) == FALSE){   
        perror("socket failed");   
        exit(EXIT_FAILURE);   
    }   
      
    if (setsockopt(sockid, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0){   
        perror("setsockopt");   
        exit(EXIT_FAILURE);   
    }   
      
    address.sin_family = AF_INET;   
    address.sin_addr.s_addr = inet_addr(ADDRESS);   
    address.sin_port = htons(PORT);   
 
    if (bind(sockid, (struct sockaddr *)&address, sizeof(address))<0){   
        perror("bind failed");   
        exit(EXIT_FAILURE);   
    }   
    write(1,"Listener on port 8080 \n", 23);   
         
    
    if (listen(sockid, 10) < 0){   
        perror("listen");   
        exit(EXIT_FAILURE);   
    }   
         
    
    int addrlen = sizeof(address);   
    write(1, "Waiting for connections\n", 24);   
         
    while(TRUE){    
        FD_ZERO(&readfds);   
        FD_SET(sockid, &readfds);   
        int max_desc = MAX_CLI;   
        for (int i = 0 ; i < MAX_CLI ; i++){   
        
            if(client_sockets[i] > 0)   
                FD_SET(client_sockets[i], &readfds);     
            if(client_sockets[i] > max_desc)   
                max_desc = client_sockets[i];   
        }   
     
          
        int activity = select( max_desc + 1 , &readfds , NULL , NULL , NULL);   
       
        if ((activity < 0) && (errno!=EINTR)){   
            write(1, "select error\n", 14);   
        }   
        
        add_client();
        get_inf();

        
    } 
             
        
         
         
    return 0;   
}   