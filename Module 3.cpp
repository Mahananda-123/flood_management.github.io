#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <limits>
#include <map>

// Define the flood threshold for risk zones
const float FLOOD_THRESHOLD = 5.0;

// Structure to represent a point in the map
struct Place
{
     // Name of the place
    std::string name;
     // Elevation of the place
    float elevation;
     // Flood depth at the place
    float floodDepth;

    // Number of rescued people
    int rescued;
    // Number of males rescued
    int maleRescued;
    // Number of females rescued
    int femaleRescued;
    // Number of injured people
    int injured;
    // Number of deaths
    int deaths;

    Place(const std::string &name = "", float elevation = 0)
        : name(name), elevation(elevation), floodDepth(0), rescued(0), maleRescued(0), femaleRescued(0), injured(0), deaths(0) {}

};

// Class to represent the flood risk map
class FloodRiskMap
{
private:
    // List of places
    std::vector<Place> places;

    void initializePlaces()
    {
        // Initialize places with sample data
        places = {
            {"Colaba", 3.0}, {"Marine Drive", 2.5}, {"Dadar", 6.0}, {"Bandra", 7.0},
            {"Andheri", 8.0}, {"Borivali", 10.0}, {"Thane", 15.0}, {"Navi Mumbai", 5.0},
            {"Panvel", 4.0}, {"Vasai", 6.5}, {"Virar", 12.0}, {"Kalyan", 7.5}};
    }

public:
    FloodRiskMap()

    {
        initializePlaces();
    }

    // Identify flood risk zones based on elevation
    void identifyFloodRiskZones()

    {
        std::cout << "\nFlood Risk Zones:\n";
        for (const auto &place : places)

        {
            if (place.elevation < FLOOD_THRESHOLD)

            {
                std::cout << "Risk Zone: " << place.name << " with elevation " << place.elevation << "\n";
            }

        }

    }

    // Calculate flood depth based on water level

    void calculateFloodDepth(float waterLevel)

    {
        std::cout << "\nFlood Depth Mapping:\n";
        for (auto &place : places)

        {
            if (place.elevation < waterLevel)

            {
                place.floodDepth = waterLevel - place.elevation;
                std::cout << "Flood depth at " << place.name << " is " << place.floodDepth << "\n";
            }
            else
            {
                place.floodDepth = 0;
            }
        }
    }

    // Save the map to a file
    void saveMapToFile(const std::string &filename)
    {
        std::ofstream outFile(filename);
        if (outFile.is_open())
        {
            for (const auto &place : places)
            {
                outFile << place.name << " " << place.elevation << " " << place.floodDepth << " "
                        << place.rescued << " " << place.maleRescued << " " << place.femaleRescued << " "
                        << place.injured << " " << place.deaths << "\n";
            }
            outFile.close();
            std::cout << "\nMap saved to " << filename << "\n";
        }
        else
        {
            std::cerr << "\nUnable to open file for writing.\n";
        }
    }

    // Print the map for visualization
    void printMap()
    {
        std::cout << "\nPlace Elevation and Flood Depth:\n";
        for (const auto &place : places)
        {
            std::cout << place.name << " - Elevation: " << std::setw(5) << place.elevation
                      << " Flood Depth: " << std::setw(5) << place.floodDepth
                      << " Rescued: " << place.rescued
                      << " Injured: " << place.injured
                      << " Deaths: " << place.deaths << "\n";
        }
    }

    // Analyze flood risk and count risk zones
    void analyzeFloodRisk()
    {
        int riskCount = 0;
        for ( const auto &place : places)
        {
            if (place.elevation < FLOOD_THRESHOLD)
            {
                riskCount++;
            }
        }
        std::cout << "\nTotal flood risk zones identified: " << riskCount << "\n";
    }

    // Display statistics about flood risk zones
    void displayFloodRiskStatistics()
    {
        int riskCount = 0;
        float totalElevation = 0;
        for (const auto &place : places)
        {
            if (place.elevation < FLOOD_THRESHOLD)
            {
                riskCount++;
                totalElevation += place.elevation;
            }

        }

        float averageElevation = (riskCount > 0) ? (totalElevation / riskCount) : 0;
        std::cout << "\nTotal flood risk zones: " << riskCount << "\n";
        std::cout << "Average elevation of risk zones: " << averageElevation << "\n";

    }

