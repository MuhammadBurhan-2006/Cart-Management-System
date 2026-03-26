#ifndef CUSTOMER_MODULE_H
#define CUSTOMER_MODULE_H

#include <iostream>
#include <string>
#include "Globals.h"    
#include "Products.h"//for Product base class and all subclasses (Perishable, Electronic, Grocery)
using namespace std;


//handles customer login, registration, and identity.
//uses Globals.h: ROLE_CUSTOMER, MAX_NAME_LENGTH, MAX_ID_LENGTH
class Customer {
private:
    int    customerID;
    string name;
    string username;
    string password;
    string phone;

public:

    Customer() {//default constructor
        customerID = 0;
        name = "";
        username = "";
        password = "";
        phone = "";
    }


    Customer(int id, string n, string user, string pass, string ph) {//Parameterized constructor
        customerID = id;
        name = n;
        username = user;
        password = pass;
        phone = ph;
    }

    //Getters
    int    getCustomerID() { return customerID; }
    string getName() { return name; }
    string getUsername() { return username; }
    string getPhone() { return phone; }

    //Setters
    void setCustomerID(int id) { customerID = id; }
    void setName(string n) { name = n; }
    void setUsername(string user) { username = user; }
    void setPassword(string pass) { password = pass; }
    void setPhone(string ph) { phone = ph; }

    //Validates username and password for login
    //Used by: Customer Login Screen
    bool login(string user, string pass) {
        return (username == user && password == pass);
    }

    //Displays basic customer info
    void display() {
        cout << "Customer ID : " << customerID << endl;
        cout << "Name        : " << name << endl;
        cout << "Username    : " << username << endl;
        cout << "Phone       : " << phone << endl;
    }
};