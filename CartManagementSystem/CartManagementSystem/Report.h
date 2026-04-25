#ifndef REPORT_H
#define REPORT_H

#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"
#include "FileManager.h"

using namespace std;

class Report {
private:
	string reportDate;
	double totalRevenue;
	double totalTax;
	double totalDiscounts;
	int    totalTransactions;
	double groceryRevenue;
	double perishRevenue;
	double electroRevenue;
	string topProductNames[MAX_REPORT_PRODUCTS];
	int    topProductUnits[MAX_REPORT_PRODUCTS];

	// Works Like line.find(word)
	bool lineContains(const string& line, const string& keyword) {
		if (keyword.size() > line.size()) return false;
		for (int i = 0; i <= (int)(line.size() - keyword.size()); i++) {
			bool match = true;
			for (int j = 0; j < (int)keyword.size(); j++) {
				if (line[i + j] != keyword[j]) {
					match = false;
					break;
				}
			}
			if (match) 
				return true;
		}
		
		return false;
	}

	// Extracts The Number After ':' in a Line
	// e.g. "Grand Total     : Rs.228.50" → 228.50. Skips Past ':' Then Skips Non-Digit Chars Like "Rs."
	double extractValue(const string& line) {
		int pos = 0;

		// Read Until ':'
		while (pos < (int)line.size() && line[pos] != ':')
			pos++;
		pos++; // Skips ':'

		// Skip Spaces
		while (pos < (int)line.size() && line[pos] == ' ')
			pos++;

		// Skip Non-Numeric Chars (e.g. "Rs.")
		while (pos < (int)line.size() &&
			line[pos] != '-' &&
			(line[pos] < '0' || line[pos] > '9'))
			pos++;

		// Work With Negative Sign
		string numStr = "";
		if (pos < (int)line.size() && line[pos] == '-') {
			numStr += '-';
			pos++;
		}

		// Build Number String
		while (pos < (int)line.size() &&
			(line[pos] == '.' || (line[pos] >= '0' && line[pos] <= '9'))) {
			numStr += line[pos];
			pos++;
		}

		if (numStr == "" || numStr == "-")
			return 0.0;
		
		return convDouble(numStr);
	}


	// Stod is Not Allowed So I Made My Own :P
	double convDouble(const string& str) {
		double result = 0.0;
		int i = 0;
		bool negative = false;

		if (!(str == "") && str[i] == '-') {
			negative = true;
			i++;
		}

		// Integer Part
		for (; i < (int)str.size() && str[i] != '.'; i++) {
			if (str[i] < '0' || str[i] > '9') break;
			result = result * 10.0 + (str[i] - '0');
		}

		// Decimal Part
		if (i < (int)str.size() && str[i] == '.') {
			i++;
			double fraction = 0.1;
			for (; i < (int)str.size(); i++) {
				if (str[i] < '0' || str[i] > '9') break;
				result += (str[i] - '0') * fraction;
				fraction *= 0.1;
			}
		}
		
		if (result < 0.0)
			return -result; //Less Than 0 So Return Neg
		else
			return result;
	}


