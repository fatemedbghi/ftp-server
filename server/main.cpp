#include "tools.h"

fd_set readfds;
fd_set datafds;

int main(int argc , char *argv[]) 
{
	int port = atoi(argv[1]);
	int data_port = atoi(argv[2]);
	int activity1,activity2,i; 
	int max_sd;
	int max_data_sd;
	struct sockaddr_in address;
	struct sockaddr_in data_address;


    int server_socket = server_socket_init(port);
	int data_socket = server_socket_init(data_port);
	int client_sockets[max_clients] = {};
	int client_data[max_clients] = {};
	
	while(TRUE) 
	{
		FD_ZERO(&readfds); 
		FD_SET(server_socket, &readfds); 

		FD_ZERO(&datafds); 
		FD_SET(data_socket, &datafds); 

		max_sd = server_socket; 
		max_data_sd = data_socket;
			
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
	
		activity1 = select( max_sd + 1 , &readfds , NULL , NULL , NULL); 
		activity2 = select( max_data_sd + 1 , &datafds , NULL , NULL , NULL); 

		if (((activity1 < 0) || (activity2<0)) && (errno!=EINTR)) 
		{ 
			cout << error;
		}

        incoming_connections(server_socket, client_sockets, 1);
		incoming_connections(data_socket, client_data, 2);
        incoming_input(client_sockets, client_data);
	} 
		
	return 0; 
}

int server_socket_init(int port){

	int server_socket;

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
	struct sockaddr_in address;
    address.sin_family = AF_INET; 
	address.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
	address.sin_port = htons(port); 
    
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
        cout << error;
		exit(EXIT_FAILURE); 
	}
    
    if (listen(server_socket, max_clients) < 0) 
	{ 
		cout << error;
		exit(EXIT_FAILURE); 
	} 

	return server_socket;
}

int* incoming_connections(int server_socket, int* client_sockets, int cOrd){

    int new_socket, i, addrlen;
    struct sockaddr_in addr;
    addrlen = sizeof(addr);
    memset(&addr, 0, sizeof(addr));

	if (cOrd == 1)
	{
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
	// if (cOrd == 2)
	// {
	// 	if (FD_ISSET(server_socket, &datafds)) 
	// 	{ 
	// 		if ((new_socket = accept(server_socket, (struct sockaddr *)&addr, (socklen_t*)&addrlen))<0) 
	// 		{ 
	// 			cout << error;
	// 			exit(EXIT_FAILURE); 
	// 		} 

	// 		for (i = 0; i < max_clients; i++) 
	// 		{
	// 			if( client_sockets[i] == 0 ) 
	// 			{
	// 				client_sockets[i] = new_socket; 
	// 				break; 
	// 			} 
	// 		}
	// 	}
	// }
    
	return client_sockets;
}

void incoming_input(int* client_sockets, int* client_data){

    int new_socket, i, addrlen;
	map <int, string> c_directory;
    struct sockaddr_in addr;
    addrlen = sizeof(addr);
    memset(&addr, 0, sizeof(addr));
	
	for (i = 0; i < max_clients; i++) 
	{
		cout << "hiii\n";
		if (FD_ISSET( client_sockets[i] , &readfds)) 
		{
			cout << "byyyyyyye\n";
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