#include "client.h"

int main(int argc , char *argv[])
{
    server_port = atoi(argv[1]);
    connect_to_server();
    return 0;
}


void connect_to_server()
{
    
    if( (client_to_sever = socket( AF_INET , SOCK_STREAM, 0)) < 0 )
    {
        cout << "500: Error\n";
        return ;
    }

    server_address.sin_family = AF_INET; 
	server_address.sin_port = htons(server_port); 

    if(inet_pton(AF_INET,SERVER_ADDRESS, &server_address.sin_addr)<=0)  
    { 
        cout << "500: Error\n";
        return ; 
    }

    if (connect(client_to_sever, (struct sockaddr*)&server_address,sizeof(server_address)) < 0) {
		cout << "500: Error\n";
        return ;
	}

    char input[2048];
    memset(input, 0, sizeof input);
	strcpy(input, pwd().c_str());
    if (send(client_to_sever, input,  strlen(input), 0) <= 0)
    {
        cout << "500: Error\n";
        return ; 
    }

    while (TRUE)
    {
        string in;
        getline(cin, in);
        memset(input, 0, sizeof input);
		strcpy(input, in.c_str());
        if (send(client_to_sever, input,  strlen(input), 0) <= 0)
        {
            cout << "500: Error\n";
            return ; 
        }
        char server_response[2048];
        memset(server_response, 0, sizeof server_response);
        if (recv(client_to_sever,&server_response,sizeof(server_response),0) <= 0)
        {
            cout << "500: Error\n";
            return ; 
        }
        cout << server_response;
    }

}

string pwd()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
       return cwd;
    }
    return "";
}