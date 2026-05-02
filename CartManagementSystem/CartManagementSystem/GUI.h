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
#include <QDate>
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
	void onRegisterClicked() { emit registerRequested(); }

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

		QLabel* lblFooter = new QLabel("Team 03", this);
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

	void loadProducts(Product* products[], int count) {
		for (int i = 0; i < productCount; i++) delete productList[i];
		productCount = 0;
		for (int i = 0; i < count && i < MAX_PRODUCTS; i++)
			productList[productCount++] = products[i];
		refreshTable();
	}

signals:
	void logoutRequested();
	void goToDashboard();
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

	void onDashboardClicked() { emit goToDashboard(); }

private:
	QGroupBox* grpForm;
	QLineEdit* txtID, * txtName, * txtPrice, * txtStock, * txtExtra, * txtExtra2;
	QLabel* lblExtra, * lblExtra2;
	QComboBox* cmbCategory;
	QTableWidget* tblProducts;
	QPushButton* btnAdd, * btnEdit, * btnDelete, * btnClear, * btnLogout, * btnDashboard;
	Product* productList[MAX_PRODUCTS];
	int productCount;
	int editingRow;

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
		if (cat == QString::fromStdString(CAT_PERISHABLE))
			return new PerishableProduct(id, name, price, stock, txtExtra->text().trimmed().toStdString());
		else if (cat == QString::fromStdString(CAT_ELECTRONIC)) {
			int w = txtExtra2->text().isEmpty() ? 0 : txtExtra2->text().toInt();
			return new ElectronicProduct(id, name, price, stock, txtExtra->text().trimmed().toStdString(), w);
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
		// Top bar
		QHBoxLayout* header = new QHBoxLayout();
		QLabel* lblTitle = new QLabel("ADMIN — Product Catalog", this);
		lblTitle->setObjectName("lblAdminTitle");
		btnDashboard = new QPushButton("Dashboard", this);
		btnDashboard->setObjectName("btnDashboard");
		btnDashboard->setFixedWidth(100);
		btnDashboard->setCursor(Qt::PointingHandCursor);
		btnLogout = new QPushButton(QString::fromStdString(LABEL_BTN_LOGOUT), this);
		btnLogout->setObjectName("btnLogout");
		btnLogout->setFixedWidth(90);
		btnLogout->setCursor(Qt::PointingHandCursor);
		header->addWidget(lblTitle);
		header->addStretch();
		header->addWidget(btnDashboard);
		header->addSpacing(8);
		header->addWidget(btnLogout);

		// Form
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
		grid->addWidget(lblExtra, 2, 2);  grid->addWidget(txtExtra, 2, 3);
		grid->addWidget(lblExtra2, 3, 2);  grid->addWidget(txtExtra2, 3, 3);
		btnAdd = new QPushButton("Add Product"); btnAdd->setObjectName("btnAdd");
		btnEdit = new QPushButton("Save Edit");   btnEdit->setObjectName("btnEdit"); btnEdit->setEnabled(false);
		btnClear = new QPushButton("Clear");
		btnAdd->setCursor(Qt::PointingHandCursor);
		btnEdit->setCursor(Qt::PointingHandCursor);
		btnClear->setCursor(Qt::PointingHandCursor);
		QHBoxLayout* fbtns = new QHBoxLayout();
		fbtns->addWidget(btnAdd); fbtns->addWidget(btnEdit); fbtns->addWidget(btnClear); fbtns->addStretch();
		grid->addLayout(fbtns, 4, 0, 1, 4);

		// Table
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
		btnDelete = new QPushButton("Delete Selected"); btnDelete->setObjectName("btnDelete");
		btnDelete->setEnabled(false); btnDelete->setCursor(Qt::PointingHandCursor);
		QHBoxLayout* tbtns = new QHBoxLayout();
		tbtns->addStretch(); tbtns->addWidget(btnDelete);
		tlay->addWidget(tblProducts); tlay->addLayout(tbtns);

		QVBoxLayout* mainLay = new QVBoxLayout(this);
		mainLay->setContentsMargins(18, 14, 18, 14); mainLay->setSpacing(12);
		mainLay->addLayout(header);
		mainLay->addWidget(grpForm);
		mainLay->addWidget(grpTable);
		setLayout(mainLay);
		setWindowTitle("Admin — Product Catalog");
		setMinimumSize(800, 650);

		connect(cmbCategory, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ProductCatalogScreen::onCategoryChanged);
		connect(btnAdd, &QPushButton::clicked, this, &ProductCatalogScreen::onAddClicked);
		connect(btnEdit, &QPushButton::clicked, this, &ProductCatalogScreen::onEditClicked);
		connect(btnDelete, &QPushButton::clicked, this, &ProductCatalogScreen::onDeleteClicked);
		connect(btnClear, &QPushButton::clicked, this, &ProductCatalogScreen::clearForm);
		connect(btnLogout, &QPushButton::clicked, this, &ProductCatalogScreen::onLogoutClicked);
		connect(btnDashboard, &QPushButton::clicked, this, &ProductCatalogScreen::onDashboardClicked);
		connect(tblProducts, &QTableWidget::itemSelectionChanged, this, &ProductCatalogScreen::onRowSelected);
	}

	void applyStyles() {
		setStyleSheet(R"(
            QWidget { background:#f4f6f9; font-family:Arial; font-size:13px; color:#2c3e50; }
            QLabel#lblAdminTitle { font-size:16px; font-weight:bold; color:#1a73e8; }
            QGroupBox { font-weight:bold; border:1px solid #d0d7de; border-radius:8px; margin-top:8px; padding-top:10px; background:white; }
            QGroupBox::title { subcontrol-origin:margin; left:12px; color:#1a73e8; }
            QLineEdit, QComboBox { border:1px solid #ccc; border-radius:5px; padding:5px 8px; background:#fafafa; min-height:26px; }
            QLineEdit:focus, QComboBox:focus { border:1px solid #1a73e8; background:white; }
            QPushButton#btnAdd    { background:#1a73e8; color:white; font-weight:bold; border:none; border-radius:6px; padding:7px 16px; }
            QPushButton#btnAdd:hover    { background:#1558b0; }
            QPushButton#btnEdit   { background:#f59e0b; color:white; font-weight:bold; border:none; border-radius:6px; padding:7px 16px; }
            QPushButton#btnEdit:hover   { background:#d97706; }
            QPushButton#btnDelete { background:#dc2626; color:white; font-weight:bold; border:none; border-radius:6px; padding:7px 16px; }
            QPushButton#btnDelete:hover { background:#b91c1c; }
            QPushButton#btnLogout { background:#6b7280; color:white; border:none; border-radius:6px; padding:5px 12px; }
            QPushButton#btnLogout:hover { background:#4b5563; }
            QPushButton#btnDashboard { background:#1a73e8; color:white; border:none; border-radius:6px; padding:5px 12px; }
            QPushButton#btnDashboard:hover { background:#1558b0; }
            QPushButton { border:1px solid #ccc; border-radius:6px; padding:5px 12px; background:#f0f0f0; }
            QPushButton:hover { background:#e0e0e0; }
            QTableWidget { border:1px solid #d0d7de; border-radius:6px; background:white; alternate-background-color:#f8f9fc; }
            QTableWidget::item:selected { background:#d0e8ff; color:#1a1a1a; }
            QHeaderView::section { background:#1a73e8; color:white; font-weight:bold; padding:5px; border:none; }
        )");
	}
};


// ============================================================
//  REGISTER SCREEN
// ============================================================
class RegisterScreen : public QWidget {
	Q_OBJECT
public:
	explicit RegisterScreen(QWidget* parent = nullptr) : QWidget(parent) {
		setupUI(); applyStyles();
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
		if (name.isEmpty() || username.isEmpty() || password.isEmpty() || phone.isEmpty()) {
			QMessageBox::warning(this, "Error", "Please fill in all fields."); return;
		}
		if (password != confirm) {
			QMessageBox::warning(this, "Error", "Passwords do not match!"); return;
		}
		if (password.length() < 6) {
			QMessageBox::warning(this, "Error", "Password must be at least 6 characters."); return;
		}
		Customer existing[MAX_CUSTOMERS];
		int count = FileManager::loadCustomers(existing, MAX_CUSTOMERS);
		for (int i = 0; i < count; i++) {
			if (QString::fromStdString(existing[i].getUsername()) == username) {
				QMessageBox::warning(this, "Error", "Username already taken."); return;
			}
		}
		int newID = 101;
		for (int i = 0; i < count; i++)
			if (existing[i].getCustomerID() >= newID) newID = existing[i].getCustomerID() + 1;
		Customer newCust(newID, name.toStdString(), username.toStdString(),
			password.toStdString(), phone.toStdString());
		FileManager::saveNewCustomer(newCust, password.toStdString());
		QMessageBox::information(this, "Success", "Account created! Welcome, " + name + "!\nYou can now login.");
		emit registrationDone(username);
	}
	void onBackClicked() { emit backToLogin(); }

private:
	QLineEdit* txtName, * txtUsername, * txtPassword, * txtConfirm, * txtPhone;
	QPushButton* btnRegister, * btnBack;

	void setupUI() {
		QVBoxLayout* layout = new QVBoxLayout(this);
		layout->setContentsMargins(60, 30, 60, 30); layout->setSpacing(12);
		QLabel* lblTitle = new QLabel("Create New Account", this);
		lblTitle->setAlignment(Qt::AlignCenter); lblTitle->setObjectName("lblRegTitle");
		txtName = new QLineEdit(this); txtName->setPlaceholderText("Full Name");              txtName->setFixedHeight(36);
		txtUsername = new QLineEdit(this); txtUsername->setPlaceholderText("Username");           txtUsername->setFixedHeight(36);
		txtPassword = new QLineEdit(this); txtPassword->setPlaceholderText("Password (min 6)");  txtPassword->setFixedHeight(36); txtPassword->setEchoMode(QLineEdit::Password);
		txtConfirm = new QLineEdit(this); txtConfirm->setPlaceholderText("Confirm Password");   txtConfirm->setFixedHeight(36);  txtConfirm->setEchoMode(QLineEdit::Password);
		txtPhone = new QLineEdit(this); txtPhone->setPlaceholderText("Phone (03001234567)");  txtPhone->setFixedHeight(36);
		btnRegister = new QPushButton("Create Account", this); btnRegister->setFixedHeight(44); btnRegister->setObjectName("btnCreateAcc"); btnRegister->setCursor(Qt::PointingHandCursor);
		btnBack = new QPushButton("← Back to Login", this); btnBack->setObjectName("btnBackLogin"); btnBack->setCursor(Qt::PointingHandCursor);
		layout->addStretch();
		layout->addWidget(lblTitle); layout->addSpacing(10);
		layout->addWidget(new QLabel("Full Name:", this));        layout->addWidget(txtName);
		layout->addWidget(new QLabel("Username:", this));         layout->addWidget(txtUsername);
		layout->addWidget(new QLabel("Password:", this));         layout->addWidget(txtPassword);
		layout->addWidget(new QLabel("Confirm Password:", this)); layout->addWidget(txtConfirm);
		layout->addWidget(new QLabel("Phone:", this));            layout->addWidget(txtPhone);
		layout->addSpacing(10);
		layout->addWidget(btnRegister); layout->addWidget(btnBack);
		layout->addStretch();
		connect(btnRegister, &QPushButton::clicked, this, &RegisterScreen::onRegisterClicked);
		connect(btnBack, &QPushButton::clicked, this, &RegisterScreen::onBackClicked);
		setWindowTitle("Register"); setMinimumSize(460, 520);
	}

	void applyStyles() {
		setStyleSheet(R"(
            QWidget { background:#f4f6f9; font-family:Arial; font-size:13px; color:#2c3e50; }
            QLabel#lblRegTitle { font-size:19px; font-weight:bold; color:#34a853; padding:6px; }
            QLineEdit { border:1px solid #ccc; border-radius:6px; padding:6px 10px; background:white; }
            QLineEdit:focus { border:1px solid #34a853; }
            QPushButton#btnCreateAcc       { background:#34a853; color:white; font-weight:bold; border:none; border-radius:8px; }
            QPushButton#btnCreateAcc:hover { background:#277a3e; }
            QPushButton#btnBackLogin       { background:transparent; color:#1a73e8; border:1px solid #1a73e8; border-radius:8px; }
            QPushButton#btnBackLogin:hover { background:#e8f0fe; }
        )");
	}
};


// ============================================================
//  CUSTOMER SCREEN
// ============================================================
class CustomerScreen : public QWidget {
	Q_OBJECT
public:
	explicit CustomerScreen(QWidget* parent = nullptr)
		: QWidget(parent), productCount(0), receiptCounter(1), bill(nullptr)
	{
		for (int i = 0; i < MAX_PRODUCTS; i++) productList[i] = nullptr;
		setupUI(); applyStyles();
	}

	~CustomerScreen() {
		for (int i = 0; i < productCount; i++) { delete productList[i]; productList[i] = nullptr; }
		if (bill) { delete bill; bill = nullptr; }
	}

	void setCustomer(const Customer& c) {
		currentCustomer = c;
		lblWelcome->setText("Welcome, " + QString::fromStdString(c.getName()) + "!");
		cart = Cart();
		if (bill) { delete bill; bill = nullptr; }
		txtCoupon->clear();
		refreshCart(); showPage(0);
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
	void refundRequested();

private slots:
	void showBrowse() { showPage(0); }
	void showCartPage() { refreshCart(); showPage(1); }
	void showCheckout() {
		if (cart.getItemCount() == 0) { QMessageBox::warning(this, "Empty Cart", "Add items first!"); return; }
		refreshBill(); showPage(2);
	}
	void onAddToCart() {
		int row = tblProducts->currentRow();
		if (row < 0) { QMessageBox::warning(this, "Select", "Please select a product first."); return; }
		Product* p = productList[row];
		if (p->getStockQty() <= 0) { QMessageBox::warning(this, "Out of Stock", "This product is out of stock."); return; }
		int qty = spinQty->value();
		if (qty > p->getStockQty()) { QMessageBox::warning(this, "Stock", "Only " + QString::number(p->getStockQty()) + " units available."); return; }
		cart.addItem(p, qty);
		QMessageBox::information(this, "Added", QString::fromStdString(p->getName()) + " x" + QString::number(qty) + " added!");
		spinQty->setValue(1); updateCartBadge();
	}
	void onRemoveFromCart() {
		int row = tblCart->currentRow();
		if (row < 0) return;
		CartItem item = cart.getItem(row);
		cart.removeItem(item.product->getProductID());
		refreshCart(); updateCartBadge();
	}
	void onUpdateCartQty() {
		int row = tblCart->currentRow();
		if (row < 0) {
			QMessageBox::warning(this, "Select Item", "Please select a cart item to update its quantity.");
			return;
		}
		CartItem item = cart.getItem(row);
		int newQty = spinCartQty->value();
		// Check stock limit
		if (newQty > item.product->getStockQty()) {
			QMessageBox::warning(this, "Stock Limit",
				"Only " + QString::number(item.product->getStockQty()) + " units available for " +
				QString::fromStdString(item.product->getName()) + ".");
			return;
		}
		cart.updateQty(item.product->getProductID(), newQty);
		refreshCart(); updateCartBadge();
	}
	void onApplyCoupon() {
		QString code = txtCoupon->text().trimmed().toUpper();
		if (code.isEmpty()) { QMessageBox::warning(this, "Coupon", "Enter a coupon code."); return; }
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

		// Decrement stock for each item purchased
		for (int i = 0; i < cart.getItemCount(); i++) {
			CartItem it = cart.getItem(i);
			FileManager::decrementStock(
				productList,
				productCount,
				it.product->getProductID(),
				it.quantity
			);
		}

		FileManager::saveReceipt(*bill, cart, currentCustomer.getName(), receiptID, timestamp);
		QString summary;
		summary += "Receipt ID : " + QString::fromStdString(receiptID) + "\n";
		summary += "Customer   : " + QString::fromStdString(currentCustomer.getName()) + "\n";
		summary += "Date/Time  : " + QString::fromStdString(timestamp) + "\n";
		summary += "──────────────────────────────\n";
		for (int i = 0; i < cart.getItemCount(); i++) {
			CartItem it = cart.getItem(i);
			double lt = it.product->getPrice() * it.quantity;
			summary += QString::fromStdString(it.product->getName()) + " x" + QString::number(it.quantity) + "  =  Rs." + QString::number(lt, 'f', 2) + "\n";
		}
		summary += "──────────────────────────────\n";
		summary += "Subtotal   : Rs." + QString::number(bill->getSubtotal(), 'f', 2) + "\n";
		summary += "Tax        : Rs." + QString::number(bill->getTotalTax(), 'f', 2) + "\n";
		summary += "Discount   : Rs." + QString::number(bill->getTotalDiscount(), 'f', 2) + "\n";
		if (bill->isCouponApplied())
			summary += "Coupon     : -Rs." + QString::number(bill->getCouponDiscount(), 'f', 2) + "\n";
		summary += "──────────────────────────────\n";
		summary += "GRAND TOTAL: Rs." + QString::number(grand, 'f', 2) + "\n\nThank you for shopping!";
		QMessageBox::information(this, "Order Placed!", summary);
		cart.clearCart();
		if (bill) { delete bill; bill = nullptr; }
		txtCoupon->clear(); updateCartBadge(); showPage(0);
	}
	void onLogoutClicked() {
		if (QMessageBox::question(this, "Logout", "Sure you want to logout?",
			QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
			cart.clearCart();
			if (bill) { delete bill; bill = nullptr; }
			txtCoupon->clear(); updateCartBadge();
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
	QPushButton* btnBrowse, * btnCartNav, * btnRefundNav, * btnLogout;
	QTableWidget* tblProducts;
	QSpinBox* spinQty;
	QPushButton* btnAddToCart;
	QTableWidget* tblCart;
	QLabel* lblCartTotal;
	QPushButton* btnRemove, * btnGoCheckout;
	QSpinBox* spinCartQty;
	QPushButton* btnUpdateQty;
	QLabel* lblSub, * lblTax, * lblDisc, * lblCouponAmt, * lblGrand;
	QLineEdit* txtCoupon;
	QPushButton* btnApplyCoupon, * btnPlaceOrder;

	void showPage(int idx) { pages->setCurrentIndex(idx); }
	void updateCartBadge() {
		int n = cart.getItemCount();
		btnCartNav->setText(n > 0 ? "My Cart (" + QString::number(n) + ")" : "My Cart");
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
			if (cat == CAT_PERISHABLE) { auto* pp = dynamic_cast<PerishableProduct*>(p); if (pp) extra = "Exp: " + QString::fromStdString(pp->getExpiryDate()); }
			else if (cat == CAT_ELECTRONIC) { auto* ep = dynamic_cast<ElectronicProduct*>(p); if (ep) extra = QString::fromStdString(ep->getBrand()) + " | " + QString::number(ep->getWarrantyMonths()) + "mo"; }
			else { auto* gp = dynamic_cast<GroceryProduct*>(p); if (gp) extra = QString::number(gp->getWeightKg(), 'f', 2) + " kg"; }
			tblProducts->setItem(row, 5, new QTableWidgetItem(extra));
			QColor c = (p->getStockQty() == 0) ? QColor("#ffe4e4") : Qt::white;
			for (int col = 0; col < 6; col++) if (tblProducts->item(row, col)) tblProducts->item(row, col)->setBackground(c);
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
		lblWelcome = new QLabel("Welcome!", this); lblWelcome->setObjectName("lblWelcome");
		btnBrowse = new QPushButton("Browse Products", this); btnBrowse->setObjectName("btnNavBrowse"); btnBrowse->setFixedHeight(36); btnBrowse->setCursor(Qt::PointingHandCursor);
		btnCartNav = new QPushButton("My Cart", this);         btnCartNav->setObjectName("btnNavCart");   btnCartNav->setFixedHeight(36); btnCartNav->setCursor(Qt::PointingHandCursor);
		btnRefundNav = new QPushButton("Request Refund", this); btnRefundNav->setObjectName("btnNavRefund"); btnRefundNav->setFixedHeight(36); btnRefundNav->setCursor(Qt::PointingHandCursor);
		btnLogout = new QPushButton("Logout", this);          btnLogout->setObjectName("btnCustLogout"); btnLogout->setFixedHeight(36);  btnLogout->setCursor(Qt::PointingHandCursor);
		topBar->addWidget(lblWelcome); topBar->addStretch();
		topBar->addWidget(btnBrowse); topBar->addWidget(btnCartNav); topBar->addWidget(btnRefundNav); topBar->addWidget(btnLogout);
		pages = new QStackedWidget(this);
		pages->addWidget(buildBrowsePage());
		pages->addWidget(buildCartPage());
		pages->addWidget(buildCheckoutPage());
		QFrame* line = new QFrame(this); line->setFrameShape(QFrame::HLine); line->setFrameShadow(QFrame::Sunken);
		QLabel* lblHint = new QLabel("Coupons: SAVE10 (10% off)  |  FLAT50 (Rs.50 off)  |  FRESH20 (20% off perishables)", this);
		lblHint->setObjectName("lblHint"); lblHint->setAlignment(Qt::AlignCenter);
		QVBoxLayout* mainLay = new QVBoxLayout(this);
		mainLay->setContentsMargins(16, 12, 16, 12); mainLay->setSpacing(10);
		mainLay->addLayout(topBar); mainLay->addWidget(line); mainLay->addWidget(pages); mainLay->addWidget(lblHint);
		setLayout(mainLay);
		connect(btnBrowse, &QPushButton::clicked, this, &CustomerScreen::showBrowse);
		connect(btnCartNav, &QPushButton::clicked, this, &CustomerScreen::showCartPage);
		connect(btnRefundNav, &QPushButton::clicked, this, [this]() { emit refundRequested(); });
		connect(btnLogout, &QPushButton::clicked, this, &CustomerScreen::onLogoutClicked);
	}

	QWidget* buildBrowsePage() {
		QWidget* page = new QWidget();
		QVBoxLayout* lay = new QVBoxLayout(page);
		QLabel* lbl = new QLabel("Available Products", page); lbl->setObjectName("lblPageTitle");
		tblProducts = new QTableWidget(0, 6, page);
		tblProducts->setHorizontalHeaderLabels({ "Name","Category","Price","Final Price","Stock","Details" });
		tblProducts->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		tblProducts->setSelectionBehavior(QAbstractItemView::SelectRows);
		tblProducts->setEditTriggers(QAbstractItemView::NoEditTriggers);
		tblProducts->setAlternatingRowColors(true);
		tblProducts->verticalHeader()->setVisible(false);
		tblProducts->setMinimumHeight(340);
		QHBoxLayout* ctrlRow = new QHBoxLayout();
		spinQty = new QSpinBox(page); spinQty->setMinimum(1); spinQty->setMaximum(99); spinQty->setValue(1); spinQty->setFixedWidth(70);
		btnAddToCart = new QPushButton("Add to Cart", page); btnAddToCart->setObjectName("btnAddCart"); btnAddToCart->setFixedHeight(38); btnAddToCart->setCursor(Qt::PointingHandCursor);
		ctrlRow->addStretch(); ctrlRow->addWidget(new QLabel("Quantity:", page)); ctrlRow->addWidget(spinQty); ctrlRow->addWidget(btnAddToCart);
		lay->addWidget(lbl); lay->addWidget(tblProducts); lay->addLayout(ctrlRow);
		connect(btnAddToCart, &QPushButton::clicked, this, &CustomerScreen::onAddToCart);
		return page;
	}

	QWidget* buildCartPage() {
		QWidget* page = new QWidget();
		QVBoxLayout* lay = new QVBoxLayout(page);
		QLabel* lbl = new QLabel("My Cart", page); lbl->setObjectName("lblPageTitle");
		tblCart = new QTableWidget(0, 4, page);
		tblCart->setHorizontalHeaderLabels({ "Product","Qty","Unit Price","Line Total" });
		tblCart->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		tblCart->setSelectionBehavior(QAbstractItemView::SelectRows);
		tblCart->setEditTriggers(QAbstractItemView::NoEditTriggers);
		tblCart->setAlternatingRowColors(true);
		tblCart->verticalHeader()->setVisible(false);
		tblCart->setMinimumHeight(300);
		lblCartTotal = new QLabel("Estimated Total: Rs.0.00", page); lblCartTotal->setObjectName("lblTotal"); lblCartTotal->setAlignment(Qt::AlignRight);

		// ── Update Qty row ──
		spinCartQty = new QSpinBox(page);
		spinCartQty->setMinimum(1); spinCartQty->setMaximum(99); spinCartQty->setValue(1);
		spinCartQty->setFixedWidth(70); spinCartQty->setFixedHeight(38);
		btnUpdateQty = new QPushButton("Update Qty", page);
		btnUpdateQty->setObjectName("btnUpdateQty"); btnUpdateQty->setFixedHeight(38);
		btnUpdateQty->setCursor(Qt::PointingHandCursor);
		btnRemove = new QPushButton("Remove Selected", page); btnRemove->setObjectName("btnRemove"); btnRemove->setFixedHeight(38); btnRemove->setCursor(Qt::PointingHandCursor);
		btnGoCheckout = new QPushButton("Proceed to Checkout", page); btnGoCheckout->setObjectName("btnCheckout"); btnGoCheckout->setFixedHeight(38); btnGoCheckout->setCursor(Qt::PointingHandCursor);

		QHBoxLayout* btns = new QHBoxLayout();
		btns->addWidget(btnRemove);
		btns->addSpacing(10);
		btns->addWidget(new QLabel("New Qty:", page));
		btns->addWidget(spinCartQty);
		btns->addWidget(btnUpdateQty);
		btns->addStretch();
		btns->addWidget(btnGoCheckout);

		lay->addWidget(lbl); lay->addWidget(tblCart); lay->addWidget(lblCartTotal); lay->addLayout(btns);
		connect(btnRemove, &QPushButton::clicked, this, &CustomerScreen::onRemoveFromCart);
		connect(btnUpdateQty, &QPushButton::clicked, this, &CustomerScreen::onUpdateCartQty);
		connect(btnGoCheckout, &QPushButton::clicked, this, &CustomerScreen::showCheckout);
		return page;
	}

	QWidget* buildCheckoutPage() {
		QWidget* page = new QWidget();
		QVBoxLayout* lay = new QVBoxLayout(page);
		QLabel* lbl = new QLabel("Checkout & Pay", page); lbl->setObjectName("lblPageTitle");
		QGroupBox* grpBill = new QGroupBox("Bill Summary", page);
		QVBoxLayout* billLay = new QVBoxLayout(grpBill);
		lblSub = new QLabel("Subtotal    : Rs.0.00");
		lblTax = new QLabel("Tax         : Rs.0.00");
		lblDisc = new QLabel("Discount    : Rs.0.00");
		lblCouponAmt = new QLabel("Coupon Disc : Rs.0.00");
		lblGrand = new QLabel("GRAND TOTAL : Rs.0.00"); lblGrand->setObjectName("lblGrand");
		QFrame* divLine = new QFrame(); divLine->setFrameShape(QFrame::HLine);
		billLay->addWidget(lblSub); billLay->addWidget(lblTax); billLay->addWidget(lblDisc);
		billLay->addWidget(lblCouponAmt); billLay->addWidget(divLine); billLay->addWidget(lblGrand);
		QGroupBox* grpCoupon = new QGroupBox("Apply Coupon", page);
		QHBoxLayout* couponLay = new QHBoxLayout(grpCoupon);
		txtCoupon = new QLineEdit(page); txtCoupon->setPlaceholderText("Enter coupon code e.g. SAVE10"); txtCoupon->setFixedHeight(36);
		btnApplyCoupon = new QPushButton("Apply", page); btnApplyCoupon->setObjectName("btnApplyCoupon"); btnApplyCoupon->setFixedHeight(36); btnApplyCoupon->setCursor(Qt::PointingHandCursor);
		couponLay->addWidget(txtCoupon); couponLay->addWidget(btnApplyCoupon);
		btnPlaceOrder = new QPushButton("Place Order & Pay", page); btnPlaceOrder->setObjectName("btnPlaceOrder"); btnPlaceOrder->setFixedHeight(48); btnPlaceOrder->setCursor(Qt::PointingHandCursor);
		QPushButton* btnBackToCart = new QPushButton("Back to Cart", page); btnBackToCart->setObjectName("btnBackCart"); btnBackToCart->setCursor(Qt::PointingHandCursor);
		QHBoxLayout* orderRow = new QHBoxLayout();
		orderRow->addWidget(btnBackToCart); orderRow->addStretch(); orderRow->addWidget(btnPlaceOrder);
		lay->addWidget(lbl); lay->addWidget(grpBill); lay->addWidget(grpCoupon); lay->addStretch(); lay->addLayout(orderRow);
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
            QGroupBox { font-weight:bold; border:1px solid #d0d7de; border-radius:8px; margin-top:8px; padding-top:10px; background:white; }
            QGroupBox::title { subcontrol-origin:margin; left:12px; color:#1a73e8; }
            QLineEdit  { border:1px solid #ccc; border-radius:6px; padding:6px 10px; background:white; }
            QLineEdit:focus { border:1px solid #34a853; }
            QSpinBox   { border:1px solid #ccc; border-radius:6px; padding:4px 8px; background:white; }
            QPushButton#btnNavBrowse { background:#1a73e8; color:white; font-weight:bold; border:none; border-radius:6px; padding:6px 14px; }
            QPushButton#btnNavBrowse:hover { background:#1558b0; }
            QPushButton#btnNavCart   { background:#34a853; color:white; font-weight:bold; border:none; border-radius:6px; padding:6px 14px; }
            QPushButton#btnNavCart:hover   { background:#277a3e; }
            QPushButton#btnNavRefund   { background:#f59e0b; color:white; font-weight:bold; border:none; border-radius:6px; padding:6px 14px; }
            QPushButton#btnNavRefund:hover   { background:#d97706; }
            QPushButton#btnCustLogout { background:#6b7280; color:white; border:none; border-radius:6px; padding:5px 12px; }
            QPushButton#btnCustLogout:hover { background:#4b5563; }
            QPushButton#btnAddCart   { background:#1a73e8; color:white; font-weight:bold; border:none; border-radius:6px; padding:6px 16px; }
            QPushButton#btnAddCart:hover   { background:#1558b0; }
            QPushButton#btnRemove    { background:#dc2626; color:white; font-weight:bold; border:none; border-radius:6px; padding:6px 14px; }
            QPushButton#btnRemove:hover    { background:#b91c1c; }
            QPushButton#btnUpdateQty { background:#1a73e8; color:white; font-weight:bold; border:none; border-radius:6px; padding:6px 14px; }
            QPushButton#btnUpdateQty:hover { background:#1558b0; }
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


// ============================================================
//  ADMIN DASHBOARD SCREEN
//  Shows stat cards + nav buttons to other admin screens
// ============================================================
class AdminDashboardScreen : public QWidget {
	Q_OBJECT
public:
	explicit AdminDashboardScreen(QWidget* parent = nullptr) : QWidget(parent) {
		setupUI(); applyStyles();
	}

	// Called by main.cpp after login so dashboard shows fresh data
	void refresh() {
		StockManager sm;
		sm.loadStock();
		int total = sm.getProductCount();
		int critical = sm.getCriticalCount();

		// Low stock count
		Product* lowItems[MAX_PRODUCTS];
		int lowCount = sm.getLowStockItems(lowItems, MAX_PRODUCTS);

		lblStatProducts->setText(QString::number(total));
		lblStatLow->setText(QString::number(lowCount));

		// Color low stock red if its a concern
		if (lowCount > 0) {
			lblStatLow->setStyleSheet("font-size:26px; font-weight:bold; color:#dc2626;");
			lblAlert->setText("  " + QString::number(lowCount) + " item(s) low/critical — check Stock Management");
			lblAlert->show();
		}
		else {
			lblStatLow->setStyleSheet("font-size:26px; font-weight:bold; color:#34a853;");
			lblAlert->hide();
		}

		// Sales stats from sales log (count Grand Total lines)
		int    orders = 0;
		double revenue = 0.0;
		ifstream file(FILE_SALES_LOG);
		if (file.is_open()) {
			string line;
			while (getline(file, line)) {
				// Manual check for "Grand Total" in line
				bool found = false;
				string keyword = "Grand Total";
				if (line.size() >= keyword.size()) {
					for (int i = 0; i <= (int)(line.size() - keyword.size()); i++) {
						bool match = true;
						for (int j = 0; j < (int)keyword.size(); j++) {
							if (line[i + j] != keyword[j]) { match = false; break; }
						}
						if (match) { found = true; break; }
					}
				}
				if (found) {
					orders++;
					// Extract number after last ':' and currency symbol
					int pos = (int)line.size() - 1;
					// Walk back to find digits
					while (pos >= 0 && line[pos] != ':') pos--;
					pos++; // skip ':'
					while (pos < (int)line.size() && (line[pos] == ' ' || (line[pos] < '0' || line[pos] > '9') && line[pos] != '.')) pos++;
					string numStr = "";
					while (pos < (int)line.size() && (line[pos] == '.' || (line[pos] >= '0' && line[pos] <= '9')))
						numStr += line[pos++];
					if (!numStr.empty()) revenue += stod(numStr);
				}
			}
			file.close();
		}
		lblStatOrders->setText(QString::number(orders));
		lblStatRevenue->setText(QString::fromStdString(CURRENCY_SYMBOL) + QString::number(revenue, 'f', 2));
	}

signals:
	void goToProducts();
	void goToStock();
	void goToReport();
	void logoutRequested();

private slots:
	void onLogoutClicked() {
		if (QMessageBox::question(this, "Logout", "Sure you want to logout?",
			QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
			emit logoutRequested();
	}

private:
	// Stat card value labels — updated by refresh()
	QLabel* lblStatProducts;
	QLabel* lblStatLow;
	QLabel* lblStatOrders;
	QLabel* lblStatRevenue;
	QLabel* lblAlert;

	// Builds one stat card — label on top, big number below
	QFrame* makeStatCard(const QString& title, QLabel*& valueLabel, const QString& color) {
		QFrame* card = new QFrame(this);
		card->setObjectName("statCard");
		QVBoxLayout* lay = new QVBoxLayout(card);
		lay->setContentsMargins(16, 14, 16, 14); lay->setSpacing(4);
		QLabel* lbl = new QLabel(title, card);
		lbl->setStyleSheet("font-size:12px; color:#6b7280;");
		valueLabel = new QLabel("—", card);
		valueLabel->setStyleSheet("font-size:26px; font-weight:bold; color:" + color + ";");
		lay->addWidget(lbl);
		lay->addWidget(valueLabel);
		return card;
	}

	void setupUI() {
		setWindowTitle("Admin Dashboard — Supermarket Billing System");
		setMinimumSize(700, 540);

		// Top bar
		QHBoxLayout* topBar = new QHBoxLayout();
		QLabel* lblTitle = new QLabel("Admin Dashboard", this);
		lblTitle->setObjectName("lblDashTitle");
		QPushButton* btnLogout = new QPushButton(QString::fromStdString(LABEL_BTN_LOGOUT), this);
		btnLogout->setObjectName("btnLogout");
		btnLogout->setFixedWidth(90);
		btnLogout->setCursor(Qt::PointingHandCursor);
		topBar->addWidget(lblTitle); topBar->addStretch(); topBar->addWidget(btnLogout);

		// Stat cards grid (2x2)
		QGridLayout* statsGrid = new QGridLayout();
		statsGrid->setSpacing(12);
		statsGrid->addWidget(makeStatCard("Total Products", lblStatProducts, "#1a73e8"), 0, 0);
		statsGrid->addWidget(makeStatCard("Low Stock Items", lblStatLow, "#dc2626"), 0, 1);
		statsGrid->addWidget(makeStatCard("Total Orders", lblStatOrders, "#34a853"), 1, 0);
		statsGrid->addWidget(makeStatCard("Total Revenue", lblStatRevenue, "#f59e0b"), 1, 1);

		// Alert banner — hidden until refresh() finds low stock
		lblAlert = new QLabel("", this);
		lblAlert->setObjectName("lblDashAlert");
		lblAlert->setWordWrap(true);
		lblAlert->hide();

		// Nav buttons
		QLabel* lblNav = new QLabel("NAVIGATE TO", this);
		lblNav->setObjectName("lblNavHeader");

		QPushButton* btnProducts = new QPushButton("  Product Management", this);
		QPushButton* btnStock = new QPushButton("  Stock Management", this);
		QPushButton* btnReport = new QPushButton("  Sales Report", this);

		for (QPushButton* btn : { btnProducts, btnStock, btnReport }) {
			btn->setObjectName("btnNav");
			btn->setFixedHeight(46);
			btn->setCursor(Qt::PointingHandCursor);
		}

		QVBoxLayout* mainLay = new QVBoxLayout(this);
		mainLay->setContentsMargins(28, 24, 28, 24); mainLay->setSpacing(14);
		mainLay->addLayout(topBar);
		mainLay->addLayout(statsGrid);
		mainLay->addWidget(lblAlert);
		mainLay->addSpacing(6);
		mainLay->addWidget(lblNav);
		mainLay->addWidget(btnProducts);
		mainLay->addWidget(btnStock);
		mainLay->addWidget(btnReport);
		mainLay->addStretch();
		setLayout(mainLay);

		connect(btnLogout, &QPushButton::clicked, this, &AdminDashboardScreen::onLogoutClicked);
		connect(btnProducts, &QPushButton::clicked, this, &AdminDashboardScreen::goToProducts);
		connect(btnStock, &QPushButton::clicked, this, &AdminDashboardScreen::goToStock);
		connect(btnReport, &QPushButton::clicked, this, &AdminDashboardScreen::goToReport);
	}

	void applyStyles() {
		setStyleSheet(R"(
            QWidget { background:#f4f6f9; font-family:Arial; font-size:13px; color:#2c3e50; }
            QLabel#lblDashTitle  { font-size:20px; font-weight:bold; color:#1a73e8; }
            QLabel#lblNavHeader  { font-size:11px; font-weight:bold; color:#9ca3af; letter-spacing:1px; }
            QLabel#lblDashAlert  { background:#fef3c7; color:#92400e; border-left:4px solid #f59e0b;
                                   border-radius:6px; padding:10px 14px; font-size:13px; }
            QFrame#statCard      { background:white; border:1px solid #e5e7eb; border-radius:10px; }
            QPushButton#btnNav   { background:white; color:#2c3e50; border:1px solid #e5e7eb;
                                   border-radius:8px; font-size:14px; text-align:left; padding-left:14px; }
            QPushButton#btnNav:hover   { background:#eff6ff; border-color:#1a73e8; color:#1a73e8; }
            QPushButton#btnLogout { background:#6b7280; color:white; border:none; border-radius:6px; padding:5px 12px; }
            QPushButton#btnLogout:hover { background:#4b5563; }
        )");
	}
};


// ============================================================
//  ADMIN STOCK SCREEN 
//  Table of all products with OK/LOW/CRITICAL tags
//  + Update Stock form at the bottom
// ============================================================
class AdminStockScreen : public QWidget {
	Q_OBJECT
public:
	explicit AdminStockScreen(QWidget* parent = nullptr) : QWidget(parent) {
		setupUI(); applyStyles();
	}

	// Loads products from file and fills the table
	void refresh() {
		sm.loadStock();
		loadTable();
	}

signals:
	void goToDashboard();

private slots:
	void onUpdateClicked() {
		QString idStr = txtProdID->text().trimmed();
		QString qtyStr = txtNewQty->text().trimmed();
		if (idStr.isEmpty() || qtyStr.isEmpty()) {
			QMessageBox::warning(this, "Error", "Enter both Product ID and New Quantity."); return;
		}
		bool ok1, ok2;
		int id = idStr.toInt(&ok1);
		int qty = qtyStr.toInt(&ok2);
		if (!ok1 || !ok2 || qty < 0) {
			QMessageBox::warning(this, "Error", "ID and Qty must be valid non-negative integers."); return;
		}
		sm.updateStock(id, qty);  // Updates + saves to file via FileManager
		refresh();                // Reload table to show new values
		txtProdID->clear(); txtNewQty->clear();
		QMessageBox::information(this, "Updated", "Stock updated successfully.");
	}

	void onBackClicked() { emit goToDashboard(); }

private:
	StockManager  sm;
	QTableWidget* tblStock;
	QLineEdit* txtProdID;
	QLineEdit* txtNewQty;

	// Returns status string based on qty vs globals.h thresholds
	QString statusTag(int qty) {
		if (qty <= STOCK_CRITICAL_THRESHOLD) return "CRITICAL";
		if (qty <= STOCK_LOW_THRESHOLD)      return "LOW";
		return "OK";
	}

	void loadTable() {
		tblStock->setRowCount(0);
		for (int i = 0; i < sm.getProductCount(); i++) {
			Product* p = sm.getProduct(i);
			int      qty = p->getStockQty();
			int      row = tblStock->rowCount();
			tblStock->insertRow(row);
			tblStock->setItem(row, 0, new QTableWidgetItem(QString::number(p->getProductID())));
			tblStock->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(p->getName())));
			tblStock->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(p->getCategory())));
			tblStock->setItem(row, 3, new QTableWidgetItem(QString::number(qty)));
			QTableWidgetItem* statusItem = new QTableWidgetItem(statusTag(qty));
			statusItem->setTextAlignment(Qt::AlignCenter);

			// Color the status cell and qty cell based on threshold
			QColor bg = Qt::white;
			if (qty <= STOCK_CRITICAL_THRESHOLD) bg = QColor("#ffe4e4");
			else if (qty <= STOCK_LOW_THRESHOLD)      bg = QColor("#fff8d6");

			for (int col = 0; col < 4; col++)
				if (tblStock->item(row, col))
					tblStock->item(row, col)->setBackground(bg);
			statusItem->setBackground(bg);
			tblStock->setItem(row, 4, statusItem);
		}
	}

	void setupUI() {
		setWindowTitle("Stock Management — Admin");
		setMinimumSize(680, 580);

		// Top bar
		QHBoxLayout* topBar = new QHBoxLayout();
		QLabel* lblTitle = new QLabel("Stock Management", this);
		lblTitle->setObjectName("lblStockTitle");
		QPushButton* btnBack = new QPushButton("← Dashboard", this);
		btnBack->setObjectName("btnBack"); btnBack->setFixedWidth(110); btnBack->setCursor(Qt::PointingHandCursor);
		topBar->addWidget(lblTitle); topBar->addStretch(); topBar->addWidget(btnBack);

		// Stock table
		tblStock = new QTableWidget(0, 5, this);
		tblStock->setHorizontalHeaderLabels({ "ID","Name","Category","Stock","Status" });
		tblStock->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		tblStock->setSelectionBehavior(QAbstractItemView::SelectRows);
		tblStock->setEditTriggers(QAbstractItemView::NoEditTriggers);
		tblStock->setAlternatingRowColors(false);
		tblStock->verticalHeader()->setVisible(false);
		tblStock->setMinimumHeight(320);

		// Update stock form
		QGroupBox* grpUpdate = new QGroupBox("Update Stock Quantity", this);
		QHBoxLayout* updateRow = new QHBoxLayout(grpUpdate);
		txtProdID = new QLineEdit(); txtProdID->setPlaceholderText("Product ID"); txtProdID->setFixedHeight(36); txtProdID->setFixedWidth(120);
		txtNewQty = new QLineEdit(); txtNewQty->setPlaceholderText("New Qty");    txtNewQty->setFixedHeight(36); txtNewQty->setFixedWidth(120);
		QPushButton* btnUpdate = new QPushButton("Update");
		btnUpdate->setObjectName("btnUpdate"); btnUpdate->setFixedHeight(36); btnUpdate->setCursor(Qt::PointingHandCursor);
		updateRow->addWidget(new QLabel("Product ID:")); updateRow->addWidget(txtProdID);
		updateRow->addSpacing(16);
		updateRow->addWidget(new QLabel("New Qty:"));    updateRow->addWidget(txtNewQty);
		updateRow->addSpacing(16);
		updateRow->addWidget(btnUpdate);
		updateRow->addStretch();

		// Legend
		QHBoxLayout* legend = new QHBoxLayout();
		auto makeLegend = [&](const QString& text, const QString& color) {
			QLabel* l = new QLabel(text, this);
			l->setStyleSheet("background:" + color + "; padding:3px 10px; border-radius:4px; font-size:12px;");
			legend->addWidget(l);
			};
		makeLegend("OK — qty > 10", "#d1fae5");
		makeLegend("LOW — qty 6-10", "#fff8d6");
		makeLegend("CRITICAL — qty <= 5", "#ffe4e4");
		legend->addStretch();

		QVBoxLayout* mainLay = new QVBoxLayout(this);
		mainLay->setContentsMargins(24, 20, 24, 20); mainLay->setSpacing(12);
		mainLay->addLayout(topBar);
		mainLay->addLayout(legend);
		mainLay->addWidget(tblStock);
		mainLay->addWidget(grpUpdate);
		mainLay->addStretch();
		setLayout(mainLay);

		connect(btnUpdate, &QPushButton::clicked, this, &AdminStockScreen::onUpdateClicked);
		connect(btnBack, &QPushButton::clicked, this, &AdminStockScreen::onBackClicked);
	}

	void applyStyles() {
		setStyleSheet(R"(
            QWidget { background:#f4f6f9; font-family:Arial; font-size:13px; color:#2c3e50; }
            QLabel#lblStockTitle { font-size:18px; font-weight:bold; color:#1a73e8; }
            QGroupBox { font-weight:bold; border:1px solid #d0d7de; border-radius:8px; margin-top:8px; padding-top:10px; background:white; }
            QGroupBox::title { subcontrol-origin:margin; left:12px; color:#1a73e8; }
            QLineEdit { border:1px solid #ccc; border-radius:6px; padding:5px 10px; background:white; }
            QLineEdit:focus { border:1px solid #1a73e8; }
            QPushButton#btnUpdate { background:#1a73e8; color:white; font-weight:bold; border:none; border-radius:6px; padding:6px 18px; }
            QPushButton#btnUpdate:hover { background:#1558b0; }
            QPushButton#btnBack { background:transparent; color:#1a73e8; border:1px solid #1a73e8; border-radius:6px; padding:5px 12px; }
            QPushButton#btnBack:hover { background:#e8f0fe; }
            QTableWidget { border:1px solid #d0d7de; border-radius:6px; background:white; }
            QTableWidget::item:selected { background:#d0e8ff; color:#1a1a1a; }
            QHeaderView::section { background:#1a73e8; color:white; font-weight:bold; padding:5px; border:none; }
        )");
	}
};


// ============================================================
//  ADMIN SALES REPORT SCREEN 
//  Reads sales_log.txt via Report class and displays summary
//  Export button saves to data/reports/RPT-<date>.txt
// ============================================================
class AdminReportScreen : public QWidget {
	Q_OBJECT
public:
	explicit AdminReportScreen(QWidget* parent = nullptr) : QWidget(parent) {
		setupUI(); applyStyles();
	}

	// Regenerates report from sales log and updates all labels
	void refresh() {
		report.setReportDate(QDate::currentDate().toString("ddMMyyyy").toStdString());
		report.generateFromLog();

		QString rs = QString::fromStdString(CURRENCY_SYMBOL);
		lblTransactions->setText(QString::number(report.getTotalTransactions()));
		lblRevenue->setText(rs + QString::number(report.getTotalRevenue(), 'f', 2));
		lblTax->setText(rs + QString::number(report.getTotalTax(), 'f', 2));
		lblDiscounts->setText(rs + QString::number(report.getTotalDiscounts(), 'f', 2));
		lblGrocery->setText(rs + QString::number(report.getGroceryRevenue(), 'f', 2));
		lblPerish->setText(rs + QString::number(report.getPerishRevenue(), 'f', 2));
		lblElectro->setText(rs + QString::number(report.getElectroRevenue(), 'f', 2));

		for (int i = 0; i < MAX_REPORT_PRODUCTS; i++) {
			string name = report.getTopProductName(i);
			int units = report.getTopProductUnits(i);
			if (!name.empty())
				lblTop[i]->setText(QString::number(i + 1) + ".  "
					+ QString::fromStdString(name)
					+ "  —  " + QString::number(units) + " units");
			else
				lblTop[i]->setText(QString::number(i + 1) + ".  —");
		}
	}

signals:
	void goToDashboard();

private slots:
	void onExportClicked() {
		report.exportToFile();
		QMessageBox::information(this, "Exported",
			"Report saved to:\ndata/reports/RPT-"
			+ QString::fromStdString(QDate::currentDate().toString("ddMMyyyy").toStdString())
			+ ".txt");
	}
	void onBackClicked() { emit goToDashboard(); }

private:
	Report  report;
	QLabel* lblTransactions;
	QLabel* lblRevenue;
	QLabel* lblTax;
	QLabel* lblDiscounts;
	QLabel* lblGrocery;
	QLabel* lblPerish;
	QLabel* lblElectro;
	QLabel* lblTop[MAX_REPORT_PRODUCTS];

	// Builds a two-column row: key on left, value label on right
	QHBoxLayout* makeRow(const QString& key, QLabel*& valLabel) {
		QHBoxLayout* row = new QHBoxLayout();
		QLabel* k = new QLabel(key + " :", this);
		k->setStyleSheet("color:#6b7280;");
		valLabel = new QLabel("—", this);
		valLabel->setStyleSheet("font-weight:bold; color:#2c3e50;");
		valLabel->setAlignment(Qt::AlignRight);
		row->addWidget(k); row->addStretch(); row->addWidget(valLabel);
		return row;
	}

	void setupUI() {
		setWindowTitle("Sales Report — Admin");
		setMinimumSize(620, 640);

		// Top bar
		QHBoxLayout* topBar = new QHBoxLayout();
		QLabel* lblTitle = new QLabel("Sales Report", this);
		lblTitle->setObjectName("lblReportTitle");
		QPushButton* btnExport = new QPushButton(QString::fromStdString(LABEL_BTN_EXPORT), this);
		btnExport->setObjectName("btnExport"); btnExport->setFixedWidth(120); btnExport->setCursor(Qt::PointingHandCursor);
		QPushButton* btnBack = new QPushButton("← Dashboard", this);
		btnBack->setObjectName("btnBack"); btnBack->setFixedWidth(110); btnBack->setCursor(Qt::PointingHandCursor);
		topBar->addWidget(lblTitle); topBar->addStretch(); topBar->addWidget(btnExport); topBar->addSpacing(8); topBar->addWidget(btnBack);

		// Summary card
		QGroupBox* grpSummary = new QGroupBox("Summary", this);
		QVBoxLayout* sumLay = new QVBoxLayout(grpSummary);
		sumLay->addLayout(makeRow("Total Transactions", lblTransactions));
		sumLay->addLayout(makeRow("Total Revenue", lblRevenue));
		sumLay->addLayout(makeRow("Tax Collected", lblTax));
		sumLay->addLayout(makeRow("Total Discounts", lblDiscounts));

		// Category card
		QGroupBox* grpCat = new QGroupBox("Revenue by Category", this);
		QVBoxLayout* catLay = new QVBoxLayout(grpCat);
		catLay->addLayout(makeRow("Grocery", lblGrocery));
		catLay->addLayout(makeRow("Perishable", lblPerish));
		catLay->addLayout(makeRow("Electronic", lblElectro));

		// Top products card
		QGroupBox* grpTop = new QGroupBox("Top Selling Products", this);
		QVBoxLayout* topLay = new QVBoxLayout(grpTop);
		for (int i = 0; i < MAX_REPORT_PRODUCTS; i++) {
			lblTop[i] = new QLabel(QString::number(i + 1) + ".  —", this);
			lblTop[i]->setStyleSheet("color:#2c3e50; font-size:13px;");
			topLay->addWidget(lblTop[i]);
		}

		QVBoxLayout* mainLay = new QVBoxLayout(this);
		mainLay->setContentsMargins(24, 20, 24, 20); mainLay->setSpacing(14);
		mainLay->addLayout(topBar);
		mainLay->addWidget(grpSummary);
		mainLay->addWidget(grpCat);
		mainLay->addWidget(grpTop);
		mainLay->addStretch();
		setLayout(mainLay);

		connect(btnExport, &QPushButton::clicked, this, &AdminReportScreen::onExportClicked);
		connect(btnBack, &QPushButton::clicked, this, &AdminReportScreen::onBackClicked);
	}

	void applyStyles() {
		setStyleSheet(R"(
            QWidget { background:#f4f6f9; font-family:Arial; font-size:13px; color:#2c3e50; }
            QLabel#lblReportTitle { font-size:18px; font-weight:bold; color:#1a73e8; }
            QGroupBox { font-weight:bold; border:1px solid #d0d7de; border-radius:8px; margin-top:8px; padding-top:10px; background:white; }
            QGroupBox::title { subcontrol-origin:margin; left:12px; color:#1a73e8; }
            QPushButton#btnExport { background:#34a853; color:white; font-weight:bold; border:none; border-radius:6px; padding:6px 14px; }
            QPushButton#btnExport:hover { background:#277a3e; }
            QPushButton#btnBack { background:transparent; color:#1a73e8; border:1px solid #1a73e8; border-radius:6px; padding:5px 12px; }
            QPushButton#btnBack:hover { background:#e8f0fe; }
        )");
	}
};


// ============================================================
//  REFUND SCREEN
//  Policy (Globals.h):
//    REFUND_MAX_DAYS       = 7   (returns within 7 days)
//    REFUND_RESTOCKING_FEE = 0.05 (5% deducted from refund)
//
//  Flow:
//    1. Customer enters Receipt ID, amount paid, purchase date
//    2. System checks 7-day window
//    3. Calculates refund = paid - (paid * 5%)
//    4. Logs to data/refunds.txt via FileManager::appendRefundLog
//    5. Shows success / error result
// ============================================================
class RefundScreen : public QWidget {
	Q_OBJECT
public:
	explicit RefundScreen(QWidget* parent = nullptr) : QWidget(parent) {
		setupUI(); applyStyles();
	}

	// Reset all fields — call before showing screen
	void reset() {
		txtReceiptID->clear();
		txtAmount->clear();
		txtPurchaseDate->clear();
		txtReason->clear();
		lblResult->hide();
		lblResult->setText("");
	}

signals:
	void backRequested();

private slots:
	void onSubmitClicked() {
		QString receiptID = txtReceiptID->text().trimmed();
		QString amountStr = txtAmount->text().trimmed();
		QString dateStr = txtPurchaseDate->text().trimmed();

		// ── Validation ──
		if (receiptID.isEmpty() || amountStr.isEmpty() || dateStr.isEmpty()) {
			showResult(false, "Please fill in Receipt ID, Amount Paid, and Purchase Date.");
			return;
		}
		if (!receiptID.startsWith("RCP-")) {
			showResult(false, "Invalid Receipt ID.\nIt must start with RCP- (e.g. RCP-0001).\nCheck your receipt.");
			return;
		}
		bool ok;
		double paidAmount = amountStr.toDouble(&ok);
		if (!ok || paidAmount <= 0) {
			showResult(false, "Please enter a valid paid amount (e.g. 1500.00).");
			return;
		}
		QDate purchaseDate = QDate::fromString(dateStr, "yyyy-MM-dd");
		if (!purchaseDate.isValid()) {
			showResult(false, "Invalid date format.\nUse YYYY-MM-DD (e.g. 2025-06-01).");
			return;
		}
		int daysDiff = purchaseDate.daysTo(QDate::currentDate());
		if (daysDiff < 0) {
			showResult(false, "Purchase date cannot be in the future!");
			return;
		}
		if (daysDiff > REFUND_MAX_DAYS) {
			showResult(false,
				QString("Refund window expired!\n"
					"Your purchase was %1 day(s) ago.\n"
					"Refunds are only accepted within %2 days of purchase.")
				.arg(daysDiff).arg(REFUND_MAX_DAYS));
			return;
		}

		// ── Calculate ──
		double restockingFee = paidAmount * REFUND_RESTOCKING_FEE; // 5%
		double refundAmount = paidAmount - restockingFee;

		// ── Log to file ──
		time_t now = time(nullptr);
		char buf[64];
		strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
		string timestamp(buf);

		FileManager::appendRefundLog(
			receiptID.toStdString(),
			"(Customer Portal)",
			refundAmount,
			restockingFee,
			timestamp
		);

		// ── Show success summary ──
		QString summary;
		summary += "REFUND APPROVED\n\n";
		summary += QString("Receipt ID       : %1\n").arg(receiptID);
		summary += QString("Amount Paid      : Rs.%1\n").arg(QString::number(paidAmount, 'f', 2));
		summary += QString("Restocking Fee   : Rs.%1  (5%%)\n").arg(QString::number(restockingFee, 'f', 2));
		summary += "---------------------------------------\n";
		summary += QString("Refund Amount    : Rs.%1\n").arg(QString::number(refundAmount, 'f', 2));
		summary += QString("\nProcessed at: %1").arg(QString::fromStdString(timestamp));
		showResult(true, summary);

		// Clear form after success
		txtReceiptID->clear();
		txtAmount->clear();
		txtPurchaseDate->clear();
		txtReason->clear();
	}

	void onBackClicked() { emit backRequested(); }

private:
	QLineEdit* txtReceiptID;
	QLineEdit* txtAmount;
	QLineEdit* txtPurchaseDate;
	QLineEdit* txtReason;
	QLabel* lblResult;

	void showResult(bool success, const QString& msg) {
		lblResult->setText(msg);
		lblResult->setStyleSheet(success
			? "background:#d1fae5; color:#065f46; border:1px solid #6ee7b7; border-radius:8px; padding:14px; font-size:13px;"
			: "background:#fee2e2; color:#991b1b; border:1px solid #fca5a5; border-radius:8px; padding:14px; font-size:13px;");
		lblResult->show();
	}

	void setupUI() {
		setMinimumSize(620, 600);

		// ── Top bar ──
		QHBoxLayout* topBar = new QHBoxLayout();
		QLabel* lblTitle = new QLabel("Request a Refund", this);
		lblTitle->setObjectName("lblRefundTitle");
		QPushButton* btnBack = new QPushButton("<- Back", this);
		btnBack->setObjectName("btnRefundBack");
		btnBack->setFixedWidth(90);
		btnBack->setCursor(Qt::PointingHandCursor);
		topBar->addWidget(lblTitle); topBar->addStretch(); topBar->addWidget(btnBack);

		// ── Policy info box ──
		QLabel* lblPolicy = new QLabel(this);
		lblPolicy->setText(
			"Refund Policy:\n"
			"  - Refunds accepted within " + QString::number(REFUND_MAX_DAYS) + " days of purchase\n"
			"  - A 5% restocking fee is deducted from your refund\n"
			"  - You need your Receipt ID from your purchase receipt (e.g. RCP-0001)"
		);
		lblPolicy->setObjectName("lblPolicy");
		lblPolicy->setWordWrap(true);

		// ── Form ──
		QGroupBox* grpForm = new QGroupBox("Refund Details", this);
		QGridLayout* grid = new QGridLayout(grpForm);
		grid->setSpacing(12); grid->setContentsMargins(18, 18, 18, 18);

		txtReceiptID = new QLineEdit(); txtReceiptID->setPlaceholderText("e.g. RCP-0001");              txtReceiptID->setFixedHeight(38);
		txtAmount = new QLineEdit(); txtAmount->setPlaceholderText("Amount you paid e.g. 1500.00"); txtAmount->setFixedHeight(38);
		txtPurchaseDate = new QLineEdit(); txtPurchaseDate->setPlaceholderText("YYYY-MM-DD e.g. 2025-06-01"); txtPurchaseDate->setFixedHeight(38);
		txtReason = new QLineEdit(); txtReason->setPlaceholderText("Reason for return (optional)"); txtReason->setFixedHeight(38);

		grid->addWidget(new QLabel("Receipt ID:"), 0, 0); grid->addWidget(txtReceiptID, 0, 1);
		grid->addWidget(new QLabel("Amount Paid (Rs.):"), 1, 0); grid->addWidget(txtAmount, 1, 1);
		grid->addWidget(new QLabel("Purchase Date:"), 2, 0); grid->addWidget(txtPurchaseDate, 2, 1);
		grid->addWidget(new QLabel("Reason:"), 3, 0); grid->addWidget(txtReason, 3, 1);

		// ── Submit button ──
		QPushButton* btnSubmit = new QPushButton("Submit Refund Request", this);
		btnSubmit->setObjectName("btnRefundSubmit");
		btnSubmit->setFixedHeight(46);
		btnSubmit->setCursor(Qt::PointingHandCursor);

		// ── Result label (hidden until submit) ──
		lblResult = new QLabel("", this);
		lblResult->setWordWrap(true);
		lblResult->setAlignment(Qt::AlignLeft | Qt::AlignTop);
		lblResult->setMinimumHeight(130);
		lblResult->hide();

		// ── Layout ──
		QVBoxLayout* mainLay = new QVBoxLayout(this);
		mainLay->setContentsMargins(28, 22, 28, 22); mainLay->setSpacing(14);
		mainLay->addLayout(topBar);
		mainLay->addWidget(lblPolicy);
		mainLay->addWidget(grpForm);
		mainLay->addWidget(btnSubmit);
		mainLay->addWidget(lblResult);
		mainLay->addStretch();
		setLayout(mainLay);

		connect(btnSubmit, &QPushButton::clicked, this, &RefundScreen::onSubmitClicked);
		connect(btnBack, &QPushButton::clicked, this, &RefundScreen::onBackClicked);
	}

	void applyStyles() {
		setStyleSheet(R"(
            QWidget { background:#f4f6f9; font-family:Arial; font-size:13px; color:#2c3e50; }
            QLabel#lblRefundTitle { font-size:18px; font-weight:bold; color:#f59e0b; }
            QLabel#lblPolicy { background:#fffbeb; border:1px solid #fde68a; border-radius:8px;
                               padding:12px 16px; color:#78350f; font-size:12px; line-height:1.5; }
            QGroupBox { font-weight:bold; border:1px solid #d0d7de; border-radius:8px;
                        margin-top:8px; padding-top:10px; background:white; }
            QGroupBox::title { subcontrol-origin:margin; left:12px; color:#f59e0b; }
            QLineEdit { border:1px solid #ccc; border-radius:6px; padding:6px 10px; background:white; }
            QLineEdit:focus { border:1px solid #f59e0b; }
            QPushButton#btnRefundSubmit { background:#f59e0b; color:white; font-size:14px;
                                          font-weight:bold; border:none; border-radius:8px; }
            QPushButton#btnRefundSubmit:hover { background:#d97706; }
            QPushButton#btnRefundBack { background:transparent; color:#1a73e8;
                                        border:1px solid #1a73e8; border-radius:6px; padding:5px 12px; }
            QPushButton#btnRefundBack:hover { background:#e8f0fe; }
        )");
	}
};

#endif 