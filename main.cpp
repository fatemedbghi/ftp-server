#include "tools.h"

int main(int argc , char *argv[]) 
{
    Json::Value root = read_json();
    cout << ls().list_transfer;
    
	return 0; 
}