#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cmath> // For anomaly detection
#include <algorithm> // For max_element (in time-series analysis)
#include <fstream>
#include <sstream>
#include <unordered_map>
#include<bits/stdc++.h>


const double g = 9.81; // Gravitational acceleration (m/s^2)


struct DrainageRoute
{
    std::string routeName;

     // in meters
    double length;

    // in liters per second
    double flowRate;

    // in percentage (0 to 100)
    double backflowRisk;

    // in liters (max water capacity of the route)
    double capacity;

    // current flow in liters per second
    double currentFlow;

     // water quality index
    double waterQualityIndex;

     // in days
    int maintenanceFrequency;

    // in L/s
    double runoffVolume;

    // in %
    double loadPercentage;

    //
    double floodRiskScore;

};


struct Village
{
    std::string name;
    std::vector<DrainageRoute> drainageRoutes;

    // in mm
    double averageRainfall;

     // population of the village
    int population;

     // total runoff in cubic meters
    double totalRunoff;

    // total capacity in cubic meters
    double totalCapacity;

    // area size in square kilometers
    double areaSize;

};

struct Region
{
    std::string name;
    std::vector<Village> villages;
};

// Represents drainage components like sluice gates or pumps
struct DrainageComponent
 {
    std::string componentName;
     // Maximum water flow it can handle (L/s)
    double capacity;

    // Current flow in the component (L/s)
    double currentFlow;

     // Percentage of risk (0-100)
    double backflowRisk;

     // Status of the component
    bool isOperational;
};

// Represents distributed control system state
struct DistributedControlState
 {
    std::vector<DrainageComponent> components;
    double totalSystemCapacity;
    double totalFlowDemand;
};
struct Edge
 {
      // Destination node
    std::string to;

    // Capacity of the drainage system (e.g., max flow rate)
    double capacity;

    // Current flow rate
    double flowRate;

    // Slope of the pipe/channel
    double slope;
    Edge(std::string to, double capacity, double flowRate, double slope)
        : to(to), capacity(capacity), flowRate(flowRate), slope(slope) {}
};

struct Edge2
{
    std::string src, dest;
    double weight;
};
// Class to represent the drainage network as a graph
class DrainageNetwork
{
private:
    std::unordered_map<std::string, std::vector<Edge>> adjList;  // Adjacency list for the graph

public:
    // Function to add a node (junction, basin, or component)
    void addNode(const std::string& nodeName)
    {
        // Node is automatically created when adding an edge, so this function is not strictly necessary
    }

    // Function to add an edge (representing a drainage route between two nodes)
    void addEdge(const std::string& from, const std::string& to, double capacity, double flowRate, double slope) {
        // Create a new edge and add it to the adjacency list
        adjList[from].push_back(Edge(to, capacity, flowRate, slope));
    }

    // Function to display the drainage network
    void displayNetwork()
    {
        for (const auto& node : adjList)
        {
            std::cout << "Node: " << node.first << "\n";
            for (const auto& edge : node.second) {
                std::cout << "  - To: " << edge.to << ", Capacity: " << edge.capacity
                     << ", Flow Rate: " << edge.flowRate << ", Slope: " << edge.slope << "\n";
            }
        }
    }
};
double manningPipeFlow(double diameter, double slope, double roughness, double Q_guess, double tol = 1e-6)
{
    double r = diameter / 2.0;  // Radius
    double A = M_PI * r * r;    // Full cross-sectional area
    double P = 2.0 * M_PI * r;  // Wetted perimeter
    double R = A / P;           // Hydraulic radius

    double Q = Q_guess;

    while (true) {
        double V = (1.0 / roughness) * std::pow(R, 2.0 / 3.0) * std::sqrt(slope);
        double Q_new = V * A;

        if (std::abs(Q_new - Q) < tol) {
            break;
        }

        Q = Q_new;
    }

    return Q;
}

void saintVenantEquations(std::vector<double>& Q, std::vector<double>& A, double S0, double n, double dx, double dt, double max_time)
{
    size_t num_points = Q.size();
    size_t time_steps = static_cast<size_t>(max_time / dt);

    for (size_t t = 0; t < time_steps; ++t)
    {
        std::vector<double> Q_new = Q;
        std::vector<double> A_new = A;

        for (size_t i = 1; i < num_points - 1; ++i)
        {
            double dQdx = (Q[i + 1] - Q[i - 1]) / (2.0 * dx);
            double dAdx = (A[i + 1] - A[i - 1]) / (2.0 * dx);

            double Sf = (n * n * Q[i] * Q[i]) / std::pow(A[i], 10.0 / 3.0);
            Q_new[i] -= dt * (g * A[i] * S0 - g * A[i] * Sf - Q[i] * dQdx);
            A_new[i] -= dt * dAdx;
        }

        Q = Q_new;
        A = A_new;
    }
}

void simulateDrainageNetwork()
{
    // Open channel parameters
    size_t num_points = 100;
    std::vector<double> Q(num_points, 0.0); // Flow rate (m^3/s)
    std::vector<double> A(num_points, 1.0); // Cross-sectional area (m^2)
    double S0 = 0.01;  // Bed slope
    double n = 0.03;   // Manning's roughness coefficient
    double dx = 1.0;   // Spatial step (m)
    double dt = 0.1;   // Time step (s)
    double max_time = 10.0; // Simulation time (s)

    // Simulate using Saint-Venant equations
    saintVenantEquations(Q, A, S0, n, dx, dt, max_time);

    std::cout << "Final flow rates in open channel:" << std::endl;
    for (const auto& q : Q)
    {
        std::cout << q << " ";
    }
    std::cout << std::endl;

    // Pipe flow parameters
    double diameter = 0.5;   // Pipe diameter (m)
    double slope = 0.02;     // Pipe slope
    double roughness = 0.015; // Manning's roughness coefficient
    double Q_guess = 0.5;    // Initial guess for flow rate (m^3/s)

    // Calculate steady-state pipe flow
    double pipe_flow = manningPipeFlow(diameter, slope, roughness, Q_guess);

    std::cout << "Steady-state pipe flow rate: " << pipe_flow << " m^3/s" << std::endl;
}

void displayDrainageRoutes(const std::vector<DrainageRoute>& routes)
{
    std::cout << "Drainage Routes Available in this Village:" << std::endl;
    for (size_t i = 0; i < routes.size(); ++i) {
        std::cout << i + 1 << ". " << routes[i].routeName
                  << " - Length: " << routes[i].length << " meters"
                  << " - Flow Rate: " << routes[i].flowRate << " L/s"
                  << " - Backflow Risk: " << routes[i].backflowRisk << "%"
                  << " - Capacity: " << routes[i].capacity << " L" << std::endl;
    }
    std::cout << "-----------------------------\n";
}


int getValidChoice(int maxChoice)
 {
    int choice;
    while (true)
    {
        std::cin >> choice;
        if (std::cin.fail() || choice < 1 || choice > maxChoice)
        {
            std::cin.clear(); // clear error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
            std::cout << "Invalid input. Please enter a number between 1 and " << maxChoice << ": ";
        }
        else
        {
            return choice;
        }
    }
}

void displayAffectedVillages(const std::vector<Village>& villages)
{
    std::cout << "Affected Villages:" << std::endl;
    for (size_t i = 0; i < villages.size(); ++i)
    {
        std::cout << i + 1 << ". " << villages[i].name << std::endl;
    }
    std::cout << "-----------------------------\n";
}

void analyzeDrainageCapacity(const Village& village)
{
    std::cout << "\nAnalyzing drainage capacity for village: " << village.name << "\n";
    for (const auto& drain : village.drainageRoutes)
        {
        double capacityUtilization = (static_cast<double>(drain.currentFlow) / drain.capacity) * 100;
        std::cout << "Drain: " << drain.routeName
                  << " | Capacity Utilization: " << capacityUtilization << "%\n";

        if (capacityUtilization > 80.0) {
            std::cout << "Warning: Drain nearing maximum capacity. Consider upgrading or rerouting.\n";
        }
    }
}

