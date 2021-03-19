#include "tools.h"

int server_socket;
struct sockaddr_in address;
int client_sockets[30];
fd_set readfds;
int port;
int max_clients = 30;

int main(int argc , char *argv[]) 
{
	port = atoi(argv[1]);
	int activity,i; 
	int max_sd;

    server_socket_init();

	while(TRUE) 
	{
		FD_ZERO(&readfds); 
		FD_SET(server_socket, &readfds); 

		max_sd = server_socket; 
			
		for ( i = 0 ; i < max_clients ; i++) 
		{ 	
			if(client_sockets[i] > 0) 
				FD_SET(client_sockets[i] , &readfds);

			if(client_sockets[i] > max_sd) 
				max_sd = client_sockets[i];
		} 
	
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL); 
	
		if ((activity < 0) && (errno!=EINTR)) 
		{ 
			cout << error;
		} 
		
        incoming_connections();
        incoming_input();
	} 
		
	return 0; 
} 

void server_socket_init(){
    
    int i;
    for (i = 0; i < max_clients; i++) 
	{ 
		client_sockets[i] = 0; 
	}

    if( (server_socket = socket( AF_INET , SOCK_STREAM, 0)) < 0 )
    {
        cout << error;
        exit(EXIT_FAILURE); 
    }

    int opt = TRUE;
    if( setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )   
    {   
        cout << error;
        exit(EXIT_FAILURE);   
    }

    address.sin_family = AF_INET; 
	address.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
	address.sin_port = htons(port); 
    
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
        cout << error;
		exit(EXIT_FAILURE); 
	}
    
    if (listen(server_socket, 30) < 0) 
	{ 
		cout << error;
		exit(EXIT_FAILURE); 
	} 

}

void incoming_connections(){

    int new_socket, i, addrlen;
    struct sockaddr_in addr;
    addrlen = sizeof(addr);
    memset(&addr, 0, sizeof(addr));

    if (FD_ISSET(server_socket, &readfds)) 
	{ 
		if ((new_socket = accept(server_socket, (struct sockaddr *)&addr, (socklen_t*)&addrlen))<0) 
		{ 
			cout << error;
			exit(EXIT_FAILURE); 
		} 
		
		for (i = 0; i < max_clients; i++) 
		{
			if( client_sockets[i] == 0 ) 
			{
				client_sockets[i] = new_socket; 
				break; 
			} 
		} 
	}

}

void incoming_input(){

    int new_socket, i, addrlen;
    struct sockaddr_in addr;
    addrlen = sizeof(addr);
    memset(&addr, 0, sizeof(addr));

	for (i = 0; i < max_clients; i++) 
	{
		if (FD_ISSET( client_sockets[i] , &readfds)) 
		{
			while (TRUE)
			{
				char buffer[256];
				if(recv(client_sockets[i] , buffer, sizeof(buffer),0) < 0)
				{
					cout << error;
				}
				cout << buffer;
				char message[256];
				cin >> message;  
				if(send(client_sockets[i], message, strlen(message), 0) < 0) 
				{ 
					cout << error;
				}
			}
		
		} 
	} 
}