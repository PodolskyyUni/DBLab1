#pragma once

#ifndef ENTITIES_H
#define ENTITIES_H

#include <string>
#include <vector>

struct Client {
    int clientId;
    char clientName[50]; 
    char clientContacts[100];
    bool isDeleted = false;
};


struct Order {
    int orderId;
    int clientId; 
    std::string orderStatus;
    std::string orderDate;
    std::string apprComplDate;
    bool isDeleted = false;
};

struct IndexRecord {
    int key; 
    long address; 
    bool isDeleted; 

    IndexRecord(int k = 0, long a = 0, bool deleted = false)
        : key(k), address(a), isDeleted(deleted) {}
};

void insertClient( Client& client);
Client getClient(int clientId);
void updateClient(int clientId, const Client& updatedClient);
void deleteClient(int clientId);
void printAllClients();
int countClients();
void countOrdersByClient();


void insertOrder( Order& order);
Order getOrder(int orderId);
void updateOrder(int orderId, const Order& updatedOrder);
void deleteOrder(int orderId);
void printAllOrders();
int countOrders();

std::vector<IndexRecord> loadIndexTable();
void saveIndexTable(const std::vector<IndexRecord>& indexTable);
std::string serializeOrder(const Order& order);
Order deserializeOrder(const std::string& data);

#endif 
