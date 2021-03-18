#include "tools.h"

int client_sockets[100];
int server_socket_1;
const int max_clients = 100;
struct sockaddr_in address_1;
int port_1;
fd_set readfds;

int main(int argc , char *argv[]) 
{
    Json::Value root = read_json();
    server_socket_init();
    int activity,i; 
	int max_sd;
	port_1 = 8000;
    
    while(TRUE) 
	{
		FD_ZERO(&readfds); 
		FD_SET(server_socket_1, &readfds); 

		max_sd = server_socket_1; 
			
		for ( i = 0 ; i < max_clients ; i++) 
		{ 	
			if(client_sockets[i] > 0) 
				FD_SET(client_sockets[i] , &readfds);

			if(client_sockets[i] > max_sd) 
				max_sd = client_sockets[i];
		} 
		
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL); 
		cout << "hi\n";
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

    if( (server_socket_1 = socket( AF_INET , SOCK_STREAM, 0)) < 0 )
    {
        cout << error;
        exit(EXIT_FAILURE); 
    }

    int opt = TRUE;
    if( setsockopt(server_socket_1, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )   
    {   
        cout << error;
        exit(EXIT_FAILURE);   
    }

    address_1.sin_family = AF_INET; 
	address_1.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
	address_1.sin_port = htons(port_1); 
    
    if (bind(server_socket_1, (struct sockaddr *)&address_1, sizeof(address_1))<0) 
	{ 
        cout << error;
		exit(EXIT_FAILURE); 
	}

    if (listen(server_socket_1, 100) < 0) 
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

    if (FD_ISSET(server_socket_1, &readfds)) 
	{
		if ((new_socket = accept(server_socket_1, (struct sockaddr *)&addr, (socklen_t*)&addrlen))<0) 
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
			int valread;
            char buffer[1024];

			valread = recv(client_sockets[i] , buffer, sizeof(buffer),0);
			buffer[valread] = '\0'; 
			cout << buffer << endl;
		} 
	}
}