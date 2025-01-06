
#include <iostream>
#include <vector>
using namespace std;

// Function for Bubble Sort
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Function for Selection Sort
void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        swap(arr[i], arr[minIndex]);
    }
}

// Function for Binary Search
int binarySearch(const vector<int>& arr, int key) {
    int low = 0, high = arr.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == key) {
            return mid;
        } else if (arr[mid] < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1; // Key not found
}

int main() {
    cout << "Welcome to Inventory Management System!\n";

    // Input for Bubble Sort (Turnover rates)
    int nTurnover;
    cout << "Enter the number of products (for turnover rate sorting): ";
    cin >> nTurnover;

    vector<int> turnoverRates(nTurnover);
    cout << "Enter the turnover rates of products: ";
    for (int i = 0; i < nTurnover; i++) {
        cin >> turnoverRates[i];
    }

    bubbleSort(turnoverRates);
    cout << "Turnover rates sorted (using Bubble Sort): ";
    for (int rate : turnoverRates) {
        cout << rate << " ";
    }
    cout << "\n";

    // Input for Selection Sort (Product values)
    int nValues;
    cout << "Enter the number of products (for value sorting): ";
    cin >> nValues;

    vector<int> productValues(nValues);
    cout << "Enter the product values: ";
    for (int i = 0; i < nValues; i++) {
        cin >> productValues[i];
    }

    selectionSort(productValues);
    cout << "Product values sorted (using Selection Sort): ";
    for (int value : productValues) {
        cout << value << " ";
    }
    cout << "\n";

    // Input for Binary Search (Search for product stock levels)
    int nStock;
    cout << "Enter the number of products in stock (for searching): ";
    cin >> nStock;

    vector<int> stockLevels(nStock);
    cout << "Enter the stock levels of products: ";
    for (int i = 0; i < nStock; i++) {
        cin >> stockLevels[i];
    }

    selectionSort(stockLevels); // Ensure the stock levels are sorted
    cout << "Stock levels sorted (using Selection Sort): ";
    for (int stock : stockLevels) {
        cout << stock << " ";
    }
    cout << "\n";

    int searchKey;
    cout << "Enter the stock level to search (for reordering check): ";
    cin >> searchKey;

    int searchResult = binarySearch(stockLevels, searchKey);
    if (searchResult != -1) {
        cout << "Stock level " << searchKey << " found at position " << searchResult << ".\n";
    } else {
        cout << "Stock level " << searchKey << " not found (consider reordering).\n";
    }

    return 0;
}
