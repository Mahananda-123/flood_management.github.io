#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

// Structure to represent a location with coordinates and an address
struct Location {
    double x, y;        // x and y coordinates
    string address;     // Address of the location
};

// Structure to represent a driver with their details
struct Driver {
    string name;         // Name of the driver
    double maxCapacity;  // Maximum load capacity
    double currentCapacity; // Current load
    Location location;   // Current location of the driver
};

// Structure to represent a delivery with its details
struct Delivery {
    Location location;   // Delivery location
    double weight;       // Weight of the delivery
    string status;       // Status of the delivery (e.g., "Pending")
};

// Utility function to calculate the Euclidean distance between two locations
double calculateDistance(Location a, Location b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// Function to insert a new city into the system
void insertCity(vector<Location>& cities) {
    Location newCity;
    cout << "Enter the coordinates of the city (x y): ";
    cin >> newCity.x >> newCity.y; // Input city coordinates
    cout << "Enter the name of the city: ";
    cin.ignore();
    getline(cin, newCity.address); // Input city address
    cities.push_back(newCity); // Add city to the list
    cout << "City added successfully!\n";
}

// Function to insert a new driver into the system
void insertDriver(vector<Driver>& drivers) {
    Driver newDriver;
    cout << "Enter the driver's name: ";
    cin.ignore();
    getline(cin, newDriver.name); // Input driver's name
    cout << "Enter the driver's maximum capacity: ";
    cin >> newDriver.maxCapacity; // Input max capacity
    cout << "Enter the driver's current capacity: ";
    cin >> newDriver.currentCapacity; // Input current capacity
    cout << "Enter the driver's location (x y): ";
    cin >> newDriver.location.x >> newDriver.location.y; // Input location coordinates
    cout << "Enter the address of the location: ";
    cin.ignore();
    getline(cin, newDriver.location.address); // Input location address
    drivers.push_back(newDriver); // Add driver to the list
    cout << "Driver added successfully!\n";
}

// Function to insert a new delivery into the system
void insertDelivery(vector<Delivery>& deliveries) {
    Delivery newDelivery;
    cout << "Enter the delivery location coordinates (x y): ";
    cin >> newDelivery.location.x >> newDelivery.location.y; // Input delivery location
    cout << "Enter the delivery location address: ";
    cin.ignore();
    getline(cin, newDelivery.location.address); // Input location address
    cout << "Enter the weight of the delivery: ";
    cin >> newDelivery.weight; // Input weight
    newDelivery.status = "Pending"; // Set initial status
    deliveries.push_back(newDelivery); // Add delivery to the list
    cout << "Delivery added successfully!\n";
}

// Function to display distances of all cities from the warehouse
void case1(const vector<Location>& cities) {
    cout << "Available cities and their distances from Warehouse: \n";
    Location warehouse = cities[0]; // Assume the first city is the warehouse
    for (int i = 1; i < cities.size(); ++i) {
        double distance = calculateDistance(warehouse, cities[i]); // Calculate distance
        cout << cities[i].address << ": " << distance << " km" << endl;
    }
}

// Function to calculate the shortest distance and time between two cities using Prim's Algorithm
void case2(const vector<Location>& cities) {
    cout << "Available cities: \n";
    for (int i = 0; i < cities.size(); ++i) {
        cout << i + 1 << ". " << cities[i].address << endl;
    }

    cout << "Enter the city numbers to calculate distance and time (1-indexed): ";
    int city1, city2;
    cin >> city1 >> city2;
    city1--; city2--; // Convert to 0-indexed

    int n = cities.size();
    vector<vector<double>> adjMatrix(n, vector<double>(n, numeric_limits<double>::infinity()));

    // Fill the adjacency matrix with distances between cities
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double dist = calculateDistance(cities[i], cities[j]);
            adjMatrix[i][j] = dist;
            adjMatrix[j][i] = dist;
        }
    }

    // Prim's algorithm to find the shortest path between city1 and city2
    vector<double> minDist(n, numeric_limits<double>::infinity()); // Minimum distances
    vector<bool> visited(n, false); // Visited nodes
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq; // Min-heap

    minDist[city1] = 0;
    pq.push({0, city1});

    while (!pq.empty()) {
        double d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (visited[u]) continue; // Skip already visited nodes
        visited[u] = true;

        for (int v = 0; v < n; ++v) {
            if (!visited[v] && adjMatrix[u][v] < numeric_limits<double>::infinity()) {
                if (minDist[v] > minDist[u] + adjMatrix[u][v]) {
                    minDist[v] = minDist[u] + adjMatrix[u][v];
                    pq.push({minDist[v], v});
                }
            }
        }
    }

    double distance = minDist[city2]; // Shortest distance
    cout << "Shortest distance between " << cities[city1].address << " and " << cities[city2].address << ": " << distance << " km" << endl;

    // Calculate time assuming a fixed speed of 50 km/h
    double time = distance / 50.0;
    cout << "Time to travel: " << time << " hours" << endl;
}

