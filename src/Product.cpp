#include "../include/Product.h" // Адаптуй шлях
#include "../include/Utils.h"   // Адаптуй шлях
#include <limits>

// ... (конструктори та геттери/сеттери без змін з попередньої версії) ...
Product::Product(const std::string& name, int quantity, const Date& supplyDate, double price) : name(name), quantity(quantity), supplyDate(supplyDate), price(price) {}
Product::Product(const std::string& name, int quantity, double price) : name(name), quantity(quantity), supplyDate(), price(price) {}
Product::Product(const std::string& name, double price) : name(name), quantity(1), supplyDate(), price(price) {}
Product::Product() : name(""), quantity(0), supplyDate(), price(0.0) {}
const std::string& Product::getName() const { return name; }
int Product::getQuantity() const { return quantity; }
void Product::setQuantity(int newQuantity) { quantity = newQuantity; }
const Date& Product::getSupplyDate() const { return supplyDate; }
double Product::getPrice() const { return price; }


void Product::serialize(std::ostream& os) const {
    os << "Product.Name: " << name << std::endl;
    os << "Product.Quantity: " << quantity << std::endl;
    supplyDate.serialize(os); // Date::serialize вже додасть свої мітки
    os << "Product.Price: " << price << std::endl;
}

Product Product::deserialize(std::istream& is) {
    Product p;
    p.name = Utils::readStringValueAfterLabel(is, "Product.Name");
    p.quantity = Utils::readIntValueAfterLabel(is, "Product.Quantity");
    p.supplyDate = Date::deserialize(is); // Date::deserialize прочитає свої мітки
    p.price = Utils::readDoubleValueAfterLabel(is, "Product.Price");
    return p;
}