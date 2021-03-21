#include "tools.h"

map <int, string> client_user;
vector <int> user_login;

Json::Value read_json() 
{
    ifstream file_input("config.json");
    Json::Reader reader;
    Json::Value root;
    reader.parse(file_input, root);
    return root;
}

string check_username(string username, Json::Value root, int client)
{
    for (int i=0; i<root["users"].size(); i++)
    {
        if (username.compare(root["users"][i]["user"].asString()) == 0)
        {
            client_user.insert({client, username});

            return valid_username;
        }
    }
    return invalid;
}

string check_password(int client, string password, Json::Value root)
{
    int flag = 0;
    string username; 
    for (map<int, string>::iterator it = client_user.begin(); it != client_user.end(); ++it)
    {
        if (it->first == client)
        {
            username = it->second;
            flag = 1;
            break;
        }   
    }
    if (flag == 0)
    {
        return no_username;
    }
    
    for (int i=0; i<root["users"].size(); i++)
    {
        if (username.compare(root["users"][i]["user"].asString()) == 0 && password.compare(root["users"][i]["password"].asString()) == 0)
        {
            user_login.push_back(client);
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
    return error;
}

string mkd(string path)
{
    mkdir(path.c_str(), 0777);
    return "257: " + path + " created.\n";
}

string delete_sth(string token, string name)
{
    if(token.compare("-f") == 0)
        return delete_file(name);
    if(token.compare("-d") == 0)
        return delete_directory(name);
    return syntax;
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

string quit(int client)
{
    client_user.erase(client);
    user_login.erase(remove(user_login.begin(), user_login.end(), client), user_login.end());
    return logout;
}

int check_if_logged_in(int client)
{
    if(std::find(user_login.begin(), user_login.end(), client) != user_login.end()) 
        return 1;
    return 0;
}

string handle_input(string input, int client)
{
    cout << input<<endl;
    vector <string> tokens;
    stringstream check1(input); 
    string intermediate; 
    Json::Value root = read_json();

    while(getline(check1, intermediate, ' ')) 
    { 
        tokens.push_back(intermediate); 
    } 

    if (tokens[0].compare(commands[USER]) == 0)
        return check_username(tokens[1], root, client);

    if (tokens[0].compare(commands[PASS]) == 0)
        return check_password(client, tokens[1], root);
    
    if (tokens[0].compare(commands[PWD]) == 0)
    {
        if (check_if_logged_in(client) == 1)
            return pwd();
        return not_logged_in;
    }

    if (tokens[0].compare(commands[MKD]) == 0)
    {
        if (check_if_logged_in(client) == 1)
            return mkd(tokens[1]);
        return not_logged_in;
    }

    if (tokens[0].compare(commands[DELE]) == 0)
    {
        if (check_if_logged_in(client) == 1)
            return delete_sth(tokens[1], tokens[2]);
        return not_logged_in;
    }   

    if (tokens[0].compare(commands[LS]) == 0)
    {
        if (check_if_logged_in(client) == 1)
            return ls().list_transfer;
        return not_logged_in;
    } 

    if (tokens[0].compare(commands[CWD]) == 0)
    {
        if (check_if_logged_in(client) == 1)
            return cwd(tokens[1]);
        return not_logged_in;
    }

    if (tokens[0].compare(commands[RENAME]) == 0)
    {
        if (check_if_logged_in(client) == 1)
            return rename_file(tokens[1],tokens[2]);
        return not_logged_in;
    }

    if (tokens[0].compare(commands[RETR]) == 0)
    {
        if (check_if_logged_in(client) == 1)
            return rtr(tokens[1]);
        return not_logged_in;
    }

    if (tokens[0].compare(commands[HELP]) == 0)
    {
        if (check_if_logged_in(client) == 1)
            return help();
        return not_logged_in;
    }

    if (tokens[0].compare(commands[HELP]) == 0)
    {
        if (check_if_logged_in(client) == 1)
            return help();
        return not_logged_in;
    }

    if (tokens[0].compare(commands[QUIT]) == 0)
    {
        if (check_if_logged_in(client) == 1)
            return quit(client);
        return not_logged_in;
    }

    return syntax;
}