// Flood Risk Assessment Function
void assessFloodRisk(const Village& village)
 {
    std::cout << "Flood Risk Assessment for " << village.name << ":\n";

    double totalCapacity = 0.0;
    double overflowVolume = 0.0;
    for (const auto& route : village.drainageRoutes) {
        totalCapacity += route.capacity;
    }

    // Calculate total expected runoff based on rainfall and area (simplified)
    double runoffVolume = village.averageRainfall * 10000; // Assuming 1mm rain = 10L/sq.m over 1 hectare

    std::cout << "Average Rainfall: " << village.averageRainfall << " mm\n";
    std::cout << "Total Drainage Capacity: " << totalCapacity << " L\n";
    std::cout << "Estimated Runoff Volume: " << runoffVolume << " L\n";

    if (runoffVolume > totalCapacity)
    {
        overflowVolume = runoffVolume - totalCapacity;
        std::cout << "WARNING: Overflow Detected!\n";
        std::cout << "Overflow Volume: " << overflowVolume << " L\n";
        std::cout << "Critical Failure Zones:\n";
        for (const auto& route : village.drainageRoutes)
        {
            if (route.backflowRisk > 50)
            {
                std::cout << "- " << route.routeName << " (Backflow Risk: " << route.backflowRisk << "%)\n";
            }
        }
    }
    else
    {
        std::cout << "Drainage System is Adequate.\n";
    }
    std::cout << "-----------------------------\n";
}

// Data-Driven Prediction and Management Function
void predictAndManageFloodRisk(const Village& village)
 {
    std::cout << "Data-Driven Prediction and Management for " << village.name << ":\n";

    // Calculate total drainage capacity for IoT simulation
    double totalCapacity = 0.0;
    for (const auto& route : village.drainageRoutes)
    {
        totalCapacity += route.capacity;
    }

    // Example: Simple machine learning-like logic for rainfall prediction (based on average and variance)
    double predictedRainfall = village.averageRainfall + std::sin(village.averageRainfall / 50.0) * 20;
    std::cout << "Predicted Rainfall: " << predictedRainfall << " mm\n";

    // Example: Anomaly detection (drainage flow rate deviations)
    std::cout << "Anomaly Detection in Drainage Flow Rates:\n";
    for (const auto& route : village.drainageRoutes)
    {
        double expectedFlow = route.length * route.flowRate * 0.8; // Hypothetical expected value
        if (std::fabs(route.flowRate - expectedFlow) / expectedFlow > 0.2)  // 20% deviation threshold
        {
            std::cout << "- Anomaly Detected in " << route.routeName << "\n";
        }
    }

    // Example: Using IoT data (simulated for now)
    std::cout << "Integration of IoT Data:\n";
    double simulatedRainGauge = village.averageRainfall * 0.9; // Simulated data
    std::cout << "- Rain Gauge Data: " << simulatedRainGauge << " mm\n";
    double simulatedWaterSensor = totalCapacity * 0.7; // Simulated water level
    std::cout << "- Water Level Sensor Data: " << simulatedWaterSensor << " L\n";

    std::cout << "-----------------------------\n";

}
// Function to calculate optimal pipe size
void optimizePipeSizes(const Village& village)
{
    std::cout << "Optimizing Pipe Sizes for " << village.name << ":\n";
    for (const auto& route : village.drainageRoutes)
    {
        double diameter = std::sqrt((4 * route.flowRate) / (3.1415 * 0.7)); // Manning's equation (simplified)
        double slope = std::max(0.001, 0.01 * (route.backflowRisk / 100.0)); // Adjust slope based on backflow risk
        std::cout << "- Route: " << route.routeName
                  << " | Optimal Diameter: " << diameter << " m"
                  << " | Optimal Slope: " << slope << " m/m\n";
    }
    std::cout << "-----------------------------\n";
}

// Function to calculate retention and detention basin sizes
void calculateBasinSizes(const Village& village)
{
    std::cout << "Calculating Basin Sizes for " << village.name << ":\n";
    double totalRunoff = village.averageRainfall * 10000; // Total runoff volume in liters
    double retentionBasinSize = totalRunoff * 0.3; // 30% retention
    double detentionBasinSize = totalRunoff * 0.7; // 70% detention
    std::cout << "- Retention Basin Size: " << retentionBasinSize << " L\n";
    std::cout << "- Detention Basin Size: " << detentionBasinSize << " L\n";
    std::cout << "-----------------------------\n";
}

// Function for dynamic routing optimization
void dynamicRoutingOptimization(const Village& village)
{
    std::cout << "Dynamic Routing Optimization for " << village.name << ":\n";
    double totalCapacity = 0.0;
    for (const auto& route : village.drainageRoutes)
    {
        totalCapacity += route.capacity;
    }

    // Simulate high-intensity rainfall scenario
    double peakRunoff = village.averageRainfall * 10000 * 1.5; // 50% increase in runoff
    if (peakRunoff > totalCapacity)
    {
        std::cout << "ALERT: Overcapacity detected during peak runoff.\n";
        std::cout << "Suggested Routing Adjustments:\n";
        for (const auto& route : village.drainageRoutes)
        {
            if (route.backflowRisk < 50)
            {
                double additionalFlow = (peakRunoff - totalCapacity) / village.drainageRoutes.size();
                std::cout << "- Increase flow in " << route.routeName
                          << " by " << additionalFlow << " L/s\n";
            }
        }
    }
    else
    {
        std::cout << "Routing is stable under high-intensity rainfall.\n";
    }
    std::cout << "-----------------------------\n";
}
void controlStrategiesForSluiceGatesAndPumps(const Village& village)
{
    std::cout << "Control Strategies for Sluice Gates and Pumps in " << village.name << ":\n";

    for (const auto& route : village.drainageRoutes)
    {
        if (route.backflowRisk > 50)
        {
            std::cout << "- Route: " << route.routeName
                      << " | Strategy: Increase pump capacity to " << route.capacity * 1.2 << " L\n";
        }
        else
        {
            std::cout << "- Route: " << route.routeName
                      << " | Strategy: Adjust sluice gate to release " << route.flowRate * 0.9 << " L/s\n";
        }
    }

    std::cout << "-----------------------------\n";
}
void realTimeFlowAdjustment(const Village& village)
{
    std::cout << "Real-Time Flow Adjustment for " << village.name << ":\n";

    for (const auto& route : village.drainageRoutes)
    {
        if (route.flowRate > route.capacity * 0.8)
        {
            std::cout << "- Route: " << route.routeName
                      << " | Redirect " << route.flowRate * 0.2 << " L/s to auxiliary channels.\n";
        }
        else
        {
            std::cout << "- Route: " << route.routeName
                      << " | Maintain current flow.\n";
        }
    }

    std::cout << "-----------------------------\n";
}
void minimizeFloodImpact(const Village& village)
{
    std::cout << "Routing Algorithms to Minimize Flood Impact for " << village.name << ":\n";

    double totalFlow = 0.0;
    for (const auto& route : village.drainageRoutes)
    {
        totalFlow += route.flowRate;
    }

    double redistributionFactor = totalFlow / village.drainageRoutes.size();

    for (auto& route : village.drainageRoutes)
        {
        if (route.backflowRisk < 40)
        {
            std::cout << "- Route: " << route.routeName
                      << " | Increase flow to " << route.flowRate + redistributionFactor * 0.1 << " L/s\n";
        }
        else
        {
            std::cout << "- Route: " << route.routeName
                      << " | Decrease flow to " << route.flowRate - redistributionFactor * 0.1 << " L/s\n";
        }
    }

    std::cout << "-----------------------------\n";
}

