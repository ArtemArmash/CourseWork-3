#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Order.h" // Order.h вже включає Date.h

class Buyer {
private:
    std::string name;
    std::string email;
    std::vector<Order> orders;

public:
    Buyer(const std::string& name, const std::string& email);
    Buyer();

    std::string getEmail() const;
    std::string getName() const;
    const std::vector<Order>& getOrders() const;
    void displayBuyerInfo() const;
    bool hasRelevantOrder(const std::string& productName, const Date& startDate,
        const Date& endDate, int minQuantity) const;
    void addOrder(const Order& order);

    void serialize(std::ostream& os) const;
    static Buyer deserialize(std::istream& is);
};