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
    mkdir(path.c_str(), 0777);
    return "257: " + path + " created.\n";
}

string delete_file(string name)
{
    unlink(name.c_str());
    return "250: " + name + " deleted.\n";
}

string delete_directory(string path)
{
    rmdir(path.c_str());
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

string cwd(string path)
{
    chdir(path.c_str());
    return change;
}

string rename_file(string from, string to)
{
    rename(from.c_str(), to.c_str());
    return change;
}

string rtr(string name)
{
    //download
    return download_st;
}

string help()
{
    string info = "214\n";
    info += "user <username>, it's argument is used to specify user's string. It's used for user authentication.\n";
    info += "pass <password>, it's argument is used to specify password's string. It's used for user authentication.\n";
    info += "pwd, it's used to show the current directory.\n";
    info += "mkd <directory path>, it's argument is used to specify directory's string. It's user for creating new directory.\n";
    info += "dele -f <file name>, it's argument is used to specify file name's string. It's user for deleting a file.\n";
    info += "dele -f <directory path>, it's argument is used to specify directory name's string. It's user for deleting a directory.\n";
    info += "ls, it's used for listing computer files in current directory.\n";
    info += "cwd <path>, it's argument is used to specify directory path's string. It's used for changing the current directory.\n";
    info += "rename <from> <to>, it's first argument is used to specify file's name and second to specify file's new name. It's used for changing file's name.\n";
    info += "retr <name>, it's argument is used to specify filename's string. It's used for downloading a file.\n";
    info += "quit, It's used for logging out.\n";
    return info;
}

string quit()
{
    // logout
    return logout;
}