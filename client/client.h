#ifndef CLIENT_H
#define CLIENT_H

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>  
#include <arpa/inet.h>
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/time.h>
#include <netdb.h>
#include <signal.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdlib.h>  
#include <cstdint>


#define TRUE   1  
#define ERROR -1
#define FALSE  0  
#define CLIENT_ADDRESS "255.255.255.255"
#define TIMEOUT 20
#define PORT 8080
#define SERVER_ADDRESS "127.0.0.1"
#define LS "ls"
#define RETR "retr"

using namespace std;

int client_socket;
int client_to_server;
int data_channel;
int data_port, server_port;

int connect_to_server(int port);
void interact_with_server(int client_to_server, int data_channel);
string pwd();

#endif