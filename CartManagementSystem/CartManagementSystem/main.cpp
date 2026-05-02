#include <QApplication>
#include <QDir>
#include <QCoreApplication>
#include <QMessageBox>
#include <QStackedWidget>
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
    // Working directory
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    //Step 1: Create data folders
    FileManager::initDataFolders();
    // ============================================================
    // SINGLE WINDOW — sab screens ek QStackedWidget mein
    // Index map:
    //   0 = LoginScreen
    //   1 = AdminDashboardScreen
    //   2 = ProductCatalogScreen
    //   3 = AdminStockScreen
    //   4 = AdminReportScreen
    //   5 = RegisterScreen
    //   6 = CustomerScreen
    //   7 = RefundScreen
    // ============================================================
    QStackedWidget* mainWindow = new QStackedWidget();
    mainWindow->setWindowTitle("Supermarket Billing System");
    mainWindow->setMinimumSize(900, 660);
    LoginScreen* loginScreen = new LoginScreen();
    AdminDashboardScreen* dashScreen = new AdminDashboardScreen();
    ProductCatalogScreen* catalogScreen = new ProductCatalogScreen();
    AdminStockScreen* stockScreen = new AdminStockScreen();
    AdminReportScreen* reportScreen = new AdminReportScreen();
    RegisterScreen* regScreen = new RegisterScreen();
    CustomerScreen* custScreen = new CustomerScreen();
    RefundScreen* refundScreen = new RefundScreen();
    mainWindow->addWidget(loginScreen);   // 0
    mainWindow->addWidget(dashScreen);    // 1
    mainWindow->addWidget(catalogScreen); // 2
    mainWindow->addWidget(stockScreen);   // 3
    mainWindow->addWidget(reportScreen);  // 4
    mainWindow->addWidget(regScreen);     // 5
    mainWindow->addWidget(custScreen);    // 6
    mainWindow->addWidget(refundScreen);  // 7
    // Helper lambda — switch page
    auto goTo = [&](int idx) {
        mainWindow->setCurrentIndex(idx);
        };
    // ---- Load products into catalog ----
    Product* loadedProducts[MAX_PRODUCTS];
    int loadedCount = FileManager::loadProducts(loadedProducts, MAX_PRODUCTS);
    if (loadedCount > 0)
        catalogScreen->loadProducts(loadedProducts, loadedCount);
    // ============================================================
    //  LOGIN
    // ============================================================
    QObject::connect(loginScreen, &LoginScreen::adminLoginRequested,
        [&](QString user, QString pass) {
            if (FileManager::validateAdminLogin(user.toStdString(), pass.toStdString())) {
                dashScreen->refresh();
                goTo(1);
            }
            else {
                QMessageBox::critical(mainWindow, "Login Failed",
                    "Invalid admin username or password.");
            }
        });
    QObject::connect(loginScreen, &LoginScreen::customerLoginRequested,
        [&](QString user, QString pass) {
            Customer customers[MAX_CUSTOMERS];
            int count = FileManager::loadCustomers(customers, MAX_CUSTOMERS);
            int idx = FileManager::validateCustomerLogin(
                customers, count, user.toStdString(), pass.toStdString());
            if (idx >= 0) {
                Product* prods[MAX_PRODUCTS];
                int pcount = FileManager::loadProducts(prods, MAX_PRODUCTS);
                custScreen->loadProducts(prods, pcount);
                custScreen->setCustomer(customers[idx]);
                goTo(6);
            }
            else {
                QMessageBox::critical(mainWindow, "Login Failed",
                    "Invalid customer username or password.");
            }
        });

    QObject::connect(loginScreen, &LoginScreen::registerRequested,
        [&]() { goTo(5); });
    // ============================================================
    //  REGISTER
    // ============================================================
    QObject::connect(regScreen, &RegisterScreen::backToLogin,
        [&]() { goTo(0); });

    QObject::connect(regScreen, &RegisterScreen::registrationDone,
        [&](QString) { goTo(0); });
    // ============================================================
    //  ADMIN DASHBOARD
    // ============================================================
    QObject::connect(dashScreen, &AdminDashboardScreen::goToProducts,
        [&]() {
            Product* all[MAX_PRODUCTS];
            int count = FileManager::loadProducts(all, MAX_PRODUCTS);
            catalogScreen->loadProducts(all, count);
            goTo(2);
        });
    QObject::connect(dashScreen, &AdminDashboardScreen::goToStock,
        [&]() { stockScreen->refresh(); goTo(3); });
    QObject::connect(dashScreen, &AdminDashboardScreen::goToReport,
        [&]() { reportScreen->refresh(); goTo(4); });
    QObject::connect(dashScreen, &AdminDashboardScreen::logoutRequested,
        [&]() { goTo(0); });
    // ============================================================
    //  PRODUCT CATALOG
    // ============================================================
    QObject::connect(catalogScreen, &ProductCatalogScreen::goToDashboard,
        [&]() { dashScreen->refresh(); goTo(1); });
    QObject::connect(catalogScreen, &ProductCatalogScreen::productAdded,
        [&](Product* p) {
            Product* all[MAX_PRODUCTS];
            int count = FileManager::loadProducts(all, MAX_PRODUCTS);
            all[count++] = p;
            FileManager::saveProducts(all, count);
        });
    QObject::connect(catalogScreen, &ProductCatalogScreen::productUpdated,
        [&](Product*) {
            Product* all[MAX_PRODUCTS];
            int count = FileManager::loadProducts(all, MAX_PRODUCTS);
            FileManager::saveProducts(all, count);
        });
    QObject::connect(catalogScreen, &ProductCatalogScreen::productDeleted,
        [&](int) {
            Product* all[MAX_PRODUCTS];
            int count = FileManager::loadProducts(all, MAX_PRODUCTS);
            FileManager::saveProducts(all, count);
            for (int i = 0; i < count; i++) { delete all[i]; all[i] = nullptr; }
        });
    QObject::connect(catalogScreen, &ProductCatalogScreen::logoutRequested,
        [&]() { goTo(0); });
    // ============================================================
    //  STOCK SCREEN
    // ============================================================
    QObject::connect(stockScreen, &AdminStockScreen::goToDashboard,
        [&]() { dashScreen->refresh(); goTo(1); });
    // ============================================================
    //  REPORT SCREEN
    // ============================================================
    QObject::connect(reportScreen, &AdminReportScreen::goToDashboard,
        [&]() { dashScreen->refresh(); goTo(1); });
    // ============================================================
    //  CUSTOMER SCREEN
    // ============================================================
    QObject::connect(custScreen, &CustomerScreen::logoutRequested,
        [&]() { goTo(0); });
    QObject::connect(custScreen, &CustomerScreen::refundRequested,
        [&]() { refundScreen->reset(); goTo(7); });
    // ============================================================
    //  REFUND SCREEN
    // ============================================================
    QObject::connect(refundScreen, &RefundScreen::backRequested,
        [&]() { goTo(6); });
    // ---- Start ----
    mainWindow->show();
    return app.exec();
}