void geospatialAnalysisAndVisualization(const Village& village)
{
    std::cout << "Geospatial Analysis and Visualization for " << village.name << ":\n";

    // GIS Mapping of Drainage Networks
    std::cout << "GIS Mapping of Drainage Networks:\n";
    for (const auto& route : village.drainageRoutes)
    {
        std::cout << "- Route: " << route.routeName
                  << " | Length: " << route.length << " meters\n";
    }

    // Integration of Topographic and Land Use Data
    std::cout << "Integrating Topographic and Land Use Data:\n";
    double topographicFactor = 0.0;
    double landUseImpact = 0.0;

    for (const auto& route : village.drainageRoutes)
    {
        topographicFactor += route.length * 0.02; // Example calculation
        landUseImpact += route.flowRate * 0.05;  // Example calculation
    }

    std::cout << "- Topographic Factor: " << topographicFactor << "\n";
    std::cout << "- Land Use Impact: " << landUseImpact << " L/s effect on drainage\n";

    // Visualization of Flood Extent
    std::cout << "Flood Extent Visualization:\n";
    for (const auto& route : village.drainageRoutes)
    {
        double floodRiskIndex = (route.flowRate / route.capacity) * 100;
        std::cout << "- Route: " << route.routeName
                  << " | Flood Risk Index: " << floodRiskIndex << "%\n";
    }

    std::cout << "-----------------------------\n";
}

void costBenefitAnalysisForNetworkUpgrades(const Village& village)
{
    std::cout << "Cost-Benefit Analysis for Network Upgrades in " << village.name << ":\n";

    double totalUpgradeCost = 0.0;
    double totalCapacityIncrease = 0.0;
    double resilienceScore = 0.0;

    std::cout << "Evaluating Costs and Efficiency for Upgrading Drainage Systems:\n";
    for (const auto& route : village.drainageRoutes)
    {
        double upgradeCost = route.length * 500; // Example: $500 per meter upgrade cost
        double capacityIncrease = route.capacity * 0.2; // 20% capacity increase with upgrade
        double resilienceIncrease = route.backflowRisk < 50 ? 15 : 10; // Higher benefit for lower-risk routes

        totalUpgradeCost += upgradeCost;
        totalCapacityIncrease += capacityIncrease;
        resilienceScore += resilienceIncrease;

        std::cout << "- Route: " << route.routeName
                  << " | Upgrade Cost: $" << upgradeCost
                  << " | Capacity Increase: " << capacityIncrease << " L"
                  << " | Resilience Increase: " << resilienceIncrease << " points\n";
    }

    std::cout << "\nMulti-Objective Optimization Results:\n";
    std::cout << "- Total Upgrade Cost: $" << totalUpgradeCost << "\n";
    std::cout << "- Total Capacity Increase: " << totalCapacityIncrease << " L\n";
    std::cout << "- Resilience Score: " << resilienceScore << " points\n";

    // Decision Support: Priority Ranking
    std::cout << "\nDecision Support Tools - Prioritizing Interventions:\n";
    std::vector<std::pair<std::string, double>> priorities;
    for (const auto& route : village.drainageRoutes) {
        double benefitCostRatio = (route.capacity * 0.2 + route.flowRate * 0.1) / (route.length * 500);
        priorities.emplace_back(route.routeName, benefitCostRatio);
    }

    std::sort(priorities.begin(), priorities.end(),
              [](const std::pair<std::string, double>& a, const std::pair<std::string, double>& b)
               {
                  return b.second < a.second; // Higher ratio first
              });

    for (const auto& [routeName, ratio] : priorities)
    {
        std::cout << "- Route: " << routeName << " | Benefit-Cost Ratio: " << ratio << "\n";
    }

    std::cout << "-----------------------------\n";
}
void resilienceAnalysisUnderClimateChange(const Village& village)
{
    std::cout << "Resilience Analysis Under Climate Change for " << village.name << ":\n";

    // Factors for climate change impact
    double projectedRainfallIncrease = 0.2; // 20% increase in rainfall (example)
    double seaLevelRiseImpact = 0.05; // 5% additional risk from sea level rise

    double totalCapacity = 0.0;
    double adjustedRainfall = village.averageRainfall * (1 + projectedRainfallIncrease);

    // Calculate adjusted runoff volume based on projected rainfall
    double adjustedRunoffVolume = adjustedRainfall * 10000; // Adjusted runoff volume (simplified)

    std::cout << "Projected Rainfall Increase: " << projectedRainfallIncrease * 100 << "%\n";
    std::cout << "Adjusted Runoff Volume: " << adjustedRunoffVolume << " L\n";

    // Summing up the drainage route capacities
    for (const auto& route : village.drainageRoutes) {
        totalCapacity += route.capacity;
    }

    // Check if the drainage system can handle the adjusted runoff
    double overflowVolume = 0.0;
    if (adjustedRunoffVolume > totalCapacity)
    {
        overflowVolume = adjustedRunoffVolume - totalCapacity;
        std::cout << "WARNING: Overflow detected under projected rainfall conditions!\n";
        std::cout << "Overflow Volume: " << overflowVolume << " L\n";
    }
    else
    {
        std::cout << "Drainage system can handle adjusted rainfall.\n";
    }

    // Assessing additional risks due to sea level rise
    double additionalRisk = 0.0;
    for (const auto& route : village.drainageRoutes)
    {
        additionalRisk += route.backflowRisk * seaLevelRiseImpact;
    }

    std::cout << "Additional Risk due to Sea Level Rise: " << additionalRisk << "%\n";
    std::cout << "Resilience score adjustment due to climate change: " << additionalRisk << "%\n";

    // Display critical areas with the highest risk
    std::cout << "Critical Drainage Routes under Climate Change Impact:\n";
    for (const auto& route : village.drainageRoutes)
    {
        double routeRisk = route.backflowRisk + additionalRisk;
        std::cout << "- Route: " << route.routeName
                  << " | Adjusted Backflow Risk: " << routeRisk << "%\n";
    }

    std::cout << "-----------------------------\n";
}
// Function to adjust water flow in each drainage component
void adjustComponentFlow(DrainageComponent& component, double adjustment)
{
    if (!component.isOperational) return;

    // Apply flow adjustments, ensuring we do not exceed component capacity
    double newFlow = component.currentFlow + adjustment;
    component.currentFlow = std::max(0.0, std::min(newFlow, component.capacity));
    std::cout << "Adjusted flow for " << component.componentName << ": " << component.currentFlow << " L/s\n";
}

// Function to update the state of the distributed control system in real time
void updateDistributedControlSystem(DistributedControlState& systemState)
{
    // Assuming real-time sensor data and conditions
    double totalFlow = 0.0;
    for (auto& component : systemState.components)
    {
        totalFlow += component.currentFlow;
    }

    // Check if the total flow exceeds the system's capacity
    if (totalFlow > systemState.totalSystemCapacity)
        {
        std::cout << "Warning: Overcapacity in the system!\n";

        // Adjust flows to avoid overflow
        double overflow = totalFlow - systemState.totalSystemCapacity;
        double adjustmentPerComponent = overflow / systemState.components.size();

        for (auto& component : systemState.components)
        {
            adjustComponentFlow(component, -adjustmentPerComponent);  // Reduce flow in each component
        }
    }
    else
    {
        std::cout << "System flow is under control. Total flow: " << totalFlow << " L/s\n";
    }
}

