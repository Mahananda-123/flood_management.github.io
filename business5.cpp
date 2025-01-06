#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

class FraudDetectionGraph {
private:
    unordered_map<int, vector<int>> adj;

public:
    // Add a transaction (edge) between two users (nodes)
    void addTransaction(int userA, int userB) {
        adj[userA].push_back(userB);
    }

    // Remove a transaction (edge) between two users
    void removeTransaction(int userA, int userB) {
        auto& neighbors = adj[userA];
        neighbors.erase(remove(neighbors.begin(), neighbors.end(), userB), neighbors.end());
    }

    // Check if a transaction exists between two users
    bool transactionExists(int userA, int userB) {
        auto& neighbors = adj[userA];
        return find(neighbors.begin(), neighbors.end(), userB) != neighbors.end();
    }

    // Display all transactions
    // Display all transactions
void displayTransactions() {
    cout << "\nAll Transactions:\n";
    if (adj.empty()) {
        cout << "No transactions available.\n";
    } else {
        for (const auto& pair : adj) {
            int userA = pair.first;
            for (int userB : pair.second) {
                cout << "User " << userA << " -> User " << userB << endl;
            }
        }
    }
}


    // Detect cycle (possible money laundering activity)
    bool detectCycleUtil(int node, unordered_set<int>& visited, unordered_set<int>& recStack) {
        visited.insert(node);
        recStack.insert(node);

        for (int neighbor : adj[node]) {
            if (recStack.find(neighbor) != recStack.end()) {
                return true;  // Cycle detected
            }
            if (visited.find(neighbor) == visited.end() && detectCycleUtil(neighbor, visited, recStack)) {
                return true;
            }
        }

        recStack.erase(node);
        return false;
    }

    bool detectCycle() {
        unordered_set<int> visited, recStack;
        for (const auto& pair : adj) {
            int node = pair.first;
            if (visited.find(node) == visited.end()) {
                if (detectCycleUtil(node, visited, recStack)) {
                    return true;  // Fraud detected due to cycle
                }
            }
        }
        return false;  // No cycle found
    }

    // Find central nodes with high centrality (suspects for fraud intermediaries)
    void findCentralNodes() {
        unordered_map<int, int> inDegree;

        // Calculate in-degree (number of incoming edges/transactions)
        for (const auto& pair : adj) {
            for (int neighbor : pair.second) {
                inDegree[neighbor]++;
            }
        }

        cout << "\nUsers with high centrality (more connections to/from them):\n";
        bool found = false;
        for (const auto& pair : inDegree) {
            if (pair.second > 2) {  // Arbitrary threshold for centrality (more than 2 connections)
                cout << "User " << pair.first << " has " << pair.second << " connections.\n";
                found = true;
            }
        }

        if (!found) {
            cout << "No users with high centrality found.\n";
        }
    }

    // Remove a user (node) from the graph
    void removeUser(int user) {
        adj.erase(user);  // Remove all outgoing edges

        // Remove all incoming edges
        for (auto& pair : adj) {
            auto& neighbors = pair.second;
            neighbors.erase(remove(neighbors.begin(), neighbors.end(), user), neighbors.end());
        }

        cout << "User " << user << " and their transactions have been removed.\n";
    }

    // Show all users in the graph
    void displayUsers() {
        cout << "\nAll Users in the System:\n";
        for (const auto& pair : adj) {
            cout << "User " << pair.first << endl;
        }
    }

    // Get fraud statistics (number of users, transactions, etc.)
    void displayStats() {
        int totalTransactions = 0;
        for (const auto& pair : adj) {
            totalTransactions += pair.second.size();
        }

        cout << "\nSystem Statistics:\n";
        cout << "Total Users: " << adj.size() << endl;
        cout << "Total Transactions: " << totalTransactions << endl;
    }

    // Clear all data from the graph
    void clearSystem() {
        adj.clear();
        cout << "\nAll data has been cleared from the system.\n";
    }
};

int main() {
    FraudDetectionGraph graph;

    // Adding predefined transactions
    graph.addTransaction(1, 2);
    graph.addTransaction(2, 3);
    graph.addTransaction(3, 4);
    graph.addTransaction(4, 5);
    graph.addTransaction(5, 6);
    graph.addTransaction(6, 1); // This creates a cycle (1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 1)

    int choice;

    while (true) {
        cout << "\nFraud Detection System - E-commerce\n";
        cout << "1. Add Transaction\n";
        cout << "2. Remove Transaction\n";
        cout << "3. Display All Transactions\n";
        cout << "4. Detect Cycles (Money Laundering)\n";
        cout << "5. Find Central Fraud Suspects\n";
        cout << "6. Remove User\n";
        cout << "7. Display All Users\n";
        cout << "8. Display System Statistics\n";
        cout << "9. Clear All Data\n";
        cout << "10. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int userA, userB;
                cout << "Enter the users involved in the transaction (userA -> userB): ";
                cin >> userA >> userB;
                graph.addTransaction(userA, userB);
                cout << "Transaction added between User " << userA << " and User " << userB << endl;
                break;
            }
            case 2: {
                int userA, userB;
                cout << "Displaying all transactions:\n";
                graph.displayTransactions();  // Show all transactions first

                cout << "Enter the users involved in the transaction to remove (userA -> userB): ";
                cin >> userA >> userB;

                // Check if the transaction exists before removing
                if (graph.transactionExists(userA, userB)) {
                    graph.removeTransaction(userA, userB);  // Remove the specified transaction
                    cout << "Transaction removed between User " << userA << " and User " << userB << endl;
                } else {
                    cout << "Transaction between User " << userA << " and User " << userB << " does not exist.\n";
                }

                // Display updated transactions
                cout << "\nUpdated Transactions:\n";
                graph.displayTransactions();
                break;
            }

            case 3:
                graph.displayTransactions();
                break;
            case 4:
                if (graph.detectCycle()) {
                    cout << "Fraud detected! Possible money laundering detected due to cycle.\n";
                } else {
                    cout << "No cycles detected. No suspicious activity.\n";
                }
                break;
            case 5:
                graph.findCentralNodes();
                break;
            case 6: {
                int user;
                cout << "Displaying all transactions:\n";
                graph.displayTransactions();  // Show all transactions first

                cout << "Enter the user ID to remove: ";
                cin >> user;

                // Remove the user and their transactions
                graph.removeUser(user);

                // Display updated transactions after user removal
                cout << "\nUpdated Transactions:\n";
                graph.displayTransactions();
                break;
            }

            case 7:
                graph.displayUsers();
                break;
            case 8:
                graph.displayStats();
                break;
            case 9:
                graph.clearSystem();
                break;
            case 10:
                cout << "Exiting the system. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice, please try again.\n";
        }
    }

    return 0;
}
