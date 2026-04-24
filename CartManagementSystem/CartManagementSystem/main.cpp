#include<iostream>
#include"Globals.h"
#include"Products.h"
using namespace std;
int main() {
	//Testing Product creation
	Product* products[MAX_PRODUCTS];
	int count = 0;
	products[count++] = new PerishableProduct(1, "Milk", 50.0, 20, "2024-12-31");
	products[count++] = new ElectronicProduct(2, "Smartphone", 15000.0, 10, "BrandX", 24);
	products[count++] = new GroceryProduct(3, "Rice", 80.0, 50, 1.0);
	cout << RECEIPT_HEADER << endl;
	for (int i = 0; i < count; i++)
	{
		products[i]->display();
	}
	cout << endl;
	cout << RECEIPT_FOOTER << endl;
	for (int i = 0; i < count; i++)
	{
		delete products[i];
		products[i] = nullptr;
	}
	return 0;
}