#include <iostream>
#include "Admin.h"

using namespace std;

//Def Const
Admin::Admin() {
	this->adminID = "";
	this->username = "";
	this->password = "";
}

Admin::Admin(string id, string user, string pass) {
	this->adminID = id;
	this->username = user;
	this->password = pass;
}

bool Admin::login(string user, string pass) {
	if (user == this->username && pass == this->password) {
		return true;
	}
	else {
		return false;
	}
}

string Admin::getAdminID() {
	return adminID;
}

string Admin::getUsername() {
	return username;
}

string Admin::getPassword() {
	return password;
}
