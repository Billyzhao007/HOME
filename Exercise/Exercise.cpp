#include <fstream> 
#include <sstream> 
#include <iostream> 
#include <unordered_map> 
#include <string> 
#include <vector> 
#include <algorithm> // For sort function

// Structure to hold summary information for each trade symbol
struct TradeSummary {
    unsigned long long lastTimeStamp = 0; // Tracks the last timestamp for calculating time gaps
    unsigned long long maxTimeGap = 0; // Maximum time gap between consecutive trades
    unsigned long long totalVolume = 0; // Total volume traded for the symbol
    unsigned long long weightedPriceSum = 0; // Sum of (Price * Quantity) for calculating weighted average price
    unsigned int maxPrice = 0; // Maximum trade price for the symbol
};

int main() {
    std::string inputFileName = "input.csv"; 
    std::string outputFileName = "output.csv"; 

    std::ifstream inputFile(inputFileName); 
    std::ofstream outputFile(outputFileName); 

    // Check if files are open
    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Error opening file(s)." << std::endl;
        return 1; 
    }

    std::unordered_map<std::string, TradeSummary> summaries; // Map to hold trade summaries for each symbol
    std::string line; // Variable to hold each line of the file

    // Read file line by line
    while (std::getline(inputFile, line)) {
        std::stringstream linestream(line); // parse the line
        std::string token; // Variable to hold each token in the line
        std::vector<std::string> tokens; // Vector to hold all tokens of the line

        // Split line into tokens based on comma delimiter
        while (std::getline(linestream, token, ',')) {
            tokens.push_back(token);
        }

        // Process the line if it has the correct number of tokens
        if (tokens.size() == 4) {
            std::string symbol = tokens[1];
            unsigned long long timeStamp = std::stoull(tokens[0]);
            unsigned int quantity = std::stoul(tokens[2]);
            unsigned int price = std::stoul(tokens[3]);

            auto& summary = summaries[symbol]; // Get or create the summary for this symbol
            if (summary.lastTimeStamp > 0) { // Check if this is not the first trade for the symbol
                unsigned long long timeGap = timeStamp - summary.lastTimeStamp;
                if (timeGap > summary.maxTimeGap) {
                    summary.maxTimeGap = timeGap; // Update max time gap if this gap is larger
                }
            }
            summary.lastTimeStamp = timeStamp; // Update last time stamp
            summary.totalVolume += quantity; // Update total volume
            summary.weightedPriceSum += static_cast<unsigned long long>(price) * quantity; // Update weighted price sum
            if (price > summary.maxPrice) {
                summary.maxPrice = price; // Update max price if this price is higher
            }
        }
    }

    // Convert map to vector for sorting by symbol
    std::vector<std::pair<std::string, TradeSummary>> sortedSummaries(summaries.begin(), summaries.end());
    std::sort(sortedSummaries.begin(), sortedSummaries.end(), [](const auto& a, const auto& b) {
        return a.first < b.first; // Sort by symbol
    });

    // Write the sorted summaries to the output file
    for (const auto& pair : sortedSummaries) {
        const std::string& symbol = pair.first;
        const TradeSummary& summary = pair.second;
        
        outputFile << symbol << ","
                << summary.maxTimeGap << ","
                << summary.totalVolume << ","
                << (summary.weightedPriceSum / summary.totalVolume) << "," 
                << summary.maxPrice << "\n"; 
    }


    // Close files
    inputFile.close();
    outputFile.close();

    return 0; // Program finished successfully
}
