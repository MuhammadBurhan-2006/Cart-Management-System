# 🛒 Cart Management System
### A GUI-based Supermarket Point-of-Sale (POS) Application in C++

<div align="center">

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Qt](https://img.shields.io/badge/Qt-41CD52?style=for-the-badge&logo=qt&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-blue?style=for-the-badge)
![OOP](https://img.shields.io/badge/Paradigm-OOP-orange?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

**A fully featured desktop POS system simulating a real-world supermarket environment — built entirely in C++ with a Qt GUI.**

[Features](#-features) • [Screenshots](#-screenshots) • [Getting Started](#-getting-started) • [Architecture](#-architecture) • [Team](#-team)

</div>

---

## 📌 Overview

The **Cart Management System** is a GUI-based desktop application that simulates a complete supermarket Point-of-Sale environment. The system supports two distinct user roles — **Admin** and **Customer** — and demonstrates core Object-Oriented Programming principles including:

- ✅ **Inheritance** — Product hierarchy (Perishable, Electronic, Grocery)
- ✅ **Polymorphism** — Overridden `calculateTax()` and `applyDiscount()` per product type
- ✅ **Encapsulation** — Private data with controlled getters/setters across all classes
- ✅ **File-based Persistence** — All data stored and reloaded from structured text files

---

## ✨ Features

### 👤 Customer Side
| Feature | Description |
|---|---|
| 🔐 Register / Login | Secure credential-based authentication |
| 🛍️ Browse Products | View all products with prices, category, and stock info |
| 🛒 Cart Management | Add, remove, and update item quantities |
| 🏷️ Coupon Codes | Apply discount coupons at checkout |
| 💳 Checkout & Billing | Full bill with tax, discount, and grand total |
| 🔄 Refund Request | Request refunds with restocking fee policy |

### 🔑 Admin Side
| Feature | Description |
|---|---|
| 📦 Product Management | Add, edit, and delete products (Perishable / Electronic / Grocery) |
| 📊 Stock Management | Monitor stock levels with low & critical alerts |
| 📈 Sales Reports | Auto-generated reports from sales log with top product rankings |
| 🧾 Receipt History | All transactions logged with receipt IDs |
| 👥 Customer Records | View registered customer data |

---

## 🏗️ Architecture

```
CartManagementSystem/
│
├── main.cpp                  # App entry point, Qt signal/slot wiring
│
├── Globals.h                 # All constants (file paths, thresholds, labels)
├── Products.h                # Product base class + Perishable/Electronic/Grocery
├── Customer.h                # Customer, Cart, CartItem, Bill classes
├── Admin.h                   # Admin authentication class
│
├── FileManager.h             # Static file I/O (load/save products, customers, logs)
├── StockManager.h            # Stock monitoring, low/critical alerts
├── Report.h                  # Sales report generation from log file
├── GUI.h                     # All Qt screen classes (8 screens)
│
└── data/
    ├── products.txt          # Product catalog (pipe-delimited)
    ├── customers.txt         # Registered customers
    ├── admins.txt            # Admin credentials
    ├── sales_log.txt         # All transaction records
    ├── refunds.txt           # Refund log
    ├── receipts/             # Per-transaction receipt files (RCP-XXXX.txt)
    └── reports/              # Generated report files (RPT-XXXX.txt)
```

---

## 🧬 Class Hierarchy

```
Product  (base)
├── PerishableProduct    → Tax: 5%  | Discount: 10% | Extra: Expiry Date
├── ElectronicProduct    → Tax: 10% | Discount: 5%  | Extra: Brand, Warranty
└── GroceryProduct       → Tax: 0%  | Discount: 8%  | Extra: Weight (kg)

Customer
Cart  ──────────► CartItem [ Product* + quantity ]
Bill  ──────────► Cart* + Customer* → Grand Total

FileManager   (static utility)
StockManager  (manages Product* array)
Report        (parses sales_log.txt → generates analytics)
```

---

## 🖥️ Screen Navigation

```
LoginScreen (0)
├── → AdminDashboardScreen (1)
│       ├── → ProductCatalogScreen (2)
│       ├── → AdminStockScreen (3)
│       └── → AdminReportScreen (4)
│
├── → RegisterScreen (5)
│
└── → CustomerScreen (6)
        └── → RefundScreen (7)
```

All screens are managed by a single `QStackedWidget` — no multiple windows.

---

## 🏷️ Coupon Codes

| Code | Discount |
|---|---|
| `SAVE10` | 10% off subtotal |
| `FLAT50` | Flat Rs. 50 off |
| `FRESH20` | 20% off subtotal |

---

## 📁 Data File Format

**products.txt**
```
P|1|Fresh Milk 1L|150.0|50|2025-06-30|
E|3|Samsung Phone|45000.0|15|Samsung|12|
G|2|Basmati Rice 5kg|800.0|100|5.0|
```
- `P` = Perishable (fields: id, name, price, qty, expiry)
- `E` = Electronic (fields: id, name, price, qty, brand, warrantyMonths)
- `G` = Grocery (fields: id, name, price, qty, weightKg)

**customers.txt**
```
1|Ali Ahmed|ali123|pass123|03001234567|
```

---

## 🚀 Getting Started

### Prerequisites
- **Qt Framework** 5.x or 6.x
- **C++ Compiler** (MSVC / MinGW / GCC)
- **Qt Creator** (recommended IDE)

### Build & Run

**Option 1 — Qt Creator (Recommended)**
1. Clone the repo:
   ```bash
   git clone https://github.com/MuhammadBurhan-2006/Cart-Management-System.git
   ```
2. Open `CartManagementSystem.pro` in Qt Creator
3. Configure the kit (MinGW or MSVC)
4. Press **Run** (`Ctrl+R`)

**Option 2 — Command Line**
```bash
git clone https://github.com/MuhammadBurhan-2006/Cart-Management-System.git
cd Cart-Management-System
qmake CartManagementSystem.pro
make          # or: mingw32-make on Windows
./CartManagementSystem
```

> **Note:** The `data/` folder and its subdirectories (`receipts/`, `reports/`) are auto-created on first run via `FileManager::initDataFolders()`.

### Default Admin Login
```
Username: admin
Password: admin123
```

---

## 📊 Stock Thresholds

| Level | Threshold | Behavior |
|---|---|---|
| ⚠️ Low Stock | ≤ 10 units | Warning shown in Admin Stock screen |
| 🔴 Critical Stock | ≤ 5 units | Critical alert highlighted |

---

## 💡 OOP Concepts Demonstrated

| Concept | Where Used |
|---|---|
| **Inheritance** | `PerishableProduct`, `ElectronicProduct`, `GroceryProduct` extend `Product` |
| **Polymorphism** | Virtual `calculateTax()`, `applyDiscount()`, `display()`, `getExtraData()` |
| **Encapsulation** | Private members with public getters/setters in all classes |
| **Abstraction** | `FileManager` hides all I/O details; `Bill` abstracts billing logic |
| **Dynamic Dispatch** | `Product*` array holds mixed types; correct methods called at runtime |
| **Composition** | `Cart` contains `CartItem[]`; `Bill` references `Cart*` and `Customer*` |

---

## 👥 Team

| Name | Role |
|---|---|
| Muhammad Burhan | Products, Tax/Discount logic, Qt GUI |
| Abdul Rehman | File I/O, Receipts, Refund system |
| Khubaib | Stock Manager, Admin Dashboard, Reports |
| Areeba | Customer Dashboard, Cart, Billing, Coupons |
| *(Team 03)* | OOP Course Project |

---

## 📄 License

This project is open source under the [MIT License](LICENSE).

---

<div align="center">

Made with ❤️ by **Team 03** — CS OOP Course Project

⭐ Star this repo if you found it useful!

</div>
