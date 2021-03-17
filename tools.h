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

#define TRUE 1
#define False 0

using namespace std;

const string valid_username = "331: User name okay, need password.\n";
const string invalid = "430: Invalid username or password\n";
const string no_username = "503: Bad sequence of commands.\n";
const string valid_password = "230: User logged in, proceed. Logged out if appropriate.\n";
const string illegal_access = "550: File unavailable.\n";

struct ls_out { 
    vector <string> list;
    string list_transfer = "226: List transfer done.\n";
}; 

typedef struct ls_out Struct; 

Json::Value read_json();
string check_username(string username, Json::Value root);
string check_password(string username, string password, Json::Value root);
string pwd();
string mkd(string path);
string delete_file(string name);
string delete_directory(string path);
Struct ls();

#endif