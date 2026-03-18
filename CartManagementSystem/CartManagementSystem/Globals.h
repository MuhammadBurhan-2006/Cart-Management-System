#ifndef GLOBAL_H
#define GLOBALS_H
#include<string>
using namespace std;
//File paths for the data handling basically would be used by Abdul Rehman
const string FILE_PRODUCTS = "data/products.txt";
const string FILE_CUSTOMERS = "data/customers.txt";
const string FILE_ADMINS = "data/admins.txt";
const string FILE_SALES_LOG = "data/sales_log.txt";
const string FILE_RECEIPTS_DIR = "data/receipts/";
const string FILE_REPORTS_DIR = "data/reports/";
const string FILE_REFUNDS_LOG = "data/refunds.txt";
//Array size limits for the system
const int MAX_PRODUCTS = 100;
const int MAX_CUSTOMERS = 200;
const int MAX_CART_ITEMS = 50;
const int MAX_RECEIPT_ITEMS = 50;
const int MAX_REPORT_PRODUCTS = 5;
const int MAX_REFUND_ITEMS = 50;
const int MAX_ADMINS = 10;
const int MAX_NAME_LENGTH = 50;
const int MAX_ID_LENGTH = 20;
//Globals for product categories mainly used by Burhan and Areeba
const string CAT_PERISHABLE = "Perishable";
const string CAT_ELECTRONIC = "Electronic";
const string CAT_GROCERY = "Grocery";
//Tax rates declaration used by Burhan if any rate changes we can easily update here
const double TAX_PERISHABLE = 0.05;
const double TAX_ELECTRONIC = 0.17;
const double TAX_GROCERY = 0.00;
//Discount rates declaration used by Burhan if any rate changes we can easily update here
const double DISC_PERISHABLE = 0.10;
const double DISC_ELECTRONIC = 0.05;
const double DISC_GROCERY = 0.08;
//Coupon codes declaration used by Areeba
const string COUPON_1_CODE = "SAVE10";
const double COUPON_1_DISC = 0.10;
const string COUPON_2_CODE = "FLAT50";
const double COUPON_2_DISC = 50.00;
const string COUPON_3_CODE = "FRESH20";
const double COUPON_3_DISC = 0.20;
#endif