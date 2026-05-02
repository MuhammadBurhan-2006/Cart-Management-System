#ifndef REPORT_H
#define REPORT_H
#include <iostream>
#include <fstream>
#include <string>
#include "Globals.h"
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
	// Works Like line.find(word) — case-sensitive substring search
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
	// Checks if line STARTS WITH keyword (after optional leading spaces)
	// Use this for summary lines to avoid false matches
	bool lineStartsWith(const string& line, const string& keyword) {
		int pos = 0;
		// Skip leading spaces/tabs
		while (pos < (int)line.size() && (line[pos] == ' ' || line[pos] == '\t'))
			pos++;
		if ((int)line.size() - pos < (int)keyword.size()) return false;
		for (int j = 0; j < (int)keyword.size(); j++) {
			if (line[pos + j] != keyword[j]) return false;
		}
		return true;
	}
	// Trim trailing whitespace/carriage-return from string
	string trimRight(const string& s) {
		int end = (int)s.size() - 1;
		while (end >= 0 && (s[end] == ' ' || s[end] == '\t' || s[end] == '\r' || s[end] == '\n'))
			end--;
		return s.substr(0, end + 1);
	}
	double extractValue(const string& line) {
		int pos = 0;
		// Read Until ':'
		while (pos < (int)line.size() && line[pos] != ':')
			pos++;
		pos++; // Skip ':'
		// Skip spaces
		while (pos < (int)line.size() && line[pos] == ' ')
			pos++;
		while (pos < (int)line.size() &&
			line[pos] != '-' &&
			!(line[pos] >= '0' && line[pos] <= '9'))
			pos++;
		// Handle negative sign
		string numStr = "";
		if (pos < (int)line.size() && line[pos] == '-') {
			numStr += '-';
			pos++;
		}
		// Build number string (digits + one decimal point)
		while (pos < (int)line.size() &&
			(line[pos] == '.' || (line[pos] >= '0' && line[pos] <= '9'))) {
			numStr += line[pos];
			pos++;
		}
		if (numStr == "" || numStr == "-")
			return 0.0;

		return convDouble(numStr);
	}
	// Stod replacement
	double convDouble(const string& str) {
		double result = 0.0;
		int i = 0;
		bool negative = false;

		if (!(str == "") && str[i] == '-') {
			negative = true;
			i++;
		}

		for (; i < (int)str.size() && str[i] != '.'; i++) {
			if (str[i] < '0' || str[i] > '9') break;
			result = result * 10.0 + (str[i] - '0');
		}

		if (i < (int)str.size() && str[i] == '.') {
			i++;
			double fraction = 0.1;
			for (; i < (int)str.size(); i++) {
				if (str[i] < '0' || str[i] > '9') break;
				result += (str[i] - '0') * fraction;
				fraction *= 0.1;
			}
		}

		return negative ? -result : result;
	}
	// Stoi replacement
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
		return negative ? -result : result;
	}
	// Extracts product name from item line: "  Milk x2  @  Rs.120  =  Rs.240"
	string extractProductName(const string& line) {
		int start = 0;
		while (start < (int)line.size() && line[start] == ' ')
			start++;
		int end = start;
		while (end < (int)line.size() - 1) {
			if (line[end] == ' ' && line[end + 1] == 'x')
				break;
			end++;
		}
		if (end >= (int)line.size() - 1) return "";
		return trimRight(line.substr(start, end - start));
	}
	// Extracts qty from item line: digits right after 'x'
	int extractProductQty(const string& line) {
		int pos = 0;
		while (pos < (int)line.size() - 1) {
			if (line[pos] == ' ' && line[pos + 1] == 'x')
				break;
			pos++;
		}
		if (pos >= (int)line.size() - 1) return 0;
		pos += 2; // Skip ' ' and 'x'
		string qtyStr = "";
		while (pos < (int)line.size() &&
			line[pos] >= '0' && line[pos] <= '9') {
			qtyStr += line[pos];
			pos++;
		}
		if (qtyStr == "") return 0;
		return convInt(qtyStr);
	}
	double extractLineTotal(const string& line) {
		// Find last occurrence of CURRENCY_SYMBOL in the line
		string rs = CURRENCY_SYMBOL; // "Rs."
		int lastPos = -1;
		for (int i = 0; i <= (int)line.size() - (int)rs.size(); i++) {
			bool match = true;
			for (int j = 0; j < (int)rs.size(); j++) {
				if (line[i + j] != rs[j]) { match = false; break; }
			}
			if (match) lastPos = i;
		}
		if (lastPos == -1) return 0.0;
		int pos = lastPos + (int)rs.size(); // skip "Rs."
		string numStr = "";
		while (pos < (int)line.size() &&
			(line[pos] == '.' || (line[pos] >= '0' && line[pos] <= '9'))) {
			numStr += line[pos];
			pos++;
		}
		if (numStr == "") return 0.0;
		return convDouble(numStr);
	}
	// Reads sales log and finds top MAX_REPORT_PRODUCTS sellers
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
			if (lineStartsWith(line, "Subtotal")) {
				inItems = false;
				continue;
			}
			if (!inItems || line == "")
				continue;
			string name = extractProductName(line);
			int    qty = extractProductQty(line);
			if (name == "" || qty <= 0) continue;
			bool found = false;
			for (int i = 0; i < count; i++) {
				if (names[i] == name) {
					units[i] += qty;
					found = true;
					break;
				}
			}
			if (!found && count < MAX_PRODUCTS) {
				names[count] = name;
				units[count] = qty;
				count++;
			}
		}
		file.close();
		// Bubble sort descending
		for (int i = 0; i < count - 1; i++) {
			for (int j = 0; j < count - i - 1; j++) {
				if (units[j] < units[j + 1]) {
					int    tmpU = units[j]; units[j] = units[j + 1]; units[j + 1] = tmpU;
					string tmpN = names[j]; names[j] = names[j + 1]; names[j + 1] = tmpN;
				}
			}
		}
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
	string findCategoryForProduct(const string& productName) {
		ifstream fin(FILE_PRODUCTS);
		if (!fin.is_open()) return "";
		string line;
		while (getline(fin, line)) {
			if (line.empty() || line[0] == '#') continue;
			// Format: P|id|name|price|qty|...
			// Extract type (first char) and name (3rd field)
			string type = "";
			string name = "";
			int pos = 0;
			// type
			while (pos < (int)line.size() && line[pos] != '|')
				type += line[pos++];
			pos++; // skip '|'
			// id — skip
			while (pos < (int)line.size() && line[pos] != '|') pos++;
			pos++; // skip '|'
			// name
			while (pos < (int)line.size() && line[pos] != '|')
				name += line[pos++];
			if (name == productName) {
				fin.close();
				if (type == "P") return CAT_PERISHABLE;
				if (type == "E") return CAT_ELECTRONIC;
				if (type == "G") return CAT_GROCERY;
			}
		}
		fin.close();
		return "";
	}
