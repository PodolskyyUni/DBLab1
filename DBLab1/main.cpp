#include <iostream>
#include "Entities.h"

int main() {
    int choice;
    bool running = true;
    system("chcp 1251");

    while (running) {
        std::cout << "Виберіть опцію:\n";
        std::cout << "1. Вставити нового клієнта\n";
        std::cout << "2. Отримати інформацію про клієнта\n";
        std::cout << "3. Оновити інформацію про клієнта\n";
        std::cout << "4. Видалити клієнта\n";
        std::cout << "5. Вивести всіх клієнтів\n";
        std::cout << "6. Підрахувати кількість клієнтів\n";
        std::cout << "7. Підрахувати кількість замовлень  для кожного клієнта\n";
        std::cout << "8. Вставити нове замовлення\n";
        std::cout << "9. Отримати інформацію про замовлення\n";
        std::cout << "10. Оновити інформацію про замовлення\n";
        std::cout << "11. Видалити замовлення\n";
        std::cout << "12. Вивести всі замовлення\n";
        std::cout << "13. Підрахувати кількість замовлень\n";
        std::cout << "0. Вийти\n";
        std::cout << "Ваш вибір: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            Client client;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            std::cout << "Введіть ім'я клієнта: ";
            std::cin.getline(client.clientName, sizeof(client.clientName));
            std::cout << "Введіть контакти клієнта: ";
            std::cin.getline(client.clientContacts, sizeof(client.clientContacts));
            client.isDeleted = false;
            insertClient(client); 
            std::cout << "Клієнт був успішно доданий.\n";
            break;
        }
        case 2: {
            int clientId;
            std::cout << "Введіть ID клієнта: ";
            std::cin >> clientId;
            Client client = getClient(clientId); 
            if (client.clientId != 0 && !client.isDeleted) {
                std::cout << "Інформація про клієнта:\nID: " << client.clientId
                    << "\nІм'я: " << client.clientName
                    << "\nКонтакти: " << client.clientContacts << "\n";
            }
            else {
                std::cout << "Клієнта з таким ID не знайдено або він був видалений.\n";
            }
            break;
        }
        case 3: {
            int clientId;
            std::cout << "Введіть ID клієнта для оновлення даних: ";
            std::cin >> clientId;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Який параметр бажаєте оновити?\n";
            std::cout << "1. Ім'я клієна\n";
            std::cout << "2. Контакти клієнта\n";
            int choice;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

            Client updatedClient = getClient(clientId); 

            switch (choice) {
            case 1: {
                std::cout << "Введіть нове ім'я клієнта: ";
                std::cin.getline(updatedClient.clientName, sizeof(updatedClient.clientName));
                break;
            }
            case 2: {
                std::cout << "Введіть нові контакти: ";
                std::cin.getline(updatedClient.clientContacts, sizeof(updatedClient.clientContacts));
                break;
            }
            default:
                std::cout << "Недопустимий вибір.\n";
            }

            updateClient(clientId, updatedClient); 
            std::cout << "Клієнт був успішно оновлений.\n";
            break;
        }


        case 4: {
            int clientId;
            std::cout << "Введіть ID клієнта для видалення: ";
            std::cin >> clientId;
            deleteClient(clientId);

            std::cout << "Клієнт з ID " << clientId << " був видалений.\n";
            break;
        }
        case 5: {
            printAllClients();
            break;
        }
        case 6: {
            int total = countClients();
            std::cout << "Загальна кількість клієнтів: " << total << std::endl;
            break;
        }
        case 7: {
            countOrdersByClient();
            break;
        }
        case 8: {
                Order newOrder;
                std::cout << "Введіть ID клієнта: ";
                std::cin >> newOrder.clientId;
                std::cin.ignore(); 
                std::cout << "Введіть статус замовлення: ";
                std::getline(std::cin, newOrder.orderStatus);
                std::cout << "Введіть дату замовлення (YYYY-MM-DD): ";
                std::getline(std::cin, newOrder.orderDate);
                std::cout << "Введіть приблизну дату виконання (YYYY-MM-DD): ";
                std::getline(std::cin, newOrder.apprComplDate);
                newOrder.isDeleted = false;
                insertOrder(newOrder);

                std::cout << "Нове замовлення було додано.\n";
            break;
        }
        case 9: {
            int orderId;
            std::cout << "Введіть ID замовлення для отримання інформації: ";
            std::cin >> orderId;

            Order order = getOrder(orderId);

            if (order.orderId != 0) { 
                std::cout << "ID клієнта: " << order.clientId << std::endl;
                std::cout << "Статус замовлення: " << order.orderStatus << std::endl;
                std::cout << "Дата замовлення: " << order.orderDate << std::endl;
                std::cout << "Приблизна дата виконання: " << order.apprComplDate << std::endl;
            }
            else {
                std::cout << "Замовлення з таким ID не знайдено.\n";
            }
            break;
        }
        case 10: {
            int orderId;
            std::cout << "Введіть ID замовлення для оновлення даних: ";
            std::cin >> orderId;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            std::cout << "Який параметр бажаєте оновити?\n";
            std::cout << "1. Статус замовлення\n";
            std::cout << "2. Дату замовлeння\n";
            std::cout << "3. Приблизну дату виконання\n";
            int choice;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

            Order updatedOrder = getOrder(orderId); 
            switch (choice) {
            case 1: {
                std::cout << "Ввести новий статус замовлення: ";
                std::getline(std::cin, updatedOrder.orderStatus);
                break;
            }
            case 2: {
                std::cout << "Ввести нову дату замовлення (YYYY-MM-DD): ";
                std::getline(std::cin, updatedOrder.orderDate);
                break;
            }
            case 3: {
                std::cout << "Ввести нову приблизну дату виконання (YYYY-MM-DD): ";
                std::getline(std::cin, updatedOrder.apprComplDate);
                break;
            }
            default:
                std::cout << "Недопустимий вибір.\n";
            }

            updateOrder(orderId, updatedOrder); 
            std::cout << "Замовлення успішно оновлено.\n";
            break;
        }

        case 11:{
            int orderId;
            std::cout << "Введіть ID замовлення для видалення: ";
            std::cin >> orderId;
            deleteOrder(orderId);
            std::cout << "Замовлення з ID " << orderId << " видалено.\n";
            break;
        }
        case 12: {
            printAllOrders();
            break;
        }
        case 13:{
                int totalOrders = countOrders();
                std::cout << "Загальна кількість замовлень: " << totalOrders << std::endl;
                break;
            }
        case 0: {
            running = false;
            break;
        }
        default:
            std::cout << "Невідома опція. Спробуйте ще раз.\n";
        }
    }

    return 0;
}