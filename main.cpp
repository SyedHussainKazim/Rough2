#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>

// Class to manage inflation rates
class InflationRate {
    std::vector<double> inflationRates;

public:
    void loadFromCSV(const std::string& fileName);
    double getCompoundedInflation(int startYear, int endYear);
};

// Class to manage property growth rates
class PropertyGrowth {
    std::vector<double> growthRates;

public:
    void loadFromCSV(const std::string& fileName);
    double getCompoundedGrowth(int startYear, int endYear);
};

// Class to calculate LTCG based on inflation and property growth
class LTCGCalculator {
    InflationRate inflationRate;
    PropertyGrowth propertyGrowth;

public:
    void loadRates(const std::string& fileName);
    double calculateLTCG(int startYear, int endYear, double costPrice);
};

void InflationRate::loadFromCSV(const std::string& fileName) {
    std::ifstream file(fileName);
    std::string line;
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    // Skip the header
    std::getline(file, line);

    // Load inflation rates
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string year, growthPrice, inflation;
        std::getline(ss, year, ',');
        std::getline(ss, growthPrice, ',');
        std::getline(ss, inflation, ',');
        inflationRates.push_back(std::stod(inflation));
    }
}

double InflationRate::getCompoundedInflation(int startYear, int endYear) {
    double compoundedInflation = 1.0;
    for (int i = startYear - 2001; i < endYear - 2001; ++i) {
        compoundedInflation *= (1 + inflationRates[i] / 100);
    }
    return compoundedInflation;
}

void PropertyGrowth::loadFromCSV(const std::string& fileName) {
    std::ifstream file(fileName);
    std::string line;
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    // Skip the header
    std::getline(file, line);

    // Load growth rates
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string year, growthPrice, inflation;
        std::getline(ss, year, ',');
        std::getline(ss, growthPrice, ',');
        std::getline(ss, inflation, ',');
        growthRates.push_back(std::stod(growthPrice));
    }
}

double PropertyGrowth::getCompoundedGrowth(int startYear, int endYear) {
    double compoundedGrowth = 1.0;
    for (int i = startYear - 2001; i < endYear - 2001; ++i) {
        compoundedGrowth *= (1 + growthRates[i] / 100);
    }
    return compoundedGrowth;
}

void LTCGCalculator::loadRates(const std::string& fileName) {
    inflationRate.loadFromCSV(fileName);
    propertyGrowth.loadFromCSV(fileName);
}

double LTCGCalculator::calculateLTCG(int startYear, int endYear, double costPrice) {
    double compoundedGrowth = propertyGrowth.getCompoundedGrowth(startYear, endYear);
    double estimatedSellingPrice = costPrice * compoundedGrowth;

    double compoundedInflation = inflationRate.getCompoundedInflation(startYear, endYear);
    double adjustedCostPrice = costPrice * compoundedInflation;

    double actualProfit = estimatedSellingPrice - adjustedCostPrice;
    double LTCGTax = 0.2 * actualProfit;

    std::cout << "Estimated Selling Price: Rs " << estimatedSellingPrice << std::endl;
    std::cout << "Long-term Capital Gains Tax (LTCG): Rs " << LTCGTax << std::endl;

    return LTCGTax;
}

int main() {
    try {
        LTCGCalculator calculator;
        calculator.loadRates("Growth-price.csv");

        int startYear = 2010;
        int endYear;
        double costPrice = 5000000; // Rs 50 lakhs

        std::cout << "Enter the selling year: ";
        std::cin >> endYear;

        calculator.calculateLTCG(startYear, endYear, costPrice);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

