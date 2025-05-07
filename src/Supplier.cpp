#include "../include/Supplier.h" // Адаптуй шлях
#include "../include/Utils.h"    // Адаптуй шлях
#include <limits>

// ... (конструктори, addSuppliedProduct, геттери - без змін) ...
Supplier::Supplier(const std::string& name, const std::string& contactInfo) : name(name), contactInfo(contactInfo) {}
Supplier::Supplier() : name(""), contactInfo("") {}
void Supplier::addSuppliedProduct(const Product& product) { suppliedProducts.push_back(product); }
const std::string& Supplier::getName() const { return name; }
const std::string& Supplier::getContactInfo() const { return contactInfo; }
const std::vector<Product>& Supplier::getSuppliedProducts() const { return suppliedProducts; }
std::vector<Product>& Supplier::getSuppliedProductsEditable() { return suppliedProducts; }


void Supplier::serialize(std::ostream& os) const {
    os << "Supplier.Name: " << name << std::endl;
    os << "Supplier.ContactInfo: " << contactInfo << std::endl;
    os << "Supplier.ProductsCount: " << suppliedProducts.size() << std::endl;
    for (size_t i = 0; i < suppliedProducts.size(); ++i) {
        os << "Supplier.Product[" << i << "]: ---BEGIN---" << std::endl;
        suppliedProducts[i].serialize(os);
        os << "Supplier.Product[" << i << "]: ---END---" << std::endl;
    }
}

Supplier Supplier::deserialize(std::istream& is) {
    Supplier s;
    s.name = Utils::readStringValueAfterLabel(is, "Supplier.Name");
    s.contactInfo = Utils::readStringValueAfterLabel(is, "Supplier.ContactInfo");

    size_t numProducts = Utils::readIntValueAfterLabel(is, "Supplier.ProductsCount");
    s.suppliedProducts.reserve(numProducts);
    for (size_t i = 0; i < numProducts; ++i) {
        Utils::readStringValueAfterLabel(is, "Supplier.Product[" + std::to_string(i) + "]: ---BEGIN---");
        s.suppliedProducts.push_back(Product::deserialize(is));
        Utils::readStringValueAfterLabel(is, "Supplier.Product[" + std::to_string(i) + "]: ---END---");
    }
    return s;
}