// Function to simulate real-time coordination of drainage components
void coordinateDistributedDrainageSystem(DistributedControlState& systemState)
 {
    std::cout << "Coordinating drainage system...\n";

    // Calculate the required flow adjustments in real-time
    double totalFlow = 0.0;
    for (const auto& component : systemState.components)
    {
        totalFlow += component.currentFlow;
    }

    double adjustmentFactor = systemState.totalFlowDemand / totalFlow;

    // Adjust the flow in each component based on the required demand
    for (auto& component : systemState.components)
    {
        if (component.isOperational)
        {
            double flowAdjustment = (component.currentFlow * adjustmentFactor) - component.currentFlow;
            adjustComponentFlow(component, flowAdjustment);
        }
    }

    // Print the updated status of each component
    for (const auto& component : systemState.components)
    {
        std::cout << "Component: " << component.componentName
                  << " | Current Flow: " << component.currentFlow
                  << " L/s | Backflow Risk: " << component.backflowRisk << "%\n";
    }

    std::cout << "Distributed Control System Coordination Complete.\n";
}

// Function to simulate distributed decision-making in the drainage system
void distributedDecisionMakingForDrainage(std::vector<DrainageComponent>& components)
{
    std::cout << "Simulating distributed decision-making for drainage...\n";

    // Each component adjusts its flow based on local conditions
    for (auto& component : components)
    {
        if (component.backflowRisk > 50)
        {
            std::cout << "Component " << component.componentName << " has high backflow risk. Adjusting flow...\n";
            // Reduce flow if backflow risk is high
            adjustComponentFlow(component, -component.currentFlow * 0.2);
        }
        else if (component.currentFlow > component.capacity * 0.8)

        {
            std::cout << "Component " << component.componentName << " is near capacity. Redirecting flow...\n";
            // Redirect excess flow if capacity is near limit
            adjustComponentFlow(component, -component.currentFlow * 0.15);
        }
        else
        {
            std::cout << "Component " << component.componentName << " is operating within safe limits.\n";
        }
    }

    std::cout << "Distributed decision-making complete.\n";
}

// Function to simulate decentralized control of sluice gates and pumps
void decentralizedControlOfSluiceGatesAndPumps(std::vector<DrainageComponent>& components)
{
    std::cout << "Performing decentralized control of sluice gates and pumps...\n";

    for (auto& component : components)
    {
        if (component.isOperational)
        {
            // Control logic based on current flow, backflow risk, and operational status
            if (component.backflowRisk > 60)
            {
                std::cout << "Component " << component.componentName << " has high backflow risk. Increasing pump capacity...\n";
                component.currentFlow = std::min(component.capacity * 1.2, component.currentFlow + 10.0);
            }
        else
            {
                std::cout << "Component " << component.componentName << " flow is stable. Adjusting sluice gates...\n";
                component.currentFlow = std::max(0.0, component.currentFlow - 5.0);
            }
        }
    }

    std::cout << "Decentralized control complete.\n";
}

// Function to dynamically optimize routing based on real-time data
void dynamicRoutingOptimizationForDrainage(std::vector<DrainageComponent>& components, double systemCapacity)
{
    std::cout << "Dynamic routing optimization based on real-time data...\n";

    double totalFlow = 0.0;
    for (const auto& component : components)
    {
        totalFlow += component.currentFlow;
    }

    if (totalFlow > systemCapacity)
    {
        std::cout << "Overcapacity detected! Rebalancing flows...\n";
        // Reduce flow in overburdened components
        for (auto& component : components)
        {
            if (component.currentFlow > component.capacity * 0.8)
            {
                adjustComponentFlow(component, -component.currentFlow * 0.1);  // Reduce flow by 10%
            }
        }
    }
    else
    {
        std::cout << "System is within capacity. No flow adjustment needed.\n";
    }

    std::cout << "Routing optimization complete.\n";
}
void applyDistributedControlForVillage(Village& village)
 {
    // Create a distributed control system state
    DistributedControlState systemState;
    systemState.totalSystemCapacity = 0.0;
    systemState.totalFlowDemand = 0.0;

    // Initialize components (sluice gates, pumps, drainage routes)
    for (auto& route : village.drainageRoutes)
    {
        DrainageComponent component;
        component.componentName = route.routeName;
        component.capacity = route.capacity;
        component.currentFlow = route.flowRate;  // Assuming flowRate is the current flow
        component.backflowRisk = route.backflowRisk;
        component.isOperational = true;  // Assuming components are operational

        systemState.components.push_back(component);
        systemState.totalSystemCapacity += route.capacity;
        systemState.totalFlowDemand += route.flowRate;
    }
       // Coordinate the drainage system in real-time
    coordinateDistributedDrainageSystem(systemState);

    // Simulate decentralized control of sluice gates and pumps
    decentralizedControlOfSluiceGatesAndPumps(systemState.components);

    // Perform dynamic routing optimization
    dynamicRoutingOptimizationForDrainage(systemState.components, systemState.totalSystemCapacity);

    // Perform distributed decision-making
    distributedDecisionMakingForDrainage(systemState.components);
}


// Find the parent of a node (Union-Find)
int findParent(const std::vector<std::string>& nodes, std::vector<int>& parent, int index)
{
    if (parent[index] == index) return index;
    return parent[index] = findParent(nodes, parent, parent[index]);
}

// Union two sets (Union-Find)
void unionSets(std::vector<int>& parent, std::vector<int>& rank, int u, int v)
{
    if (rank[u] < rank[v]) {
        parent[u] = v;
    } else if (rank[u] > rank[v]) {
        parent[v] = u;
    } else {
        parent[v] = u;
        rank[u]++;
    }
}

std::vector<Edge2> kruskal(const std::vector<std::string>& nodes, std::vector<Edge2>& edges)
{
    std::vector<Edge2> mst;

    // Sort edges by weight
    std::sort(edges.begin(), edges.end(), [](const Edge2& a, const Edge2& b)
    {
        return a.weight < b.weight;
    });

    int n = nodes.size();
    std::vector<int> parent(n);
    std::vector<int> rank(n, 0);

    // Initialize Union-Find structure
    for (int i = 0; i < n; ++i)
    {
        parent[i] = i;
    }

    // Process each edge
    for (const auto& edge : edges)
    {
        auto srcIt = std::find(nodes.begin(), nodes.end(), edge.src);
        auto destIt = std::find(nodes.begin(), nodes.end(), edge.dest);

        if (srcIt != nodes.end() && destIt != nodes.end())
        {
            int srcIndex = std::distance(nodes.begin(), srcIt);
            int destIndex = std::distance(nodes.begin(), destIt);

            int srcParent = findParent(nodes, parent, srcIndex);
            int destParent = findParent(nodes, parent, destIndex);

            if (srcParent != destParent)
            {
                mst.push_back(edge);
                unionSets(parent, rank, srcParent, destParent);
            }
        }
    }

    return mst;
}
void displayMST(const std::vector<Edge2>& mst)
{
    std::cout << "\nOptimized Drainage Network (MST):\n";
    double totalWeight = 0.0;
    for (const auto& edge : mst)
    {
        std::cout << edge.src << " --(" << edge.weight << ")--> " << edge.dest << "\n";
        totalWeight += edge.weight;
    }
    std::cout << "Total Cost of Network: " << totalWeight << "\n";
}

void generateEmergencyResponsePlan(const Region& region, const Village& village, int supplyUnits = 100)
 {
    std::cout << "\nGenerating Emergency Response Plan for region: " << region.name
              << ", village: " << village.name << "\n";
    std::cout << "1. Identify safe evacuation routes.\n";
    std::cout << "2. Deploy rescue teams to high-risk areas.\n";
    std::cout << "3. Provide emergency supplies (" << supplyUnits << " units of water, food, medical kits) to shelters.\n";
    std::cout << "4. Activate communication channels to notify residents.\n";
    std::cout << "5. Set up water pumps and barriers to control floodwater.\n";
    std::cout << "Plan ready. Ensure periodic drills are conducted to improve readiness.\n";
}

