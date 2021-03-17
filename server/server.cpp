#include "tools.h"

int max_clients = 100;

void server_socket_init(){
    
    int i;
    for (i = 0; i < max_clients; i++) 
	{ 
		client_sockets[i] = 0; 
	}

    if( (server_socket_1 = socket( AF_INET , SOCK_STREAM, 0)) < 0 )
    {
        exit(EXIT_FAILURE); 
    }

    int opt = TRUE;
    if( setsockopt(server_socket_1, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )   
    {   
        exit(EXIT_FAILURE);   
    }

    address_1.sin_family = AF_INET; 
	address_1.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
	address_1.sin_port = htons(port_1); 
    
    if (bind(server_socket_1, (struct sockaddr *)&address_1, sizeof(address_1))<0) 
	{ 
		exit(EXIT_FAILURE); 
	}
    
    if (listen(server_socket_1, 100) < 0) 
	{
		exit(EXIT_FAILURE); 
	} 

}