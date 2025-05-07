#include "../include/Seller.h" // Адаптуй шлях
#include "../include/Sale.h"   // Потрібен для deserialize Sale
#include "../include/Utils.h"  // Адаптуй шлях
#include <limits>

int Seller::nextId = 1;

// ... (конструктори, generateSellerId, set/getCurrentNextId, addSale, геттери - без змін) ...
Seller::Seller(const std::string& name, double salary) : name(name), id(generateSellerId()), salary(salary) {}
Seller::Seller() : name(""), id(0), salary(0.0) {}
int Seller::generateSellerId() { return nextId++; }
void Seller::setNextId(int id_val) { nextId = id_val; }
int Seller::getCurrentNextId() { return nextId; }
void Seller::addSale(const Sale& sale) { sales.push_back(sale); }
std::string Seller::getName() const { return name; }
int Seller::getId() const { return id; }
double Seller::getSalary() const { return salary; }
const std::vector<Sale>& Seller::getSales() const { return sales; }
std::vector<Sale>& Seller::getSalesEditable() { return sales; }


void Seller::serialize(std::ostream& os) const {
    os << "Seller.ID: " << id << std::endl;
    os << "Seller.Name: " << name << std::endl;
    os << "Seller.Salary: " << salary << std::endl;
    os << "Seller.SalesCount: " << sales.size() << std::endl;
    for (size_t i = 0; i < sales.size(); ++i) {
        os << "Seller.Sale[" << i << "]: ---BEGIN---" << std::endl;
        sales[i].serialize(os);
        os << "Seller.Sale[" << i << "]: ---END---" << std::endl;
    }
}

Seller Seller::deserialize(std::istream& is) {
    Seller s;
    s.id = Utils::readIntValueAfterLabel(is, "Seller.ID");
    s.name = Utils::readStringValueAfterLabel(is, "Seller.Name");
    s.salary = Utils::readDoubleValueAfterLabel(is, "Seller.Salary");

    size_t numSales = Utils::readIntValueAfterLabel(is, "Seller.SalesCount");
    s.sales.reserve(numSales);
    for (size_t i = 0; i < numSales; ++i) {
        Utils::readStringValueAfterLabel(is, "Seller.Sale[" + std::to_string(i) + "]: ---BEGIN---");
        s.sales.push_back(Sale::deserialize(is)); // Sale::deserialize зчитає свої поля, включаючи ID продавця та email покупця
        Utils::readStringValueAfterLabel(is, "Seller.Sale[" + std::to_string(i) + "]: ---END---");
    }
    // Зв'язування Sale.seller_ptr та Sale.buyer_ptr відбудеться в DataManager::loadData
    return s;
}