#ifndef FILEMANAGER_H
#define FILEMANAGER_H

// ============================================================
//  FileManager.h  �  Abdul Rehman (25L-2074)
//  Handles ALL file I/O for the Supermarket Billing System.
//
//  OPTIONAL INCLUDE � main.cpp works WITHOUT this file.
//  Add #include "FileManager.h" only when you want persistence.
//
//  Depends on: Globals.h, Products.h, Customer.h
//  Data folder structure required:
//      data/
//      data/receipts/
//      data/reports/
// ============================================================

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

    // ============================================================
    //  SECTION 1 � PRODUCT FILE  (data/products.txt)
    //
    //  File format (one product per line):
    //      P|id|name|price|qty|expiryDate
    //      E|id|name|price|qty|brand|warrantyMonths
    //      G|id|name|price|qty|weightKg
    // ============================================================

    // Loads all products from file into caller's array.
    // Returns number of products loaded.
    // Usage: int n = FileManager::loadProducts(arr, MAX_PRODUCTS);
    static int loadProducts(Product* products[], int maxSize) {
        ifstream fin(FILE_PRODUCTS);
        if (!fin.is_open()) {
            cout << "[FileManager] Warning: Cannot open " << FILE_PRODUCTS
                << " � starting with empty product list." << endl;
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


    // ============================================================
    //  SECTION 2 � CUSTOMER FILE  (data/customers.txt)
    //
    //  File format:
    //      id|name|username|password|phone
    // ============================================================

    // Loads all customers. Returns count.
    static int loadCustomers(Customer customers[], int maxSize) {
        ifstream fin(FILE_CUSTOMERS);
        if (!fin.is_open()) {
            cout << "[FileManager] Warning: Cannot open " << FILE_CUSTOMERS
                << " � no existing customers." << endl;
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
    // Called from: registration screen.
    // NOTE: Customer::password is private � add getPassword() to Customer if needed.
    //       For now we write a placeholder; replace with c.getPassword() once getter exists.
    static void saveNewCustomer(const Customer& c, const string& password) {
        ofstream fout(FILE_CUSTOMERS, ios::app);
        if (!fout.is_open()) {
            cout << "[FileManager] Error: Cannot write to " << FILE_CUSTOMERS << endl;
            return;
        }
        fout << c.getCustomerID() << "|"
            << c.getName() << "|"
            << c.getUsername() << "|"
            << password << "|"   // pass raw password here from registration form
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


    // ============================================================
    //  SECTION 3 � ADMIN LOGIN  (data/admins.txt)
    //
    //  File format:
    //      id|name|username|password
    //
    //  Falls back to DEFAULT_ADMIN_USER / DEFAULT_ADMIN_PASS from Globals.h
    //  if file does not exist yet.
    // ============================================================

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
            getline(ss, tok, '|'); // id � unused here
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


    // ============================================================
    //  SECTION 4 � SALES LOG  (data/sales_log.txt)
    //  Appends one transaction block per checkout.
    // ============================================================

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
        fout << RECEIPT_DIVIDER << "\n";
        fout << "Receipt ID : " << receiptID << "\n";
        fout << "Customer   : " << customerName << "\n";
        fout << "Timestamp  : " << timestamp << "\n";
        fout << "Items:\n";
        for (int i = 0; i < cart.getItemCount(); i++) {
            CartItem item = cart.getItem(i);
            double lineTotal = item.product->getPrice() * item.quantity;
            fout << "  " << item.product->getName()
                << " x" << item.quantity
                << "  @  " << CURRENCY_SYMBOL << item.product->getPrice()
                << "  =  " << CURRENCY_SYMBOL << lineTotal << "\n";
        }
        fout << "Subtotal        : " << CURRENCY_SYMBOL << subtotal << "\n";
        fout << "Tax             : " << CURRENCY_SYMBOL << tax << "\n";
        fout << "Item Discounts  : " << CURRENCY_SYMBOL << discount << "\n";
        fout << "Coupon Discount : " << CURRENCY_SYMBOL << couponDiscount << "\n";
        fout << "Grand Total     : " << CURRENCY_SYMBOL << grandTotal << "\n";
        fout << RECEIPT_DIVIDER << "\n\n";
        fout.close();
        cout << "[FileManager] Sale logged: " << receiptID << endl;
    }


    // ============================================================
    //  SECTION 5 � RECEIPT FILE  (data/receipts/RCP-XXXX.txt)
    //  One .txt file per transaction.
    // ============================================================

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
        fout << RECEIPT_HEADER << "\n";
        fout << "Receipt ID  : " << receiptID << "\n";
        fout << "Customer    : " << customerName << "\n";
        fout << "Date / Time : " << timestamp << "\n";
        fout << RECEIPT_DIVIDER << "\n";
        fout << "  #  Name                     Qty    Price         Total\n";
        fout << RECEIPT_DIVIDER << "\n";
        for (int i = 0; i < cart.getItemCount(); i++) {
            CartItem item = cart.getItem(i);
            double   lineTotal = item.product->getPrice() * item.quantity;
            fout << "  " << (i + 1) << "  "
                << item.product->getName() << "\t\t"
                << item.quantity << "\t"
                << CURRENCY_SYMBOL << item.product->getPrice() << "\t"
                << CURRENCY_SYMBOL << lineTotal << "\n";
        }
        fout << RECEIPT_DIVIDER << "\n";
        fout << "Subtotal        : " << CURRENCY_SYMBOL << bill.getSubtotal() << "\n";
        fout << "Tax             : " << CURRENCY_SYMBOL << bill.getTotalTax() << "\n";
        fout << "Item Discounts  : " << CURRENCY_SYMBOL << bill.getTotalDiscount() << "\n";
        if (bill.isCouponApplied()) {
            fout << "Coupon (" << bill.getCouponCode() << ")   : -"
                << CURRENCY_SYMBOL << bill.getCouponDiscount() << "\n";
        }
        fout << RECEIPT_DIVIDER << "\n";
        fout << "GRAND TOTAL     : " << CURRENCY_SYMBOL << bill.getGrandTotal() << "\n";
        fout << RECEIPT_FOOTER << "\n";
        fout.close();
        cout << "[FileManager] Receipt saved: " << path << endl;
    }


    // ============================================================
    //  SECTION 6 � REFUND LOG  (data/refunds.txt)
    // ============================================================

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
        fout << RECEIPT_DIVIDER << "\n";
        fout << "Original Receipt : " << originalReceiptID << "\n";
        fout << "Customer         : " << customerName << "\n";
        fout << "Timestamp        : " << timestamp << "\n";
        fout << "Restocking Fee   : " << CURRENCY_SYMBOL << restockingFee << "\n";
        fout << "Refund Amount    : " << CURRENCY_SYMBOL << refundAmount << "\n";
        fout << RECEIPT_DIVIDER << "\n\n";
        fout.close();
        cout << "[FileManager] Refund logged for receipt: " << originalReceiptID << endl;
    }


    // ============================================================
    //  SECTION 7 � SALES REPORT  (data/reports/RPT-XXXX.txt)
    //  Reads sales_log.txt and writes a summary.
    // ============================================================

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
        fout << REPORT_HEADER << "\n";
        fout << "Report ID          : " << reportID << "\n";
        fout << "Generated On       : " << timestamp << "\n";
        fout << REPORT_DIVIDER << "\n";
        fout << "Total Transactions : " << transactions << "\n";
        fout << "Total Revenue      : " << CURRENCY_SYMBOL << totalRevenue << "\n";
        fout << REPORT_DIVIDER << "\n";
        fout << "(See sales_log.txt for full per-transaction detail)\n";
        fout.close();
        cout << "[FileManager] Report saved: " << path << endl;
    }


    // ============================================================
    //  SECTION 8 � UTILITY HELPERS
    // ============================================================

    // Generate receipt ID like "RCP-0042"
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

    // Creates the required data folders if they don't exist.
    // Call once at program startup.
    // Works on Windows (Visual Studio / MSVC environment).
    static void initDataFolders() {
        system("if not exist data mkdir data");
        system("if not exist data\\receipts mkdir data\\receipts");
        system("if not exist data\\reports  mkdir data\\reports");
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
#endif // FILEMANAGER_H