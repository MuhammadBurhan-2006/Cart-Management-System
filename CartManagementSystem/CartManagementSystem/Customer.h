#ifndef CUSTOMER_MODULE_H
#define CUSTOMER_MODULE_H

#include <iostream>
#include <string>
#include "Globals.h"    
#include "Products.h"//for Product base class and all subclasses (Perishable, Electronic, Grocery)
using namespace std;


//handles customer login, registration, and identity.
//uses Globals.h: ROLE_CUSTOMER, MAX_NAME_LENGTH, MAX_ID_LENGTH
class Customer {
private:
    int    customerID;
    string name;
    string username;
    string password;
    string phone;

public:

    Customer() {//default constructor
        customerID = 0;
        name = "";
        username = "";
        password = "";
        phone = "";
    }


    Customer(int id, string n, string user, string pass, string ph) {//Parameterized constructor
        customerID = id;
        name = n;
        username = user;
        password = pass;
        phone = ph;
    }

    //Getters
    int    getCustomerID() { return customerID; }
    string getName() { return name; }
    string getUsername() { return username; }
    string getPhone() { return phone; }

    //Setters
    void setCustomerID(int id) { customerID = id; }
    void setName(string n) { name = n; }
    void setUsername(string user) { username = user; }
    void setPassword(string pass) { password = pass; }
    void setPhone(string ph) { phone = ph; }

    //Validates username and password for login
    //Used by: Customer Login Screen
    bool login(string user, string pass) {
        return (username == user && password == pass);
    }

    //Displays basic customer info
    void display() {
        cout << "Customer ID : " << customerID << endl;
        cout << "Name        : " << name << endl;
        cout << "Username    : " << username << endl;
        cout << "Phone       : " << phone << endl;
    }


};

//Holds a pointer to  Product and the quantity selected
//Uses Products.h: Product*
struct CartItem {
    Product* product;//Pointer to any Product subclass
    int  quantity;

    CartItem() {
        product = nullptr;
        quantity = 0;
    }
};



//Manages all items the customer wants to purchase
//Uses Globals.h : MAX_CART_ITEMS, CURRENCY_SYMBOL
//Uses Products.h: Product* 
class Cart {
private:
    CartItem items[MAX_CART_ITEMS];//fixed-size array from Globals.h
    int  itemCount;//tracks how many items are currently in cart

public:
    Cart() {
        itemCount = 0;
    }

    //Getters
    int      getItemCount() { return itemCount; }
    CartItem getItem(int i) { return items[i]; }

    //adds a product to cart; if it already exists, increases quantity
    //called from: Browse products screen when customer clicks "Add to Cart"
    void addItem(Product* p, int qty) {
        for (int i = 0; i < itemCount; i++) {//check 
            if (items[i].product->getProductID() == p->getProductID()) {
                items[i].quantity += qty;
                return;
            }
        }
        //add new entry if space available
        if (itemCount < MAX_CART_ITEMS) {
            items[itemCount].product = p;
            items[itemCount].quantity = qty;
            itemCount++;
        }
        else {
            cout << "Cart is full! Max " << MAX_CART_ITEMS << " items allowed." << endl;
        }
    }

    //removes a product by its ID
    //called from: Live Cart screen when customer clicks "Remove Item"
    void removeItem(int productID) {
        for (int i = 0; i < itemCount; i++) {
            if (items[i].product->getProductID() == productID) {
                //shift remaining items left
                for (int j = i; j < itemCount - 1; j++) {
                    items[j] = items[j + 1];
                }
                itemCount--;
                return;
            }
        }
        cout << "Product not found in cart." << endl;
    }

    //updates the quantity of an existing cart item
    //called from: Live Cart screen
    void updateQty(int productID, int newQty) {
        for (int i = 0; i < itemCount; i++) {
            if (items[i].product->getProductID() == productID) {
                if (newQty <= 0) {
                    removeItem(productID);
                }
                else {
                    items[i].quantity = newQty;
                }
                return;
            }
        }
        cout << "Product not found in cart." << endl;
    }

    //calculates subtotal BEFORE tax/discount
    //uses: Product::getPrice() 
    double getSubtotal() {
        double total = 0.0;
        for (int i = 0; i < itemCount; i++) {
            total += items[i].product->getPrice() * items[i].quantity;
        }
        return total;
    }

    //calculates total TAX across all items
    //uses: Product::calculateTax() — overridden in each subclass
    double getTotalTax() {
        double tax = 0.0;
        for (int i = 0; i < itemCount; i++) {
            tax += items[i].product->calculateTax() * items[i].quantity;
        }
        return tax;
    }

    //calculates total DISCOUNT across all items
    //uses: Product::applyDiscount() — overridden in each subclass
    double getTotalDiscount() {
        double disc = 0.0;
        for (int i = 0; i < itemCount; i++) {
            disc += items[i].product->applyDiscount() * items[i].quantity;
        }
        return disc;
    }

    //empties the cart after checkout
    void clearCart() {
        itemCount = 0;
    }

