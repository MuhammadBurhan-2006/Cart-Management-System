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
	//Fuctions that would be overriden in other classes
	double calculateTax() {
		return 0.0;
	}
	double applyDiscount() {
		return 0.0;
	}
	double getFinalPrice() {
		return price + calculateTax() - applyDiscount();
	}
	void display(){
		cout << "Product ID: " << productID << " | ";
		cout << name << " | ";
		cout << category << " | ";
		cout << CURRENCY_SYMBOL << price << " | ";
		cout << "Stocks: " << stockQty << endl;
	}
};
//Perishable product class has been inherited from Product class and an extra thing expiry date has been added
clas PerishableProduct : public Product{
private:
	string expiryDate;
public:
	PerishableProduct() : Product() {
		expiryDate = "";
		category = CAT_PERISHABLE;
	}
	PerishableProduct(int id, string n, double p, int qty, string expiry) : Product(id, n, CAT_PERISHABLE, p, qty) {
		expiryDate = expiry;
	}
	//Getter for expiry date
	string getExpiryDate() {
		return expiryDate;
	}
	//Setter for expiry date
	void setExpiryDate(string date){
		expiryDate = date;
	}
}
