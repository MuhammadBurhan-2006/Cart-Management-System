#include <iostream>
#include "Globals.h"
#include "Products.h"
#include "Customer.h"
// ---------------------------------------------------------------
//  FileManager is OPTIONAL.
//  Comment the next two lines out and main still compiles fine.
//  Un-comment them to enable file persistence (save/load/receipt).
#include "FileManager.h"
#define USE_FILE_MANAGER   // comment this out to skip FM demo
// ---------------------------------------------------------------
using namespace std;

int main() {
    cout << "========================================" << endl;
    cout << "  CUSTOMER MODULE TEST PROGRAM" << endl;
    cout << "  Areeba Akbar (25L-2047)" << endl;
    cout << "========================================" << endl << endl;

    // ===== TEST 1: Create Customer =====
    cout << "TEST 1: Creating Customer..." << endl;
    cout << "-----------------------------------" << endl;
    Customer customer(101, "Ali Ahmed", "ali123", "password123", "03001234567");
    customer.display();
    cout << endl;

    // ===== TEST 2: Test Login =====
    cout << "TEST 2: Testing Login..." << endl;
    cout << "-----------------------------------" << endl;
    cout << "Attempting login with correct credentials..." << endl;
    if (customer.login("ali123", "password123"))
        cout << "Login SUCCESSFUL!" << endl;
    else
        cout << "Login FAILED!" << endl;

    cout << "\nAttempting login with wrong password..." << endl;
    if (customer.login("ali123", "wrongpass"))
        cout << "Login SUCCESSFUL!" << endl;
    else
        cout << "Login FAILED (as expected)!" << endl;
    cout << endl;

    // ===== TEST 3: Create Products =====
    cout << "TEST 3: Creating Products..." << endl;
    cout << "-----------------------------------" << endl;
    Product* milk = new PerishableProduct(1, "Fresh Milk 1L", 150.0, 50, "2025-06-30");
    Product* rice = new GroceryProduct(2, "Basmati Rice 5kg", 800.0, 100, 5.0);
    Product* phone = new ElectronicProduct(3, "Samsung Phone", 45000.0, 15, "Samsung", 12);
    Product* apple = new PerishableProduct(4, "Red Apple", 50.0, 200, "2025-05-15");

    cout << "Products created successfully!" << endl;
    milk->display();
    rice->display();
    phone->display();
    apple->display();
    cout << endl;

    // ===== TEST 4: Add Items to Cart =====
    cout << "TEST 4: Adding Items to Cart..." << endl;
    cout << "-----------------------------------" << endl;
    Cart cart;
    cout << "Adding 2x Fresh Milk..." << endl; cart.addItem(milk, 2);
    cout << "Adding 1x Basmati Rice..." << endl; cart.addItem(rice, 1);
    cout << "Adding 1x Samsung Phone..." << endl; cart.addItem(phone, 1);
    cout << "Adding 5x Red Apple..." << endl; cart.addItem(apple, 5);
    cout << "\nTotal items in cart: " << cart.getItemCount() << endl << endl;

    // ===== TEST 5: Display Cart =====
    cout << "TEST 5: Displaying Cart Contents..." << endl;
    cout << "-----------------------------------" << endl;
    cart.displayCart();
    cout << endl;

    // ===== TEST 6: Update Quantity =====
    cout << "TEST 6: Updating Quantities..." << endl;
    cout << "-----------------------------------" << endl;
    cout << "Changing Milk quantity from 2 to 3..." << endl;
    cart.updateQty(1, 3);
    cout << "\nUpdated Cart:" << endl;
    cart.displayCart();
    cout << endl;

    // ===== TEST 7: Remove Item =====
    cout << "TEST 7: Removing Item from Cart..." << endl;
    cout << "-----------------------------------" << endl;
    cout << "Removing Red Apple from cart..." << endl;
    cart.removeItem(4);
    cout << "\nCart after removal:" << endl;
    cart.displayCart();
    cout << endl;

    // ===== TEST 8: Bill — No Coupon =====
    cout << "TEST 8: Creating Bill (No Coupon)..." << endl;
    cout << "-----------------------------------" << endl;
    Bill bill1(&cart, &customer);
    bill1.calculateGrandTotal();
    cout << "Subtotal    : " << CURRENCY_SYMBOL << bill1.getSubtotal() << endl;
    cout << "Tax         : " << CURRENCY_SYMBOL << bill1.getTotalTax() << endl;
    cout << "Discount    : " << CURRENCY_SYMBOL << bill1.getTotalDiscount() << endl;
    cout << "Grand Total : " << CURRENCY_SYMBOL << bill1.getGrandTotal() << endl << endl;

    // ===== TEST 9: Coupon SAVE10 (10% off) =====
    cout << "TEST 9: Testing Coupon - SAVE10..." << endl;
    cout << "-----------------------------------" << endl;
    Bill bill2(&cart, &customer);
    bill2.applyCoupon("SAVE10");
    bill2.calculateGrandTotal();
    cout << "Grand Total with SAVE10: " << CURRENCY_SYMBOL << bill2.getGrandTotal() << endl << endl;

    // ===== TEST 10: Coupon FLAT50 (Rs.50 off) =====
    cout << "TEST 10: Testing Coupon - FLAT50..." << endl;
    cout << "-----------------------------------" << endl;
    Bill bill3(&cart, &customer);
    bill3.applyCoupon("FLAT50");
    bill3.calculateGrandTotal();
    cout << "Grand Total with FLAT50: " << CURRENCY_SYMBOL << bill3.getGrandTotal() << endl << endl;

    // ===== TEST 11: Coupon FRESH20 (20% off) =====
    cout << "TEST 11: Testing Coupon - FRESH20..." << endl;
    cout << "-----------------------------------" << endl;
    Bill bill4(&cart, &customer);
    bill4.applyCoupon("FRESH20");
    bill4.calculateGrandTotal();
    cout << "Grand Total with FRESH20: " << CURRENCY_SYMBOL << bill4.getGrandTotal() << endl << endl;

    // ===== TEST 12: Invalid Coupon =====
    cout << "TEST 12: Testing Invalid Coupon..." << endl;
    cout << "-----------------------------------" << endl;
    Bill bill5(&cart, &customer);
    bill5.applyCoupon("INVALID123");
    cout << endl;

    // ===== TEST 13: Complete Bill Summary =====
    cout << "TEST 13: Complete Bill Summary..." << endl;
    cout << "-----------------------------------" << endl;
    Bill finalBill(&cart, &customer);
    finalBill.applyCoupon("SAVE10");
    finalBill.calculateGrandTotal();
    finalBill.displayBillSummary();
    cout << endl;

    // ===== TEST 14: Clear Cart =====
    cout << "TEST 14: Clearing Cart..." << endl;
    cout << "-----------------------------------" << endl;
    cart.clearCart();
    cout << "Items in cart after clearing: " << cart.getItemCount() << endl << endl;

    cout << "========================================" << endl;
    cout << "  ALL ORIGINAL TESTS COMPLETED!" << endl;
    cout << "========================================" << endl << endl;

    // ============================================================
    //  FILE MANAGER DEMO  (only runs if #define USE_FILE_MANAGER)
    //  Remove the #define above to skip this entire block.
    // ============================================================
#ifdef USE_FILE_MANAGER

    cout << "========================================" << endl;
    cout << "  FILE MANAGER DEMO" << endl;
    cout << "  Abdul Rehman (25L-2074)" << endl;
    cout << "========================================" << endl << endl;

    // Step 1 — create data/ folders automatically
    FileManager::initDataFolders();
    cout << endl;

    // Step 2 — save the products we created above to file
    cout << "--- Saving products to file ---" << endl;
    Product* allProducts[MAX_PRODUCTS];
    int productCount = 0;
    allProducts[productCount++] = milk;
    allProducts[productCount++] = rice;
    allProducts[productCount++] = phone;
    allProducts[productCount++] = apple;
    FileManager::saveProducts(allProducts, productCount);
    cout << endl;

    // Step 3 — reload products from file and display them
    cout << "--- Loading products back from file ---" << endl;
    Product* loaded[MAX_PRODUCTS];
    int loadedCount = FileManager::loadProducts(loaded, MAX_PRODUCTS);
    for (int i = 0; i < loadedCount; i++) {
        loaded[i]->display();
    }
    cout << endl;

    // Step 4 — save the customer to file
    cout << "--- Saving customer to file ---" << endl;
    FileManager::saveNewCustomer(customer, "password123");
    cout << endl;

    // Step 5 — reload customers and test login
    cout << "--- Loading customers & validating login ---" << endl;
    Customer customers[MAX_CUSTOMERS];
    int custCount = FileManager::loadCustomers(customers, MAX_CUSTOMERS);
    int idx = FileManager::validateCustomerLogin(customers, custCount, "ali123", "password123");
    if (idx >= 0)
        cout << "Customer login verified from file: " << customers[idx].getName() << endl;
    else
        cout << "Customer login failed from file." << endl;
    cout << endl;

    // Step 6 — rebuild a cart and bill to save receipt + sales log
    cout << "--- Generating receipt & sales log ---" << endl;
    Cart demoCart;
    demoCart.addItem(milk, 2);
    demoCart.addItem(rice, 1);
    demoCart.addItem(phone, 1);

    Bill demoBill(&demoCart, &customer);
    demoBill.applyCoupon("SAVE10");
    demoBill.calculateGrandTotal();

    string receiptID = FileManager::generateReceiptID(1);
    string timestamp = "2025-06-01 14:30:00"; // replace with real time in full system

    FileManager::saveReceipt(demoBill, demoCart,
        customer.getName(), receiptID, timestamp);

    FileManager::appendSalesLog(receiptID, customer.getName(), demoCart,
        demoBill.getSubtotal(),
        demoBill.getTotalTax(),
        demoBill.getTotalDiscount(),
        demoBill.getCouponDiscount(),
        demoBill.getGrandTotal(),
        timestamp);
    cout << endl;

    // Step 7 — generate sales report
    cout << "--- Generating sales report ---" << endl;
    string reportID = FileManager::generateReportID(1);
    FileManager::generateSalesReport(reportID, timestamp);
    cout << endl;

    // Step 8 — log a refund
    cout << "--- Logging a refund ---" << endl;
    double refundAmt = demoBill.getGrandTotal() * (1.0 - REFUND_RESTOCKING_FEE);
    double restockFee = demoBill.getGrandTotal() * REFUND_RESTOCKING_FEE;
    FileManager::appendRefundLog(receiptID, customer.getName(),
        refundAmt, restockFee, timestamp);
    cout << endl;

    // Admin login check
    cout << "--- Admin login check ---" << endl;
    if (FileManager::validateAdminLogin("admin", "admin123"))
        cout << "Admin login: SUCCESS" << endl;
    else
        cout << "Admin login: FAILED" << endl;
    cout << endl;

    // Cleanup loaded[] array (these are new objects from loadProducts)
    for (int i = 0; i < loadedCount; i++) {
        delete loaded[i];
        loaded[i] = nullptr;
    }

    cout << "========================================" << endl;
    cout << "  FILE MANAGER DEMO COMPLETE!" << endl;
    cout << "  Check the data/ folder for output." << endl;
    cout << "========================================" << endl << endl;

#endif // USE_FILE_MANAGER

    // Cleanup original products
    delete milk;
    delete rice;
    delete phone;
    delete apple;

    system("pause");
    return 0;
}