// Function to find the closest driver to a delivery location
void searchClosestDriver(const vector<Driver>& drivers, const vector<Location>& cities) {
    cout << "Available cities: \n";
    for (int i = 0; i < cities.size(); ++i) {
        cout << i + 1 << ". " << cities[i].address << endl;
    }

    cout << "Enter the city number to find the closest driver: ";
    int cityIndex;
    cin >> cityIndex;
    cityIndex--; // Convert to 0-indexed

    if (cityIndex < 0 || cityIndex >= cities.size()) {
        cout << "Invalid city number.\n";
        return;
    }

    Location cityLocation = cities[cityIndex]; // Get selected city's location
    double minDistance = numeric_limits<double>::max(); // Minimum distance
    Driver closestDriver; // Closest driver

    // Find the closest driver to the selected city
    for (const auto& driver : drivers) {
        double distance = calculateDistance(driver.location, cityLocation);
        if (distance < minDistance) {
            minDistance = distance;
            closestDriver = driver;
        }
    }

    cout << "The closest driver to the city " << cities[cityIndex].address << " is " << closestDriver.name << endl;
}

// Function to optimize the delivery route using a greedy approach (TSP Approximation)
void optimizeDeliveryRoute(const vector<Location>& cities, const Location& warehouse) {
    cout << "Enter the number of cities to optimize the route: ";
    int numCities;
    cin >> numCities;

    vector<int> cityIndexes(numCities);
    cout << "Enter city numbers (1-indexed) to visit (e.g. 1 for Warehouse, 2 for Jaipur, etc.): ";
    for (int i = 0; i < numCities; ++i) {
        cin >> cityIndexes[i];
        cityIndexes[i]--; // Convert to 0-indexed
    }

    vector<bool> visited(cities.size(), false); // Visited status
    vector<Location> optimizedRoute; // Optimized route
    Location currentLocation = warehouse; // Start from

        visited[0] = true; // Mark warehouse as visited
    optimizedRoute.push_back(currentLocation); // Add warehouse to the route

    while (optimizedRoute.size() <= numCities) {
        double minDistance = numeric_limits<double>::max();
        int nextCityIndex = -1;

        // Find the closest unvisited city
        for (int i = 0; i < numCities; ++i) {
            if (!visited[cityIndexes[i]]) {
                double distance = calculateDistance(currentLocation, cities[cityIndexes[i]]);
                if (distance < minDistance) {
                    minDistance = distance;
                    nextCityIndex = cityIndexes[i];
                }
            }
        }

        // If no more cities are unvisited, break the loop
        if (nextCityIndex == -1) break;

        // Move to the next closest city
        visited[nextCityIndex] = true;
        currentLocation = cities[nextCityIndex];
        optimizedRoute.push_back(currentLocation);
    }

    // Add the warehouse back to complete the route
    optimizedRoute.push_back(warehouse);

    // Print the optimized route
    cout << "Optimized delivery route: \n";
    for (const auto& loc : optimizedRoute) {
        cout << loc.address << " (" << loc.x << ", " << loc.y << ")\n";
    }

    // Calculate total distance
    double totalDistance = 0.0;
    for (size_t i = 1; i < optimizedRoute.size(); ++i) {
        totalDistance += calculateDistance(optimizedRoute[i - 1], optimizedRoute[i]);
    }

    cout << "Total distance covered: " << totalDistance << " km\n";
}

