#ifndef TOOLS_H
#define TOOLS_H

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
#include <map>
#include <vector>
#include <algorithm>

#define TRUE 1
#define False 0
#define PORT 8888
#define SERVER_ADDRESS "127.0.0.1"
#define USER 0
#define PASS 1
#define PWD 2
#define MKD 3
#define DELE 4
#define LS 5
#define CWD 6
#define RENAME 7
#define RETR 8
#define HELP 9
#define QUIT 10

using namespace std;

const string valid_username = "331: User name okay, need password.\n";
const string invalid = "430: Invalid username or password\n";
const string no_username = "503: Bad sequence of commands.\n";
const string valid_password = "230: User logged in, proceed. Logged out if appropriate.\n";
const string illegal_access = "550: File unavailable.\n";
const string change = "250: Successful change.\n";
const string download_st = "226: Successful download.\n";
const string logout = "221: Successful Quit.\n";

// ERRORS
const string not_logged_in = "332: Need account for login.\n";
const string syntax = "501: Syntax error in parameters or arguments.\n";
const string error = "500: Error\n";
const string data = "425: Can't open data connection.";

struct ls_out { 
    vector <string> list;
    string list_transfer = "226: List transfer done.\n";
}; 

typedef struct ls_out Struct; 

const vector <string> commands {"user", "pass", "pwd", "mkd", "dele", "ls", "cwd", "rename", "retr", "help", "quit"};

Json::Value read_json();
string check_username(string username, Json::Value root, int client);
string check_password(int client, string password, Json::Value root);
string pwd(int client, map<int,string> c_directory);
string mkd(string path, int client, map <int, string> c_directory);
string delete_sth(string token, string name, int client, map<int,string> c_directory);
string delete_file(string pre, string name);
string delete_directory(string pre, string path);
Struct ls(int client, map <int, string> c_directory);
string cwd(string path, int client, map<int,string> &c_directory);
string rename_file(string from, string to, int client, map<int,string> c_directory);
string rtr(string name, int client, map<int,string> c_directory);
string help();
string quit(int client, map <int, string> &c_directory);

string handle_input(string input, int client, map<int,string> &c_directory);
int check_if_logged_in(int client);
fstream create_log();

void server_socket_init();
void incoming_connections();
void incoming_input();

#endif