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
	//Getters
	int getProductID() {
		return productID;
	}
	string getName() {
		return name;
	}
	string getCategory() {
		return category;
	}
	double getPrice() {
		return price;
	}
	int getStockQty() {
		return stockQty;
	}
	//Setters
	void setProductID(int id) {
		productID = id;
	}
	void setName(string n) {
		name = n;
	}
	void setCategory(string cat) {
		category = cat;
	}
	void setPrice(double p) {
		return p;
	}
	void setStockQty(int qty) {
		stockQty = qty;
	}
};