// Main function to manage the system
int main() {
    // Predefined cities
    vector<Location> cities = {
        {0, 0, "Warehouse"},
        {26.9124, 75.7873, "Jaipur"},
        {24.5854, 73.7125, "Udaipur"},
        {26.2957, 73.0163, "Jodhpur"},
        {26.9157, 70.9075, "Jaisalmer"},
        {28.0229, 73.3120, "Bikaner"},
        {26.4927, 74.5516, "Pushkar"},
        {26.4453, 74.6399, "Ajmer"},
        {24.8797, 74.6261, "Chittorgarh"},
        {25.2138, 75.8689, "Kota"},
        {27.3963, 76.6129, "Alwar"},
        {27.6100, 75.2130, "Sikar"},
        {25.7988, 73.0011, "Pali"},
        {25.7585, 71.4187, "Barmer"},
        {29.9160, 72.2707, "Sri Ganganagar"},
        {26.0955, 75.7899, "Tonk"}
    };

    // Predefined drivers
    vector<Driver> drivers = {
        {"Alice", 50.0, 30.0, {0, 0, "Warehouse"}},
        {"Bob", 40.0, 20.0, {26.9124, 75.7873, "Jaipur"}},
        {"Charlie", 60.0, 40.0, {24.5854, 73.7125, "Udaipur"}},
        {"David", 50.0, 30.0, {26.2957, 73.0163, "Jodhpur"}},
        {"Eve", 45.0, 25.0, {26.9157, 70.9075, "Jaisalmer"}},
        {"Frank", 60.0, 35.0, {28.0229, 73.3120, "Bikaner"}},
        {"Grace", 50.0, 28.0, {26.4927, 74.5516, "Pushkar"}},
        {"Hank", 70.0, 50.0, {26.4453, 74.6399, "Ajmer"}},
        {"Ivy", 55.0, 40.0, {24.8797, 74.6261, "Chittorgarh"}},
        {"Jack", 65.0, 45.0, {25.2138, 75.8689, "Kota"}},
        {"John", 55.0, 35.0, {27.3963, 76.6129, "Alwar"}},
        {"Liam", 50.0, 30.0, {27.6100, 75.2130, "Sikar"}},
        {"Mia", 45.0, 30.0, {25.7988, 73.0011, "Pali"}},
        {"Nina", 50.0, 30.0, {25.7585, 71.4187, "Barmer"}},
        {"Oliver", 60.0, 40.0, {29.9160, 72.2707, "Sri Ganganagar"}},
        {"Paul", 65.0, 45.0, {26.0955, 75.7899, "Tonk"}}
    };

    // Predefined delivery
    vector<Delivery> deliveries = {
        {{26.9124, 75.7873, "Jaipur"}, 10.0, "Pending"}
    };

    int choice;
    do {
        cout << "\n--- Delivery Management System ---\n";
        cout << "1. Display distances of all cities from the warehouse\n";
        cout << "2. Calculate shortest distance and time between two cities\n";
        cout << "3. Insert a new city\n";
        cout << "4. Insert a new driver\n";
        cout << "5. Insert a new delivery\n";
        cout << "6. Search for the closest driver to a delivery location\n";
        cout << "7. Optimize delivery route\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                case1(cities);
                break;
            case 2:
                case2(cities);
                break;
            case 3:
                insertCity(cities);
                break;
            case 4:
                insertDriver(drivers);
                break;
            case 5:
                insertDelivery(deliveries);
                break;
            case 6:
                searchClosestDriver(drivers, cities);
                break;
            case 7:
                optimizeDeliveryRoute(cities, cities[0]); // Pass the warehouse as the starting point
                break;
            case 8:
                cout << "Exiting the system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 8);

    return 0;
}
