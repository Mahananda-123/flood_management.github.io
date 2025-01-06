

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// Function for String Matching (search by name or description)
void stringMatch(vector<string>& productNames, string keyword) {
    bool found = false;
    for (const auto& name : productNames) {
        if (name.find(keyword) != string::npos) { // Partial match
            cout << "Product found: " << name << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "No product found with the keyword \"" << keyword << "\".\n";
    }
}

// Function for Linear Search (search by category)
void linearSearch(vector<string>& categories, vector<string>& productNames, string category) {
    bool found = false;
    cout << "Products in category \"" << category << "\":\n";
    for (size_t i = 0; i < categories.size(); i++) {
        if (categories[i] == category) {
            cout << "- " << productNames[i] << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "No products found in the category \"" << category << "\".\n";
    }
}

// Function for Binary Search (search by price)
int binarySearch(vector<int>& prices, int target) {
    int low = 0, high = prices.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (prices[mid] == target) {
            return mid; // Found
        } else if (prices[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1; // Not found
}

int main() {
    cout << "Welcome to the Product Catalog Management System!\n";

    // Input for Product Data
    int n;
    cout << "Enter the number of products: ";
    cin >> n;

    vector<string> productNames(n);
    vector<int> productPrices(n);
    vector<string> productCategories(n);

    cout << "Enter product details (name, price, category):\n";
    for (int i = 0; i < n; i++) {
        cin.ignore(); // Clear input buffer
        cout << "Product " << i + 1 << " Name: ";
        getline(cin, productNames[i]);
        cout << "Product " << i + 1 << " Price: ";
        cin >> productPrices[i];
        cin.ignore();
        cout << "Product " << i + 1 << " Category: ";
        getline(cin, productCategories[i]);
    }

    // String Matching (Search by Name/Description)
    string keyword;
    cout << "\nEnter a keyword to search for a product (by name/description): ";
    cin.ignore();
    getline(cin, keyword);
    stringMatch(productNames, keyword);

    // Linear Search (Search by Category)
    string category;
    cout << "\nEnter a category to list all products: ";
    getline(cin, category);
    linearSearch(productCategories, productNames, category);

    // Binary Search (Search by Price)
    sort(productPrices.begin(), productPrices.end()); // Sort prices for Binary Search
    cout << "\nSorted Prices: ";
    for (int price : productPrices) {
        cout << price << " ";
    }
    cout << "\n";

    int targetPrice;
    cout << "Enter a price to search for a product: ";
    cin >> targetPrice;
    int priceIndex = binarySearch(productPrices, targetPrice);
    if (priceIndex != -1) {
        cout << "Product with price " << targetPrice << " found in the catalog.\n";
    } else {
        cout << "No product found with the price " << targetPrice << ".\n";
    }

    return 0;
}
