#include "client.h"

int main(int argc , char *argv[])
{
    // cout<<argv[2];
    string word;
    // cin >> word;
    server_port = atoi(argv[1]);
    data_port = atoi(argv[2]);
    
    client_to_server = connect_to_server(server_port, server_address);

    data_channel = connect_to_server(data_port, client_address);
    cout<< client_to_server << ", "<< data_channel<<endl;
    interact_with_server();
    cout<<"heeeeeeeeey";
    return 0;
}



int connect_to_server(int port, sockaddr_in address)
{
    int sock;
    if( (sock = socket( AF_INET , SOCK_STREAM, 0)) < 0 )
    {
        cout << "500: Error\n";
        return ERROR;
    }

    server_address.sin_family = AF_INET; 
	server_address.sin_port = htons(port); 

    if(inet_pton(AF_INET,SERVER_ADDRESS, &address.sin_addr)<=0)  
    { 
        cout << "500: Error\n";
        return ERROR;
    }

    if (connect(sock, (struct sockaddr*)&address,sizeof(address)) < 0) {
		cout << "500: Error\n";
        return ERROR;
	}
    return sock;

}
void interact_with_server()
{
    char input[2048];
    memset(input, 0, sizeof(input));
	strcpy(input, pwd().c_str());
    // cout<<pwd();
    if (send(client_to_server, input,  strlen(input), 0) <= 0)
    {
        cout << "500: Error\n";
        return ; 
    }
// cout<<"line 57\n";
    while (TRUE)
    {
        string in;
        getline(cin, in);
        memset(input, 0, sizeof(input));
		strcpy(input, in.c_str());

        
        if (send(client_to_server, input,  strlen(input), 0) <= 0)
        {
            cout << "500: Error\n";
            return ; 
        }

        istringstream ss(in);
        string token;
        ss >> token;
        cout << token<<endl;

        // char *token = strtok(&in[0], " ");
        if (token.compare(LS) == 0 || token.compare(RETR) == 0)
        {
            char response_data[2048];
            if (recv(data_channel,&response_data,sizeof(response_data),0) <= 0)
            {
                cout << "500: Error\n";
                return ; 
            }
            cout << response_data;
        }
        char server_response[2048];
        memset(server_response, 0, sizeof server_response);
        if (recv(client_to_server,&server_response,sizeof(server_response),0) <= 0)
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