void optimizeRainwaterHarvesting(const Region& region, const Village& village, double minRunoffVolume = 10000.0)
{
    std::cout << "\nOptimizing Rainwater Harvesting Points for region: " << region.name
              << ", village: " << village.name << "\n";
    for (const auto& drain : village.drainageRoutes)
        {
        if (drain.runoffVolume > minRunoffVolume)
        {
            std::cout << "Suggested harvesting point near: " << drain.routeName
                      << " | Runoff volume: " << drain.runoffVolume << " cubic meters\n";
        }
    }
    std::cout << "Rainwater harvesting plan generated.\n";
}

// Function to monitor water quality
void monitorWaterQuality(const Region& region, const Village& village, double qualityThreshold = 50.0)
 {
    std::cout << "\nMonitoring Water Quality in region: " << region.name
              << ", village: " << village.name << "\n";
    for (const auto& drain : village.drainageRoutes)
        {
        std::cout << "Drain: " << drain.routeName
                  << " | Water Quality Index: " << drain.waterQualityIndex << "\n";

        if (drain.waterQualityIndex < qualityThreshold) {
            std::cout << "Action Required: Poor water quality. Investigate sources of pollution.\n";
        }
    }
}

// Function to predict urban expansion impact
void predictUrbanExpansionImpact(const Region& region, const Village& village, double populationGrowthRate = 0.2, double runoffIncreaseRate = 1.5)
{
    std::cout << "\nPredicting Urban Expansion Impact for region: " << region.name
              << ", village: " << village.name << "\n";
    int additionalPopulation = village.population * populationGrowthRate;
    double increasedRunoff = runoffIncreaseRate * village.totalRunoff;

    std::cout << "Projected additional population: " << additionalPopulation << "\n";
    std::cout << "Projected increased runoff: " << increasedRunoff << " cubic meters\n";

    if (increasedRunoff > village.totalCapacity)
    {
        std::cout << "Warning: Infrastructure upgrades needed to handle projected growth.\n";
    }
    else
     {
        std::cout << "Current infrastructure can accommodate growth.\n";
    }
}

void simulateFloodEvent(const Village& village, double rainfallIntensity, double duration)
{
    std::cout << "Simulating flood event for " << village.name << "...\n";
    double estimatedFloodLevel = rainfallIntensity * duration / village.areaSize;
    std::cout << "Estimated flood level: " << estimatedFloodLevel << " meters.\n";
    if (estimatedFloodLevel > 1.0)
    {
        std::cout << "High risk of flooding detected!\n";
    }
    else
    {
        std::cout << "Flood risk is manageable.\n";
    }
}


void analyzeRainfallTrends(const Region& region)
{
    std::cout << "Analyzing rainfall trends for region: " << region.name << "...\n";
    // Simulated data
    double avgRainfall = 1200.0; // mm/year
    double rainfallDeviation = 15.0; // percentage
    std::cout << "Average annual rainfall: " << avgRainfall << " mm\n";
    std::cout << "Year-to-year rainfall deviation: " << rainfallDeviation << "%\n";
}
void recommendFloodInsurance(const Village& village)
{
    std::cout << "Recommending flood insurance policies for " << village.name << "...\n";
    for (const auto& drain : village.drainageRoutes)
        {
        if (drain.floodRiskScore > 50)
        {
            std::cout << "High-risk drain: " << drain.routeName << ". Suggest comprehensive insurance.\n";
        }
        else
            {

            std::cout << "Moderate-risk drain: " << drain.routeName << ". Suggest basic insurance.\n";
        }
    }
}
void optimizePumpDesign(const Village& village)
{
    std::cout << "Optimizing pump design for energy efficiency in " << village.name << "...\n";
    double energySavings = village.areaSize * 0.05; // 5% efficiency gain
    std::cout << "Estimated energy savings: " << energySavings << " kWh/year.\n";
}

// Helper function to get the current date as a string
std::string getCurrentDate()
{
    std::time_t t = std::time(nullptr);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", std::localtime(&t));
    return std::string(buffer);
}


// Helper function to add days to a date and return the new date as a string
std::string addDaysToDate(const std::string& date, int days) {
    std::tm tm = {};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    tm.tm_mday += days;
    std::mktime(&tm); // Normalize the date
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tm);
    return std::string(buffer);
}

// Function to schedule drain maintenance
void scheduleDrainMaintenance(const Village& village)
{
    std::cout << "Scheduling maintenance for drains in " << village.name << ":\n";

    // Step 1: Iterate through each drainage route in the village
    for (const auto& route : village.drainageRoutes)
        {
        std::cout << "\nDrain: " << route.routeName << "\n";

        // Step 2: Calculate the next maintenance date based on the current date and maintenance frequency
        int daysToNextMaintenance = route.maintenanceFrequency;
        std::cout << "Maintenance Frequency: Every " << daysToNextMaintenance << " days\n";

        // Step 3: Simulate the scheduling of maintenance tasks
        std::cout << "Scheduling maintenance...\n";

        // Step 4: Display the scheduled maintenance details
        std::cout << "Next Maintenance Date: ";
        std::string nextMaintenanceDate = addDaysToDate(getCurrentDate(), daysToNextMaintenance);
        std::cout << nextMaintenanceDate << "\n";
    }
}

void organizeCommunityAwareness(const Village& village)
 {
    std::cout << "Organizing community awareness programs in " << village.name << ":\n";

    // Step 1: Define the list of topics to cover in the awareness program
    std::vector<std::string> topics = {
        "Flood Risk Awareness",
        "Safety Measures During Floods",
        "Emergency Contact Information",
        "Importance of Drain Maintenance",
        "Community Participation in Flood Prevention"
    };

    // Step 2: Iterate through each topic and display it
    for (const auto& topic : topics)
        {
        std::cout << "Topic: " << topic << "\n";

        // Step 3: Simulate the preparation of materials for each topic
        std::cout << "Preparing materials for " << topic << "...\n";

        // Step 4: Simulate the scheduling of awareness sessions
        std::cout << "Scheduling awareness sessions for " << topic << "...\n";
    }

    // Step 5: Conclude the organization of the awareness program
    std::cout << "Community awareness programs organized successfully in " << village.name << ".\n";
}

// Define the WeatherData structure
struct WeatherData {
    std::string currentCondition;
    double expectedRainfall; // in mm
    double windSpeed; // in km/h
    double temperature; // in �C
    std::string floodRisk;
    std::string recommendedActions;
};

// Function to simulate fetching real-time weather data
WeatherData fetchWeatherData(const std::string& villageName)
 {
    // Simulated data fetching
    WeatherData data;
    data.currentCondition = "Partly Cloudy";
    data.expectedRainfall = 50.0; // Example data
    data.windSpeed = 15.0; // Example data
    data.temperature = 22.0; // Example data
    data.floodRisk = "Moderate";
    data.recommendedActions = "Monitor water levels and stay alert for updates.";
    return data;
}

// Function to simulate analyzing the impact of weather data on the drainage system
void analyzeWeatherImpact(WeatherData& weatherData)
 {
    // Simulated analysis
    if (weatherData.expectedRainfall > 100.0)
        {
        weatherData.floodRisk = "High";
        weatherData.recommendedActions = "Prepare for potential flooding and evacuate if necessary.";
    }
     else if (weatherData.expectedRainfall > 50.0)
    {
        weatherData.floodRisk = "Moderate";
        weatherData.recommendedActions = "Monitor water levels and stay alert for updates.";
    }
    else
    {
        weatherData.floodRisk = "Low";
        weatherData.recommendedActions = "No immediate action required.";
    }
}

// Function to simulate integrating weather data into the flood management system
void integrateWeatherData(const WeatherData& weatherData) {
    // Simulated integration
    std::cout << "Weather data integrated into the system.\n";
}

