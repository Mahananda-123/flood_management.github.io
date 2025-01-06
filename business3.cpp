#include<iostream>
#include<vector>
#include<string>
using namespace std;

// Binary Search Tree Node
struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int value) : data(value), left(nullptr), right(nullptr) {}
};

// Function to insert a value into the BST
TreeNode* insertBST(TreeNode* root, int value) {
    if (!root) return new TreeNode(value);
    if (value < root->data)
        root->left = insertBST(root->left, value);
    else
        root->right = insertBST(root->right, value);
    return root;
}

// Inorder Traversal of BST (sorted output)
void inorderBST(TreeNode* root) {
    if (!root) return;
    inorderBST(root->left);
    cout << root->data << " ";
    inorderBST(root->right);
}

// Quick Sort
void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (arr[j] < pivot) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        int pi = i + 1;

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Merge Sort
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Main function
int main() {
    cout << "Welcome to E-Commerce Order Management System!\n";

    // Binary Search Tree for storing customer order IDs
    TreeNode* root = nullptr;
    int nOrders;
    cout << "Enter the number of customer orders: ";
    cin >> nOrders;

    cout << "Enter the order IDs (to store in BST): ";
    for (int i = 0; i < nOrders; i++) {
        int orderID;
        cin >> orderID;
        root = insertBST(root, orderID);
    }

    cout << "\nOrder IDs (sorted using BST inorder traversal): ";
    inorderBST(root);
    cout << "\n";

    // Quick Sort for sorting delivery times
    int nTimes;
    cout << "\nEnter the number of delivery times to sort: ";
    cin >> nTimes;

    vector<int> deliveryTimes(nTimes);
    cout << "Enter the delivery times (in minutes): ";
    for (int i = 0; i < nTimes; i++) {
        cin >> deliveryTimes[i];
    }

    quickSort(deliveryTimes, 0, nTimes - 1);
    cout << "\nSorted Delivery Times (using Quick Sort): ";
    for (int time : deliveryTimes) {
        cout << time << " ";
    }
    cout << "\n";

    // Merge Sort for sorting order amounts
    int nAmounts;
    cout << "\nEnter the number of order amounts to sort: ";
    cin >> nAmounts;

    vector<int> orderAmounts(nAmounts);
    cout << "Enter the order amounts: ";
    for (int i = 0; i < nAmounts; i++) {
        cin >> orderAmounts[i];
    }

    mergeSort(orderAmounts, 0, nAmounts - 1);
    cout << "\nSorted Order Amounts (using Merge Sort): ";
    for (int amount : orderAmounts) {
        cout << amount << " ";
    }
    cout << "\n";

    return 0;
}
