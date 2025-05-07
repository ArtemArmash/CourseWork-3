#include "../include/Delivery.h"

Delivery::Delivery(int id, const Supplier& supplier, const std::vector<Product>& products, const std::string& deliveryDate)
    : id(id), supplier(supplier), products(products), deliveryDate(deliveryDate) {}

void Delivery::addProductToDelivery(const Product& product) {
    products.push_back(product);
}
