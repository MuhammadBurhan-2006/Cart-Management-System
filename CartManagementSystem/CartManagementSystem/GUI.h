#ifndef GUI_H
#define GUI_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QFont>
#include <QStackedWidget>
#include <QSpinBox>
#include <QFrame>
#include <ctime>
#include "Globals.h"
#include "Products.h"
#include "Customer.h"
#include "Admin.h"
#include "FileManager.h"
#include "StockManager.h"
#include "Report.h"
using namespace std;

// ============================================================
//  LOGIN SCREEN
// ============================================================
class LoginScreen : public QWidget {
    Q_OBJECT
public:
    explicit LoginScreen(QWidget* parent = nullptr) : QWidget(parent) {
        setupUI();
        applyStyles();
    }

signals:
    void adminLoginRequested(QString username, QString password);
    void customerLoginRequested(QString username, QString password);
    void registerRequested();

private slots:
    void onAdminClicked() {
        QString user = txtUsername->text().trimmed();
        QString pass = txtPassword->text().trimmed();
        if (user.isEmpty() || pass.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please enter username and password.");
            return;
        }
        emit adminLoginRequested(user, pass);
    }
    void onCustomerClicked() {
        QString user = txtUsername->text().trimmed();
        QString pass = txtPassword->text().trimmed();
        if (user.isEmpty() || pass.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please enter username and password.");
            return;
        }
        emit customerLoginRequested(user, pass);
    }
    void onRegisterClicked() {
        emit registerRequested();
    }

private:
    QLineEdit* txtUsername;
    QLineEdit* txtPassword;
    QPushButton* btnAdmin;
    QPushButton* btnCustomer;
    QPushButton* btnRegister;

    void setupUI() {
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setContentsMargins(60, 40, 60, 40);
        layout->setSpacing(14);

        QLabel* lblTitle = new QLabel(QString::fromStdString(LABEL_LOGIN_TITLE), this);
        lblTitle->setAlignment(Qt::AlignCenter);
        lblTitle->setObjectName("lblTitle");

        QLabel* lblSub = new QLabel("Please enter your credentials to continue", this);
        lblSub->setAlignment(Qt::AlignCenter);

        txtUsername = new QLineEdit(this);
        txtUsername->setPlaceholderText("Username");
        txtUsername->setFixedHeight(38);

        txtPassword = new QLineEdit(this);
        txtPassword->setPlaceholderText("Password");
        txtPassword->setEchoMode(QLineEdit::Password);
        txtPassword->setFixedHeight(38);

        btnAdmin = new QPushButton(QString::fromStdString(LABEL_BTN_ADMIN), this);
        btnCustomer = new QPushButton(QString::fromStdString(LABEL_BTN_CUSTOMER), this);
        btnRegister = new QPushButton("New Customer? Register", this);
        btnAdmin->setFixedHeight(46);
        btnCustomer->setFixedHeight(46);
        btnRegister->setFixedHeight(36);
        btnAdmin->setObjectName("btnAdmin");
        btnCustomer->setObjectName("btnCustomer");
        btnRegister->setObjectName("btnRegister");
        btnAdmin->setCursor(Qt::PointingHandCursor);
        btnCustomer->setCursor(Qt::PointingHandCursor);
        btnRegister->setCursor(Qt::PointingHandCursor);

        QHBoxLayout* btnRow = new QHBoxLayout();
        btnRow->setSpacing(16);
        btnRow->addWidget(btnAdmin);
        btnRow->addWidget(btnCustomer);

        QLabel* lblFooter = new QLabel("© 2025 Team 03 — FAST NUCES | CS1004 OOP", this);
        lblFooter->setAlignment(Qt::AlignCenter);
        lblFooter->setObjectName("lblFooter");

        layout->addStretch();
        layout->addWidget(lblTitle);
        layout->addWidget(lblSub);
        layout->addSpacing(16);
        layout->addWidget(new QLabel("Username:", this));
        layout->addWidget(txtUsername);
        layout->addWidget(new QLabel("Password:", this));
        layout->addWidget(txtPassword);
        layout->addSpacing(10);
        layout->addLayout(btnRow);
        layout->addWidget(btnRegister);
        layout->addStretch();
        layout->addWidget(lblFooter);

        connect(btnAdmin, &QPushButton::clicked, this, &LoginScreen::onAdminClicked);
        connect(btnCustomer, &QPushButton::clicked, this, &LoginScreen::onCustomerClicked);
        connect(btnRegister, &QPushButton::clicked, this, &LoginScreen::onRegisterClicked);

        setWindowTitle("Supermarket Billing System");
        setMinimumSize(480, 440);
    }

