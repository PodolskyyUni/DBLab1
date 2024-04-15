#include "Entities.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <set>

std::vector<IndexRecord> loadIndexTable();
void saveIndexTable(const std::vector<IndexRecord>& indexTable);
std::string serializeOrder(const Order& order);
Order deserializeOrder(const std::string& data);

void insertClient(Client& client) {
    std::vector<IndexRecord> indexTable = loadIndexTable();
    std::set<int> deletedClientIds;
    std::ifstream deletedClientsFile("deletedClients.txt");
    int tempId;
    while (deletedClientsFile >> tempId) {
        deletedClientIds.insert(tempId);
    }
    deletedClientsFile.close();

    bool reusedID = false;
    if (!deletedClientIds.empty()) {
        client.clientId = *deletedClientIds.begin();
        deletedClientIds.erase(deletedClientIds.begin());
        std::ofstream outDeletedClientsFile("deletedClients.txt", std::ios::trunc);
        for (int id : deletedClientIds) {
            outDeletedClientsFile << id << std::endl;
        }
        outDeletedClientsFile.close();
        for (auto& record : indexTable) {
            if (record.key == client.clientId) {
                record.isDeleted = false;
                reusedID = true;
                break;
            }
        }
    }

    std::fstream file("Clients.fl", reusedID ? std::ios::in | std::ios::out | std::ios::binary : std::ios::out | std::ios::app | std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open Clients.fl file." << std::endl;
        return;
    }

    if (reusedID) {
        for (const auto& record : indexTable) {
            if (record.key == client.clientId) {
                file.seekp(record.address * sizeof(Client));
                break;
            }
        }
    }
    else {
        client.clientId = indexTable.empty() ? 1 : indexTable.back().key + 1;
        IndexRecord newRecord{ client.clientId, static_cast<long>(indexTable.size()), false };
        indexTable.push_back(newRecord);
    }
    file.write(reinterpret_cast<const char*>(&client), sizeof(Client));
    file.close();
    saveIndexTable(indexTable);
}


Client getClient(int clientId) {
    std::vector<IndexRecord> indexTable = loadIndexTable();
    auto it = std::find_if(indexTable.begin(), indexTable.end(), [clientId](const IndexRecord& ir) { return ir.key == clientId && !ir.isDeleted; });

    if (it == indexTable.end()) {
        std::cerr << "Client not found or deleted." << std::endl;
        return {};
    }

    Client client;
    std::ifstream file("Clients.fl", std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file." << std::endl;
        return {};
    }
    file.seekg(it->address * sizeof(Client)); 
    file.read(reinterpret_cast<char*>(&client), sizeof(Client));
    file.close();
    return client;
}

void updateClient(int clientId, const Client& updatedClient) {
    std::vector<IndexRecord> indexTable = loadIndexTable();
    auto it = std::find_if(indexTable.begin(), indexTable.end(), [clientId](const IndexRecord& ir) { return ir.key == clientId && !ir.isDeleted; });

    if (it == indexTable.end()) {
        std::cerr << "Client not found or deleted." << std::endl;
        return;
    }

    std::fstream file("Clients.fl", std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    file.seekp(it->address * sizeof(Client));
    file.write(reinterpret_cast<const char*>(&updatedClient), sizeof(Client));
    file.close();
}

void deleteClientOrders(int clientId) {
    std::vector<Order> orders;
    std::ifstream fileIn("Orders.fl");
    std::string line;
    std::set<int> deletedOrderIds;

    while (std::getline(fileIn, line)) {
        Order order = deserializeOrder(line);
        if (order.clientId == clientId) {
            order.isDeleted = true; 
            deletedOrderIds.insert(order.orderId); 
        }
        orders.push_back(order);
    }
    fileIn.close();
    std::ofstream deletedOrdersFile("deletedOrders.txt", std::ios::app);
    for (int orderId : deletedOrderIds) {
        deletedOrdersFile << orderId << std::endl;
    }
    deletedOrdersFile.close();
    std::ofstream fileOut("Orders.fl", std::ios::trunc);
    for (const auto& order : orders) {
        fileOut << serializeOrder(order) << std::endl;
    }
    fileOut.close();
}

void deleteClient(int clientId) {
    std::vector<IndexRecord> indexTable = loadIndexTable();
    auto it = std::find_if(indexTable.begin(), indexTable.end(), [clientId](const IndexRecord& ir) {
        return ir.key == clientId;
        });

    if (it != indexTable.end()) {
        it->isDeleted = true;
        std::fstream dataFile("Clients.fl", std::ios::in | std::ios::out | std::ios::binary);
        if (!dataFile) {
            std::cerr << "Failed to open data file for update." << std::endl;
            return;
        }
        size_t recordSize = sizeof(Client); 
        dataFile.seekp(it->address * recordSize, std::ios::beg);
        Client updatedClient;
        dataFile.read(reinterpret_cast<char*>(&updatedClient), sizeof(Client));
        updatedClient.isDeleted = true;
        dataFile.seekp(it->address * recordSize, std::ios::beg);
        dataFile.write(reinterpret_cast<const char*>(&updatedClient), sizeof(Client));
        dataFile.close();
        deleteClientOrders(clientId);
        std::ofstream deletedClientsFile("deletedClients.txt", std::ios::app);
        if (!deletedClientsFile) {
            std::cerr << "Failed to open the file for deleted clients." << std::endl;
        }
        else {
            deletedClientsFile << clientId << std::endl;
            deletedClientsFile.close();
        }
    }
    else {
        std::cerr << "Client not found." << std::endl;
        return;
    }

    saveIndexTable(indexTable);
}

void printAllClients() {
    std::ifstream file("Clients.fl", std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open Clients.fl" << std::endl;
        return;
    }

    Client client;
    while (file.read(reinterpret_cast<char*>(&client), sizeof(Client))) {
        if (!client.isDeleted) {
            std::cout << "Client ID: " << client.clientId
                << ", Name: " << client.clientName
                << ", Contacts: " << client.clientContacts << std::endl;
        }
    }
}

int countClients() {
    std::ifstream file("Clients.fl", std::ios::binary);
    int count = 0;
    Client client;
    while (file.read(reinterpret_cast<char*>(&client), sizeof(Client))) {
        if (!client.isDeleted) {
            count++;
        }
    }
    return count;
}

void countOrdersByClient() {
    std::ifstream ordersFile("Orders.fl");
    if (!ordersFile) {
        std::cerr << "Не вдалося відкрити файл замовлень." << std::endl;
        return;
    }

    std::map<int, int> ordersCountByClient;
    std::string line;

    while (std::getline(ordersFile, line)) {
        Order order = deserializeOrder(line);
        if (!order.isDeleted) {
            ordersCountByClient[order.clientId]++;
        }
    }
    ordersFile.close();

    for (const auto& pair : ordersCountByClient) {
        std::cout << "Client ID: " << pair.first << " has " << pair.second << " orders." << std::endl;
    }
}