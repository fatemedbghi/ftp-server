#include "client.h"

int main(int argc , char *argv[])
{
    assign_ports();
    client_to_server = connect_to_server(server_port);
    data_channel = connect_to_server(data_port);

    interact_with_server(client_to_server, data_channel);

    return 0;
}

void assign_ports()
{
    Json::Value config = read_json();
    server_port = config["commandChannelPort"].asInt();
    data_port = config["dataChannelPort"].asInt();
}

int connect_to_server(int port)
{
    int sock;
    if( (sock = socket( AF_INET , SOCK_STREAM, 0)) < 0 )
    {
        cout << "500: Error\n";
        return ERROR;
    }
    struct sockaddr_in address;
    address.sin_family = AF_INET; 
	address.sin_port = htons(port); 

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

void interact_with_server(int client_to_server, int data_channel)
{
    char input[2048];
    memset(input, 0, sizeof(input));
	strcpy(input, pwd().c_str());

    if (send(client_to_server, input,  strlen(input), 0) <= 0)
    {
        cout << "500: Error\n";
        return ; 
    }

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
       
        if ((token.compare(LS) == 0 && token.size() == 1) || (token.compare(RETR) == 0 && token.size() == 2))
        {
            char response_data[2048];
            memset(response_data, 0, sizeof(response_data));
            if (recv(data_channel, &response_data,sizeof(response_data),0) <= 0)
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

Json::Value read_json() 
{
    ifstream file_input("config.json");
    Json::Reader reader;
    Json::Value root;
    reader.parse(file_input, root);
    return root;
}