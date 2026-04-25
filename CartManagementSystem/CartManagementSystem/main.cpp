// ============================================================
//  main.cpp  —  Team 03 | CS1004 OOP | FAST-NUCES
// ============================================================

#include <QApplication>
#include <QMessageBox>
#include "Globals.h"
#include "Products.h"
#include "Customer.h"
#include "Admin.h"
#include "FileManager.h"
#include "StockManager.h"
#include "Report.h"
#include "GUI.h"
using namespace std;

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setFont(QFont("Arial", 12));

    // ---- Step 1: Create data folders ----
    FileManager::initDataFolders();

    // ---- Step 2: Create screens ----
    LoginScreen* loginScreen = new LoginScreen();
    ProductCatalogScreen* catalogScreen = new ProductCatalogScreen();
    RegisterScreen* registerScreen = new RegisterScreen();
    CustomerScreen* customerScreen = new CustomerScreen();

    // ---- Step 3: Load products into Admin catalog ----
    Product* loadedProducts[MAX_PRODUCTS];
    int loadedCount = FileManager::loadProducts(loadedProducts, MAX_PRODUCTS);
    if (loadedCount > 0)
        catalogScreen->loadProducts(loadedProducts, loadedCount);

    // ---- Step 4: Admin login ----
    QObject::connect(loginScreen, &LoginScreen::adminLoginRequested,
        [&](QString user, QString pass) {
            if (FileManager::validateAdminLogin(user.toStdString(), pass.toStdString())) {
                loginScreen->hide();
                catalogScreen->show();
            }
            else {
                QMessageBox::critical(loginScreen, "Login Failed",
                    "Invalid admin username or password.");
            }
        });

    // ---- Step 5: Customer login ----
    QObject::connect(loginScreen, &LoginScreen::customerLoginRequested,
        [&](QString user, QString pass) {
            Customer customers[MAX_CUSTOMERS];
            int count = FileManager::loadCustomers(customers, MAX_CUSTOMERS);
            int idx = FileManager::validateCustomerLogin(
                customers, count,
                user.toStdString(), pass.toStdString());
            if (idx >= 0) {
                // Load fresh products into customer screen
                Product* prods[MAX_PRODUCTS];
                int pcount = FileManager::loadProducts(prods, MAX_PRODUCTS);
                customerScreen->loadProducts(prods, pcount);
                customerScreen->setCustomer(customers[idx]);
                loginScreen->hide();
                customerScreen->show();
            }
            else {
                QMessageBox::critical(loginScreen, "Login Failed",
                    "Invalid customer username or password.");
            }
        });

    // ---- Step 6: Login → Register ----
    QObject::connect(loginScreen, &LoginScreen::registerRequested,
        [&]() {
            loginScreen->hide();
            registerScreen->show();
        });

    // ---- Step 7: Register → back to Login ----
    QObject::connect(registerScreen, &RegisterScreen::backToLogin,
        [&]() {
            registerScreen->hide();
            loginScreen->show();
        });

    // ---- Step 8: Registration done → back to Login ----
    QObject::connect(registerScreen, &RegisterScreen::registrationDone,
        [&](QString) {
            registerScreen->hide();
            loginScreen->show();
        });

    // ---- Step 9: Customer logout ----
    QObject::connect(customerScreen, &CustomerScreen::logoutRequested,
        [&]() {
            customerScreen->hide();
            loginScreen->show();
        });

    // ---- Step 10: Admin — Save product when added ----
    QObject::connect(catalogScreen, &ProductCatalogScreen::productAdded,
        [&](Product* p) {
            Product* all[MAX_PRODUCTS];
            int count = FileManager::loadProducts(all, MAX_PRODUCTS);
            all[count++] = p;
            FileManager::saveProducts(all, count);
        });

    // ---- Step 11: Admin — Save on delete ----
    QObject::connect(catalogScreen, &ProductCatalogScreen::productDeleted,
        [&](int) {
            Product* all[MAX_PRODUCTS];
            int count = FileManager::loadProducts(all, MAX_PRODUCTS);
            FileManager::saveProducts(all, count);
            for (int i = 0; i < count; i++) { delete all[i]; all[i] = nullptr; }
        });

    // ---- Step 12: Admin logout ----
    QObject::connect(catalogScreen, &ProductCatalogScreen::logoutRequested,
        [&]() {
            catalogScreen->hide();
            loginScreen->show();
        });

    // ---- Start ----
    loginScreen->show();
    return app.exec();
}