    // Find the nearest flood risk zone to a given place
    void findNearestFloodRiskZone(const std::string &placeName)
    {
        auto it = std::find_if(places.begin(), places.end(), [&](const Place &place)
                               { return place.name == placeName; });

        if (it == places.end())
        {
            std::cout << "\nPlace " << placeName << " not found.\n";
            return;
        }

        const Place &currentPlace = *it;
        float minDistance = std::numeric_limits<float>::max();
        std::string nearestPlace = "None";

        for (const auto &place : places)
        {
            if (place.elevation < FLOOD_THRESHOLD && place.name != currentPlace.name)
            {
                float distance = std::abs(currentPlace.elevation - place.elevation);
                if (distance < minDistance)
                {
                    minDistance = distance;
                    nearestPlace = place.name;
                }
            }
        }

        if (nearestPlace != "None")
        {
            std::cout << "\nNearest flood risk zone to " << placeName << " is " << nearestPlace << "\n";
        }
        else
        {
            std::cout << "\nNo flood risk zones found.\n";
        }
    }

    // Simulate flood event
    void simulateFloodEvent(float waterLevel)
    {
        std::cout << "\nSimulating flood event with water level: " << waterLevel << "\n";
        calculateFloodDepth(waterLevel);
        identifyFloodRiskZones();
    }

    // Track rescue details for a specific place
    void trackRescueDetails(const std::string &placeName, int males, int females, int injured, int deaths)
    {
        auto it = std::find_if(places.begin(), places.end(), [&](const Place &place) { return place.name == placeName; });
        if (it != places.end())
        {
            it->maleRescued += males;
            it->femaleRescued += females;
            it->rescued += (males + females);
            it->injured += injured;
            it->deaths += deaths;

            std::cout << "Rescue details updated for " << placeName << ".\n";
        }
        else
        {
            std::cout << "Place " << placeName << " not found.\n";
        }
    }

    // Display rescue statistics for all places
    void displayRescueStatistics()
    {

        std::cout << "\nRescue Statistics:\n";
        std::cout << std::setw(15) << "Place"
                  << std::setw(10) << "Rescued"
                  << std::setw(10) << "Males"
                  << std::setw(10) << "Females"
                  << std::setw(10) << "Injured"
                  << std::setw(10) << "Deaths" << "\n";
        for (const auto &place : places)

        {
            std::cout << std::setw(15) << place.name
                      << std::setw(10) << place.rescued
                      << std::setw(10) << place.maleRescued
                      << std::setw(10) << place.femaleRescued
                      << std::setw(10) << place.injured
                      << std::setw(10) << place.deaths << "\n";

        }

    }

    // Analyze rescue operations

   // Analyze rescue operations
void analyzeRescueOperations()
{

    int totalRescued = 0;
    int totalMales = 0;
    int totalFemales = 0;
    int totalInjured = 0;
    int totalDeaths = 0;
    for (const auto &place : places)

    {
        // Corrected line
        totalRescued += place.rescued;
        totalMales += place.maleRescued;
        totalFemales += place.femaleRescued;
        totalInjured += place.injured;
        totalDeaths += place.deaths;

    }

    std::cout << "\nRescue Operation Summary:\n";
    std::cout << "Total Rescued: " << totalRescued << "\n";
    std::cout << "Males Rescued: " << totalMales << "\n";
    std::cout << "Females Rescued: " << totalFemales << "\n";
    std::cout << "Total Injured: " << totalInjured << "\n";
    std::cout << "Total Deaths: " << totalDeaths << "\n";
    if (totalRescued > 0)

    {
        std::cout << "Percentage of Males Rescued: " << (static_cast<float>(totalMales) / totalRescued) * 100 << "%\n";
        std::cout << "Percentage of Females Rescued: " << (static_cast<float>(totalFemales) / totalRescued) * 100 << "%\n";
    }

}

    // Sort places by elevation
    void sortPlacesByElevation()
    {
        std::sort(places.begin(), places.end(), [](const Place &a, const Place &b) {
            return a.elevation < b.elevation;
        });

        std::cout << "\nPlaces sorted by elevation.\n";
    }

