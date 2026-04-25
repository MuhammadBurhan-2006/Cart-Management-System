#ifndef STOCKMANAGER_H
#define STOCKMANAGER_H

#include <iostream>
#include <string>
#include "Globals.h"
#include "Products.h"
#include "FileManager.h"
#include <fstream> //For File Handling

using namespace std;

class StockManager {
private:
	Product* products[MAX_PRODUCTS];
	int productCount;

	//Helper Function For Finding ProdInd by ID
	//Used Majorly By updateStock() & removeProduct() Functions
	int findProductIndex(int productID) {
		for (int i = 0; i < productCount; i++) {
			if (products[i]->getProductID() == productID) {
				return i;
			}
		}
		return -1; // Not found
	}

	//Helper Funcs
	//Stoi is Not Allowed, So I Made My Own :P
	int convInt(const string& str) {
		int result = 0;
		int i = 0;
		bool negative = false;
		if (!str.empty() && str[i] == '-') {
			negative = true;
			i++;
		}

		for (; i < (int)str.size(); i++) {
			if (str[i] < '0' || str[i] > '9') break;
			result = result * 10 + (str[i] - '0');
		}

		if (result < 0)
			return -result; //Less Than 0 So Return Neg
		else
			return result;
	}

	//Stod is Not Allowed, So I Made My Own :P
	double convDouble(const string& str) {
		double result = 0.0;
		int i = 0;
		bool negative = false;
		//Check if Number is Negative
		if (!str.empty() && str[i] == '-') {
			negative = true;
			i++;
		}

		//If Number is an Integer
		for (; i < (int)str.size() && str[i] != '.'; i++) {
			if (str[i] < '0' || str[i] > '9')
				break;
			result = result * 10.0 + (str[i] - '0');
		}

		//If Number is a Decimal
		if (i < (int)str.size() && str[i] == '.') {
			i++;
			double fraction = 0.1;
			for (; i < (int)str.size(); i++) {
				if (str[i] < '0' || str[i] > '9')
					break;
				result += (str[i] - '0') * fraction;
				fraction *= 0.1;
			}
		}

		if (result < 0.0)
			return -result; //Less Than 0 So Return Neg
		else
			return result;
	}

	// Extracts Next Comma-Delimited Field From Line Starting at Pos
	void getField(const string& line, int& pos, string& out) {
		out = "";
		while (line[pos] != ',' && line[pos] != '\n' && line[pos] != '\0')
			out += line[pos++];
		pos++;
	}

	// Checks if str1 == str2 Manually
	bool strEquals(const string& str1, const string& str2) {
		if (str1.size() != str2.size()) return false;

		for (int i = 0; i < (int)str1.size(); i++) {
			if (str1[i] != str2[i]) return false;
		}

		return true;
	}

public:
	// Default Const
	StockManager() {
		this->productCount = 0;
		for (int i = 0; i < MAX_PRODUCTS; i++)
			this->products[i] = nullptr;
	}

	// Parameterized Const
	StockManager(Product* prods[], int count) {
		this->productCount = 0;
		for (int i = 0; i < MAX_PRODUCTS; i++)
			this->products[i] = nullptr;
		for (int i = 0; i < count; i++)
			addProduct(prods[i]);
	}

	//Adding Product(s) To Stock
	void addProduct(Product* prod) {
		if (productCount < MAX_PRODUCTS) {
			this->products[productCount] = prod;
			this->productCount++;
		}
		else {
			cout << "Stock is Full! Can't Add More Products." << endl;
		}
	}

	//Destructor
	~StockManager() {
		for (int i = 0; i < productCount; i++) {
			delete this->products[i];
			this->products[i] = nullptr;
		}
	}

	//Remove Prod by ID(Shift Array to The Left)
	void removeProduct(int productID) {
		int index = findProductIndex(productID);
		if (index == -1) {
			cout << "Error! Product ID " << productID << " Does NOT Exist!" << endl;

			return;
		}

		delete this->products[index];
		//Shifts Remaining Products to Left
		for (int i = index; i < this->productCount - 1; i++) {
			this->products[i] = this->products[i + 1];
		}

		this->products[this->productCount - 1] = nullptr;
		this->productCount--;
	}

	//Update Stock Quantity of Product by ID
	void updateStock(int prodID, int newQty) {
		int index = findProductIndex(prodID);
		if (index == -1) {
			cout << "Error! Product ID " << prodID << " Does NOT Exist!" << endl;
			return;
		}

		this->products[index]->setStockQty(newQty);
		saveStock();
	}

	// Reads All the Prods from FILE_PRODUCTS and Populates the products[] Array
	void loadStock() {
		// First We Clean Data Up/Delete Existing Prods in Array
		for (int i = 0; i < this->productCount; i++) {
			delete this->products[i];
			this->products[i] = nullptr;
		}

		this->productCount = FileManager::loadProducts(this->products, MAX_PRODUCTS);
	}

	void saveStock() {
		FileManager::saveProducts(this->products, this->productCount);
	}

	int getProductCount() const {
		return this->productCount;
	}

	// Returns Count of Products at or Below STOCK_CRITICAL_THRESHOLD
	int getCriticalCount() const {
		int Count = 0;
		for (int i = 0; i < this->productCount; i++) {
			if (this->products[i]->getStockQty() <= STOCK_CRITICAL_THRESHOLD)
				Count++;
		}

		return Count;
	}

	Product* getProduct(int index) const {
		if (index >= 0 && index < productCount)
			return this->products[index];

		return nullptr;
	}

	// Fills result[] With Products at or Below STOCK_LOW_THRESHOLD And Returns How Many Were Found
	int getLowStockItems(Product* result[], int maxSize) {
		int Count = 0;
		for (int i = 0; i < productCount && Count < maxSize; i++) {
			if (this->products[i]->getStockQty() <= STOCK_LOW_THRESHOLD) {
				result[Count] = this->products[i];
				Count++;
			}
		}

		return Count;
	}

	int getCriticalStockThreshold() const {
		int count = 0;
		for (int i = 0; i < productCount; i++) {
			if (this->products[i]->getStockQty() <= STOCK_CRITICAL_THRESHOLD) {
				count++;
			}
		}

		return count;
	}

	void displayAll() {
		cout << "Total Products: " << getProductCount() << endl;
		for (int i = 0; i < productCount; i++) {
			this->products[i]->display();
		}
	}

	void displayLowStock() {
		cout << "Products with Low Stock (<= " << STOCK_LOW_THRESHOLD << "):" << endl;
		for (int i = 0; i < productCount; i++) {
			if (products[i]->getStockQty() <= STOCK_LOW_THRESHOLD) {
				products[i]->display();
			}
		}
	}

	// Prints Alert Message to Console if Any Low/Critical Items Exist
	// GUI Version in AdminDashboard Uses getLowStockItems() Directly
	void alertLowStock() {
		int lowCount = 0;
		int critCount = 0;

		for (int i = 0; i < productCount; i++) {
			int qty = products[i]->getStockQty();
			if (qty <= STOCK_CRITICAL_THRESHOLD)
				critCount++;
			else if (qty <= STOCK_LOW_THRESHOLD)
				lowCount++;
		}

		if (critCount > 0)
			cout << "ALERT: " << critCount << " Product(s) Are CRITICALLY Low on Stock!" << endl;
		if (lowCount > 0)
			cout << "WARNING: " << lowCount << " Product(s) Are Running Low on Stock." << endl;
		if (critCount == 0 && lowCount == 0)
			cout << "All Stock Levels Are OK." << endl;
	}
};

#endif // !STOCKMANAGER_H