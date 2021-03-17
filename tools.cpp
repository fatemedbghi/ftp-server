#include "tools.h"

void read_json() 
{
    ifstream file_input("config.json");
    Json::Reader reader;
    Json::Value root;
    reader.parse(file_input, root);
    cout << root["users"]["user"];
}