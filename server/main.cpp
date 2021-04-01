#include "tools.h"

int server_socket;
struct sockaddr_in address;
struct sockaddr_in data_address;
int client_sockets[max_clients];
int client_data[max_clients];
fd_set readfds;
fd_set datafds;
int port;
int data_port;
map <int, string> c_directory;

int main(int argc , char *argv[]) 
{
	port = atoi(argv[1]);
	data_port = atoi(argv[2]);
	int activity,i; 
	int max_sd;
	int max_data_sd;
	
    int server_socket = server_socket_init(port, address);
	int data_socket = server_socket_init(data_port, data_address);

	while(TRUE) 
	{
		FD_ZERO(&readfds); 
		FD_SET(server_socket, &readfds); 

		FD_ZERO(&datafds); 
		FD_SET(data_socket, &datafds); 

		max_sd = server_socket; 
		
			
		for ( i = 0 ; i < max_clients ; i++) 
		{ 	
			if(client_sockets[i] > 0) 
				FD_SET(client_sockets[i] , &readfds);

			if(client_sockets[i] > max_sd) 
				max_sd = client_sockets[i];

			if(client_data[i] > 0) 
				FD_SET(client_data[i] , &datafds);

			if(client_data[i] > max_data_sd) 
				max_data_sd = client_data[i];
		} 
	
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL); 
		// select data?


		if ((activity < 0) && (errno!=EINTR)) 
		{ 
			cout << error;
		} 

        incoming_connections(server_socket,client_sockets);
		incoming_connections(data_socket,client_data);
        incoming_input();
	} 
		
	return 0; 
} 


int server_socket_init(int port, sockaddr_in address){

	int server_socket;
    
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

	return server_socket;

}

void incoming_connections(int server_socket, int client_sockets[max_clients]){

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
			char buffer[2048];
			memset(buffer, 0, sizeof buffer);
			if(recv(client_sockets[i] , buffer, sizeof(buffer),0) < 0)
			{
				cout << error;
			}

			c_directory.insert({client_sockets[i],buffer});

			while (TRUE)
			{
				memset(buffer, 0, sizeof buffer);
				if(recv(client_sockets[i] , buffer, sizeof(buffer),0) < 0)
				{
					cout << error;
				}

				string input = buffer;
				string output = handle_input(input, client_sockets[i], client_data[i], c_directory);
				char message[2048];
				memset(message, 0, sizeof message);
				strcpy(message, output.c_str());
				if(send(client_sockets[i], message, strlen(message), 0) < 0) 
				{ 
					cout << error;
				}
			}
		
		} 
	} 
}