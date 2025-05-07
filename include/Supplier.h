#pragma once
#include <string>
#include <vector>
#include <iostream> // ��� ostream, istream
#include "Product.h"

class Supplier {
private:
    std::string name;
    std::string contactInfo;
    std::vector<Product> suppliedProducts;

public:
    Supplier(const std::string& name, const std::string& contactInfo);
    Supplier();

    void addSuppliedProduct(const Product& product);

    const std::string& getName() const;
    const std::string& getContactInfo() const; // ����� ������
    const std::vector<Product>& getSuppliedProducts() const;
    std::vector<Product>& getSuppliedProductsEditable(); // ��� DataManager


    void serialize(std::ostream& os) const;
    static Supplier deserialize(std::istream& is);
};