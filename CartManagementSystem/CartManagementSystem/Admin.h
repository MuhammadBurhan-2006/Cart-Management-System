#ifndef ADMIN_H
#define ADMIN_H
#include <string>
#include "Globals.h"
using namespace std;
class Admin {
private:
    string adminID;
    string username;
    string password;
public:
    Admin() {
        adminID = "";
        username = "";
        password = "";
    }
    // Parameterized Constructor
    Admin(string id, string user, string pass) {
        adminID = id;
        username = user;
        password = pass;
    }
    // Compares given credentials against stored ones
    bool login(string user, string pass) {
        return (user == username && pass == password);
    }
    // Getters
    string getAdminID() { return adminID; }
    string getUsername() { return username; }
    string getPassword() { return password; }
};
#endif