	// Stoi is Not Allowed So I Made My Own :P
	int convInt(const string& str) {
		int result = 0;
		int i = 0;
		bool negative = false;

		if (!(str == "") && str[i] == '-') {
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

	// Extracts Product Name From an Item Line in Sales Log
	// Line Format: "  Milk x2  @  Rs.120  =  Rs.240"
	// Name is Everything From First Non-Space Char Up to " x
	string extractProductName(const string& line) {
		// Skip Leading Spaces
		int start = 0;
		while (start < (int)line.size() && line[start] == ' ')
			start++;

		// Find " x" — Marks End of Name
		int end = start;
		while (end < (int)line.size() - 1) {
			if (line[end] == ' ' && line[end + 1] == 'x')
				break;
			end++;
		}

		if (end >= (int)line.size() - 1) return "";
		
		return line.substr(start, end - start);
	}

	// Extracts Qty From an Item Line E.g. Line Format: "  Milk x2  @  Rs.120  =  Rs.240"
	// Qty is Digits Right After 'x'
	int extractProductQty(const string& line) {
		// Find " x"
		int pos = 0;
		while (pos < (int)line.size() - 1) {
			if (line[pos] == ' ' && line[pos + 1] == 'x')
				break;
			pos++;
		}
		if (pos >= (int)line.size() - 1) return 0;

		pos += 2; // Skip ' ' and 'x'

		// Read Digits
		string qtyStr = "";
		while (pos < (int)line.size() &&
			line[pos] >= '0' && line[pos] <= '9') {
			qtyStr += line[pos];
			pos++;
		}

		if (qtyStr == "") 
			return 0;
		
		return convInt(qtyStr);
	}

	// Reads Sales Log and Finds Top MAX_REPORT_PRODUCTS Sellers
	// Tracks Units Sold Per Product in Parallel Arrays
	// Then Bubble Sorts Descending by Unit
	void findTopProducts() {
		string names[MAX_PRODUCTS];
		int    units[MAX_PRODUCTS];
		int    count = 0;

		for (int i = 0; i < MAX_PRODUCTS; i++) {
			names[i] = "";
			units[i] = 0;
		}

		ifstream file(FILE_SALES_LOG);
		if (!file.is_open()) return;

		string line;
		bool inItems = false;

		while (getline(file, line)) {
			if (lineContains(line, "Items:")) {
				inItems = true;
				continue;
			}
			if (lineContains(line, "Subtotal")) {
				inItems = false;
				continue;
			}
			if (!inItems || line == "") 
				continue;

			string name = extractProductName(line);
			int    qty = extractProductQty(line);

			if (name == "" || qty <= 0) continue;

			// If Already Tracked, Add to Its Units
			bool found = false;
			for (int i = 0; i < count; i++) {
				if (names[i] == name) {
					units[i] += qty;
					found = true;
					break;
				}
			}
			// New Product — Add to Tracking Arrays
			if (!found && count < MAX_PRODUCTS) {
				names[count] = name;
				units[count] = qty;
				count++;
			}
		}

		file.close();

		// Bubble Sort Descending by Units Sold
		for (int i = 0; i < count - 1; i++) {
			for (int j = 0; j < count - i - 1; j++) {
				if (units[j] < units[j + 1]) {
					int    tmpU = units[j];
					units[j] = units[j + 1];
					units[j + 1] = tmpU;

					string tmpN = names[j];
					names[j] = names[j + 1];
					names[j + 1] = tmpN;
				}
			}
		}

		// Store Top MAX_REPORT_PRODUCTS
		for (int i = 0; i < MAX_REPORT_PRODUCTS; i++) {
			if (i < count) {
				topProductNames[i] = names[i];
				topProductUnits[i] = units[i];
			}
			else {
				topProductNames[i] = "";
				topProductUnits[i] = 0;
			}
		}
	}


public:
	// Default Const
	Report() {
		this->reportDate = "";
		this->totalRevenue = 0.0;
		this->totalTax = 0.0;
		this->totalDiscounts = 0.0;
		this->totalTransactions = 0;
		this->groceryRevenue = 0.0;
		this->perishRevenue = 0.0;
		this->electroRevenue = 0.0;
		for (int i = 0; i < MAX_REPORT_PRODUCTS; i++) {
			this->topProductNames[i] = "";
			this->topProductUnits[i] = 0;
		}
	}

	// Para Const, I Couldve Used Initializer List But I Wanted To Keep It Simple And Make it Less GPT-Like Warna Itiraz Kartay :P
	Report(string date, double totalRev, int totalTrans,
		double groceryRev, double perishRev, double electroRev,
		string topNames[], int topUnits[]) {
		this->reportDate = date;
		this->totalRevenue = totalRev;
		this->totalTransactions = totalTrans;
		this->totalTax = 0.0;
		this->totalDiscounts = 0.0;
		this->groceryRevenue = groceryRev;
		this->perishRevenue = perishRev;
		this->electroRevenue = electroRev;
		for (int i = 0; i < MAX_REPORT_PRODUCTS; i++) {
			this->topProductNames[i] = topNames[i];
			this->topProductUnits[i] = topUnits[i];
		}
	}

	// Getters And 1 Setter Used by AdminSalesReport GUI Screen
	double getTotalRevenue() {
		return totalRevenue;
	}

	double getTotalTax() {
		return totalTax;
	}

	double getTotalDiscounts() {
		return totalDiscounts;
	}

	int getTotalTransactions() {
		return totalTransactions;
	}

	double getGroceryRevenue() {
		return groceryRevenue;
	}

	double getPerishRevenue() {
		return perishRevenue;
	}

	double getElectroRevenue() {
		return electroRevenue;
	}

	string getTopProductName(int i) {
		if (i >= 0 && i < MAX_REPORT_PRODUCTS)
			return topProductNames[i];
		return "";
	}

	int getTopProductUnits(int i) {
		if (i >= 0 && i < MAX_REPORT_PRODUCTS)
			return topProductUnits[i];
		return 0;
	}

	void setReportDate(string date) {
		reportDate = date;
	}


	// Reads Sales Log Line by Line and Accumulates All Totals
	// Then Calls findTopProducts() For Top Sellers
	// Log Block Format (Written by FileManager::appendSalesLog):
	//   Receipt ID : RCP-0001
	//   Items:
	//     Milk x2  @  Rs.120  =  Rs.240
	//   Subtotal        : Rs.240
	//   Tax             : Rs.12
	//   Item Discounts  : Rs.24
	//   Coupon Discount : Rs.0
	//   Grand Total     : Rs.228
	void generateFromLog() {
		// Reset All Totals
		totalRevenue = 0.0;
		totalTax = 0.0;
		totalDiscounts = 0.0;
		totalTransactions = 0;
		groceryRevenue = 0.0;
		perishRevenue = 0.0;
		electroRevenue = 0.0;

		ifstream file(FILE_SALES_LOG);
		if (!file.is_open()) {
			cout << "ERROR! Could Not Open " << FILE_SALES_LOG <<endl;
			return;
		}

		string line;
		while (getline(file, line)) {
			if (lineContains(line, "Grand Total")) {
				totalRevenue += extractValue(line);
				totalTransactions++;
			} else if (lineContains(line, "Tax             :")) {
				totalTax += extractValue(line);
			} else if (lineContains(line, "Item Discounts  :")) {
				totalDiscounts += extractValue(line);
			} else if (lineContains(line, "Coupon Discount :")) {
				totalDiscounts += extractValue(line);
			}
		}

		file.close();
		findTopProducts();
	}

	// Console Output(Good For Testing)
	void displayOnScreen() {
		cout << REPORT_HEADER << endl;
		cout << "Date              : " << reportDate << endl;
		cout << REPORT_DIVIDER << endl;
		cout << "Total Transactions: " << totalTransactions << endl;
		cout << "Total Revenue     : " << CURRENCY_SYMBOL << totalRevenue << endl;
		cout << "Tax Collected     : " << CURRENCY_SYMBOL << totalTax << endl;
		cout << "Total Discounts   : " << CURRENCY_SYMBOL << totalDiscounts << endl;
		cout << REPORT_DIVIDER << endl;
		cout << "Grocery Revenue   : " << CURRENCY_SYMBOL << groceryRevenue << endl;
		cout << "Perishable Rev    : " << CURRENCY_SYMBOL << perishRevenue << endl;
		cout << "Electronic Rev    : " << CURRENCY_SYMBOL << electroRevenue << endl;
		cout << REPORT_DIVIDER << endl;
		cout << "Top Products:" << endl;
		for (int i = 0; i < MAX_REPORT_PRODUCTS; i++) {
			if (!(topProductNames[i] == "")) {
				cout << "  " << (i + 1) << ". "
					<< topProductNames[i]
					<< " — " << topProductUnits[i] << " units" << endl;
			}
		}
		cout << REPORT_HEADER << endl;
	}

	// Saves Report to data/reports/RPT-<date>.txt
	void exportToFile() {
		ofstream outFile(FILE_REPORTS_DIR + REPORT_ID_PREFIX + reportDate + ".txt");
		if (!outFile) {
			cout << "Error Creating Report File!" << endl;
			return;
		}

		outFile << REPORT_HEADER << endl;
		outFile << "Date              : " << reportDate << endl;
		outFile << REPORT_DIVIDER << endl;
		outFile << "Total Transactions: " << totalTransactions << endl;
		outFile << "Total Revenue     : " << CURRENCY_SYMBOL << totalRevenue << endl;
		outFile << "Tax Collected     : " << CURRENCY_SYMBOL << totalTax << endl;
		outFile << "Total Discounts   : " << CURRENCY_SYMBOL << totalDiscounts << endl;
		outFile << REPORT_DIVIDER << endl;
		outFile << "Grocery Revenue   : " << CURRENCY_SYMBOL << groceryRevenue << endl;
		outFile << "Perishable Rev    : " << CURRENCY_SYMBOL << perishRevenue << endl;
		outFile << "Electronic Rev    : " << CURRENCY_SYMBOL << electroRevenue << endl;
		outFile << REPORT_DIVIDER << endl;
		outFile << "Top Selling Products:" << endl;
		for (int i = 0; i < MAX_REPORT_PRODUCTS; i++) {
			if (!(topProductNames[i] == "")) {
				outFile << "  " << (i + 1) << ". "
					<< topProductNames[i]
					<< " — " << topProductUnits[i] << " units" << endl;
			}
		}

		outFile.close();
	}

};

#endif // REPORT_H