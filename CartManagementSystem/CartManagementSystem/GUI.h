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
        lblSub->setObjectName("lblSub");

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
        setMinimumSize(500, 480);
    }

    void applyStyles() {
        setStyleSheet(R"(
            QWidget {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                    stop:0 #f0f7ff, stop:0.5 #e8f4fd, stop:1 #f5f0ff);
                font-family:'Segoe UI'; font-size:13px; color:#1a202c;
            }
            QLabel#lblTitle {
                font-size:22px; font-weight:800; color:#4f46e5;
                letter-spacing:0.5px; padding:8px 0 2px 0;
            }
            QLabel#lblSub { color:#64748b; font-size:12px; }
            QLabel#lblFooter { font-size:10px; color:#94a3b8; }
            QLabel { color:#374151; }
            QLineEdit {
                border: 2px solid #e2e8f0;
                border-radius: 10px;
                padding: 10px 14px;
                background: white;
                color: #1a202c;
                font-size: 13px;
            }
            QLineEdit:focus {
                border: 2px solid #4f46e5;
                background: #fafbff;
            }
            QPushButton#btnAdmin {
                background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                    stop:0 #4f46e5, stop:1 #7c3aed);
                color:white; font-weight:700; font-size:13px;
                border:none; border-radius:10px; padding:12px 20px;
            }
            QPushButton#btnAdmin:hover {
                background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                    stop:0 #4338ca, stop:1 #6d28d9);
            }
            QPushButton#btnCustomer {
                background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                    stop:0 #059669, stop:1 #0d9488);
                color:white; font-weight:700; font-size:13px;
                border:none; border-radius:10px; padding:12px 20px;
            }
            QPushButton#btnCustomer:hover {
                background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                    stop:0 #047857, stop:1 #0f766e);
            }
            QPushButton#btnRegister {
                background: transparent;
                color:#4f46e5;
                border: 2px solid #c7d2fe;
                border-radius:10px; font-size:12px; padding:8px;
            }
            QPushButton#btnRegister:hover {
                background:#eef2ff; border-color:#4f46e5;
            }
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
            QWidget {
                background: #f8fafc;
                font-family:'Segoe UI'; font-size:13px; color:#1e293b;
            }
            QLabel#lblAdminTitle {
                font-size:18px; font-weight:800; color:#4f46e5;
                letter-spacing:0.5px;
            }
            QGroupBox {
                font-weight:700; font-size:11px; letter-spacing:1px;
                border: 2px solid #e2e8f0;
                border-radius:12px;
                margin-top:12px; padding-top:16px;
                background: white;
                color:#64748b;
            }
            QGroupBox::title {
                subcontrol-origin:margin; left:14px; top:0px;
                color:#4f46e5; font-size:10px; letter-spacing:2px;
                padding: 0 6px;
            }
            QLineEdit, QComboBox {
                border: 2px solid #e2e8f0;
                border-radius:8px;
                padding:7px 10px;
                background:white;
                color:#1e293b;
                min-height:28px;
            }
            QLineEdit:focus, QComboBox:focus {
                border: 2px solid #4f46e5;
                background:#fafbff;
            }
            QComboBox::drop-down { border:none; padding-right:8px; }
            QComboBox QAbstractItemView {
                background:white; color:#1e293b;
                selection-background-color:#eef2ff;
                selection-color:#4f46e5;
                border:1px solid #e2e8f0;
                border-radius:8px;
            }
            QPushButton#btnAdd {
                background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                    stop:0 #059669, stop:1 #0d9488);
                color:white; font-weight:700;
                border:none; border-radius:8px; padding:8px 18px;
            }
            QPushButton#btnAdd:hover { background:#047857; }
            QPushButton#btnEdit {
                background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                    stop:0 #f59e0b, stop:1 #ef4444);
                color:white; font-weight:700;
                border:none; border-radius:8px; padding:8px 18px;
            }
            QPushButton#btnEdit:hover { background:#d97706; }
            QPushButton#btnDelete {
                background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                    stop:0 #ef4444, stop:1 #dc2626);
                color:white; font-weight:700;
                border:none; border-radius:8px; padding:8px 18px;
            }
            QPushButton#btnDelete:hover { background:#b91c1c; }
            QPushButton#btnLogout {
                background:#f1f5f9; color:#64748b;
                border:2px solid #e2e8f0;
                border-radius:8px; padding:6px 14px; font-weight:600;
            }
            QPushButton#btnLogout:hover { background:#fee2e2; color:#ef4444; border-color:#fca5a5; }
            QPushButton {
                background:white; color:#475569;
                border:2px solid #e2e8f0; border-radius:8px; padding:6px 14px;
            }
            QPushButton:hover { background:#f8fafc; border-color:#cbd5e1; }
            QPushButton:disabled { background:#f1f5f9; color:#cbd5e1; border-color:#e2e8f0; }
            QTableWidget {
                border:2px solid #e2e8f0; border-radius:10px;
                background:white; gridline-color:#f1f5f9;
                alternate-background-color:#f8faff;
            }
            QTableWidget::item { padding:7px; border:none; color:#1e293b; }
            QTableWidget::item:selected {
                background:#eef2ff; color:#4f46e5;
            }
            QHeaderView::section {
                background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                    stop:0 #4f46e5, stop:1 #7c3aed);
                color:white; font-weight:700; font-size:11px;
                letter-spacing:0.5px; padding:9px 6px;
                border:none; border-right:1px solid #6366f1;
            }
            QScrollBar:vertical {
                background:#f1f5f9; width:8px; border-radius:4px;
            }
            QScrollBar::handle:vertical {
                background:#cbd5e1; border-radius:4px; min-height:20px;
            }
            QScrollBar::handle:vertical:hover { background:#4f46e5; }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height:0; }
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
            QWidget {
                background: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                    stop:0 #f0f7ff, stop:0.5 #f5f0ff, stop:1 #f0fff4);
                font-family:'Segoe UI'; font-size:13px; color:#1a202c;
            }
            QLabel#lblRegTitle {
                font-size:21px; font-weight:800; color:#4f46e5;
                letter-spacing:0.5px; padding:6px 0 2px 0;
            }
            QLabel { color:#374151; }
            QLineEdit {
                border:2px solid #e2e8f0; border-radius:10px;
                padding:9px 14px; background:white;
                color:#1a202c; font-size:13px;
            }
            QLineEdit:focus {
                border:2px solid #4f46e5; background:#fafbff;
            }
            QPushButton#btnCreateAcc {
                background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                    stop:0 #059669, stop:1 #0d9488);
                color:white; font-weight:800; font-size:14px;
                border:none; border-radius:10px; padding:11px;
            }
            QPushButton#btnCreateAcc:hover { background:#047857; }
            QPushButton#btnBackLogin {
                background:transparent; color:#4f46e5;
                border:2px solid #c7d2fe; border-radius:10px; padding:8px;
                font-weight:600;
            }
            QPushButton#btnBackLogin:hover { background:#eef2ff; border-color:#4f46e5; }
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
        : QWidget(parent), bill(nullptr), productCount(0), receiptCounter(1)
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
        // Set receiptCounter based on existing sales log entries to avoid duplicate IDs
        receiptCounter = FileManager::getNextReceiptCounter();
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

        // Decrement stock for each purchased item
        Product* allProds[MAX_PRODUCTS];
        int prodCount = FileManager::loadProducts(allProds, MAX_PRODUCTS);
        for (int i = 0; i < cart.getItemCount(); i++) {
            CartItem it = cart.getItem(i);
            FileManager::decrementStock(allProds, prodCount,
                it.product->getProductID(), it.quantity);
        }

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
    Product* productList[MAX_PRODUCTS];
    int       productCount;
    int       receiptCounter;
    Bill* bill;

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
            QWidget {
                background: #f8fafc;
                font-family:'Segoe UI'; font-size:13px; color:#1e293b;
            }
            QLabel#lblWelcome {
                font-size:15px; font-weight:800;
                color:#4f46e5; letter-spacing:0.3px;
            }
            QLabel#lblPageTitle {
                font-size:17px; font-weight:800; color:#1e293b;
                letter-spacing:0.3px; margin-bottom:4px;
            }
            QLabel#lblTotal {
                font-size:14px; font-weight:700; color:#f59e0b; margin-top:4px;
            }
            QLabel#lblGrand {
                font-size:16px; font-weight:800; color:#059669;
            }
            QLabel#lblHint { font-size:11px; color:#94a3b8; }
            QLabel { color:#475569; }
            QGroupBox {
                font-weight:700; font-size:10px; letter-spacing:2px;
                border:2px solid #e2e8f0; border-radius:12px;
                margin-top:12px; padding-top:16px;
                background:white;
            }
            QGroupBox::title {
                subcontrol-origin:margin; left:14px;
                color:#4f46e5; padding:0 6px;
            }
            QLineEdit {
                border:2px solid #e2e8f0; border-radius:8px;
                padding:8px 12px; background:white;
                color:#1e293b; font-size:13px;
            }
            QLineEdit:focus { border:2px solid #4f46e5; background:#fafbff; }
            QSpinBox {
                border:2px solid #e2e8f0; border-radius:8px;
                padding:6px 10px; background:white; color:#1e293b;
            }
            QSpinBox:focus { border:2px solid #4f46e5; }
            QPushButton#btnNavBrowse {
                background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                    stop:0 #4f46e5, stop:1 #7c3aed);
                color:white; border:none;
                border-radius:8px; padding:8px 18px; font-weight:700;
            }
            QPushButton#btnNavBrowse:hover { background:#4338ca; }
            QPushButton#btnNavCart {
                background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                    stop:0 #059669, stop:1 #0d9488);
                color:white; border:none;
                border-radius:8px; padding:8px 18px; font-weight:700;
            }
            QPushButton#btnNavCart:hover { background:#047857; }
            QPushButton#btnCustLogout {
                background:#f1f5f9; color:#64748b;
                border:2px solid #e2e8f0;
                border-radius:8px; padding:7px 16px; font-weight:600;
            }
            QPushButton#btnCustLogout:hover { background:#fee2e2; color:#ef4444; border-color:#fca5a5; }
            QPushButton#btnAddCart {
                background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                    stop:0 #4f46e5, stop:1 #7c3aed);
                color:white; font-weight:700;
                border:none; border-radius:8px; padding:9px 22px;
            }
            QPushButton#btnAddCart:hover { background:#4338ca; }
            QPushButton#btnRemove {
                background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                    stop:0 #ef4444, stop:1 #dc2626);
                color:white; font-weight:700;
                border:none; border-radius:8px; padding:8px 20px;
            }
            QPushButton#btnRemove:hover { background:#b91c1c; }
            QPushButton#btnCheckout {
                background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                    stop:0 #059669, stop:1 #0d9488);
                color:white; font-weight:700;
                border:none; border-radius:8px; padding:8px 24px;
            }
            QPushButton#btnCheckout:hover { background:#047857; }
            QPushButton#btnApplyCoupon {
                background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                    stop:0 #f59e0b, stop:1 #ef4444);
                color:white; font-weight:700;
                border:none; border-radius:8px; padding:8px 18px;
            }
            QPushButton#btnApplyCoupon:hover { background:#d97706; }
            QPushButton#btnPlaceOrder {
                background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                    stop:0 #059669, stop:1 #0d9488);
                color:white; font-size:15px; font-weight:800;
                border:none; border-radius:10px;
            }
            QPushButton#btnPlaceOrder:hover { background:#047857; }
            QPushButton#btnBackCart {
                background:white; color:#4f46e5;
                border:2px solid #c7d2fe; border-radius:8px; padding:7px 18px;
                font-weight:600;
            }
            QPushButton#btnBackCart:hover { background:#eef2ff; border-color:#4f46e5; }
            QPushButton {
                background:white; color:#475569;
                border:2px solid #e2e8f0; border-radius:8px; padding:6px 14px;
            }
            QPushButton:hover { background:#f8fafc; border-color:#4f46e5; color:#4f46e5; }
            QTableWidget {
                border:2px solid #e2e8f0; border-radius:10px;
                background:white; gridline-color:#f1f5f9;
                alternate-background-color:#f8faff;
            }
            QTableWidget::item { padding:8px; border:none; color:#1e293b; }
            QTableWidget::item:selected { background:#eef2ff; color:#4f46e5; }
            QHeaderView::section {
                background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                    stop:0 #4f46e5, stop:1 #7c3aed);
                color:white; font-weight:700; font-size:11px;
                letter-spacing:0.5px; padding:9px 6px;
                border:none; border-right:1px solid #6366f1;
            }
            QFrame[frameShape="4"] { color:#e2e8f0; }
            QScrollBar:vertical {
                background:#f1f5f9; width:8px; border-radius:4px;
            }
            QScrollBar::handle:vertical {
                background:#cbd5e1; border-radius:4px; min-height:20px;
            }
            QScrollBar::handle:vertical:hover { background:#4f46e5; }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height:0; }
        )");
    }
};
#endif // GUI_H