    // Sort places by flood depth
    void sortPlacesByFloodDepth()
    {
        std::sort(places.begin(), places.end(), [](const Place &a, const Place &b) {
            return a.floodDepth < b.floodDepth;
        });
        std::cout << "\nPlaces sorted by flood depth.\n";
    }

    // Export rescue statistics to a file
    void exportRescueStatistics(const std::string &filename)
    {
        std::ofstream outFile(filename);
        if (outFile.is_open())
        {
            outFile << std::setw(15) << "Place"
                    << std::setw(10) << "Rescued"
                    << std::setw(10) << "Males"
                    << std::setw(10) << "Females"
                    << std::setw(10) << "Injured"
                    << std::setw(10) << "Deaths" << "\n";
            for (const auto &place : places)
            {
                outFile << std::setw(15) << place.name
                        << std::setw(10) << place.rescued
                        << std::setw(10) << place.maleRescued
                        << std::setw(10) << place.femaleRescued
                        << std::setw(10) << place.injured
                        << std::setw(10) << place.deaths << "\n";
            }
            outFile.close();
            std::cout << "Rescue statistics exported to " << filename << ".\n";
        }
        else
        {
            std::cerr << "Unable to open file for writing.\n";
        }
    }

    // Extended run function with new menu options
    void run()
    {
        int choice;
        float waterLevel;
        std::string filename, placeName;
        int males, females, injured, deaths;

        do
        {
            displayMainMenu();
            std::cin >> choice;

            switch (choice)
            {
            case 1:
                identifyFloodRiskZones();
                break;

            case 2:
                std::cout << "Enter water level: ";
                std::cin >> waterLevel;
                calculateFloodDepth(waterLevel);
                break;

            case 3:
                std::cout << "Enter filename to save map: ";
                std::cin >> filename;
                saveMapToFile(filename);
                break;

            case 4:
                printMap();
                break;

            case 5:
                analyzeFloodRisk();
                break;

            case 6:
                displayFloodRiskStatistics();
                break;

            case 7:
                std::cout << "Enter place name to find nearest flood risk zone: ";
                std::cin.ignore();
                std::getline(std::cin, placeName);
                findNearestFloodRiskZone(placeName);
                break;

            case 8:
                std::cout << "Enter water level for flood simulation: ";
                std::cin >> waterLevel;
                simulateFloodEvent(waterLevel);
                break;

            case 9:
                std::cout << "Enter place name to add: ";
                std::cin.ignore();
                std::getline(std::cin, placeName);
                float elevation;
 std::cout << "Enter elevation for " << placeName << ": ";
                std::cin >> elevation;
                addPlace(placeName, elevation);
                break;

            case 10:
                std::cout << "Enter place name to remove: ";
                std::cin.ignore();
                std::getline(std::cin, placeName);
                removePlace(placeName);
                break;

            case 11:
                std::cout << "Enter place name for rescue tracking: ";
                std::cin.ignore();
                std::getline(std::cin, placeName);
                std::cout << "Enter number of males rescued: ";
                std::cin >> males;
                std::cout << "Enter number of females rescued: ";
                std::cin >> females;
                std::cout << "Enter number of injured people: ";
                std::cin >> injured;
                std::cout << "Enter number of deaths: ";
                std::cin >> deaths;
                trackRescueDetails(placeName, males, females, injured, deaths);
                break;

            case 12:
                displayRescueStatistics();
                break;

            case 13:
                analyzeRescueOperations();
                break;

            case 14:
                std::cout << "Enter filename to load places: ";
                std::cin >> filename;
                loadPlacesFromFile(filename);
                break;

            case 15:
                std::cout << "Enter filename to save places: ";
                std::cin >> filename;
                savePlacesToFile(filename);
                break;

            case 16:
                displayAllPlaces();
                break;

            case 17:
                std::cout << "Enter place name to find: ";
                std::cin.ignore();
                std::getline(std::cin, placeName);
                findPlaceByName(placeName);
                break;

            case 18:
                std::cout << "Enter place name to update elevation: ";
                std::cin.ignore();
                std::getline(std::cin, placeName);
                std::cout << "Enter new elevation for " << placeName << ": ";
                std::cin >> elevation;
                updatePlaceElevation(placeName, elevation);
                break;

            case 19:
                sortPlacesByElevation();
                break;

            case 20:
                sortPlacesByFloodDepth();
                break;

            case 21:
                std::cout << "Enter filename to export rescue statistics: ";
                std::cin >> filename;
                exportRescueStatistics(filename);
                break;

            case 22:
                std::cout << "Enter place name to display detailed statistics: ";
                std::cin.ignore();
                std::getline(std::cin, placeName);
                displayDetailedStatistics(placeName);
                break;

            case 23:
                std::cout << "Enter elevation threshold to filter places: ";
                std::cin >> elevation;
                filterPlacesByElevation(elevation);
                break;

            case 24:
                std::cout << "Enter place name to reset statistics: ";
                std::cin.ignore();
                std::getline(std::cin, placeName);
                resetPlaceStatistics(placeName);
                break;

            case 25:
                std::cout << "Enter filename to load rescue statistics: ";
                std::cin >> filename;
                loadRescueStatisticsFromFile(filename);
                break;

            case 0:
                std::cout << "\nExiting program.\n";
                break;

            default:
                std::cout << "\nInvalid choice. Please try again.\n";
            }

        } while (choice != 0);

    }

