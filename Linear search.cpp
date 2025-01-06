#include <iostream>
#include <vector>
#include <string>

int linearSearchStrings(const std::vector<std::string>& data, const std::string& target) {
    for (int i = 0; i < data.size(); i++) {
        if (data[i] == target) {
            return i; // Found the string
        }
    }
    return -1; // Not found
}

int main() {
    std::vector<std::string> catalog = {"Laptop", "Smartphone", "Tablet", "Headphones"};
    std::string searchItem = "Tablet";

    int index = linearSearchStrings(catalog, searchItem);
    if (index != -1) {
        std::cout << "Found: " << catalog[index] << std::endl;
    } else {
        std::cout << "Not Found" << std::endl;
    }

    return 0;
}
