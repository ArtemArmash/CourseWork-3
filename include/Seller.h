#pragma once
#include <vector>
#include <string>
#include <iostream> // Для ostream, istream
#include "Sale.h"
class Sale;
class Seller {
private:
    std::string name;
    int id;
    double salary;
    std::vector<Sale> sales;

    static int nextId;

public:
    Seller(const std::string& name, double salary);
    Seller();

    void addSale(const Sale& sale);

    std::string getName() const;
    int getId() const;
    double getSalary() const;
    const std::vector<Sale>& getSales() const;
    std::vector<Sale>& getSalesEditable(); // Для DataManager

    static int generateSellerId();
    static void setNextId(int id_val);
    static int getCurrentNextId();

    void serialize(std::ostream& os) const;
    static Seller deserialize(std::istream& is); // Потрібно передати Menu для зв'язування Sale
};