#include "Entities.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <set>

const std::string ordersFilePath = "Orders.fl";

std::string serializeOrder(const Order& order) {
    std::stringstream ss;
    ss << order.orderId << ","
        << order.clientId << ","
        << order.orderStatus << ","
        << order.orderDate << ","
        << order.apprComplDate << ","
        << (order.isDeleted ? "1" : "0");
    return ss.str();
}

Order deserializeOrder(const std::string& data) {
    std::stringstream ss(data);
    Order order;
    std::string temp;
    std::getline(ss, temp, ',');
    order.orderId = std::stoi(temp);
    std::getline(ss, temp, ',');
    order.clientId = std::stoi(temp);
    std::getline(ss, order.orderStatus, ',');
    std::getline(ss, order.orderDate, ',');
    std::getline(ss, order.apprComplDate, ',');
    std::getline(ss, temp);
    order.isDeleted = (temp == "1");
    return order;
}

void insertOrder(Order& order) {
    std::ifstream deletedOrdersFile("deletedOrders.txt");
    std::set<int> deletedOrderIds;
    int tempId;

    // Load IDs from the trash zone
    while (deletedOrdersFile >> tempId) {
        deletedOrderIds.insert(tempId);
    }
    deletedOrdersFile.close();

    if (!deletedOrderIds.empty()) {
        int reusedOrderId = *deletedOrderIds.begin();
        order.orderId = reusedOrderId;
        deletedOrderIds.erase(reusedOrderId);
        std::ofstream outDeletedOrdersFile("deletedOrders.txt", std::ios::trunc);
        for (const auto& id : deletedOrderIds) {
            outDeletedOrdersFile << id << std::endl;
        }
        outDeletedOrdersFile.close();
        std::vector<Order> orders;
        bool orderFound = false;
        std::ifstream ordersFileRead("Orders.fl");
        std::string line;
        while (std::getline(ordersFileRead, line)) {
            Order existingOrder = deserializeOrder(line);
            if (existingOrder.orderId == reusedOrderId) {
                orders.push_back(order); 
                orderFound = true;
            }
            else {
                orders.push_back(existingOrder);
            }
        }
        ordersFileRead.close();
        if (!orderFound) {
            orders.push_back(order);
        }
        std::ofstream ordersFileWrite("Orders.fl", std::ios::trunc);
        for (const auto& ord : orders) {
            ordersFileWrite << serializeOrder(ord) << std::endl;
        }
        ordersFileWrite.close();
    }
    else {
        std::ifstream ordersFileRead("Orders.fl");
        std::string line;
        int maxId = 0;
        while (std::getline(ordersFileRead, line)) {
            Order existingOrder = deserializeOrder(line);
            if (existingOrder.orderId > maxId) {
                maxId = existingOrder.orderId;
            }
        }
        ordersFileRead.close();

        order.orderId = maxId + 1;
        std::ofstream ordersFileWrite("Orders.fl", std::ios::out | std::ios::app);
        ordersFileWrite << serializeOrder(order) << std::endl;
        ordersFileWrite.close();
    }
}


void deleteOrder(int orderId) {
    std::ifstream fileIn("Orders.fl");
    std::ofstream fileOut("tempOrders.fl", std::ios::trunc); 
    std::ofstream deletedOrdersFile("deletedOrders.txt", std::ios::app);
    std::string line;
    bool found = false;

    while (std::getline(fileIn, line)) {
        Order order = deserializeOrder(line);
        if (order.orderId == orderId && !order.isDeleted) {
            found = true;
            order.isDeleted = true; 
            if (deletedOrdersFile) {
                deletedOrdersFile << orderId << std::endl; 
            }
        }
        fileOut << serializeOrder(order) << std::endl;
    }

    fileIn.close();
    fileOut.close();
    deletedOrdersFile.close();

    if (found) {
        if (std::remove("Orders.fl") != 0 || std::rename("tempOrders.fl", "Orders.fl") != 0) {
            std::cerr << "Error updating orders file." << std::endl;
        }
    }
    else {
        std::remove("tempOrders.fl"); 
        std::cerr << "Order with ID " << orderId << " not found." << std::endl;
    }
}

Order getOrder(int orderId) {
    std::ifstream ordersFile("Orders.fl");
    std::string line;

    if (!ordersFile) {
        std::cerr << "Cannot open the file." << std::endl;
        return Order{};
    }

    while (std::getline(ordersFile, line)) {
        Order order = deserializeOrder(line);
        if (order.orderId == orderId && !order.isDeleted) {
            return order;
        }
    }

    return Order{};
}
void updateOrder(int orderId, const Order& updatedOrder) {
    std::vector<Order> orders;
    std::ifstream fileIn("Orders.fl");
    std::string line;
    bool found = false;

    while (std::getline(fileIn, line)) {
        Order order = deserializeOrder(line);
        if (order.orderId == orderId) {
            found = true;
            orders.push_back(updatedOrder);
        }
        else {
            orders.push_back(order);
        }
    }
    fileIn.close();

    if (!found) {
        std::cerr << "Order with ID " << orderId << " not found." << std::endl;
        return;
    }

    std::ofstream fileOut("Orders.fl", std::ios::trunc);
    for (const auto& order : orders) {
        fileOut << serializeOrder(order) << std::endl;
    }
    fileOut.close();
}
void printAllOrders() {
    std::ifstream ordersFile("Orders.fl");
    if (!ordersFile) {
        std::cerr << "Cannot open the file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(ordersFile, line)) {
        Order order = deserializeOrder(line);
        if (!order.isDeleted) {
            std::cout << "Order ID: " << order.orderId
                << ", Client ID: " << order.clientId
                << ", Status: " << order.orderStatus
                << ", Order Date: " << order.orderDate
                << ", Approx Completion Date: " << order.apprComplDate << std::endl;
        }
    }
    ordersFile.close();
}
int countOrders() {
    std::ifstream ordersFile("Orders.fl");
    if (!ordersFile) {
        std::cerr << "Cannot open the file." << std::endl;
        return 0;
    }

    int count = 0;
    std::string line;
    while (std::getline(ordersFile, line)) {
        Order order = deserializeOrder(line);
        if (!order.isDeleted) {
            ++count;
        }
    }
    ordersFile.close();
    return count;
}