public:
	// Default Constructor
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
	// Parameterized Constructor
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
	// Getters
	double getTotalRevenue() { return totalRevenue; }
	double getTotalTax() { return totalTax; }
	double getTotalDiscounts() { return totalDiscounts; }
	int    getTotalTransactions() { return totalTransactions; }
	double getGroceryRevenue() { return groceryRevenue; }
	double getPerishRevenue() { return perishRevenue; }
	double getElectroRevenue() { return electroRevenue; }
	string getTopProductName(int i) {
		if (i >= 0 && i < MAX_REPORT_PRODUCTS) return topProductNames[i];
		return "";
	}
	int getTopProductUnits(int i) {
		if (i >= 0 && i < MAX_REPORT_PRODUCTS) return topProductUnits[i];
		return 0;
	}
	void setReportDate(string date) { reportDate = date; }

	// ============================================================
	// Log block format (written by FileManager::appendSalesLog):
	//   -----------------------------------------
	//   Receipt ID : RCP-0001
	//   Customer   : Ali Ahmed
	//   Timestamp  : 2025-06-01 12:00:00
	//   Items:
	//     Fresh Milk 1L x2  @  Rs.150  =  Rs.300
	//   Subtotal        : Rs.300
	//   Tax             : Rs.15
	//   Item Discounts  : Rs.30
	//   Coupon Discount : Rs.0
	//   Grand Total     : Rs.285
	//   -----------------------------------------
	// ============================================================
	void generateFromLog() {
		// Reset all totals
		totalRevenue = 0.0;
		totalTax = 0.0;
		totalDiscounts = 0.0;
		totalTransactions = 0;
		groceryRevenue = 0.0;
		perishRevenue = 0.0;
		electroRevenue = 0.0;
		ifstream file(FILE_SALES_LOG);
		if (!file.is_open()) {
			cout << "ERROR! Could Not Open " << FILE_SALES_LOG << endl;
			return;
		}
		string line;
		bool inItems = false;
		while (getline(file, line)) {
			// Track when we're inside the Items block
			if (lineContains(line, "Items:")) {
				inItems = true;
				continue;
			}
			if (lineStartsWith(line, "Subtotal")) {
				inItems = false;
				// don't skip — fall through to parse if needed
			}
			// ---- Item lines → category revenue ----
			if (inItems && !line.empty() && line[0] == ' ') {
				string prodName = extractProductName(line);
				double lineTotal = extractLineTotal(line);
				if (prodName != "" && lineTotal > 0.0) {
					string cat = findCategoryForProduct(prodName);
					if (cat == CAT_GROCERY)         groceryRevenue += lineTotal;
					else if (cat == CAT_PERISHABLE) perishRevenue += lineTotal;
					else if (cat == CAT_ELECTRONIC) electroRevenue += lineTotal;
				}
				continue;
			}
			// ---- Summary lines — use lineStartsWith so spacing doesn't matter ----
			if (lineStartsWith(line, "Grand Total")) {
				totalRevenue += extractValue(line);
				totalTransactions++;
			}
			else if (lineStartsWith(line, "Tax")) {
				// Make sure it's the "Tax :" line, NOT "Total" or other Tax-containing words
				// lineStartsWith already ensures line begins with "Tax"
				totalTax += extractValue(line);
			}
			else if (lineStartsWith(line, "Item Discounts")) {
				totalDiscounts += extractValue(line);
			}
			else if (lineStartsWith(line, "Coupon Discount")) {
				totalDiscounts += extractValue(line);
			}
		}
		file.close();
		findTopProducts();
	}
	// Console output
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
	// Export to data/reports/RPT-<date>.txt
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
#endif