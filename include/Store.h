#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Product.h"
#include "Seller.h" // Store містить об'єкти Seller

class Store {
private:
    std::string name;
    std::string type;
    std::string location;
    // Додаткові поля з ТЗ, які можна додати:
    // double rentalPayment;
    // double utilityPayment;
    // int counterCount;
    // int hallCount; (для універмагів/магазинів)
    // std::vector<std::string> sections; (для універмагів)
    // int floor; (для секцій універмагу)

    std::vector<Product> products;
    std::vector<Seller> sellers; // Зберігаємо копії продавців, прив'язаних до цього магазину

public:
    Store(const std::string& name, const std::string& type, const std::string& location);
    Store();

    std::string getName() const { return name; }
    std::string getType() const { return type; }
    std::string getLocation() const { return location; }

    const std::vector<Product>& getProducts() const { return products; }
    std::vector<Product>& getProductsEditable();

    const std::vector<Seller>& getSellers() const { return sellers; }
    std::vector<Seller>& getSellersEditable();


    void addProduct(const Product& product);
    void addSeller(const Seller& seller); // Додає копію продавця
    void displayStoreInfo() const;
    void displayProductCatalog() const;

    void serialize(std::ostream& os) const;
    static Store deserialize(std::istream& is);
};