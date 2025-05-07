#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Product.h"
#include "Date.h"

class Order {
private:
    int id;
    std::vector<Product> products;
    double totalPrice;
    Date order_date;

    static int next_id; // Оголошення статичного члена даних

    void calculateTotalPrice();

public:
    Order();
    Order(int id, const std::vector<Product>& products, const Date& order_date);

    static int generateOrderId();
    static void setNextId(int id_val);
    static int getCurrentNextId();

    int getId() const;
    Date getOrderDate() const;
    const std::vector<Product>& getProducts() const;
    double getTotalPrice() const;

    bool containsProduct(const std::string& productName, int minQuantity) const;
    bool isInDateRange(const Date& startDate, const Date& endDate) const;

    void serialize(std::ostream& os) const;
    static Order deserialize(std::istream& is);
};