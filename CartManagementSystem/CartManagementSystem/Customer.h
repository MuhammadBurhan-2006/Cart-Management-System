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