// Function to integrate real-time weather data
void integrateRealTimeWeather(const std::string& villageName)
{
    std::cout << "Integrating real-time weather data for " << villageName << ":\n";

    // Step 1: Simulate fetching real-time weather data from a weather API
    std::cout << "Fetching weather data...\n";
    WeatherData weatherData = fetchWeatherData(villageName);

    // Step 2: Display the fetched weather data
    std::cout << "Current Weather: " << weatherData.currentCondition << "\n";
    std::cout << "Expected Rainfall: " << weatherData.expectedRainfall << " mm in the next 24 hours\n";
    std::cout << "Wind Speed: " << weatherData.windSpeed << " km/h\n";
    std::cout << "Temperature: " << weatherData.temperature << " �C\n";

    // Step 3: Analyze the impact of the weather data on the drainage system
    std::cout << "Analyzing the impact of the weather data on the drainage system...\n";
    analyzeWeatherImpact(weatherData);

    // Step 4: Display the analysis results
    std::cout << "Analysis Results:\n";
    std::cout << "Potential Flood Risk: " << weatherData.floodRisk << "\n";
    std::cout << "Recommended Actions: " << weatherData.recommendedActions << "\n";

    // Step 5: Integrate the weather data into the flood management system
    std::cout << "Integrating weather data into the flood management system...\n";
    integrateWeatherData(weatherData);

    // Step 6: Conclude the integration process
    std::cout << "Real-time weather data integrated successfully for " << villageName << ".\n";
}
void calculateDrainageEfficiency(const Village& village)
{
    std::cout << "Calculating drainage efficiency for " << village.name << ":\n";

    // Step 1: Iterate through each drainage route in the village
    for (const auto& route : village.drainageRoutes)
    {
        std::cout << "Drain: " << route.routeName << "\n";

        // Step 2: Calculate the efficiency as a percentage
        double efficiency = (static_cast<double>(route.flowRate) / route.capacity) * 100;

        // Step 3: Display the calculated efficiency
        std::cout << "Flow Rate: " << route.flowRate << " units\n";
        std::cout << "Capacity: " << route.capacity << " units\n";
        std::cout << "Efficiency: " << efficiency << "%\n";

        // Step 4: Determine efficiency status
        std::string status = (efficiency > 75) ? "Efficient" : "Needs Improvement";
        std::cout << "Status: " << status << "\n";
    }
}

void simulateWaterFlowDynamics(const Village& village)
 {
    std::cout << "Simulating water flow dynamics for " << village.name << ":\n";

    // Step 1: Initialize simulation parameters
    double timeStep = 0.1; // Time step for simulation (in hours)
    double totalTime = 24.0; // Total time for simulation (in hours)
    double currentTime = 0.0;

    // Step 2: Iterate through the simulation time
    while (currentTime < totalTime)
        {
        std::cout << "Current Time: " << currentTime << " hours\n";

        // Step 3: Iterate through each drainage route in the village
        for (const auto& route : village.drainageRoutes)
        {
            // Step 4: Calculate water flow for the current time step
            double waterFlow = route.flowRate * timeStep;

            // Step 5: Display the simulated water flow
            std::cout << "Drain: " << route.routeName << " - Water Flow: " << waterFlow << " units\n";
        }

        // Step 6: Increment the current time by the time step
        currentTime += timeStep;
    }

    // Step 7: Conclude the simulation
    std::cout << "Water flow dynamics simulation completed for " << village.name << ".\n";
}
// Function to simulate progress of a task
void simulateProgress(const std::string& task)
{
    std::cout << task << " implementation started...\n";
    for (int i = 0; i <= 100; i += 20)
    {
        std::cout << task << " progress: " << i << "%\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simulate time delay
    }
    std::cout << task << " implementation completed.\n";
}

// Function to implement flood mitigation measures
void implementFloodMitigationMeasures(const Village& village)
 {
    std::cout << "Implementing flood mitigation measures for " << village.name << ":\n";

    // Step 1: Define a list of mitigation measures
    std::vector<std::string> measures =
    {
        "Constructing flood barriers",
        "Installing early warning systems",
        "Improving drainage capacity",
        "Creating flood retention ponds",
        "Implementing community evacuation plans"
    };

    // Step 2: Iterate through each mitigation measure
    for (const auto& measure : measures)
    {
        std::cout << "Measure: " << measure << "\n";

        // Step 3: Simulate the implementation of the measure
        std::cout << "Implementing " << measure << "...\n";

        // Step 4: Display the progress of implementation
        simulateProgress(measure);

        // Step 5: Display the completion of the measure
        std::cout << measure << " implemented successfully.\n";
    }

    // Step 6: Conclude the implementation of all measures
    std::cout << "All flood mitigation measures implemented successfully for " << village.name << ".\n";
}

void evaluateDrainageSystemPerformance(const Village& village)
 {
    std::cout << "Evaluating drainage system performance for " << village.name << ":\n";

    // Step 1: Ask the user for custom thresholds
    double efficiencyThreshold, maintenanceThreshold;
    std::cout << "Enter efficiency threshold (percentage): ";
    std::cin >> efficiencyThreshold;
    std::cout << "Enter maintenance threshold (days): ";
    std::cin >> maintenanceThreshold;

    // Step 2: Iterate through each drainage route in the village
    for (const auto& route : village.drainageRoutes)
        {
        std::cout << "Drain: " << route.routeName << "\n";

        // Step 3: Calculate the efficiency as a percentage
        double efficiency = (static_cast<double>(route.flowRate) / route.capacity) * 100;

        // Step 4: Display the calculated efficiency
        std::cout << "Flow Rate: " << route.flowRate << " units\n";
        std::cout << "Capacity: " << route.capacity << " units\n";
        std::cout << "Efficiency: " << efficiency << "%\n";

        // Step 5: Determine efficiency status
        std::string efficiencyStatus = (efficiency > efficiencyThreshold) ? "Efficient" : "Needs Improvement";
        std::cout << "Efficiency Status: " << efficiencyStatus << "\n";

        // Step 6: Check maintenance schedule
        std::string maintenanceStatus = (route.maintenanceFrequency <= maintenanceThreshold) ? "On Schedule" : "Overdue";
        std::cout << "Maintenance Frequency: Every " << route.maintenanceFrequency << " days\n";
        std::cout << "Maintenance Status: " << maintenanceStatus << "\n";
    }
}

void planDrainageNetworkExpansion(const Village& village)
 {
    std::cout << "Planning drainage network expansion for " << village.name << ":\n";

    // Step 1: Ask the user for projected population increase and budget constraints
    double projectedPopulationIncrease, budget;
    std::cout << "Enter projected population increase percentage: ";
    std::cin >> projectedPopulationIncrease;
    std::cout << "Enter budget for expansion (in units): ";
    std::cin >> budget;

    // Step 2: Iterate through each drainage route in the village
    for (const auto& route : village.drainageRoutes)
        {
        std::cout << "Drain: " << route.routeName << "\n";

        // Step 3: Calculate the expanded capacity based on projected population increase
        double expandedCapacity = route.capacity * (1 + projectedPopulationIncrease / 100);
        std::cout << "Current Capacity: " << route.capacity << " units\n";
        std::cout << "Expanded Capacity: " << expandedCapacity << " units\n";

        // Step 4: Estimate the cost of expansion
        double expansionCost = expandedCapacity * 10; // Assuming a cost factor (for simplicity)
        std::cout << "Estimated Expansion Cost: " << expansionCost << " units\n";

        // Step 5: Check if the expansion is within budget
        if (expansionCost <= budget) {
            std::cout << "Expansion is within budget.\n";
        } else {
            std::cout << "Expansion exceeds budget. Consider revising the plan.\n";
        }
    }
}

// Define the FloodEvent structure
struct FloodEvent
{
    std::string date;
    double rainfall; // in mm
    double waterLevel; // in meters
    std::string impact;
};

