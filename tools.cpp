#include "tools.h"

Json::Value read_json() 
{
    ifstream file_input("config.json");
    Json::Reader reader;
    Json::Value root;
    reader.parse(file_input, root);
    return root;
}

string check_username(string username, Json::Value root)
{
    for (int i=0; i<root["users"].size(); i++)
    {
        if (username.compare(root["users"][i]["user"].asString()) == 0)
        {
            return valid_username;
        }
    }
    return invalid;
}

string check_password(string username, string password, Json::Value root)
{
    if (username.empty())
    {
        return no_username;
    }
    for (int i=0; i<root["users"].size(); i++)
    {
        if (username.compare(root["users"][i]["user"].asString()) == 0 && password.compare(root["users"][i]["password"].asString()) == 0)
        {
            return valid_password;
        }
    }
    return invalid;
}

string pwd()
{
    char cwd[PATH_MAX];
    string direc = "257: ";
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
       return direc + cwd + "\n";
    } 
    return "";
}

string mkd(string path)
{
    const char *p = path.c_str();
    mkdir(p, 0777);
    return "257: " + path + " created.\n";
}

string delete_file(string name)
{
    const char *n = name.c_str();
    unlink(n);
    return "250: " + name + " deleted.\n";
}

string delete_directory(string path)
{
    const char *p = path.c_str();
    rmdir(p);
    return "250: " + path + " deleted.\n";
}

Struct ls()
{
    Struct out;
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(cwd)) != NULL) {
      while ((ent = readdir (dir)) != NULL) {
        if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, ".."))
        {
            out.list.push_back(ent->d_name);
        }
      }
      closedir (dir);
    }
    return out;
}