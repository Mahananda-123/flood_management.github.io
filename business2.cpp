#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

// Structure to hold gadget data
struct Gadget {
    string name;
    int salesCount;
    double price;
};

// Quick Sort partition function
int partition(vector<Gadget>& gadgets, int low, int high) {
    int pivot = gadgets[high].salesCount; // Pivot is the salesCount of the last element
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (gadgets[j].salesCount <= pivot) {
            ++i;
            swap(gadgets[i], gadgets[j]);
        }
    }
    swap(gadgets[i + 1], gadgets[high]);
    return i + 1;
}

// Quick Sort function
void quickSort(vector<Gadget>& gadgets, int low, int high) {
    if (low < high) {
        int pi = partition(gadgets, low, high);
        quickSort(gadgets, low, pi - 1);
        quickSort(gadgets, pi + 1, high);
    }
}

// Function to display all gadgets
void displayGadgets(const vector<Gadget>& gadgets) {
    if (gadgets.empty()) {
        cout << "No gadgets available.\n";
        return;
    }

    cout << "\nGadgets List:\n";
    cout << "Name\t\tSalesCount\tPrice\n";
    for (const auto& gadget : gadgets) {
        cout << gadget.name << "\t\t" << gadget.salesCount
             << "\t\t" << fixed << setprecision(2) << gadget.price << "\n";
    }
}

// Function to increment or decrement prices
void adjustPrices(vector<Gadget>& gadgets, bool increment) {
    // Sort gadgets by sales count before applying the operation
    quickSort(gadgets, 0, gadgets.size() - 1);
    displayGadgets(gadgets); // Show sorted list to users

    string name;
    cout << "Enter gadget name to adjust price (type 'done' to finish): ";
    while (cin >> name && name != "done") {
        bool found = false;
        for (auto& gadget : gadgets) {
            if (gadget.name == name) {
                double adjustment = gadget.price * (gadget.salesCount / 100.0);
                if (increment) {
                    gadget.price += adjustment;
                    cout << "Price incremented for " << gadget.name << "\n";
                } else {
                    gadget.price -= adjustment;
                    if (gadget.price < 0) gadget.price = 0;
                    cout << "Price decremented for " << gadget.name << "\n";
                }
                found = true;
                break;
            }
        }
        if (!found) cout << "Gadget " << name << " not found!\n";
        cout << "Enter another gadget name (or 'done' to finish): ";
    }
}

// Other Functions (Add, Search, Remove, Apply Discount) remain the same
void addGadget(vector<Gadget>& gadgets) {
    Gadget gadget;
    cout << "Enter gadget name: ";
    cin.ignore();
    getline(cin, gadget.name);
    cout << "Enter sales count: ";
    cin >> gadget.salesCount;
    cout << "Enter price: ";
    cin >> gadget.price;
    gadgets.push_back(gadget);
    cout << "Gadget added successfully.\n";
}

void searchGadget(const vector<Gadget>& gadgets, const string& name) {
    for (const auto& gadget : gadgets) {
        if (gadget.name == name) {
            cout << "Gadget found: " << gadget.name
                 << ", Sales Count: " << gadget.salesCount
                 << ", Price: " << fixed << setprecision(2) << gadget.price << "\n";
            return;
        }
    }
    cout << "Gadget not found!\n";
}

void removeLowSales(vector<Gadget>& gadgets, int threshold) {
    size_t originalSize = gadgets.size();
    gadgets.erase(
        remove_if(gadgets.begin(), gadgets.end(), [threshold](const Gadget& g) {
            return g.salesCount < threshold;
        }),
        gadgets.end()
    );
    if (gadgets.size() < originalSize)
        cout << "Gadgets with sales count below " << threshold << " have been removed.\n";
    else
        cout << "No gadgets met the removal criteria.\n";
}

void applyDiscount(vector<Gadget>& gadgets, double discountPercentage) {
    for (auto& gadget : gadgets) {
        gadget.price -= gadget.price * (discountPercentage / 100.0);
        if (gadget.price < 0) gadget.price = 0;
    }
    cout << "Discount of " << discountPercentage << "% applied to all gadgets.\n";
}

int main() {
    vector<Gadget> gadgets = {
        {"Smartphone", 50, 299.99}, {"Tablet", 30, 199.99}, {"Laptop", 20, 999.99},
        {"Smartwatch", 70, 149.99}, {"Headphones", 80, 89.99}, {"Camera", 15, 499.99},
        {"Speaker", 60, 79.99}, {"Monitor", 25, 199.99}, {"Keyboard", 40, 49.99},
        {"Mouse", 45, 29.99}, {"Drone", 10, 599.99}, {"Projector", 5, 449.99},
        {"PowerBank", 90, 39.99}, {"Charger", 100, 19.99}, {"Router", 35, 129.99}
    };

    int choice;

    while (true) {
        cout << "\n--- Gadget Management System ---\n";
        cout << "1. Display All Gadgets\n";
        cout << "2. Search Gadget by Name\n";
        cout << "3. Increment Prices for Selected Gadgets\n";
        cout << "4. Decrement Prices for Selected Gadgets\n";
        cout << "5. Add New Gadget\n";
        cout << "6. Remove Gadgets with Low Sales Count\n";
        cout << "7. Apply Discount to All Gadgets\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayGadgets(gadgets);
                break;
            case 2: {
                string name;
                cout << "Enter gadget name to search: ";
                cin.ignore();
                getline(cin, name);
                searchGadget(gadgets, name);
                break;
            }
            case 3:
                adjustPrices(gadgets, true);
                break;
            case 4:
                adjustPrices(gadgets, false);
                break;
            case 5:
                addGadget(gadgets);
                break;
            case 6: {
                int threshold;
                cout << "Enter sales count threshold: ";
                cin >> threshold;
                removeLowSales(gadgets, threshold);
                break;
            }
            case 7: {
                double discountPercentage;
                cout << "Enter discount percentage: ";
                cin >> discountPercentage;
                applyDiscount(gadgets, discountPercentage);
                break;
            }
            case 8:
                cout << "Exiting program. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
    return 0;
}
