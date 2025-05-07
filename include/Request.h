#pragma once
#include <iostream>
#include <vector>
#include <string>
class Request {
private:
    int id;
    Buyer buyer;
    std::vector<Product> requestedProducts;
    std::string requestDate;

public:
    Request(int id, const Buyer& buyer, const std::vector<Product>& requestedProducts, const std::string& requestDate)
        : id(id), buyer(buyer), requestedProducts(requestedProducts), requestDate(requestDate) {}

    void addRequestedProduct(const Product& product) {
        requestedProducts.push_back(product);
    }
};