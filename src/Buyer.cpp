#include "../include/Buyer.h" // Адаптуй шлях
#include "../include/Utils.h" // Адаптуй шлях
#include <limits>

// ... (конструктори, геттери, displayBuyerInfo, hasRelevantOrder, addOrder - без змін) ...
Buyer::Buyer(const std::string& name, const std::string& email) : name(name), email(email) {}
Buyer::Buyer() : name(""), email("") {}
std::string Buyer::getEmail() const { return email; }
std::string Buyer::getName() const { return name; }
const std::vector<Order>& Buyer::getOrders() const { return orders; }
void Buyer::displayBuyerInfo() const { std::cout << "Name: " << name << "\nEmail: " << email << std::endl; }
bool Buyer::hasRelevantOrder(const std::string& productName, const Date& startDate, const Date& endDate, int minQuantity) const { for (const auto& order : orders) { bool productMatch = productName.empty() || order.containsProduct(productName, minQuantity); bool dateMatch = order.isInDateRange(startDate, endDate); if (!productName.empty()) { if (productMatch && dateMatch) return true; } else { if (dateMatch) return true; } } return false; }
void Buyer::addOrder(const Order& order) { orders.push_back(order); }


void Buyer::serialize(std::ostream& os) const {
    os << "Buyer.Name: " << name << std::endl;
    os << "Buyer.Email: " << email << std::endl;
    os << "Buyer.OrdersCount: " << orders.size() << std::endl;
    for (size_t i = 0; i < orders.size(); ++i) {
        os << "Buyer.Order[" << i << "]: ---BEGIN---" << std::endl;
        orders[i].serialize(os);
        os << "Buyer.Order[" << i << "]: ---END---" << std::endl;
    }
}

Buyer Buyer::deserialize(std::istream& is) {
    Buyer b;
    b.name = Utils::readStringValueAfterLabel(is, "Buyer.Name");
    b.email = Utils::readStringValueAfterLabel(is, "Buyer.Email");

    size_t numOrders = Utils::readIntValueAfterLabel(is, "Buyer.OrdersCount");
    b.orders.reserve(numOrders);
    for (size_t i = 0; i < numOrders; ++i) {
        Utils::readStringValueAfterLabel(is, "Buyer.Order[" + std::to_string(i) + "]: ---BEGIN---");
        b.orders.push_back(Order::deserialize(is));
        Utils::readStringValueAfterLabel(is, "Buyer.Order[" + std::to_string(i) + "]: ---END---");
    }
    return b;
}