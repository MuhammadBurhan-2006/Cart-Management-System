#ifndef PRODUCTS.H
#define PRODUCTS.H

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
	virtual ~Product(){ }
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
		price = p;
	}
	void setStockQty(int qty) {
		stockQty = qty;
	}
	//Fuctions that would be overriden in other classes
	virtual double calculateTax() {
		return 0.0;
	}
	virtual double applyDiscount() {
		return 0.0;
	}
	double getFinalPrice() {
		return price + calculateTax() - applyDiscount();
	}
	virtual string getExtraData() {
		return "";
	}
	virtual void display(){
		cout << "Product ID: " << productID << " | ";
		cout << name << " | ";
		cout << category << " | ";
		cout << CURRENCY_SYMBOL << price << " | ";
		cout << "Stocks: " << stockQty << endl;
	}
};
//Perishable product class has been inherited from Product class and an extra thing expiry date has been added
class PerishableProduct : public Product{
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
	string getExpiryDate() const {
		return expiryDate;
	}
	//Setter for expiry date
	void setExpiryDate(string date) {
		expiryDate = date;
	}
	//Overriding function for perishable products
	double calculateTax() override {
		return price * TAX_PERISHABLE;
	}
	double applyDiscount() override {
		return price * DISC_PERISHABLE;
	}
	string getExtraData() override {
		return expiryDate;
	}
	void display() override {
		cout << "Product ID: " << productID << " | ";
		cout << name << " | ";
		cout << CURRENCY_SYMBOL << price << " | ";
		cout << "Stocks: " << stockQty << " | ";
		cout << "Expiry Date: " << expiryDate << " | ";
		cout << "Tax 5%";
		cout << "Discount 10%" << " | ";
		cout << "Final: " << CURRENCY_SYMBOL << getFinalPrice() << endl;
	}
};
class ElectronicProduct : public Product {
private:
	string brand;
	int warrantyMonths;
public:
	ElectronicProduct() : Product() {
		brand = "";
		warrantyMonths = 0;
		category = CAT_ELECTRONIC;
	}
	ElectronicProduct(int id, string n, double p, int qty, string b, int warranty) : Product(id, n, CAT_ELECTRONIC, p, qty) {
		brand = b;
		warrantyMonths = warranty;
	}
	//Getters
	string getBrand() const {
		return brand;
	}
	int getWarrantyMonths() const {
		return warrantyMonths;
	}
	//Setters
	void setBrand(string b) {
		brand = b;
	}
	void setWarrantyMonths(int warranty) {
		warrantyMonths = warranty;
	}
	//Overriding function for electronic products
	double calculateTax() override {
		return price * TAX_ELECTRONIC;
	}
	double applyDiscount() override {
		return price * DISC_ELECTRONIC;
	}
	string getExtraData() override {
		return brand + "," + to_string(warrantyMonths);
	}
	void display() override {
		cout << "Product ID: " << productID << " | ";
		cout << name << " | Brand: ";
		cout << brand << " | ";
		cout << CURRENCY_SYMBOL << price << " | ";
		cout << "Stocks: " << stockQty << " | ";
		cout << "Warranty: " << warrantyMonths << " months | ";
		cout << "Tax 17%";
		cout << "Discount 5%" << " | ";
		cout << "Final: " << CURRENCY_SYMBOL << getFinalPrice() << endl;
	}
};
class GroceryProduct : public Product {
private:
	double weightKg;
public:
	GroceryProduct() : Product(){
		weightKg = 0.0;
		category = CAT_GROCERY;
	}
	GroceryProduct(int id, string n, double p, int qty, double weight) : Product(id, n, CAT_GROCERY, p, qty) {
		weightKg = weight;
	}
	//Getter
	double getWeightKg() const {
		return weightKg;
	}
	//Setter
	void setWeightKg(double weight) {
		weightKg = weight;
	}
	//Overriding function for grocery products
	double calculateTax() override {
		return price * TAX_GROCERY;
	}
	double applyDiscount() override {
		return price * DISC_GROCERY;
	}
	string getExtraData() override {
		return to_string(weightKg);
	}
	void display() override {
		cout << "Product ID: " << productID << " | ";
		cout << name << " | ";
		cout << CURRENCY_SYMBOL << price << " | ";
		cout << "Stocks: " << stockQty << " | ";
		cout << "Weight: " << weightKg << " kg | ";
		cout << "Tax 0%";
		cout << "Discount 8%" << " | ";
		cout << "Final: " << CURRENCY_SYMBOL << getFinalPrice() << endl;
	}
};
#endif