#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include "globals.h"
using namespace std;

class Admin {
private:
    string adminID;
    string username;
    string password;

public:
    //Constructor
    Admin(string id, string user, string pass);
    
    //Default Constructor In Case Needed
    Admin();

    //Compares Given Credentials Against Stored Ones
    bool login(string user, string pass);

    //Getters
    //Get Admin ID  
    string getAdminID();

	//Get The Username Of Admin
    string getUsername();

	//Get The Password Of Admin
	string getPassword();
};

#endif