// Function to analyze historical flood data
void analyzeHistoricalFloodData(const Village& village, const std::vector<FloodEvent>& floodEvents)
 {
    std::cout << "Analyzing historical flood data for " << village.name << ":\n";

    // Step 1: Display the historical flood events
    std::cout << "Historical Flood Events:\n";
    for (const auto& event : floodEvents)
    {
        std::cout << "Date: " << event.date
                  << ", Rainfall: " << event.rainfall << " mm"
                  << ", Water Level: " << event.waterLevel << " meters"
                  << ", Impact: " << event.impact << "\n";
    }

    // Step 2: Analyze trends and patterns in the flood data
    double totalRainfall = 0.0;
    double maxWaterLevel = 0.0;
    for (const auto& event : floodEvents) {
        totalRainfall += event.rainfall;
        if (event.waterLevel > maxWaterLevel) {
            maxWaterLevel = event.waterLevel;
        }
    }
    double averageRainfall = totalRainfall / floodEvents.size();

    // Step 3: Display the analysis results
    std::cout << "\nAnalysis Results:\n";
    std::cout << "Total Rainfall: " << totalRainfall << " mm\n";
    std::cout << "Average Rainfall: " << averageRainfall << " mm\n";
    std::cout << "Maximum Water Level: " << maxWaterLevel << " meters\n";

    // Step 4: Provide recommendations based on the analysis
    std::cout << "\nRecommendations:\n";
    if (averageRainfall > 100.0)
       {
        std::cout << "1. Consider implementing additional flood barriers.\n";
        std::cout << "2. Enhance the drainage capacity of the village.\n";
    }
    else
    {
        std::cout << "1. Regular maintenance of existing drainage systems.\n";
        std::cout << "2. Monitor weather forecasts and prepare for potential floods.\n";
    }

    std::cout << "Historical flood data analysis completed for " << village.name << ".\n";

    // Step 3: Display the analysis results
    std::cout << "\nAnalysis Results:\n";
    std::cout << "Total Rainfall: " << totalRainfall << " mm\n";
    std::cout << "Average Rainfall: " << averageRainfall << " mm\n";
    std::cout << "Maximum Water Level: " << maxWaterLevel << " meters\n";

    // Step 4: Provide recommendations based on the analysis
    std::cout << "\nRecommendations:\n";
    if (averageRainfall > 100.0)
        {
        std::cout << "1. Consider implementing additional flood barriers.\n";
        std::cout << "2. Enhance the drainage capacity of the village.\n";
    }
    else
        {
        std::cout << "1. Regular maintenance of existing drainage systems.\n";
        std::cout << "2. Monitor weather forecasts and prepare for potential floods.\n";
    }

    std::cout << "Historical flood data analysis completed for " << village.name << ".\n";
}


void evaluateEnvironmentalImpact(const Village& village)
{
    std::cout << "Evaluating environmental impact for " << village.name << ":\n";

    // Step 1: Ask the user for pollution levels
    double pollutionLevel;
    std::cout << "Enter pollution level (in ppm): ";
    std::cin >> pollutionLevel;

    // Step 2: Ask the user for biodiversity indices
    double biodiversityIndex;
    std::cout << "Enter biodiversity index: ";
    std::cin >> biodiversityIndex;

    // Step 3: Display the input parameters
    std::cout << "Pollution Level: " << pollutionLevel << " ppm\n";
    std::cout << "Biodiversity Index: " << biodiversityIndex << "\n";

    // Step 4: Iterate through each drainage route in the village
    for (const auto& route : village.drainageRoutes)
        {
        std::cout << "Drain: " << route.routeName << "\n";

        // Step 5: Simulate the environmental impact assessment
        std::cout << "Assessing environmental impact for " << route.routeName << "...\n";

        // Step 6: Calculate the impact score (example calculation)
        double impactScore = (pollutionLevel * 0.7) + (biodiversityIndex * 0.3); // Example calculation

        // Step 7: Display the impact score
        std::cout << "Impact Score: " << impactScore << "\n";

        // Step 8: Determine the impact level
        std::string impactLevel = (impactScore > 100) ? "Severe" : (impactScore > 50) ? "Moderate" : "Low";
        std::cout << "Impact Level: " << impactLevel << "\n";
    }

    // Step 9: Provide recommendations based on the assessment
    std::cout << "Recommendations:\n";
    if (pollutionLevel > 50) {
        std::cout << "1. Implement pollution control measures\n";
    }
    if (biodiversityIndex < 2.0) {
        std::cout << "1. Enhance biodiversity conservation efforts\n";
    }

    // Step 10: Conclude the environmental impact evaluation
    std::cout << "Environmental impact evaluation completed for " << village.name << ".\n";
}