    void displayMainMenu()
    {

        std::cout << "\nFlood Risk Mapping Menu:\n";
        std::cout << "1. Identify Flood Risk Zones\n";
        std::cout << "2. Calculate Flood Depth\n";
        std::cout << "3. Save Map to File\n";
        std::cout << "4. Print Map\n";
        std::cout << "5. Analyze Flood Risk\n";
        std::cout << "6. Display Flood Risk Statistics\n";
        std::cout << "7. Find Nearest Flood Risk Zone\n";
        std::cout << "8. Simulate Flood Event\n";
        std::cout << "9. Add New Place\n";
        std::cout << "10. Remove Place\n";
        std::cout << "11. Track Rescue Details\n";
        std::cout << "12. Display Rescue Statistics\n";
        std::cout << "13. Analyze Rescue Operations\n";
        std::cout << "14. Load Places from File\n";
        std::cout << "15. Save Places to File\n";
        std::cout << "16. Display All Places\n";
        std::cout << "17. Find Place by Name\n";
        std::cout << "18. Update Place Elevation\n";
        std::cout << "19. Sort Places by Elevation\n";
        std::cout << "20. Sort Places by Flood Depth\n";
        std::cout << "21. Export Rescue Statistics\n";
        std::cout << "22. Display Detailed Statistics for a Place\n";
        std::cout << "23. Filter Places by Elevation\n";
        std::cout << "24. Reset Place Statistics\n";
        std::cout << "25. Load Rescue Statistics from File\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";

    }

    // Reset statistics for a specific place
    void resetPlaceStatistics(const std::string &name)
    {
        auto it = std::find_if(places.begin(), places.end(), [&](const Place &place) { return place.name == name; });
        if (it != places.end())
        {

            it->rescued = 0;
            it->maleRescued = 0;
            it->femaleRescued = 0;
            it->injured = 0;
            it->deaths = 0;
            std::cout << "Statistics reset for " << name << ".\n";

        }
        else
        {
            std::cout << "Place " << name << " not found.\n";
        }
    }

    // Load rescue statistics from a file

    void loadRescueStatisticsFromFile(const std::string &filename)
    {
        std::ifstream inFile(filename);
        if (inFile.is_open())
        {

            std::string name;
            int rescued, males, females, injured, deaths;
            while (inFile >> name >> rescued >> males >> females >> injured >> deaths)
            {

                auto it = std::find_if(places.begin(), places.end(), [&](const Place &place) { return place.name == name; });
                if (it != places.end())
                {

                    it->rescued = rescued;
                    it->maleRescued = males;
                    it->femaleRescued = females;
                    it->injured = injured;
                    it->deaths = deaths;

                }

                else

                {
                    std::cout << "Place " << name << " not found in the map.\n";
                }

            }
            inFile.close();
            std::cout << "Rescue statistics loaded from " << filename << ".\n";

        }
        else

        {
            std::cerr << "Unable to open file for reading.\n";
        }

    }

