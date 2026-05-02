#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Globals.h"
#include "Products.h"
#include "Customer.h"
using namespace std;
class FileManager {
public:
    static int loadProducts(Product* products[], int maxSize) {
        ifstream fin(FILE_PRODUCTS);
        if (!fin.is_open()) {
            cout << "[FileManager] Warning: Cannot open " << FILE_PRODUCTS
                << " — starting with empty product list." << endl;
            return 0;
        }
        int count = 0;
        string line;
        while (getline(fin, line) && count < maxSize) {
            if (line.empty() || line[0] == '#') continue; // skip blank/comment lines
            stringstream ss(line);
            string type, tok, name;
            int    id, qty;
            double price;
            getline(ss, type, '|');
            getline(ss, tok, '|'); id = stoi(tok);
            getline(ss, name, '|');
            getline(ss, tok, '|'); price = stod(tok);
            getline(ss, tok, '|'); qty = stoi(tok);
            if (type == "P") {
                string expiry;
                getline(ss, expiry, '|');
                products[count++] = new PerishableProduct(id, name, price, qty, expiry);
            }
            else if (type == "E") {
                string brand, tok2;
                int    warranty;
                getline(ss, brand, '|');
                getline(ss, tok2, '|'); warranty = stoi(tok2);
                products[count++] = new ElectronicProduct(id, name, price, qty, brand, warranty);
            }
            else if (type == "G") {
                string tok2;
                double weight;
                getline(ss, tok2, '|'); weight = stod(tok2);
                products[count++] = new GroceryProduct(id, name, price, qty, weight);
            }
        }
        fin.close();
        cout << "[FileManager] Loaded " << count << " products from file." << endl;
        return count;
    }
    // Saves entire product array to file (overwrites previous data).
    // Call after any add / edit / delete / stock change.
    static void saveProducts(Product* products[], int count) {
        ofstream fout(FILE_PRODUCTS);
        if (!fout.is_open()) {
            cout << "[FileManager] Error: Cannot write to " << FILE_PRODUCTS << endl;
            return;
        }
        for (int i = 0; i < count; i++) {
            Product* p = products[i];
            string   cat = p->getCategory();

            if (cat == CAT_PERISHABLE) {
                PerishableProduct* pp = dynamic_cast<PerishableProduct*>(p);
                fout << "P|" << p->getProductID() << "|" << p->getName() << "|"
                    << p->getPrice() << "|" << p->getStockQty() << "|"
                    << (pp ? pp->getExpiryDate() : "") << "|\n";
            }
            else if (cat == CAT_ELECTRONIC) {
                ElectronicProduct* ep = dynamic_cast<ElectronicProduct*>(p);
                fout << "E|" << p->getProductID() << "|" << p->getName() << "|"
                    << p->getPrice() << "|" << p->getStockQty() << "|"
                    << (ep ? ep->getBrand() : "") << "|"
                    << (ep ? ep->getWarrantyMonths() : 0) << "|\n";
            }
            else if (cat == CAT_GROCERY) {
                GroceryProduct* gp = dynamic_cast<GroceryProduct*>(p);
                fout << "G|" << p->getProductID() << "|" << p->getName() << "|"
                    << p->getPrice() << "|" << p->getStockQty() << "|"
                    << (gp ? gp->getWeightKg() : 0.0) << "|\n";
            }
        }
        fout.close();
        cout << "[FileManager] Saved " << count << " products to file." << endl;
    }
    // Decrements stock for one product by ID, then re-saves whole file.
    // Called after successful checkout.
    static void decrementStock(Product* products[], int count, int productID, int qtySold) {
        for (int i = 0; i < count; i++) {
            if (products[i]->getProductID() == productID) {
                int newQty = products[i]->getStockQty() - qtySold;
                products[i]->setStockQty(newQty < 0 ? 0 : newQty);
                break;
            }
        }
        saveProducts(products, count);
    }
    static int loadCustomers(Customer customers[], int maxSize) {
        ifstream fin(FILE_CUSTOMERS);
        if (!fin.is_open()) {
            cout << "[FileManager] Warning: Cannot open " << FILE_CUSTOMERS
                << " — no existing customers." << endl;
            return 0;
        }
        int    count = 0;
        string line;
        while (getline(fin, line) && count < maxSize) {
            if (line.empty()) continue;
            stringstream ss(line);
            string tok, name, username, password, phone;
            int    id;
            getline(ss, tok, '|'); id = stoi(tok);
            getline(ss, name, '|');
            getline(ss, username, '|');
            getline(ss, password, '|');
            getline(ss, phone, '|');
            customers[count++] = Customer(id, name, username, password, phone);
        }
        fin.close();
        return count;
    }
    // Appends one new customer to the file.
    // Called from registration screen.
    static void saveNewCustomer(const Customer& c, const string& password) {
        ofstream fout(FILE_CUSTOMERS, ios::app);
        if (!fout.is_open()) {
            cout << "[FileManager] Error: Cannot write to " << FILE_CUSTOMERS << endl;
            return;
        }
        fout << c.getCustomerID() << "|"
            << c.getName() << "|"
            << c.getUsername() << "|"
            << password << "|"
            << c.getPhone() << "|\n";
        fout.close();
    }
    // Validates customer login against file. Returns index in array or -1.
    static int validateCustomerLogin(Customer customers[], int count,
        const string& username, const string& password) {
        for (int i = 0; i < count; i++) {
            if (customers[i].login(username, password)) return i;
        }
        return -1;
    }
    static bool validateAdminLogin(const string& username, const string& password) {
        // Always allow hardcoded default from Globals.h
        if (username == DEFAULT_ADMIN_USER && password == DEFAULT_ADMIN_PASS)
            return true;
        ifstream fin(FILE_ADMINS);
        if (!fin.is_open()) return false;
        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string tok, name, user, pass;
            getline(ss, tok, '|'); // id — unused here
            getline(ss, name, '|');
            getline(ss, user, '|');
            getline(ss, pass, '|');
            if (user == username && pass == password) {
                fin.close();
                return true;
            }
        }
        fin.close();
        return false;
    }
    static void appendSalesLog(const string& receiptID,
        const string& customerName,
        Cart& cart,
        double        subtotal,
        double        tax,
        double        discount,
        double        couponDiscount,
        double        grandTotal,
        const string& timestamp)
    {
        ofstream fout(FILE_SALES_LOG, ios::app);
        if (!fout.is_open()) {
            cout << "[FileManager] Error: Cannot write to " << FILE_SALES_LOG << endl;
            return;
        }
        fout << RECEIPT_DIVIDER << endl;
        fout << "Receipt ID : " << receiptID << endl;
        fout << "Customer   : " << customerName << endl;
        fout << "Timestamp  : " << timestamp << endl;
        fout << "Items:\n";
        for (int i = 0; i < cart.getItemCount(); i++) {
            CartItem item = cart.getItem(i);
            double lineTotal = item.product->getPrice() * item.quantity;
            fout << "  " << item.product->getName()
                << " x" << item.quantity
                << "  @  " << CURRENCY_SYMBOL << item.product->getPrice()
                << "  =  " << CURRENCY_SYMBOL << lineTotal << endl;
        }
        fout << "Subtotal        : " << CURRENCY_SYMBOL << subtotal << endl;
        fout << "Tax             : " << CURRENCY_SYMBOL << tax << endl;
        fout << "Item Discounts  : " << CURRENCY_SYMBOL << discount << endl;
        fout << "Coupon Discount : " << CURRENCY_SYMBOL << couponDiscount << endl;
        fout << "Grand Total     : " << CURRENCY_SYMBOL << grandTotal << endl;
        fout << RECEIPT_DIVIDER << endl;
        fout.close();
        cout << "[FileManager] Sale logged: " << receiptID << endl;
    }
    static void saveReceipt(Bill& bill,
        Cart& cart,
        const string& customerName,
        const string& receiptID,
        const string& timestamp)
    {
        string path = FILE_RECEIPTS_DIR + receiptID + ".txt";
        ofstream fout(path);
        if (!fout.is_open()) {
            cout << "[FileManager] Error: Cannot create receipt at " << path
                << "\n  Make sure the data/receipts/ folder exists." << endl;
            return;
        }
        fout << RECEIPT_HEADER << endl;
        fout << "Receipt ID  : " << receiptID << endl;
        fout << "Customer    : " << customerName << endl;
        fout << "Date / Time : " << timestamp << endl;
        fout << RECEIPT_DIVIDER << endl;
        fout << "  #  Name                     Qty    Price         Total\n";
        fout << RECEIPT_DIVIDER << endl;
        for (int i = 0; i < cart.getItemCount(); i++) {
            CartItem item = cart.getItem(i);
            double   lineTotal = item.product->getPrice() * item.quantity;
            fout << "  " << (i + 1) << "  "
                << item.product->getName() << "\t\t"
                << item.quantity << "\t"
                << CURRENCY_SYMBOL << item.product->getPrice() << "\t"
                << CURRENCY_SYMBOL << lineTotal << endl;
        }
        fout << RECEIPT_DIVIDER << endl;
        fout << "Subtotal        : " << CURRENCY_SYMBOL << bill.getSubtotal() << endl;
        fout << "Tax             : " << CURRENCY_SYMBOL << bill.getTotalTax() << endl;
        fout << "Item Discounts  : " << CURRENCY_SYMBOL << bill.getTotalDiscount() << endl;
        if (bill.isCouponApplied()) {
            fout << "Coupon (" << bill.getCouponCode() << ")   : -"
                << CURRENCY_SYMBOL << bill.getCouponDiscount() << endl;
        }
        fout << RECEIPT_DIVIDER << endl;
        fout << "GRAND TOTAL     : " << CURRENCY_SYMBOL << bill.getGrandTotal() << endl;
        fout << RECEIPT_FOOTER << endl;
        fout.close();
        cout << "[FileManager] Receipt saved: " << path << endl;
    }

    static void appendRefundLog(const string& originalReceiptID,
        const string& customerName,
        double        refundAmount,
        double        restockingFee,
        const string& timestamp)
    {
        ofstream fout(FILE_REFUNDS_LOG, ios::app);
        if (!fout.is_open()) {
            cout << "[FileManager] Error: Cannot write to " << FILE_REFUNDS_LOG << endl;
            return;
        }
        fout << RECEIPT_DIVIDER << endl;
        fout << "Original Receipt : " << originalReceiptID << endl;
        fout << "Customer         : " << customerName << endl;
        fout << "Timestamp        : " << timestamp << endl;
        fout << "Restocking Fee   : " << CURRENCY_SYMBOL << restockingFee << endl;
        fout << "Refund Amount    : " << CURRENCY_SYMBOL << refundAmount << endl;
        fout << RECEIPT_DIVIDER << "\n\n";
        fout.close();
        cout << "[FileManager] Refund logged for receipt: " << originalReceiptID << endl;
    }
    static void generateSalesReport(const string& reportID, const string& timestamp) {
        ifstream fin(FILE_SALES_LOG);
        if (!fin.is_open()) {
            cout << "[FileManager] Error: Cannot read " << FILE_SALES_LOG << endl;
            return;
        }
        int    transactions = 0;
        double totalRevenue = 0.0;
        string line;
        while (getline(fin, line)) {
            if (line.find("Grand Total") != string::npos) {
                size_t pos = line.rfind(CURRENCY_SYMBOL);
                if (pos != string::npos) {
                    string val = line.substr(pos + CURRENCY_SYMBOL.length());
                    try { totalRevenue += stod(val); }
                    catch (...) {}
                    transactions++;
                }
            }
        }
        fin.close();

        string   path = FILE_REPORTS_DIR + reportID + ".txt";
        ofstream fout(path);
        if (!fout.is_open()) {
            cout << "[FileManager] Error: Cannot create report at " << path
                << "\n  Make sure the data/reports/ folder exists." << endl;
            return;
        }
        fout << REPORT_HEADER << endl;
        fout << "Report ID          : " << reportID << endl;
        fout << "Generated On       : " << timestamp << endl;
        fout << REPORT_DIVIDER << endl;
        fout << "Total Transactions : " << transactions << endl;
        fout << "Total Revenue      : " << CURRENCY_SYMBOL << totalRevenue << endl;
        fout << REPORT_DIVIDER << endl;
        fout << "(See sales_log.txt for full per-transaction detail)\n";
        fout.close();
        cout << "[FileManager] Report saved: " << path << endl;
    }
    static string generateReceiptID(int counter) {
        string n = to_string(counter);
        while (n.length() < 4) n = "0" + n;
        return RECEIPT_ID_PREFIX + n;
    }
    // Generate report ID like "RPT-0001"
    static string generateReportID(int counter) {
        string n = to_string(counter);
        while (n.length() < 4) n = "0" + n;
        return REPORT_ID_PREFIX + n;
    }
    static void initDataFolders() {
#ifdef _WIN32
        // Windows
        system("if not exist data mkdir data");
        system("if not exist data\\receipts mkdir data\\receipts");
        system("if not exist data\\reports  mkdir data\\reports");
#else
        // Linux / Mac
        system("mkdir -p data/receipts");
        system("mkdir -p data/reports");
#endif
        cout << "[FileManager] Data folders ready." << endl;
    }
    // Counts existing receipts in sales log to generate non-duplicate receipt IDs
    static int getNextReceiptCounter() {
        ifstream fin(FILE_SALES_LOG);
        if (!fin.is_open()) return 1;
        int count = 0;
        string line;
        while (getline(fin, line)) {
            if (line.find("Receipt ID") != string::npos)
                count++;
        }
        fin.close();
        return count + 1;
    }
};
#endif