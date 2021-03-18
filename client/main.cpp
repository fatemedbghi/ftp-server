#include "client.h"

int main()
{
    connect_to_server();
    return 0;
}

void connect_to_server()
{
    
    if( (client_to_sever = socket( AF_INET , SOCK_STREAM, 0)) < 0 )
    {
        cout << "500: Error\n";
        return;
    }

    server_address.sin_family = AF_INET; 
	server_address.sin_port = htons(server_port); 

	
	
    if(inet_pton(AF_INET,SERVER_ADDRESS, &server_address.sin_addr)<=0)  
    {
        cout << "500: Error\n";
        return ; 
    }
	//error
    if (connect(client_to_sever, (struct sockaddr*)&server_address,sizeof(server_address)) < 0) {
		
		cout << "500: Error\n";
        return ;
	}
    
    while (TRUE)
    {
        char number[256];
        cin >> number;
        if (send(client_to_sever, number,  strlen(number), 0) <= 0)
        {
            cout << "500: Error\n";
            return ; 
        }
        char server_response[256];
        recv(client_to_sever,&server_response,sizeof(server_response),0);
    }

}