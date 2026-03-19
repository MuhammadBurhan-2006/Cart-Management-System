#include<iostream>
#include<string>
#include "Globals.h"
using namespace std;
class Product {
protected:
	int productID;
	string name;
	string category;
	double price;
	int stockQty;
public:
	Product() {
		productID = 0;
		name = "";
		category = "";
		price = 0.0;
		stockQty = 0;
	}
	Product(int id, string n, string cat, double p, int qty) {
		productID = id;
		name = n;
		category = cat;
		price = p;
		stockQty = qty;
	}

};