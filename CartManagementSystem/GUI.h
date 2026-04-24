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
#include "Globals.h"
#include "Products.h"
#include "Customer.h"
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

private:
    QLineEdit* txtUsername;
    QLineEdit* txtPassword;
    QPushButton* btnAdmin;
    QPushButton* btnCustomer;

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
        btnAdmin->setFixedHeight(46);
        btnCustomer->setFixedHeight(46);
        btnAdmin->setObjectName("btnAdmin");
        btnCustomer->setObjectName("btnCustomer");
        btnAdmin->setCursor(Qt::PointingHandCursor);
        btnCustomer->setCursor(Qt::PointingHandCursor);

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
        layout->addStretch();
        layout->addWidget(lblFooter);

        connect(btnAdmin, &QPushButton::clicked, this, &LoginScreen::onAdminClicked);
        connect(btnCustomer, &QPushButton::clicked, this, &LoginScreen::onCustomerClicked);

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
        )");
    }
};


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

    // Called by FileManager (Abdur-Rehman) to populate table from file
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

            // Color by stock level using Globals.h thresholds
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

#endif // GUI_H
