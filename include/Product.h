#pragma once
#include <string>
#include <iostream> // Для ostream, istream
#include "Date.h"

class Product {
public:
    std::string name;
    int quantity;
    Date supplyDate;
    double price;

    Product(const std::string& name, int quantity, const Date& supplyDate, double price);
    Product(const std::string& name, int quantity, double price);
    Product(const std::string& name, double price);
    Product(); // Конструктор за замовчуванням

    const std::string& getName() const;
    int getQuantity() const;
    void setQuantity(int newQuantity);
    const Date& getSupplyDate() const;
    double getPrice() const;

    void serialize(std::ostream& os) const;
    static Product deserialize(std::istream& is);
};