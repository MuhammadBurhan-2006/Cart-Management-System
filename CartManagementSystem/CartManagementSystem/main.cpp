// ============================================================
//  main.cpp  —  Team 03 | CS1004 OOP | FAST-NUCES
//
//  Members:
//    Burhan    (25L-2061) — Products.h, GUI.h (Login + ProductCatalog)
//    Areeba    (25L-2047) — Customer.h (Customer, Cart, Bill)
//    Khubaib              — Admin.h, StockManager.h, Report.h
//    Abdul Rehman (25L-2074) — FileManager.h
//
//  This is the ONLY .cpp file in the project.
//  All logic is in header files — this just connects everything.
//
//  Flow:
//    App starts → LoginScreen
//    Admin login  → ProductCatalogScreen (loads products from file)
//    Customer login → [placeholder — connect Areeba's screen here]
//    Logout → back to LoginScreen
// ============================================================

#include <QApplication>
#include <QMessageBox>
#include "Globals.h"
#include "Products.h"       // Burhan
#include "Customer.h"       // Areeba  (Cart, Bill also here)
#include "Admin.h"          // Khubaib
#include "FileManager.h"    // Abdul Rehman
#include "StockManager.h"   // Khubaib
#include "Report.h"         // Khubaib
#include "GUI.h"            // Burhan  (LoginScreen, ProductCatalogScreen)
using namespace std;

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setFont(QFont("Arial", 12));

    // ---- Step 1: Create data folders if they don't exist ----
    FileManager::initDataFolders();

    // ---- Step 2: Create screens ----
    LoginScreen* loginScreen = new LoginScreen();
    ProductCatalogScreen* catalogScreen = new ProductCatalogScreen();

    // ---- Step 3: Load products from file into catalog screen ----
    Product* loadedProducts[MAX_PRODUCTS];
    int loadedCount = FileManager::loadProducts(loadedProducts, MAX_PRODUCTS);
    if (loadedCount > 0)
        catalogScreen->loadProducts(loadedProducts, loadedCount);

    // ---- Step 4: Admin login → validate → show catalog ----
    QObject::connect(loginScreen, &LoginScreen::adminLoginRequested,
        [&](QString user, QString pass) {
            // Check against file first, then fallback to Globals.h defaults
            if (FileManager::validateAdminLogin(user.toStdString(), pass.toStdString())) {
                loginScreen->hide();
                catalogScreen->show();
            }
            else {
                QMessageBox::critical(loginScreen, "Login Failed",
                    "Invalid admin username or password.");
            }
        });

    // ---- Step 5: Customer login → load from file → validate ----
    QObject::connect(loginScreen, &LoginScreen::customerLoginRequested,
        [&](QString user, QString pass) {
            Customer customers[MAX_CUSTOMERS];
            int count = FileManager::loadCustomers(customers, MAX_CUSTOMERS);
            int idx = FileManager::validateCustomerLogin(
                customers, count,
                user.toStdString(), pass.toStdString());
            if (idx >= 0) {
                // TODO: Areeba — replace this with her CustomerScreen
                // Example: customerScreen->setCustomer(customers[idx]); customerScreen->show();
                QMessageBox::information(loginScreen, "Customer Login",
                    "Welcome, " + QString::fromStdString(customers[idx].getName()) + "!\n"
                    "(Customer screen — Areeba connects here)");
            }
            else {
                QMessageBox::critical(loginScreen, "Login Failed",
                    "Invalid customer username or password.");
            }
        });

    // ---- Step 6: Save product when added from catalog ----
    QObject::connect(catalogScreen, &ProductCatalogScreen::productAdded,
        [&](Product* p) {
            // Reload all → save all (simplest approach with fixed arrays)
            Product* all[MAX_PRODUCTS];
            int count = FileManager::loadProducts(all, MAX_PRODUCTS);
            // Add new one at end
            all[count++] = p;
            FileManager::saveProducts(all, count);
            // Don't delete all[] items — catalog owns them
        });

    // ---- Step 7: Save on delete ----
    QObject::connect(catalogScreen, &ProductCatalogScreen::productDeleted,
        [&](int) {
            // After delete, catalog's internal list is already updated
            // We reload from screen's state — simplest: just re-save via StockManager
            // For now trigger a full reload+save cycle
            Product* all[MAX_PRODUCTS];
            int count = FileManager::loadProducts(all, MAX_PRODUCTS);
            FileManager::saveProducts(all, count);
            for (int i = 0; i < count; i++) { delete all[i]; all[i] = nullptr; }
        });

    // ---- Step 8: Logout → back to login ----
    QObject::connect(catalogScreen, &ProductCatalogScreen::logoutRequested,
        [&]() {
            catalogScreen->hide();
            loginScreen->show();
        });

    // ---- Step 9: Start ----
    loginScreen->show();
    return app.exec();
}
