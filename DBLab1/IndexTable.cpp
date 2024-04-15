#include <fstream>
#include <vector>
#include <algorithm> 
#include "Entities.h"
#include <sstream>
#include <iostream>

void saveIndexTable(const std::vector<IndexRecord>& indexTable) {
    std::ofstream file("Clients.ind");
    if (!file) {
        std::cerr << "Failed to open Clients.ind for writing." << std::endl;
        return; 
    }
    for (const auto& record : indexTable) {
        file << record.key << "," << record.address << "," << (record.isDeleted ? "1" : "0") << std::endl;
    }
}

std::vector<IndexRecord> loadIndexTable() {
    std::vector<IndexRecord> indexTable;
    std::ifstream file("Clients.ind");
    if (!file) {
        std::cerr << "Failed to open Clients.ind for reading." << std::endl;
        return indexTable;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        IndexRecord record;
        std::string temp;
        std::getline(ss, temp, ',');
        record.key = std::stoi(temp);
        std::getline(ss, temp, ',');
        record.address = std::stoi(temp);
        std::getline(ss, temp, ',');
        record.isDeleted = (temp == "1");
        indexTable.push_back(record);
    }
    return indexTable;
}
