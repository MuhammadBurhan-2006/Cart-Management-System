#include <iostream>
#include "Globals.h"
#include "Products.h"
#include "Customer.h"
using namespace std;

int main() {

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
    if (customer.login("ali123", "password123")) {
        cout << "✓ Login SUCCESSFUL!" << endl;
    }
    else {
        cout << "✗ Login FAILED!" << endl;
    }

    cout << "\nAttempting login with wrong password..." << endl;
    if (customer.login("ali123", "wrongpass")) {
        cout << "✓ Login SUCCESSFUL!" << endl;
    }
    else {
        cout << "✗ Login FAILED (as expected)!" << endl;
    }
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

    // ===== TEST 4: Create Cart and Add Items =====
    cout << "TEST 4: Adding Items to Cart..." << endl;
    cout << "-----------------------------------" << endl;

    Cart cart;
    cout << "Adding 2x Fresh Milk..." << endl;
    cart.addItem(milk, 2);

    cout << "Adding 1x Basmati Rice..." << endl;
    cart.addItem(rice, 1);

    cout << "Adding 1x Samsung Phone..." << endl;
    cart.addItem(phone, 1);

    cout << "Adding 5x Red Apple..." << endl;
    cart.addItem(apple, 5);

    cout << "\nTotal items in cart: " << cart.getItemCount() << endl;
    cout << endl;

    // ===== TEST 5: Display Cart =====
    cout << "TEST 5: Displaying Cart Contents..." << endl;
    cout << "-----------------------------------" << endl;
    cart.displayCart();
    cout << endl;

    // ===== TEST 6: Update Quantity =====
    cout << "TEST 6: Updating Quantities..." << endl;
    cout << "-----------------------------------" << endl;
    cout << "Changing Milk quantity from 2 to 3..." << endl;
    cart.updateQty(1, 3);  // Product ID 1 (Milk) to 3 units

    cout << "\nUpdated Cart:" << endl;
    cart.displayCart();
    cout << endl;

    // ===== TEST 7: Remove Item =====
    cout << "TEST 7: Removing Item from Cart..." << endl;
    cout << "-----------------------------------" << endl;
    cout << "Removing Red Apple from cart..." << endl;
    cart.removeItem(4);  // Remove Product ID 4 (Apple)

    cout << "\nCart after removal:" << endl;
    cart.displayCart();
    cout << endl;

    // ===== TEST 8: Create Bill (No Coupon) =====
    cout << "TEST 8: Creating Bill (No Coupon)..." << endl;
    cout << "-----------------------------------" << endl;
    Bill bill1(&cart, &customer);
    bill1.calculateGrandTotal();

    cout << "Subtotal: " << CURRENCY_SYMBOL << bill1.getSubtotal() << endl;
    cout << "Tax: " << CURRENCY_SYMBOL << bill1.getTotalTax() << endl;
    cout << "Discount: " << CURRENCY_SYMBOL << bill1.getTotalDiscount() << endl;
    cout << "Grand Total: " << CURRENCY_SYMBOL << bill1.getGrandTotal() << endl;
    cout << endl;

    // ===== TEST 9: Apply Coupon - SAVE10 (10% off) =====
    cout << "TEST 9: Testing Coupon - SAVE10..." << endl;
    cout << "-----------------------------------" << endl;
    Bill bill2(&cart, &customer);
    cout << "Applying coupon code: SAVE10" << endl;
    bill2.applyCoupon("SAVE10");
    bill2.calculateGrandTotal();
    cout << "Grand Total with SAVE10: " << CURRENCY_SYMBOL << bill2.getGrandTotal() << endl;
    cout << endl;

    // ===== TEST 10: Apply Coupon - FLAT50 (Rs.50 off) =====
    cout << "TEST 10: Testing Coupon - FLAT50..." << endl;
    cout << "-----------------------------------" << endl;
    Bill bill3(&cart, &customer);
    cout << "Applying coupon code: FLAT50" << endl;
    bill3.applyCoupon("FLAT50");
    bill3.calculateGrandTotal();
    cout << "Grand Total with FLAT50: " << CURRENCY_SYMBOL << bill3.getGrandTotal() << endl;
    cout << endl;

    // ===== TEST 11: Apply Coupon - FRESH20 (20% off) =====
    cout << "TEST 11: Testing Coupon - FRESH20..." << endl;
    cout << "-----------------------------------" << endl;
    Bill bill4(&cart, &customer);
    cout << "Applying coupon code: FRESH20" << endl;
    bill4.applyCoupon("FRESH20");
    bill4.calculateGrandTotal();
    cout << "Grand Total with FRESH20: " << CURRENCY_SYMBOL << bill4.getGrandTotal() << endl;
    cout << endl;

    // ===== TEST 12: Invalid Coupon =====
    cout << "TEST 12: Testing Invalid Coupon..." << endl;
    cout << "-----------------------------------" << endl;
    Bill bill5(&cart, &customer);
    cout << "Applying invalid coupon code: INVALID123" << endl;
    bill5.applyCoupon("INVALID123");
    cout << endl;

    // ===== TEST 13: Final Bill Summary =====
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
    cout << "Items in cart after clearing: " << cart.getItemCount() << endl;
    cout << endl;

    // ===== CLEANUP =====
    delete milk;
    delete rice;
    delete phone;
    delete apple;

    // ===== ALL TESTS COMPLETE =====
    cout << "========================================" << endl;
    cout << "  ✓ ALL TESTS COMPLETED SUCCESSFULLY!" << endl;
    cout << "========================================" << endl;

    system("pause");  // Keeps console window open
    return 0;
}