    // Add a new place
    void addPlace(const std::string &name, float elevation)

    {
        if (elevation < 0)

        {
            std::cout << "Elevation cannot be negative. Place not added.\n";
            return;
        }

        places.emplace_back(name, elevation);
        std::cout << "Place " << name << " added with elevation " << elevation << ".\n";
    }

    // Remove a place from the map

    void removePlace(const std::string &name)

    {
        auto it = std::remove_if(places.begin(), places.end(), [&](const Place &place) { return place.name == name; });
        if (it != places.end())

        {
            places.erase(it, places.end());
            std::cout << "Place " << name << " removed from the map.\n";
        }
        else

        {
            std::cout << "Place " << name << " not found.\n";
        }

    }

    // Load places from a file

    void loadPlacesFromFile(const std::string &filename)
    {

        std::ifstream inFile(filename);
        if (inFile.is_open())
        {

            std::string name;
            float elevation;
            while (inFile >> name >> elevation)

            {
                addPlace(name, elevation);
            }

            inFile.close();
            std::cout << "Places loaded from " << filename << ".\n";
        }

        else

        {

            std::cerr << "Unable to open file for reading.\n";

        }

    }

    // Save places to a file

    void savePlacesToFile(const std::string &filename)
    {

        std::ofstream outFile(filename);
        if (outFile.is_open())
        {

            for (const auto &place : places)

            {
                outFile << place.name << " " << place.elevation << "\n";
            }

            outFile.close();
            std::cout << "Places saved to " << filename << ".\n";
        }
        else

        {
            std::cerr << "Unable to open file for writing.\n";

        }

    }

    // Display all places

    void displayAllPlaces()

    {
        std::cout << "\nAll Places:\n";
        for (const auto &place : places )

        {
            std::cout << place.name << " - Elevation: " << place.elevation << "\n";
        }

    }

    // Find a place by name

    void findPlaceByName(const std::string &name)

    {
        auto it = std::find_if(places.begin(), places.end(), [&](const Place &place) { return place.name == name; });
        if (it != places.end())

        {

            std::cout << "Found: " << it->name << " - Elevation: " << it->elevation << "\n";

        }
        else

        {
            std::cout << "Place " << name << " not found.\n";
        }

    }

    // Update the elevation of a place
    void updatePlaceElevation(const std::string &name, float newElevation)
    {

        if (newElevation < 0)
        {
            std::cout << "Elevation cannot be negative. Update failed.\n";
            return;
        }

        auto it = std::find_if(places.begin(), places.end(), [&](const Place &place) { return place.name == name; });
        if (it != places.end())

        {
            it->elevation = newElevation;
            std::cout << "Updated elevation of " << name << " to " << newElevation << ".\n";

        }
        else

        {
            std::cout << "Place " << name << " not found.\n";
        }

    }

    // Display detailed statistics for a specific place

    void displayDetailedStatistics(const std::string &placeName)
    {

        auto it = std::find_if(places.begin(), places.end(), [&](const Place &place) { return place.name == placeName; });
        if (it != places.end())
        {

            std::cout << "\nDetailed Statistics for " << it->name << ":\n";
            std::cout << "Elevation: " << it->elevation << "\n";
            std::cout << "Flood Depth: " << it->floodDepth << "\n";
            std::cout << "Total Rescued: " << it->rescued << "\n";
            std::cout << "Males Rescued: " << it->maleRescued << "\n";
            std::cout << "Females Rescued: " << it->femaleRescued << "\n";
            std::cout << "Injured: " << it->injured << "\n";
            std::cout << "Deaths: " << it->deaths << "\n";

        }
        else

        {
            std::cout << "Place " << placeName << " not found.\n";
        }

    }

    // Filter places by elevation threshold

    void filterPlacesByElevation(float threshold)
    {

        std::cout << "\nPlaces with elevation below " << threshold << ":\n";
        for (const auto &place : places)
        {

            if (place.elevation < threshold)
            {
                std::cout << place.name << " - Elevation: " << place.elevation << "\n";
            }

        }
    }
};

// Main function to run the program
int main()
{
    FloodRiskMap floodMap;
    floodMap.run();
    return 0;
}
