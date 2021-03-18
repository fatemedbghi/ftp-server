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
#define FALSE  0  
#define CLIENT_ADDRESS "255.255.255.255"
#define SERVER_ADDRESS "127.0.0.1"

using namespace std;

int client_socket;
int client_to_sever;
struct sockaddr_in client_address;
struct sockaddr_in server_address;
int port;
int group_n;
int id;
char buffer[1024];
int turn;
int hmap[5][4];
int vmap[4][5];
int score;
int move;
const int server_port = 8000;

void connect_to_server();

#endif