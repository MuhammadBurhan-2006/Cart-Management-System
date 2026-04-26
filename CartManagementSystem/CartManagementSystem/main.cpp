// ============================================================
//  main.cpp  —  Team 03 | CS1004 OOP | FAST-NUCES
//  FIXED: AdminDashboard, StockScreen, ReportScreen connected
// ============================================================

#include <QApplication>
#include <QDir>
#include <QCoreApplication>
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

    QDir::setCurrent(QCoreApplication::applicationDirPath());

    // ---- Step 1: Create data folders ----
    FileManager::initDataFolders();

    // ---- Step 2: Create ALL screens ----
    LoginScreen* loginScreen = new LoginScreen();
    AdminDashboardScreen* dashScreen = new AdminDashboardScreen();
    ProductCatalogScreen* catalogScreen = new ProductCatalogScreen();
    AdminStockScreen* stockScreen = new AdminStockScreen();
    AdminReportScreen* reportScreen = new AdminReportScreen();
    RegisterScreen* registerScreen = new RegisterScreen();
    CustomerScreen* customerScreen = new CustomerScreen();

    // ---- Step 3: Load products into Admin catalog ----
    Product* loadedProducts[MAX_PRODUCTS];
    int loadedCount = FileManager::loadProducts(loadedProducts, MAX_PRODUCTS);
    if (loadedCount > 0)
        catalogScreen->loadProducts(loadedProducts, loadedCount);

    // ---- Step 4: Admin login → show Dashboard ----
    QObject::connect(loginScreen, &LoginScreen::adminLoginRequested,
        [&](QString user, QString pass) {
            if (FileManager::validateAdminLogin(user.toStdString(), pass.toStdString())) {
                loginScreen->hide();
                dashScreen->refresh();
                dashScreen->show();
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

    // =========================================================
    // DASHBOARD NAVIGATION
    // =========================================================

    // Dashboard → Product Catalog
    QObject::connect(dashScreen, &AdminDashboardScreen::goToProducts,
        [&]() {
            Product* all[MAX_PRODUCTS];
            int count = FileManager::loadProducts(all, MAX_PRODUCTS);
            catalogScreen->loadProducts(all, count);
            dashScreen->hide();
            catalogScreen->show();
        });

    // Dashboard → Stock Management
    QObject::connect(dashScreen, &AdminDashboardScreen::goToStock,
        [&]() {
            dashScreen->hide();
            stockScreen->refresh();
            stockScreen->show();
        });

    // Dashboard → Sales Report
    QObject::connect(dashScreen, &AdminDashboardScreen::goToReport,
        [&]() {
            dashScreen->hide();
            reportScreen->refresh();
            reportScreen->show();
        });

    // Dashboard logout
    QObject::connect(dashScreen, &AdminDashboardScreen::logoutRequested,
        [&]() {
            dashScreen->hide();
            loginScreen->show();
        });

    // =========================================================
    // PRODUCT CATALOG NAVIGATION
    // =========================================================

    // Catalog → Dashboard button
    QObject::connect(catalogScreen, &ProductCatalogScreen::goToDashboard,
        [&]() {
            catalogScreen->hide();
            dashScreen->refresh();
            dashScreen->show();
        });

    // Catalog: product added → save
    QObject::connect(catalogScreen, &ProductCatalogScreen::productAdded,
        [&](Product* p) {
            Product* all[MAX_PRODUCTS];
            int count = FileManager::loadProducts(all, MAX_PRODUCTS);
            all[count++] = p;
            FileManager::saveProducts(all, count);
        });

    // Catalog: product updated → save
    QObject::connect(catalogScreen, &ProductCatalogScreen::productUpdated,
        [&](Product*) {
            Product* all[MAX_PRODUCTS];
            int count = FileManager::loadProducts(all, MAX_PRODUCTS);
            FileManager::saveProducts(all, count);
        });

    // Catalog: product deleted → save
    QObject::connect(catalogScreen, &ProductCatalogScreen::productDeleted,
        [&](int) {
            Product* all[MAX_PRODUCTS];
            int count = FileManager::loadProducts(all, MAX_PRODUCTS);
            FileManager::saveProducts(all, count);
            for (int i = 0; i < count; i++) { delete all[i]; all[i] = nullptr; }
        });

    // Catalog logout
    QObject::connect(catalogScreen, &ProductCatalogScreen::logoutRequested,
        [&]() {
            catalogScreen->hide();
            loginScreen->show();
        });

    // =========================================================
    // STOCK SCREEN NAVIGATION
    // =========================================================

    QObject::connect(stockScreen, &AdminStockScreen::goToDashboard,
        [&]() {
            stockScreen->hide();
            dashScreen->refresh();
            dashScreen->show();
        });

    // =========================================================
    // REPORT SCREEN NAVIGATION
    // =========================================================

    QObject::connect(reportScreen, &AdminReportScreen::goToDashboard,
        [&]() {
            reportScreen->hide();
            dashScreen->refresh();
            dashScreen->show();
        });

    // ---- Start ----
    loginScreen->show();
    return app.exec();
}