    void applyStyles() {
        setStyleSheet(R"(
            QWidget          { background:#f4f6f9; font-family:Arial; font-size:13px; color:#2c3e50; }
            QLabel#lblTitle  { font-size:19px; font-weight:bold; color:#1a73e8; padding:8px; }
            QLabel#lblFooter { font-size:11px; color:#aaa; }
            QLineEdit        { border:1px solid #ccc; border-radius:6px; padding:6px 10px; background:white; }
            QLineEdit:focus  { border:1px solid #1a73e8; }
            QPushButton#btnAdmin          { background:#1a73e8; color:white; font-weight:bold; border:none; border-radius:8px; }
            QPushButton#btnAdmin:hover    { background:#1558b0; }
            QPushButton#btnCustomer       { background:#34a853; color:white; font-weight:bold; border:none; border-radius:8px; }
            QPushButton#btnCustomer:hover { background:#277a3e; }
            QPushButton#btnRegister       { background:transparent; color:#1a73e8; border:1px solid #1a73e8; border-radius:8px; font-size:12px; }
            QPushButton#btnRegister:hover { background:#e8f0fe; }
        )");
    }
};  // BUG FIX: missing closing brace of LoginScreen was here


// ============================================================
//  PRODUCT CATALOG SCREEN (Admin)
// ============================================================
class ProductCatalogScreen : public QWidget {
    Q_OBJECT
public:
    explicit ProductCatalogScreen(QWidget* parent = nullptr)
        : QWidget(parent), productCount(0), editingRow(-1)
    {
        for (int i = 0; i < MAX_PRODUCTS; i++) productList[i] = nullptr;
        setupUI();
        applyStyles();
    }

    ~ProductCatalogScreen() {
        for (int i = 0; i < productCount; i++) {
            delete productList[i];
            productList[i] = nullptr;
        }
    }

    void loadProducts(Product* products[], int count) {
        for (int i = 0; i < productCount; i++) delete productList[i];
        productCount = 0;
        for (int i = 0; i < count && i < MAX_PRODUCTS; i++)
            productList[productCount++] = products[i];
        refreshTable();
    }

signals:
    void logoutRequested();
    void productAdded(Product* p);
    void productUpdated(Product* p);
    void productDeleted(int productID);

private slots:
    void onCategoryChanged(int) {
        QString cat = cmbCategory->currentText();
        lblExtra->show(); txtExtra->show();
        lblExtra2->hide(); txtExtra2->hide();
        if (cat == QString::fromStdString(CAT_PERISHABLE)) {
            lblExtra->setText("Expiry Date:");
            txtExtra->setPlaceholderText("e.g. 2025-12-31");
        }
        else if (cat == QString::fromStdString(CAT_ELECTRONIC)) {
            lblExtra->setText("Brand:");
            txtExtra->setPlaceholderText("e.g. Samsung");
            lblExtra2->show(); txtExtra2->show();
        }
        else {
            lblExtra->setText("Weight (kg):");
            txtExtra->setPlaceholderText("e.g. 1.5");
        }
    }

    void onAddClicked() {
        if (!validateForm()) return;
        if (productCount >= MAX_PRODUCTS) {
            QMessageBox::warning(this, "Limit", "Maximum product limit reached."); return;
        }
        int id = txtID->text().toInt();
        if (findByID(id) != -1) {
            QMessageBox::warning(this, "Duplicate", "Product ID already exists."); return;
        }
        Product* p = makeProduct();
        productList[productCount++] = p;
        refreshTable(); clearForm();
        emit productAdded(p);
        QMessageBox::information(this, "Success", "Product added successfully.");
    }

    void onEditClicked() {
        if (!validateForm() || editingRow < 0) return;
        delete productList[editingRow];
        Product* p = makeProduct();
        productList[editingRow] = p;
        refreshTable(); clearForm();
        emit productUpdated(p);
        QMessageBox::information(this, "Success", "Product updated.");
    }

    void onDeleteClicked() {
        int row = tblProducts->currentRow();
        if (row < 0) return;
        if (QMessageBox::question(this, "Delete",
            "Delete: " + tblProducts->item(row, 1)->text() + "?",
            QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) return;
        int deletedID = productList[row]->getProductID();
        delete productList[row];
        for (int i = row; i < productCount - 1; i++)
            productList[i] = productList[i + 1];
        productList[--productCount] = nullptr;
        refreshTable(); clearForm();
        emit productDeleted(deletedID);
    }

    void onRowSelected() {
        int row = tblProducts->currentRow();
        if (row < 0 || row >= productCount) return;
        editingRow = row;
        Product* p = productList[row];
        txtID->setText(QString::number(p->getProductID()));
        txtName->setText(QString::fromStdString(p->getName()));
        txtPrice->setText(QString::number(p->getPrice(), 'f', 2));
        txtStock->setText(QString::number(p->getStockQty()));
        string cat = p->getCategory();
        if (cat == CAT_PERISHABLE) {
            cmbCategory->setCurrentIndex(0);
            auto* pp = dynamic_cast<PerishableProduct*>(p);
            if (pp) txtExtra->setText(QString::fromStdString(pp->getExpiryDate()));
        }
        else if (cat == CAT_ELECTRONIC) {
            cmbCategory->setCurrentIndex(1);
            auto* ep = dynamic_cast<ElectronicProduct*>(p);
            if (ep) {
                txtExtra->setText(QString::fromStdString(ep->getBrand()));
                txtExtra2->setText(QString::number(ep->getWarrantyMonths()));
            }
        }
        else {
            cmbCategory->setCurrentIndex(2);
            auto* gp = dynamic_cast<GroceryProduct*>(p);
            if (gp) txtExtra->setText(QString::number(gp->getWeightKg(), 'f', 2));
        }
        btnEdit->setEnabled(true);
        btnDelete->setEnabled(true);
        grpForm->setTitle("Edit Product");
    }

    void clearForm() {
        txtID->clear(); txtName->clear(); txtPrice->clear();
        txtStock->clear(); txtExtra->clear(); txtExtra2->clear();
        cmbCategory->setCurrentIndex(0);
        editingRow = -1;
        btnEdit->setEnabled(false);
        btnDelete->setEnabled(false);
        tblProducts->clearSelection();
        grpForm->setTitle("Add New Product");
    }

    void onLogoutClicked() {
        if (QMessageBox::question(this, "Logout", "Sure you want to logout?",
            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
            emit logoutRequested();
    }

private:
    QGroupBox* grpForm;
    QLineEdit* txtID, * txtName, * txtPrice, * txtStock, * txtExtra, * txtExtra2;
    QLabel* lblExtra, * lblExtra2;
    QComboBox* cmbCategory;
    QTableWidget* tblProducts;
    QPushButton* btnAdd, * btnEdit, * btnDelete, * btnClear, * btnLogout;
    Product* productList[MAX_PRODUCTS];
    int           productCount;
    int           editingRow;

    bool validateForm() {
        if (txtID->text().isEmpty() || txtName->text().isEmpty() ||
            txtPrice->text().isEmpty() || txtStock->text().isEmpty()) {
            QMessageBox::warning(this, "Error", "Fill in ID, Name, Price and Stock.");
            return false;
        }
        bool ok1, ok2, ok3;
        txtID->text().toInt(&ok1);
        txtPrice->text().toDouble(&ok2);
        txtStock->text().toInt(&ok3);
        if (!ok1 || !ok2 || !ok3) {
            QMessageBox::warning(this, "Error", "ID & Stock must be integers. Price must be a number.");
            return false;
        }
        return true;
    }

    Product* makeProduct() {
        int    id = txtID->text().toInt();
        string name = txtName->text().trimmed().toStdString();
        double price = txtPrice->text().toDouble();
        int    stock = txtStock->text().toInt();
        QString cat = cmbCategory->currentText();
        if (cat == QString::fromStdString(CAT_PERISHABLE)) {
            return new PerishableProduct(id, name, price, stock,
                txtExtra->text().trimmed().toStdString());
        }
        else if (cat == QString::fromStdString(CAT_ELECTRONIC)) {
            int w = txtExtra2->text().isEmpty() ? 0 : txtExtra2->text().toInt();
            return new ElectronicProduct(id, name, price, stock,
                txtExtra->text().trimmed().toStdString(), w);
        }
        else {
            double wkg = txtExtra->text().isEmpty() ? 0.0 : txtExtra->text().toDouble();
            return new GroceryProduct(id, name, price, stock, wkg);
        }
    }

    int findByID(int id) {
        for (int i = 0; i < productCount; i++)
            if (productList[i]->getProductID() == id) return i;
        return -1;
    }

    void refreshTable() {
        tblProducts->setRowCount(0);
        for (int i = 0; i < productCount; i++) {
            Product* p = productList[i];
            int row = tblProducts->rowCount();
            tblProducts->insertRow(row);
            tblProducts->setItem(row, 0, new QTableWidgetItem(QString::number(p->getProductID())));
            tblProducts->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(p->getName())));
            tblProducts->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(p->getCategory())));
            tblProducts->setItem(row, 3, new QTableWidgetItem(
                QString::fromStdString(CURRENCY_SYMBOL) + QString::number(p->getPrice(), 'f', 2)));
            tblProducts->setItem(row, 4, new QTableWidgetItem(QString::number(p->getStockQty())));
            QString extra;
            string cat = p->getCategory();
            if (cat == CAT_PERISHABLE) {
                auto* pp = dynamic_cast<PerishableProduct*>(p);
                if (pp) extra = "Expiry: " + QString::fromStdString(pp->getExpiryDate());
            }
            else if (cat == CAT_ELECTRONIC) {
                auto* ep = dynamic_cast<ElectronicProduct*>(p);
                if (ep) extra = "Brand: " + QString::fromStdString(ep->getBrand())
                    + " | " + QString::number(ep->getWarrantyMonths()) + " mo";
            }
            else {
                auto* gp = dynamic_cast<GroceryProduct*>(p);
                if (gp) extra = QString::number(gp->getWeightKg(), 'f', 2) + " kg";
            }
            tblProducts->setItem(row, 5, new QTableWidgetItem(extra));
            tblProducts->setItem(row, 6, new QTableWidgetItem(
                QString::fromStdString(CURRENCY_SYMBOL) + QString::number(p->getFinalPrice(), 'f', 2)));
            QColor c = Qt::white;
            if (p->getStockQty() <= STOCK_CRITICAL_THRESHOLD) c = QColor("#ffe4e4");
            else if (p->getStockQty() <= STOCK_LOW_THRESHOLD)      c = QColor("#fff8d6");
            for (int col = 0; col < 7; col++)
                if (tblProducts->item(row, col))
                    tblProducts->item(row, col)->setBackground(c);
        }
    }

    void setupUI() {
        QHBoxLayout* header = new QHBoxLayout();
        QLabel* lblTitle = new QLabel("ADMIN PANEL — Product Catalog", this);
        lblTitle->setObjectName("lblAdminTitle");
        btnLogout = new QPushButton(QString::fromStdString(LABEL_BTN_LOGOUT), this);
        btnLogout->setObjectName("btnLogout");
        btnLogout->setFixedWidth(90);
        btnLogout->setCursor(Qt::PointingHandCursor);
        header->addWidget(lblTitle);
        header->addStretch();
        header->addWidget(btnLogout);

        grpForm = new QGroupBox("Add New Product", this);
        QGridLayout* grid = new QGridLayout(grpForm);
        grid->setSpacing(9); grid->setContentsMargins(14, 14, 14, 14);

        txtID = new QLineEdit(); txtID->setPlaceholderText("e.g. 101");
        txtName = new QLineEdit(); txtName->setPlaceholderText("e.g. Apple");
        txtPrice = new QLineEdit(); txtPrice->setPlaceholderText("e.g. 150.00");
        txtStock = new QLineEdit(); txtStock->setPlaceholderText("e.g. 100");
        txtExtra = new QLineEdit(); txtExtra->setPlaceholderText("e.g. 2025-12-31");
        txtExtra2 = new QLineEdit(); txtExtra2->setPlaceholderText("Warranty months");

        cmbCategory = new QComboBox();
        cmbCategory->addItem(QString::fromStdString(CAT_PERISHABLE));
        cmbCategory->addItem(QString::fromStdString(CAT_ELECTRONIC));
        cmbCategory->addItem(QString::fromStdString(CAT_GROCERY));

        lblExtra = new QLabel("Expiry Date:");
        lblExtra2 = new QLabel("Warranty (months):");
        lblExtra2->hide(); txtExtra2->hide();

        grid->addWidget(new QLabel("Product ID:"), 0, 0); grid->addWidget(txtID, 0, 1);
        grid->addWidget(new QLabel("Name:"), 0, 2); grid->addWidget(txtName, 0, 3);
        grid->addWidget(new QLabel("Category:"), 1, 0); grid->addWidget(cmbCategory, 1, 1);
        grid->addWidget(new QLabel("Price (Rs.):"), 1, 2); grid->addWidget(txtPrice, 1, 3);
        grid->addWidget(new QLabel("Stock Qty:"), 2, 0); grid->addWidget(txtStock, 2, 1);
        grid->addWidget(lblExtra, 2, 2); grid->addWidget(txtExtra, 2, 3);
        grid->addWidget(lblExtra2, 3, 2); grid->addWidget(txtExtra2, 3, 3);

        btnAdd = new QPushButton("Add Product");
        btnEdit = new QPushButton("Save Edit");
        btnClear = new QPushButton("Clear");
        btnAdd->setObjectName("btnAdd");
        btnEdit->setObjectName("btnEdit");
        btnEdit->setEnabled(false);
        btnAdd->setCursor(Qt::PointingHandCursor);
        btnEdit->setCursor(Qt::PointingHandCursor);
        btnClear->setCursor(Qt::PointingHandCursor);
        QHBoxLayout* fbtns = new QHBoxLayout();
        fbtns->addWidget(btnAdd); fbtns->addWidget(btnEdit);
        fbtns->addWidget(btnClear); fbtns->addStretch();
        grid->addLayout(fbtns, 4, 0, 1, 4);

        QGroupBox* grpTable = new QGroupBox("Product List", this);
        QVBoxLayout* tlay = new QVBoxLayout(grpTable);
        tblProducts = new QTableWidget(0, 7, this);
        tblProducts->setHorizontalHeaderLabels({ "ID","Name","Category","Price","Stock","Extra","Final Price" });
        tblProducts->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        tblProducts->setSelectionBehavior(QAbstractItemView::SelectRows);
        tblProducts->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblProducts->setAlternatingRowColors(true);
        tblProducts->verticalHeader()->setVisible(false);
        tblProducts->setMinimumHeight(280);

        btnDelete = new QPushButton("Delete Selected");
        btnDelete->setObjectName("btnDelete");
        btnDelete->setEnabled(false);
        btnDelete->setCursor(Qt::PointingHandCursor);
        QHBoxLayout* tbtns = new QHBoxLayout();
        tbtns->addStretch(); tbtns->addWidget(btnDelete);
        tlay->addWidget(tblProducts); tlay->addLayout(tbtns);

        QVBoxLayout* mainLay = new QVBoxLayout(this);
        mainLay->setContentsMargins(18, 14, 18, 14); mainLay->setSpacing(12);
        mainLay->addLayout(header);
        mainLay->addWidget(grpForm);
        mainLay->addWidget(grpTable);
        setLayout(mainLay);
        setWindowTitle("Admin — Product Catalog Management");
        setMinimumSize(800, 650);

        connect(cmbCategory, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ProductCatalogScreen::onCategoryChanged);
        connect(btnAdd, &QPushButton::clicked, this, &ProductCatalogScreen::onAddClicked);
        connect(btnEdit, &QPushButton::clicked, this, &ProductCatalogScreen::onEditClicked);
        connect(btnDelete, &QPushButton::clicked, this, &ProductCatalogScreen::onDeleteClicked);
        connect(btnClear, &QPushButton::clicked, this, &ProductCatalogScreen::clearForm);
        connect(btnLogout, &QPushButton::clicked, this, &ProductCatalogScreen::onLogoutClicked);
        connect(tblProducts, &QTableWidget::itemSelectionChanged,
            this, &ProductCatalogScreen::onRowSelected);
    }

    void applyStyles() {
        setStyleSheet(R"(
            QWidget { background:#f4f6f9; font-family:Arial; font-size:13px; color:#2c3e50; }
            QLabel#lblAdminTitle { font-size:16px; font-weight:bold; color:#1a73e8; }
            QGroupBox { font-weight:bold; border:1px solid #d0d7de; border-radius:8px;
                        margin-top:8px; padding-top:10px; background:white; }
            QGroupBox::title { subcontrol-origin:margin; left:12px; color:#1a73e8; }
            QLineEdit, QComboBox { border:1px solid #ccc; border-radius:5px;
                                   padding:5px 8px; background:#fafafa; min-height:26px; }
            QLineEdit:focus, QComboBox:focus { border:1px solid #1a73e8; background:white; }
            QPushButton#btnAdd    { background:#1a73e8; color:white; font-weight:bold;
                                    border:none; border-radius:6px; padding:7px 16px; }
            QPushButton#btnAdd:hover    { background:#1558b0; }
            QPushButton#btnEdit   { background:#f59e0b; color:white; font-weight:bold;
                                    border:none; border-radius:6px; padding:7px 16px; }
            QPushButton#btnEdit:hover   { background:#d97706; }
            QPushButton#btnDelete { background:#dc2626; color:white; font-weight:bold;
                                    border:none; border-radius:6px; padding:7px 16px; }
            QPushButton#btnDelete:hover { background:#b91c1c; }
            QPushButton#btnLogout { background:#6b7280; color:white; border:none;
                                    border-radius:6px; padding:5px 12px; }
            QPushButton#btnLogout:hover { background:#4b5563; }
            QPushButton { border:1px solid #ccc; border-radius:6px;
                          padding:5px 12px; background:#f0f0f0; }
            QPushButton:hover { background:#e0e0e0; }
            QTableWidget { border:1px solid #d0d7de; border-radius:6px;
                           background:white; alternate-background-color:#f8f9fc; }
            QTableWidget::item:selected { background:#d0e8ff; color:#1a1a1a; }
            QHeaderView::section { background:#1a73e8; color:white;
                                   font-weight:bold; padding:5px; border:none; }
        )");
    }
};

// ============================================================
//  REGISTER SCREEN (New Customer)
// ============================================================
class RegisterScreen : public QWidget {
    Q_OBJECT
public:
    explicit RegisterScreen(QWidget* parent = nullptr) : QWidget(parent) {
        setupUI();
        applyStyles();
    }

signals:
    void backToLogin();
    void registrationDone(QString username);

private slots:
    void onRegisterClicked() {
        QString name = txtName->text().trimmed();
        QString username = txtUsername->text().trimmed();
        QString password = txtPassword->text().trimmed();
        QString confirm = txtConfirm->text().trimmed();
        QString phone = txtPhone->text().trimmed();

        // Validation
        if (name.isEmpty() || username.isEmpty() || password.isEmpty() || phone.isEmpty()) {
            QMessageBox::warning(this, "Error", "Please fill in all fields.");
            return;
        }
        if (password != confirm) {
            QMessageBox::warning(this, "Error", "Passwords do not match!");
            return;
        }
        if (password.length() < 6) {
            QMessageBox::warning(this, "Error", "Password must be at least 6 characters.");
            return;
        }

        // Check username not already taken
        Customer existing[MAX_CUSTOMERS];
        int count = FileManager::loadCustomers(existing, MAX_CUSTOMERS);
        for (int i = 0; i < count; i++) {
            if (QString::fromStdString(existing[i].getUsername()) == username) {
                QMessageBox::warning(this, "Error", "Username already taken. Choose another.");
                return;
            }
        }

        // Generate new ID (max existing ID + 1)
        int newID = 101;
        for (int i = 0; i < count; i++) {
            if (existing[i].getCustomerID() >= newID)
                newID = existing[i].getCustomerID() + 1;
        }

        // Save new customer
        Customer newCust(newID,
            name.toStdString(),
            username.toStdString(),
            password.toStdString(),
            phone.toStdString());
        FileManager::saveNewCustomer(newCust, password.toStdString());

        QMessageBox::information(this, "Success",
            "Account created! Welcome, " + name + "!\nYou can now login.");
        emit registrationDone(username);
    }

    void onBackClicked() {
        emit backToLogin();
    }

private:
    QLineEdit* txtName;
    QLineEdit* txtUsername;
    QLineEdit* txtPassword;
    QLineEdit* txtConfirm;
    QLineEdit* txtPhone;
    QPushButton* btnRegister;
    QPushButton* btnBack;

    void setupUI() {
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setContentsMargins(60, 30, 60, 30);
        layout->setSpacing(12);

        QLabel* lblTitle = new QLabel("Create New Account", this);
        lblTitle->setAlignment(Qt::AlignCenter);
        lblTitle->setObjectName("lblRegTitle");

        QLabel* lblSub = new QLabel("Fill in your details to register", this);
        lblSub->setAlignment(Qt::AlignCenter);

        txtName = new QLineEdit(this); txtName->setPlaceholderText("Full Name");     txtName->setFixedHeight(36);
        txtUsername = new QLineEdit(this); txtUsername->setPlaceholderText("Username");  txtUsername->setFixedHeight(36);
        txtPassword = new QLineEdit(this); txtPassword->setPlaceholderText("Password (min 6 chars)"); txtPassword->setFixedHeight(36);
        txtPassword->setEchoMode(QLineEdit::Password);
        txtConfirm = new QLineEdit(this); txtConfirm->setPlaceholderText("Confirm Password"); txtConfirm->setFixedHeight(36);
        txtConfirm->setEchoMode(QLineEdit::Password);
        txtPhone = new QLineEdit(this); txtPhone->setPlaceholderText("Phone (e.g. 03001234567)"); txtPhone->setFixedHeight(36);

        btnRegister = new QPushButton("Create Account", this);
        btnRegister->setFixedHeight(44);
        btnRegister->setObjectName("btnCreateAcc");
        btnRegister->setCursor(Qt::PointingHandCursor);

        btnBack = new QPushButton("← Back to Login", this);
        btnBack->setObjectName("btnBackLogin");
        btnBack->setCursor(Qt::PointingHandCursor);

        layout->addStretch();
        layout->addWidget(lblTitle);
        layout->addWidget(lblSub);
        layout->addSpacing(10);
        layout->addWidget(new QLabel("Full Name:", this));    layout->addWidget(txtName);
        layout->addWidget(new QLabel("Username:", this));     layout->addWidget(txtUsername);
        layout->addWidget(new QLabel("Password:", this));     layout->addWidget(txtPassword);
        layout->addWidget(new QLabel("Confirm Password:", this)); layout->addWidget(txtConfirm);
        layout->addWidget(new QLabel("Phone:", this));        layout->addWidget(txtPhone);
        layout->addSpacing(10);
        layout->addWidget(btnRegister);
        layout->addWidget(btnBack);
        layout->addStretch();

        connect(btnRegister, &QPushButton::clicked, this, &RegisterScreen::onRegisterClicked);
        connect(btnBack, &QPushButton::clicked, this, &RegisterScreen::onBackClicked);

        setWindowTitle("Register — Supermarket Billing System");
        setMinimumSize(460, 520);
    }

    void applyStyles() {
        setStyleSheet(R"(
            QWidget              { background:#f4f6f9; font-family:Arial; font-size:13px; color:#2c3e50; }
            QLabel#lblRegTitle   { font-size:19px; font-weight:bold; color:#34a853; padding:6px; }
            QLineEdit            { border:1px solid #ccc; border-radius:6px; padding:6px 10px; background:white; }
            QLineEdit:focus      { border:1px solid #34a853; }
            QPushButton#btnCreateAcc       { background:#34a853; color:white; font-weight:bold; border:none; border-radius:8px; }
            QPushButton#btnCreateAcc:hover { background:#277a3e; }
            QPushButton#btnBackLogin       { background:transparent; color:#1a73e8; border:1px solid #1a73e8; border-radius:8px; }
            QPushButton#btnBackLogin:hover { background:#e8f0fe; }
        )");
    }
};

// ============================================================
//  CUSTOMER SCREEN — Areeba (25L-2047)
//  3 Pages: Browse Products | My Cart | Checkout/Bill
// ============================================================
class CustomerScreen : public QWidget {
    Q_OBJECT

public:
    explicit CustomerScreen(QWidget* parent = nullptr)
        : QWidget(parent), productCount(0), receiptCounter(1), bill(nullptr)
    {
        for (int i = 0; i < MAX_PRODUCTS; i++) productList[i] = nullptr;
        setupUI();
        applyStyles();
    }

    ~CustomerScreen() {
        for (int i = 0; i < productCount; i++) {
            delete productList[i];
            productList[i] = nullptr;
        }
        if (bill) { delete bill; bill = nullptr; }
    }

    void setCustomer(const Customer& c) {
        currentCustomer = c;
        lblWelcome->setText("Welcome, " + QString::fromStdString(c.getName()) + "!");
        cart = Cart();
        if (bill) { delete bill; bill = nullptr; }
        txtCoupon->clear();
        refreshCart();
        showPage(0);
    }

    void loadProducts(Product* products[], int count) {
        for (int i = 0; i < productCount; i++) delete productList[i];
        productCount = 0;
        for (int i = 0; i < count && i < MAX_PRODUCTS; i++)
            productList[productCount++] = products[i];
        refreshProductTable();
    }

signals:
    void logoutRequested();

private slots:
    void showBrowse() { showPage(0); }
    void showCartPage() { refreshCart(); showPage(1); }
    void showCheckout() {
        if (cart.getItemCount() == 0) {
            QMessageBox::warning(this, "Empty Cart", "Add items to cart first!");
            return;
        }
        refreshBill();
        showPage(2);
    }

    void onAddToCart() {
        int row = tblProducts->currentRow();
        if (row < 0) {
            QMessageBox::warning(this, "Select", "Please select a product first.");
            return;
        }
        Product* p = productList[row];
        if (p->getStockQty() <= 0) {
            QMessageBox::warning(this, "Out of Stock", "This product is out of stock.");
            return;
        }
        int qty = spinQty->value();
        if (qty > p->getStockQty()) {
            QMessageBox::warning(this, "Stock",
                "Only " + QString::number(p->getStockQty()) + " units available.");
            return;
        }
        cart.addItem(p, qty);
        QMessageBox::information(this, "Added",
            QString::fromStdString(p->getName()) +
            " x" + QString::number(qty) + " added to cart!");
        spinQty->setValue(1);
        updateCartBadge();
    }

    void onRemoveFromCart() {
        int row = tblCart->currentRow();
        if (row < 0) return;
        CartItem item = cart.getItem(row);
        cart.removeItem(item.product->getProductID());
        refreshCart();
        updateCartBadge();
    }

    void onApplyCoupon() {
        QString code = txtCoupon->text().trimmed().toUpper();
        if (code.isEmpty()) {
            QMessageBox::warning(this, "Coupon", "Enter a coupon code.");
            return;
        }
        if (bill) { bill->applyCoupon(code.toStdString()); refreshBill(); }
    }

    void onPlaceOrder() {
        if (!bill || cart.getItemCount() == 0) return;
        bill->calculateGrandTotal();
        double grand = bill->getGrandTotal();

        string receiptID = FileManager::generateReceiptID(receiptCounter++);
        time_t now = time(nullptr);
        char buf[64];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        string timestamp(buf);

        FileManager::appendSalesLog(receiptID, currentCustomer.getName(), cart,
            bill->getSubtotal(), bill->getTotalTax(), bill->getTotalDiscount(),
            bill->getCouponDiscount(), grand, timestamp);
        FileManager::saveReceipt(*bill, cart, currentCustomer.getName(), receiptID, timestamp);

        QString summary;
        summary += "Receipt ID  : " + QString::fromStdString(receiptID) + "\n";
        summary += "Customer    : " + QString::fromStdString(currentCustomer.getName()) + "\n";
        summary += "Date        : " + QString::fromStdString(timestamp) + "\n";
        summary += "─────────────────────────────\n";
        for (int i = 0; i < cart.getItemCount(); i++) {
            CartItem it = cart.getItem(i);
            double lt = it.product->getPrice() * it.quantity;
            summary += QString::fromStdString(it.product->getName())
                + " x" + QString::number(it.quantity)
                + "  =  Rs." + QString::number(lt, 'f', 2) + "\n";
        }
        summary += "─────────────────────────────\n";
        summary += "Subtotal    : Rs." + QString::number(bill->getSubtotal(), 'f', 2) + "\n";
        summary += "Tax         : Rs." + QString::number(bill->getTotalTax(), 'f', 2) + "\n";
        summary += "Discount    : Rs." + QString::number(bill->getTotalDiscount(), 'f', 2) + "\n";
        if (bill->isCouponApplied())
            summary += "Coupon      : -Rs." + QString::number(bill->getCouponDiscount(), 'f', 2) + "\n";
        summary += "─────────────────────────────\n";
        summary += "GRAND TOTAL : Rs." + QString::number(grand, 'f', 2) + "\n\nThank you for shopping!";

        QMessageBox::information(this, "Order Placed!", summary);
        cart.clearCart();
        if (bill) { delete bill; bill = nullptr; }
        txtCoupon->clear();
        updateCartBadge();
        showPage(0);
    }

    void onLogoutClicked() {
        if (QMessageBox::question(this, "Logout", "Sure you want to logout?",
            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            cart.clearCart();
            if (bill) { delete bill; bill = nullptr; }
            txtCoupon->clear();
            updateCartBadge();
            emit logoutRequested();
        }
    }

private:
    Customer  currentCustomer;
    Cart      cart;
    Bill* bill;
    Product* productList[MAX_PRODUCTS];
    int       productCount;
    int       receiptCounter;

    QStackedWidget* pages;
    QLabel* lblWelcome;
    QPushButton* btnBrowse, * btnCartNav, * btnLogout;

    // Page 0
    QTableWidget* tblProducts;
    QSpinBox* spinQty;
    QPushButton* btnAddToCart;

    // Page 1
    QTableWidget* tblCart;
    QLabel* lblCartTotal;
    QPushButton* btnRemove, * btnGoCheckout;

    // Page 2
    QLabel* lblSub, * lblTax, * lblDisc, * lblCouponAmt, * lblGrand;
    QLineEdit* txtCoupon;
    QPushButton* btnApplyCoupon, * btnPlaceOrder;

    void showPage(int idx) { pages->setCurrentIndex(idx); }

    void updateCartBadge() {
        int n = cart.getItemCount();
        btnCartNav->setText(n > 0
            ? "My Cart (" + QString::number(n) + ")"
            : "My Cart");
    }

    void refreshProductTable() {
        tblProducts->setRowCount(0);
        for (int i = 0; i < productCount; i++) {
            Product* p = productList[i];
            int row = tblProducts->rowCount();
            tblProducts->insertRow(row);
            tblProducts->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(p->getName())));
            tblProducts->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(p->getCategory())));
            tblProducts->setItem(row, 2, new QTableWidgetItem("Rs." + QString::number(p->getPrice(), 'f', 2)));
            tblProducts->setItem(row, 3, new QTableWidgetItem("Rs." + QString::number(p->getFinalPrice(), 'f', 2)));
            tblProducts->setItem(row, 4, new QTableWidgetItem(QString::number(p->getStockQty())));
            QString extra;
            string cat = p->getCategory();
            if (cat == CAT_PERISHABLE) {
                auto* pp = dynamic_cast<PerishableProduct*>(p);
                if (pp) extra = "Exp: " + QString::fromStdString(pp->getExpiryDate());
            }
            else if (cat == CAT_ELECTRONIC) {
                auto* ep = dynamic_cast<ElectronicProduct*>(p);
                if (ep) extra = QString::fromStdString(ep->getBrand())
                    + " | " + QString::number(ep->getWarrantyMonths()) + "mo";
            }
            else {
                auto* gp = dynamic_cast<GroceryProduct*>(p);
                if (gp) extra = QString::number(gp->getWeightKg(), 'f', 2) + " kg";
            }
            tblProducts->setItem(row, 5, new QTableWidgetItem(extra));
            QColor c = (p->getStockQty() == 0) ? QColor("#ffe4e4") : Qt::white;
            for (int col = 0; col < 6; col++)
                if (tblProducts->item(row, col))
                    tblProducts->item(row, col)->setBackground(c);
        }
    }

    void refreshCart() {
        tblCart->setRowCount(0);
        double total = 0;
        for (int i = 0; i < cart.getItemCount(); i++) {
            CartItem it = cart.getItem(i);
            double lineTotal = it.product->getFinalPrice() * it.quantity;
            total += lineTotal;
            int row = tblCart->rowCount();
            tblCart->insertRow(row);
            tblCart->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(it.product->getName())));
            tblCart->setItem(row, 1, new QTableWidgetItem(QString::number(it.quantity)));
            tblCart->setItem(row, 2, new QTableWidgetItem("Rs." + QString::number(it.product->getFinalPrice(), 'f', 2)));
            tblCart->setItem(row, 3, new QTableWidgetItem("Rs." + QString::number(lineTotal, 'f', 2)));
        }
        lblCartTotal->setText("Estimated Total: Rs." + QString::number(total, 'f', 2));
    }

    void refreshBill() {
        if (bill) { delete bill; bill = nullptr; }
        bill = new Bill(&cart, &currentCustomer);
        bill->calculateGrandTotal();
        lblSub->setText("Subtotal    : Rs." + QString::number(bill->getSubtotal(), 'f', 2));
        lblTax->setText("Tax         : Rs." + QString::number(bill->getTotalTax(), 'f', 2));
        lblDisc->setText("Discount    : Rs." + QString::number(bill->getTotalDiscount(), 'f', 2));
        lblCouponAmt->setText("Coupon Disc : Rs." + QString::number(bill->getCouponDiscount(), 'f', 2));
        lblGrand->setText("GRAND TOTAL : Rs." + QString::number(bill->getGrandTotal(), 'f', 2));
    }

    void setupUI() {
        setWindowTitle("Customer — Supermarket Billing System");
        setMinimumSize(860, 620);

        QHBoxLayout* topBar = new QHBoxLayout();
        lblWelcome = new QLabel("Welcome!", this);
        lblWelcome->setObjectName("lblWelcome");
        btnBrowse = new QPushButton("Browse Products", this);
        btnCartNav = new QPushButton("My Cart", this);
        btnLogout = new QPushButton("Logout", this);
        btnBrowse->setObjectName("btnNavBrowse");
        btnCartNav->setObjectName("btnNavCart");
        btnLogout->setObjectName("btnCustLogout");
        btnBrowse->setFixedHeight(36);  btnCartNav->setFixedHeight(36); btnLogout->setFixedHeight(36);
        btnBrowse->setCursor(Qt::PointingHandCursor);
        btnCartNav->setCursor(Qt::PointingHandCursor);
        btnLogout->setCursor(Qt::PointingHandCursor);
        topBar->addWidget(lblWelcome);
        topBar->addStretch();
        topBar->addWidget(btnBrowse);
        topBar->addWidget(btnCartNav);
        topBar->addWidget(btnLogout);

        pages = new QStackedWidget(this);
        pages->addWidget(buildBrowsePage());
        pages->addWidget(buildCartPage());
        pages->addWidget(buildCheckoutPage());

        QFrame* line = new QFrame(this);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        QLabel* lblHint = new QLabel(
            "Coupon codes: SAVE10 (10% off)  |  FLAT50 (Rs.50 off)  |  FRESH20 (20% off)", this);
        lblHint->setObjectName("lblHint");
        lblHint->setAlignment(Qt::AlignCenter);

        QVBoxLayout* mainLay = new QVBoxLayout(this);
        mainLay->setContentsMargins(16, 12, 16, 12);
        mainLay->setSpacing(10);
        mainLay->addLayout(topBar);
        mainLay->addWidget(line);
        mainLay->addWidget(pages);
        mainLay->addWidget(lblHint);
        setLayout(mainLay);

        connect(btnBrowse, &QPushButton::clicked, this, &CustomerScreen::showBrowse);
        connect(btnCartNav, &QPushButton::clicked, this, &CustomerScreen::showCartPage);
        connect(btnLogout, &QPushButton::clicked, this, &CustomerScreen::onLogoutClicked);
    }

    QWidget* buildBrowsePage() {
        QWidget* page = new QWidget();
        QVBoxLayout* lay = new QVBoxLayout(page);
        QLabel* lbl = new QLabel("Available Products", page);
        lbl->setObjectName("lblPageTitle");

        tblProducts = new QTableWidget(0, 6, page);
        tblProducts->setHorizontalHeaderLabels({ "Name","Category","Price","Final Price","Stock","Details" });
        tblProducts->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        tblProducts->setSelectionBehavior(QAbstractItemView::SelectRows);
        tblProducts->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblProducts->setAlternatingRowColors(true);
        tblProducts->verticalHeader()->setVisible(false);
        tblProducts->setMinimumHeight(340);

        QHBoxLayout* ctrlRow = new QHBoxLayout();
        QLabel* lblQty = new QLabel("Quantity:", page);
        spinQty = new QSpinBox(page);
        spinQty->setMinimum(1); spinQty->setMaximum(99); spinQty->setValue(1);
        spinQty->setFixedWidth(70);
        btnAddToCart = new QPushButton("Add to Cart", page);
        btnAddToCart->setObjectName("btnAddCart");
        btnAddToCart->setFixedHeight(38);
        btnAddToCart->setCursor(Qt::PointingHandCursor);
        ctrlRow->addStretch();
        ctrlRow->addWidget(lblQty);
        ctrlRow->addWidget(spinQty);
        ctrlRow->addWidget(btnAddToCart);

        lay->addWidget(lbl);
        lay->addWidget(tblProducts);
        lay->addLayout(ctrlRow);
        connect(btnAddToCart, &QPushButton::clicked, this, &CustomerScreen::onAddToCart);
        return page;
    }

    QWidget* buildCartPage() {
        QWidget* page = new QWidget();
        QVBoxLayout* lay = new QVBoxLayout(page);
        QLabel* lbl = new QLabel("My Cart", page);
        lbl->setObjectName("lblPageTitle");

        tblCart = new QTableWidget(0, 4, page);
        tblCart->setHorizontalHeaderLabels({ "Product","Qty","Unit Price","Line Total" });
        tblCart->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        tblCart->setSelectionBehavior(QAbstractItemView::SelectRows);
        tblCart->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tblCart->setAlternatingRowColors(true);
        tblCart->verticalHeader()->setVisible(false);
        tblCart->setMinimumHeight(300);

        lblCartTotal = new QLabel("Estimated Total: Rs.0.00", page);
        lblCartTotal->setObjectName("lblTotal");
        lblCartTotal->setAlignment(Qt::AlignRight);

        btnRemove = new QPushButton("Remove Selected", page);
        btnGoCheckout = new QPushButton("Proceed to Checkout", page);
        btnRemove->setObjectName("btnRemove");
        btnGoCheckout->setObjectName("btnCheckout");
        btnRemove->setFixedHeight(38); btnGoCheckout->setFixedHeight(38);
        btnRemove->setCursor(Qt::PointingHandCursor);
        btnGoCheckout->setCursor(Qt::PointingHandCursor);

        QHBoxLayout* btns = new QHBoxLayout();
        btns->addWidget(btnRemove); btns->addStretch(); btns->addWidget(btnGoCheckout);

        lay->addWidget(lbl);
        lay->addWidget(tblCart);
        lay->addWidget(lblCartTotal);
        lay->addLayout(btns);
        connect(btnRemove, &QPushButton::clicked, this, &CustomerScreen::onRemoveFromCart);
        connect(btnGoCheckout, &QPushButton::clicked, this, &CustomerScreen::showCheckout);
        return page;
    }

    QWidget* buildCheckoutPage() {
        QWidget* page = new QWidget();
        QVBoxLayout* lay = new QVBoxLayout(page);
        QLabel* lbl = new QLabel("Checkout & Pay", page);
        lbl->setObjectName("lblPageTitle");

        QGroupBox* grpBill = new QGroupBox("Bill Summary", page);
        QVBoxLayout* billLay = new QVBoxLayout(grpBill);
        lblSub = new QLabel("Subtotal    : Rs.0.00");
        lblTax = new QLabel("Tax         : Rs.0.00");
        lblDisc = new QLabel("Discount    : Rs.0.00");
        lblCouponAmt = new QLabel("Coupon Disc : Rs.0.00");
        lblGrand = new QLabel("GRAND TOTAL : Rs.0.00");
        lblGrand->setObjectName("lblGrand");
        QFrame* divLine = new QFrame(); divLine->setFrameShape(QFrame::HLine);
        billLay->addWidget(lblSub); billLay->addWidget(lblTax);
        billLay->addWidget(lblDisc); billLay->addWidget(lblCouponAmt);
        billLay->addWidget(divLine); billLay->addWidget(lblGrand);

        QGroupBox* grpCoupon = new QGroupBox("Apply Coupon", page);
        QHBoxLayout* couponLay = new QHBoxLayout(grpCoupon);
        txtCoupon = new QLineEdit(page);
        txtCoupon->setPlaceholderText("Enter coupon code e.g. SAVE10");
        txtCoupon->setFixedHeight(36);
        btnApplyCoupon = new QPushButton("Apply", page);
        btnApplyCoupon->setObjectName("btnApplyCoupon");
        btnApplyCoupon->setFixedHeight(36);
        btnApplyCoupon->setCursor(Qt::PointingHandCursor);
        couponLay->addWidget(txtCoupon); couponLay->addWidget(btnApplyCoupon);

        btnPlaceOrder = new QPushButton("Place Order & Pay", page);
        btnPlaceOrder->setObjectName("btnPlaceOrder");
        btnPlaceOrder->setFixedHeight(48);
        btnPlaceOrder->setCursor(Qt::PointingHandCursor);

        QPushButton* btnBackToCart = new QPushButton("Back to Cart", page);
        btnBackToCart->setObjectName("btnBackCart");
        btnBackToCart->setCursor(Qt::PointingHandCursor);

        QHBoxLayout* orderRow = new QHBoxLayout();
        orderRow->addWidget(btnBackToCart); orderRow->addStretch(); orderRow->addWidget(btnPlaceOrder);

        lay->addWidget(lbl); lay->addWidget(grpBill);
        lay->addWidget(grpCoupon); lay->addStretch(); lay->addLayout(orderRow);

        connect(btnApplyCoupon, &QPushButton::clicked, this, &CustomerScreen::onApplyCoupon);
        connect(btnPlaceOrder, &QPushButton::clicked, this, &CustomerScreen::onPlaceOrder);
        connect(btnBackToCart, &QPushButton::clicked, this, &CustomerScreen::showCartPage);
        return page;
    }

    void applyStyles() {
        setStyleSheet(R"(
            QWidget { background:#f4f6f9; font-family:Arial; font-size:13px; color:#2c3e50; }
            QLabel#lblWelcome   { font-size:15px; font-weight:bold; color:#34a853; }
            QLabel#lblPageTitle { font-size:16px; font-weight:bold; color:#1a73e8; margin-bottom:6px; }
            QLabel#lblTotal     { font-size:14px; font-weight:bold; color:#e65100; margin-top:4px; }
            QLabel#lblGrand     { font-size:15px; font-weight:bold; color:#34a853; }
            QLabel#lblHint      { font-size:11px; color:#888; }
            QGroupBox { font-weight:bold; border:1px solid #d0d7de; border-radius:8px;
                        margin-top:8px; padding-top:10px; background:white; }
            QGroupBox::title { subcontrol-origin:margin; left:12px; color:#1a73e8; }
            QLineEdit  { border:1px solid #ccc; border-radius:6px; padding:6px 10px; background:white; }
            QLineEdit:focus { border:1px solid #34a853; }
            QSpinBox   { border:1px solid #ccc; border-radius:6px; padding:4px 8px; background:white; }
            QPushButton#btnNavBrowse { background:#1a73e8; color:white; font-weight:bold; border:none; border-radius:6px; padding:6px 14px; }
            QPushButton#btnNavBrowse:hover { background:#1558b0; }
            QPushButton#btnNavCart   { background:#34a853; color:white; font-weight:bold; border:none; border-radius:6px; padding:6px 14px; }
            QPushButton#btnNavCart:hover   { background:#277a3e; }
            QPushButton#btnCustLogout { background:#6b7280; color:white; border:none; border-radius:6px; padding:5px 12px; }
            QPushButton#btnCustLogout:hover { background:#4b5563; }
            QPushButton#btnAddCart   { background:#1a73e8; color:white; font-weight:bold; border:none; border-radius:6px; padding:6px 16px; }
            QPushButton#btnAddCart:hover   { background:#1558b0; }
            QPushButton#btnRemove    { background:#dc2626; color:white; font-weight:bold; border:none; border-radius:6px; padding:6px 14px; }
            QPushButton#btnRemove:hover    { background:#b91c1c; }
            QPushButton#btnCheckout  { background:#34a853; color:white; font-weight:bold; border:none; border-radius:6px; padding:6px 18px; }
            QPushButton#btnCheckout:hover  { background:#277a3e; }
            QPushButton#btnApplyCoupon { background:#f59e0b; color:white; font-weight:bold; border:none; border-radius:6px; padding:6px 14px; }
            QPushButton#btnApplyCoupon:hover { background:#d97706; }
            QPushButton#btnPlaceOrder  { background:#34a853; color:white; font-size:15px; font-weight:bold; border:none; border-radius:8px; }
            QPushButton#btnPlaceOrder:hover  { background:#277a3e; }
            QPushButton#btnBackCart    { background:transparent; color:#1a73e8; border:1px solid #1a73e8; border-radius:6px; padding:6px 14px; }
            QPushButton#btnBackCart:hover    { background:#e8f0fe; }
            QPushButton { border:1px solid #ccc; border-radius:6px; padding:5px 12px; background:#f0f0f0; }
            QPushButton:hover { background:#e0e0e0; }
            QTableWidget { border:1px solid #d0d7de; border-radius:6px; background:white; alternate-background-color:#f8f9fc; }
            QTableWidget::item:selected { background:#d0e8ff; color:#1a1a1a; }
            QHeaderView::section { background:#1a73e8; color:white; font-weight:bold; padding:5px; border:none; }
        )");
    }
};

#endif // GUI_H