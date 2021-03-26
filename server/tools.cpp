#include "tools.h"

map <int, string> client_user;
vector <int> user_login;
vector <int> user_admin;

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
            fstream file = create_log();
            time_t my_time = time(NULL); 
            file << "user " + username + " logged in: " + ctime(&my_time) << "\n";
            string tr = "true";
            if (tr.compare(root["users"][i]["admin"].asString()) == 0)
                user_admin.push_back(client);
            return valid_password;
        }
    }
    return invalid;
}

string pwd(int client, map <int, string> c_directory)
{
    char cwd[PATH_MAX];
    string direc = "257: " + c_directory[client] + "\n";
    return direc;
}

string mkd(string path, int client, map <int, string> c_directory)
{
    mkdir((c_directory[client] + "/" + path).c_str(), 0777);
    fstream file = create_log();
    time_t my_time = time(NULL); 
    file << "directory " + path + " created by user " + client_user[client] + ": " + ctime(&my_time) << "\n";
    return "257: " + path + " created.\n";
}

string delete_sth(string token, string name, int client, map <int, string> c_directory)
{
    if(token.compare("-f") == 0)
        return delete_file(c_directory[client]+"/", name);
    if(token.compare("-d") == 0)
        return delete_directory(c_directory[client]+"/", name);
    fstream file = create_log();
    time_t my_time = time(NULL); 
    file << name + " deleted by user " + client_user[client] + ": " + ctime(&my_time) << "\n";
    return syntax;
}

string delete_file(string pre, string name)
{
    string p = pre + name;
    unlink(p.c_str());
    return "250: " + name + " deleted.\n";
}

string delete_directory(string pre, string path)
{   
    string p = pre + path;
    rmdir(p.c_str());
    return "250: " + path + " deleted.\n";
}

Struct ls(int client, map <int, string> c_directory)
{
    Struct out;
    string c = c_directory[client];
    char cwd[PATH_MAX];
    strcpy(cwd, c.c_str());
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

string cwd(string path, int client, map<int,string> &c_directory)
{
    if (path.compare("..") == 0)
    {
        vector <string> tokens;
        stringstream check1(c_directory[client]);
        string intermediate; 
        while(getline(check1, intermediate, '/')) 
        {
            tokens.push_back(intermediate); 
        }
        string new_path = "";
        for (int i=1; i<tokens.size()-1;i++)
            new_path = new_path + "/" + tokens[i];
        c_directory[client] = new_path;
    }
    else
        c_directory[client] = c_directory[client] + "/" + path;
    return change;
}

string rename_file(string from, string to, int client, map<int,string> c_directory)
{
    rename((c_directory[client]+"/"+ from).c_str(), (c_directory[client]+"/"+ to).c_str());
    fstream file = create_log();
    time_t my_time = time(NULL); 
    file << from + " renamed to " + to + " by user " + client_user[client] + ": " + ctime(&my_time) << "\n";
    return change;
}

string rtr(string name, int client, map<int,string> c_directory)
{
    //download
    fstream file = create_log();
    time_t my_time = time(NULL); 
    file << "file " + name + " downloaded by user " + client_user[client] + ": " + ctime(&my_time) << "\n";
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

string quit(int client, map <int, string> &c_directory)
{
    client_user.erase(client);
    c_directory.erase(client);
    user_login.erase(remove(user_login.begin(), user_login.end(), client), user_login.end());
    user_admin.erase(remove(user_admin.begin(), user_admin.end(), client), user_admin.end());
    return logout;
}

int check_if_logged_in(int client)
{
    if(std::find(user_login.begin(), user_login.end(), client) != user_login.end()) 
        return 1;
    return 0;
}

fstream create_log()
{
    fstream file;
    file.open("log", ios_base::app);
    return file;
}

vector<string> unaccessed_files(Json::Value root)
{
    vector<string> files;
    for (int i=0; i<root["files"].size(); i++)
    {
        files.push_back(root["files"][i].asString());
    }
    return files;
}

int check_if_file_accessed(int client, vector<string> files, string file_name)
{
    if (std::find(files.begin(), files.end(), file_name) != files.end())
        if(std::find(user_admin.begin(), user_admin.end(), client) != user_admin.end()) 
            return 1;
        else
            return 0;
    return 1;
}

string handle_input(string input, int client, map<int,string> &c_directory)
{
    vector <string> tokens;
    stringstream check1(input); 
    string intermediate; 
    Json::Value root = read_json();
    vector <string> files = unaccessed_files(root);

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
            return pwd(client, c_directory);
        return not_logged_in;
    }

    if (tokens[0].compare(commands[MKD]) == 0)
    {
        if (check_if_logged_in(client) == 1)
            return mkd(tokens[1], client, c_directory);
        return not_logged_in;
    }

    if (tokens[0].compare(commands[DELE]) == 0)
    {
        if (check_if_logged_in(client) == 1)
        {
            if (check_if_file_accessed(client, files, tokens[2]) == 1)
                return delete_sth(tokens[1], tokens[2], client, c_directory);
            return illegal_access;

        }
        return not_logged_in;
    }   

    if (tokens[0].compare(commands[LS]) == 0)
    {
        if (check_if_logged_in(client) == 1)
            return ls(client, c_directory).list_transfer;
        return not_logged_in;
    } 

    if (tokens[0].compare(commands[CWD]) == 0)
    {
        if (check_if_logged_in(client) == 1)
            return cwd(tokens[1], client, c_directory);
        return not_logged_in;
    }

    if (tokens[0].compare(commands[RENAME]) == 0)
    {
        if (check_if_logged_in(client) == 1)
        {
            if (check_if_file_accessed(client, files, tokens[1]) == 1)
                return rename_file(tokens[1],tokens[2], client, c_directory);
            return illegal_access;
        }
        return not_logged_in;
    }

    if (tokens[0].compare(commands[RETR]) == 0)
    {
        if (check_if_logged_in(client) == 1)
        {
            if (check_if_file_accessed(client, files, tokens[1]) == 1)
                return rtr(tokens[1], client, c_directory);
            return illegal_access;
        }
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
            return quit(client, c_directory);
        return not_logged_in;
    }

    return syntax;
}