int main() {
    // Define data for each city
    std::vector<Region> regions =
    {

        {"Mumbai", {
            {"Dharavi", {{"Mithi River Drain", 500, 10, 75, 20000, 15000, 45, 30}, {"Sion East Drain", 300, 20, 30, 15000, 12000, 55, 45}, {"Mahim Creek Drain", 600, 15, 85, 25000, 20000, 60, 60}}, 1200, 50000, 30000, 40000, 10.5},
            {"Kurla", {{"Kurla East Drain", 400, 12, 65, 18000, 15000, 50, 40}, {"LBS Drain", 550, 18, 45, 23000, 18000, 70, 50}}, 1300, 30000, 20000, 30000, 8.0}
        }},
        {"Pune", {
            {"Kothrud", {{"Khadakwasla Outlet", 700, 10, 50, 30000, 25000, 65, 35}, {"Wakad Drain", 450, 15, 90, 20000, 18000, 40, 30}}, 1100, 40000, 25000, 35000, 9.0},
            {"Aundh", {{"Aundh Creek Drain", 800, 13, 40, 32000, 25000, 75, 50}, {"Katraj Drain", 650, 17, 80, 28000, 22000, 50, 45}}, 1200, 45000, 30000, 40000, 10.0}
        }},
        {"Nagpur", {
            {"Nagpur East", {{"Nag Nadi Drain", 600, 9, 60, 22000, 20000, 55, 40}, {"Ambazari Drain", 550, 14, 72, 20000, 18000, 45, 50}}, 1000, 35000, 20000, 30000, 7.5},
            {"Nagpur West", {{"Brahmapuri Drain", 700, 16, 55, 25000, 23000, 60, 45}, {"Chandrapur Road Drain", 450, 12, 85, 18000, 16000, 50, 60}}, 1100, 38000, 25000, 32000, 8.5}
        }},
        {"Nashik", {
            {"Nashik East", {{"Ganga Nadi Drain", 550, 11, 95, 20000, 19000, 48, 30}, {"Godavari River Drain", 500, 13, 65, 19000, 17000, 52, 45}}, 1050, 37000, 22000, 34000, 8.0},
            {"Nashik West", {{"Nashik-Pune Highway Drain", 650, 14, 25, 25000, 20000, 68, 50}, {"Nashik Road Drain", 600, 20, 90, 23000, 21000, 55, 60}}, 1150, 40000, 26000, 36000, 9.0}
        }}
    };

    int cityChoice = -1;
    while (true)
    {
        std::cout << "Which city's info do you want to access? (Enter 0 to exit)\n";
        for (size_t i = 0; i < regions.size(); ++i)
        {
            std::cout << i + 1 << ". " << regions[i].name << std::endl;
        }

        //cityChoice = getValidChoice(regions.size());
        std::cin>>cityChoice;

        if (cityChoice == 0)
        {
            std::cout << "Exiting program...\n";
            break;
        }

        Region& selectedRegion = regions[cityChoice - 1];

        // Display affected villages in selected city
        displayAffectedVillages(selectedRegion.villages);

        int villageChoice = getValidChoice(selectedRegion.villages.size());

        Village& selectedVillage = selectedRegion.villages[villageChoice - 1];

        while (true)
        {
            std::cout << "\nSelect an operation to perform for the selected village (Enter 0 to go back):\n";
            std::cout << "1. Shortest path from node points to drains\n";
            std::cout << "2. Perform Flood Risk Assessment\n";
            std::cout << "3. Predict and Manage Flood Risk\n";
            std::cout << "4. Optimize Pipe Sizes\n";
            std::cout << "5. Calculate Basin Sizes\n";
            std::cout << "6. Dynamic Routing Optimization\n";
            std::cout << "7. Develop Control Strategies for Sluice Gates and Pumps\n";
            std::cout << "8. Real-time Flow Adjustment\n";
            std::cout << "9. Minimize Flood Impact\n";
            std::cout << "10. Geospatial Analysis and Visualization\n";
            std::cout << "11. Cost-Benefit Analysis for Network Upgrades\n";
            std::cout << "12. Resilience Analysis Under Climate Change\n";
            std::cout << "13. Apply Distributed Control for Village\n";
            std::cout << "14. Display Drainage Routes\n";
            std::cout << "15. Analyze Drainage Capacity\n";
            std::cout << "16. Generate Emergency Response Plan\n";
            std::cout << "17. Evaluate Environmental Impact\n";
            std::cout << "18. Optimize Rainwater Harvesting Points\n";
            std::cout << "19. Monitor Water Quality\n";
            std::cout << "20. Predict Urban Expansion Impact\n";
            std::cout << "21. Simulate Flood event\n";
            std::cout << "22. Schedule Drain Maintenance\n";
            std::cout << "23. Organize community organization\n";
            std::cout << "24. Real-time Weather Data Integration\n";
            std::cout << "25. Rainfall Trend Analysis\n";
            std::cout << "26. Flood Insurance Recommendations\n";
            std::cout << "27. Energy-Efficient Pump Design\n";
            std::cout << "28. Evaluate Environmental Impact\n";
            std::cout << "29. Evaluate Drainage System Performance\n";
            std::cout << "30. Implement Flood Mitigation Measures\n";
            std::cout << "31. Simulate Water FlowDynamics\n";
            std::cout << "32. Calculate Drainage Efficiency\n";
            std::cout << "33. Fetch Weather Data\n";
            std::cout << "34. Plan Drainage Network Expansion\n";
            std::cout << "35. Analyze Historical FloodData\n";


            int operationChoice;
            std::cin>>operationChoice;

            if (operationChoice == 0)
            {
                std::cout << "Returning to city selection...\n";
                break; // Exit the inner loop
            }

            switch (operationChoice) {
                case 14:
                    displayDrainageRoutes(selectedVillage.drainageRoutes);
                    break;
                case 2:
                    assessFloodRisk(selectedVillage);
                    break;
                case 3:
                    predictAndManageFloodRisk(selectedVillage);
                    break;
                case 4:
                    optimizePipeSizes(selectedVillage);
                    break;
                case 5:
                    calculateBasinSizes(selectedVillage);
                    break;
                case 6:
                    dynamicRoutingOptimization(selectedVillage);
                    break;
                case 7:
                    controlStrategiesForSluiceGatesAndPumps(selectedVillage);
                    break;
                case 8:
                    realTimeFlowAdjustment(selectedVillage);
                    break;
                case 9:
                    minimizeFloodImpact(selectedVillage);
                    break;
                case 10:
                    geospatialAnalysisAndVisualization(selectedVillage);
                    break;
                case 11:
                    costBenefitAnalysisForNetworkUpgrades(selectedVillage);
                    break;
                case 12:
                    resilienceAnalysisUnderClimateChange(selectedVillage);
                    break;
                case 13:
                    applyDistributedControlForVillage(selectedVillage);
                    break;
                case 1:{
                        // Collect all drainage points and edges
                        std::vector<std::string> drainagePoints;
                        std::vector<Edge2> drainageEdges;

                        // Populate drainage points and edges
                        for (const auto& region : regions)
                        {
                            for (const auto& village : region.villages)
                            {
                                for (const auto& route : village.drainageRoutes)
                                {
                                    drainagePoints.push_back(route.routeName);
                                }
                            }
                        }

                        // Create unique edges between drainage points (example connections)
                        for (size_t i = 0; i < drainagePoints.size(); ++i)
                            {
                            for (size_t j = i + 1; j < drainagePoints.size(); ++j)
                            {
                                double weight = static_cast<double>(rand() % 100 + 1); // Simulate weight dynamically
                                drainageEdges.push_back({drainagePoints[i], drainagePoints[j], weight});
                            }
                        }

                        // Run Kruskal's Algorithm
                        std::vector<Edge2> mst = kruskal(drainagePoints, drainageEdges);

                        // Display the optimized network (MST)
                        std::cout << "\nOptimized Network (Minimum Spanning Tree):\n";
                        displayMST(mst);
                        break;
                    }
                case 15:
                    analyzeDrainageCapacity(selectedVillage); // Set custom threshold
                    break;
                case 16:
                    generateEmergencyResponsePlan(selectedRegion, selectedVillage, 200); // Custom supply units
                    break;
                case 17:
                    evaluateEnvironmentalImpact(selectedVillage); // Custom impact threshold
                    break;
                case 18:
                    optimizeRainwaterHarvesting(selectedRegion, selectedVillage, 15000.0); // Custom runoff volume threshold
                    break;
                case 19:
                    monitorWaterQuality(selectedRegion, selectedVillage, 40.0); // Custom quality threshold
                    break;
                case 20:
                    predictUrbanExpansionImpact(selectedRegion, selectedVillage, 0.3, 2.0); // Custom growth rates
                    break;
                case 21:
                    simulateFloodEvent(selectedVillage, 50.0, 2.0);
                    break;
                case 22:
                    scheduleDrainMaintenance(selectedVillage);
                    break;
                case 23:
                    organizeCommunityAwareness(selectedVillage);
                    break;
                case 24:
                    integrateRealTimeWeather(selectedVillage.name);
                    break;
                case 25:
                    analyzeRainfallTrends(selectedRegion);
                    break;
                case 26:
                    recommendFloodInsurance(selectedVillage);
                    break;
                case 27:
                    optimizePumpDesign(selectedVillage);
                    break;
                case 28:
                     evaluateEnvironmentalImpact(selectedVillage);
                    break;
                case 29:
                    evaluateDrainageSystemPerformance(selectedVillage);
                    break;
                case 30:
                    implementFloodMitigationMeasures(selectedVillage);
                    break;
                case 31:
                    simulateWaterFlowDynamics(selectedVillage);
                    break;
                case 32:
                    calculateDrainageEfficiency(selectedVillage);
                    break;
                case 33:
                    {
                    WeatherData weatherData = fetchWeatherData(selectedVillage.name);
                    std::cout << "Weather data for " << selectedVillage.name << ":\n";
                    std::cout << "Current Condition: " << weatherData.currentCondition << "\n";
                    std::cout << "Expected Rainfall: " << weatherData.expectedRainfall << " mm\n";
                    std::cout << "Wind Speed: " << weatherData.windSpeed << " km/h\n";
                    std::cout << "Temperature: " << weatherData.temperature << " �C\n";
                    std::cout << "Flood Risk: " << weatherData.floodRisk << "\n";
                    std::cout << "Recommended Actions: " << weatherData.recommendedActions << "\n";
            }
                    break;
                case 34:
                    planDrainageNetworkExpansion(selectedVillage);
                     break;
                case 35:
                    {
                    // Example historical flood events data
                    std::vector<FloodEvent> floodEvents = {
                        {"2024-06-15", 120.0, 1.5, "High impact, severe flooding in low-lying areas"},
                        {"2023-08-20", 80.0, 1.2, "Moderate impact, some flooding in residential areas"},
                        {"2022-07-10", 150.0, 1.8, "Severe impact, widespread flooding and property damage"}
                    };
                    analyzeHistoricalFloodData(selectedVillage, floodEvents);
                }
                    break;
                default:
                    std::cout << "Invalid choice. Please select a valid option.\n";
                    break;
            }
        }
    }

    return 0;
}