    // Uses: CURRENCY_SYMBOL from Globals.h
    void displayCart() {
        cout << RECEIPT_DIVIDER << endl; //from Globals.h
        cout << "  #  Name                  Qty    Price        Total" << endl;
        cout << RECEIPT_DIVIDER << endl;
        for (int i = 0; i < itemCount; i++) {
            double lineTotal = items[i].product->getPrice() * items[i].quantity;
            cout << "  " << (i + 1) << "  "
                << items[i].product->getName() << "\t\t"
                << items[i].quantity << "\t"
                << CURRENCY_SYMBOL << items[i].product->getPrice() << "\t"
                << CURRENCY_SYMBOL << lineTotal << endl;
        }
        cout << RECEIPT_DIVIDER << endl;
        cout << "Subtotal  : " << CURRENCY_SYMBOL << getSubtotal() << endl;
        cout << "Tax       : " << CURRENCY_SYMBOL << getTotalTax() << endl;
        cout << "Discount  : " << CURRENCY_SYMBOL << getTotalDiscount() << endl;
        cout << RECEIPT_DIVIDER << endl;
    }
};


//handles final price calculation, coupon application, and prepares totals for receipt generation 
//uses Globals.h:
//  COUPON_1_CODE, COUPON_1_DISC  ("SAVE10" ? 10%)
//  COUPON_2_CODE, COUPON_2_DISC  ("FLAT50" ? Rs.50 flat off)
//  COUPON_3_CODE, COUPON_3_DISC  ("FRESH20" ? 20%)
//  CURRENCY_SYMBOL
//uses Products.h: indirectly via Cart 
class Bill {
private:
    Cart* cart; //pointer to the customer's cart
    Customer* customer;//pointer to the logged-in customer
    double    subtotal;
    double    totalTax;
    double    totalDiscount;
    double    couponDiscount;
    double    grandTotal;
    string    couponCode; 
    bool      couponApplied;

public:
    Bill(Cart* c, Customer* cust) {
        cart = c;
        customer = cust;
        subtotal = 0.0;
        totalTax = 0.0;
        totalDiscount = 0.0;
        couponDiscount = 0.0;
        grandTotal = 0.0;
        couponCode = "";
        couponApplied = false;
    }

    //Getters
    double getSubtotal() { return subtotal; }
    double getTotalTax() { return totalTax; }
    double getTotalDiscount() { return totalDiscount; }
    double getCouponDiscount() { return couponDiscount; }
    double getGrandTotal() { return grandTotal; }
    string getCouponCode() { return couponCode; }
    bool   isCouponApplied() { return couponApplied; }

    //validates and applies a coupon code to the bill
    //coupon codes and values come from Globals.h
    //called from: Checkout screen when customer clicks "Apply"
    void applyCoupon(string code) {
        if (couponApplied) {
            cout << "A coupon has already been applied." << endl;
            return;
        }
        if (code == COUPON_1_CODE) {// "SAVE10" ? 10% off
            couponCode = code;
            couponDiscount = subtotal * COUPON_1_DISC;
            couponApplied = true;
            cout << "Coupon applied: 10% off!" << endl;
        }
        else if (code == COUPON_2_CODE) {// "FLAT50" ? Rs.50 flat off
            couponCode = code;
            couponDiscount = COUPON_2_DISC;
            couponApplied = true;
            cout << "Coupon applied: Flat Rs.50 off!" << endl;
        }
        else if (code == COUPON_3_CODE) {// "FRESH20" ? 20% off
            couponCode = code;
            couponDiscount = subtotal * COUPON_3_DISC;
            couponApplied = true;
            cout << "Coupon applied: 20% off!" << endl;
        }
        else {
            cout << "Invalid coupon code." << endl;
        }
    }

    //core billing calculation 
    //call this BEFORE displayBillSummary() or passing to FileManager
    double calculateGrandTotal() {
        subtotal = cart->getSubtotal();
        totalTax = cart->getTotalTax();
        totalDiscount = cart->getTotalDiscount();
        grandTotal = subtotal + totalTax - totalDiscount - couponDiscount;
        if (grandTotal < 0) grandTotal = 0; // check
        return grandTotal;
    }

    //prints final bill summary to screen (Checkout screen)
    //uses: CURRENCY_SYMBOL, RECEIPT_DIVIDER, RECEIPT_HEADER, RECEIPT_FOOTER from Globals.h
    void displayBillSummary() {
        cout << RECEIPT_HEADER << endl;
        cout << "Customer: " << customer->getName() << endl;
        cout << RECEIPT_DIVIDER << endl;
        cart->displayCart();
        cout << RECEIPT_DIVIDER << endl;
        if (couponApplied) {
            cout << "Coupon (" << couponCode << "): -"
                << CURRENCY_SYMBOL << couponDiscount << endl;
        }
        cout << "GRAND TOTAL : " << CURRENCY_SYMBOL << grandTotal << endl;
        cout << RECEIPT_FOOTER << endl;
    }
};
