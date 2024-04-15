#include <iostream>
#include "Entities.h"

int main() {
    int choice;
    bool running = true;
    system("chcp 1251");

    while (running) {
        std::cout << "������� �����:\n";
        std::cout << "1. �������� ������ �볺���\n";
        std::cout << "2. �������� ���������� ��� �볺���\n";
        std::cout << "3. ������� ���������� ��� �볺���\n";
        std::cout << "4. �������� �볺���\n";
        std::cout << "5. ������� ��� �볺���\n";
        std::cout << "6. ϳ��������� ������� �볺���\n";
        std::cout << "7. ϳ��������� ������� ���������  ��� ������� �볺���\n";
        std::cout << "8. �������� ���� ����������\n";
        std::cout << "9. �������� ���������� ��� ����������\n";
        std::cout << "10. ������� ���������� ��� ����������\n";
        std::cout << "11. �������� ����������\n";
        std::cout << "12. ������� �� ����������\n";
        std::cout << "13. ϳ��������� ������� ���������\n";
        std::cout << "0. �����\n";
        std::cout << "��� ����: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            Client client;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            std::cout << "������ ��'� �볺���: ";
            std::cin.getline(client.clientName, sizeof(client.clientName));
            std::cout << "������ �������� �볺���: ";
            std::cin.getline(client.clientContacts, sizeof(client.clientContacts));
            client.isDeleted = false;
            insertClient(client); 
            std::cout << "�볺�� ��� ������ �������.\n";
            break;
        }
        case 2: {
            int clientId;
            std::cout << "������ ID �볺���: ";
            std::cin >> clientId;
            Client client = getClient(clientId); 
            if (client.clientId != 0 && !client.isDeleted) {
                std::cout << "���������� ��� �볺���:\nID: " << client.clientId
                    << "\n��'�: " << client.clientName
                    << "\n��������: " << client.clientContacts << "\n";
            }
            else {
                std::cout << "�볺��� � ����� ID �� �������� ��� �� ��� ���������.\n";
            }
            break;
        }
        case 3: {
            int clientId;
            std::cout << "������ ID �볺��� ��� ��������� �����: ";
            std::cin >> clientId;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "���� �������� ������ �������?\n";
            std::cout << "1. ��'� �볺��\n";
            std::cout << "2. �������� �볺���\n";
            int choice;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

            Client updatedClient = getClient(clientId); 

            switch (choice) {
            case 1: {
                std::cout << "������ ���� ��'� �볺���: ";
                std::cin.getline(updatedClient.clientName, sizeof(updatedClient.clientName));
                break;
            }
            case 2: {
                std::cout << "������ ��� ��������: ";
                std::cin.getline(updatedClient.clientContacts, sizeof(updatedClient.clientContacts));
                break;
            }
            default:
                std::cout << "������������ ����.\n";
            }

            updateClient(clientId, updatedClient); 
            std::cout << "�볺�� ��� ������ ���������.\n";
            break;
        }


        case 4: {
            int clientId;
            std::cout << "������ ID �볺��� ��� ���������: ";
            std::cin >> clientId;
            deleteClient(clientId);

            std::cout << "�볺�� � ID " << clientId << " ��� ���������.\n";
            break;
        }
        case 5: {
            printAllClients();
            break;
        }
        case 6: {
            int total = countClients();
            std::cout << "�������� ������� �볺���: " << total << std::endl;
            break;
        }
        case 7: {
            countOrdersByClient();
            break;
        }
        case 8: {
                Order newOrder;
                std::cout << "������ ID �볺���: ";
                std::cin >> newOrder.clientId;
                std::cin.ignore(); 
                std::cout << "������ ������ ����������: ";
                std::getline(std::cin, newOrder.orderStatus);
                std::cout << "������ ���� ���������� (YYYY-MM-DD): ";
                std::getline(std::cin, newOrder.orderDate);
                std::cout << "������ ��������� ���� ��������� (YYYY-MM-DD): ";
                std::getline(std::cin, newOrder.apprComplDate);
                newOrder.isDeleted = false;
                insertOrder(newOrder);

                std::cout << "���� ���������� ���� ������.\n";
            break;
        }
        case 9: {
            int orderId;
            std::cout << "������ ID ���������� ��� ��������� ����������: ";
            std::cin >> orderId;

            Order order = getOrder(orderId);

            if (order.orderId != 0) { 
                std::cout << "ID �볺���: " << order.clientId << std::endl;
                std::cout << "������ ����������: " << order.orderStatus << std::endl;
                std::cout << "���� ����������: " << order.orderDate << std::endl;
                std::cout << "��������� ���� ���������: " << order.apprComplDate << std::endl;
            }
            else {
                std::cout << "���������� � ����� ID �� ��������.\n";
            }
            break;
        }
        case 10: {
            int orderId;
            std::cout << "������ ID ���������� ��� ��������� �����: ";
            std::cin >> orderId;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            std::cout << "���� �������� ������ �������?\n";
            std::cout << "1. ������ ����������\n";
            std::cout << "2. ���� ������e���\n";
            std::cout << "3. ��������� ���� ���������\n";
            int choice;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

            Order updatedOrder = getOrder(orderId); 
            switch (choice) {
            case 1: {
                std::cout << "������ ����� ������ ����������: ";
                std::getline(std::cin, updatedOrder.orderStatus);
                break;
            }
            case 2: {
                std::cout << "������ ���� ���� ���������� (YYYY-MM-DD): ";
                std::getline(std::cin, updatedOrder.orderDate);
                break;
            }
            case 3: {
                std::cout << "������ ���� ��������� ���� ��������� (YYYY-MM-DD): ";
                std::getline(std::cin, updatedOrder.apprComplDate);
                break;
            }
            default:
                std::cout << "������������ ����.\n";
            }

            updateOrder(orderId, updatedOrder); 
            std::cout << "���������� ������ ��������.\n";
            break;
        }

        case 11:{
            int orderId;
            std::cout << "������ ID ���������� ��� ���������: ";
            std::cin >> orderId;
            deleteOrder(orderId);
            std::cout << "���������� � ID " << orderId << " ��������.\n";
            break;
        }
        case 12: {
            printAllOrders();
            break;
        }
        case 13:{
                int totalOrders = countOrders();
                std::cout << "�������� ������� ���������: " << totalOrders << std::endl;
                break;
            }
        case 0: {
            running = false;
            break;
        }
        default:
            std::cout << "������� �����. ��������� �� ���.\n";
        }
    }

    return 0;
}