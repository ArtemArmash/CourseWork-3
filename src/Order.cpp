#include "../include/Order.h"
#include "../include/Utils.h"
#include <limits>

int Order::next_id = 1;

// Конструктори
Order::Order() : id(0), totalPrice(0.0), order_date() {}
Order::Order(int id, const std::vector<Product>& products, const Date& order_date)
    : id(id), products(products), order_date(order_date), totalPrice(0.0) {
    calculateTotalPrice();
}

// Управління ID
int Order::generateOrderId() { return next_id++; }
void Order::setNextId(int id_val) { next_id = id_val; }
int Order::getCurrentNextId() { return next_id; }

// Геттери
int Order::getId() const { return id; }
Date Order::getOrderDate() const { return order_date; }
const std::vector<Product>& Order::getProducts() const { return products; }
double Order::getTotalPrice() const { return totalPrice; }

// Методи логіки
void Order::calculateTotalPrice() {
    totalPrice = 0;
    for (const auto& product : products) {
        totalPrice += product.getPrice() * product.getQuantity();
    }
}

// ОДНЕ ЄДИНЕ ВИЗНАЧЕННЯ containsProduct
bool Order::containsProduct(const std::string& productName, int minQuantity) const {
    int current_quantity = 0;
    for (const auto& product : products) {
        if (Utils::containsIgnoreCase(product.getName(), productName)) {
            current_quantity += product.getQuantity();
        }
    }
    return current_quantity >= minQuantity;
}

bool Order::isInDateRange(const Date& startDate, const Date& endDate) const {
    if (!order_date.isValid()) return false;
    if (startDate.isValid() && endDate.isValid()) {
        return order_date >= startDate && order_date <= endDate;
    }
    else if (startDate.isValid()) {
        return order_date >= startDate;
    }
    else if (endDate.isValid()) {
        return order_date <= endDate;
    }
    return true;
}

// Серіалізація / Десеріалізація
void Order::serialize(std::ostream& os) const {
    os << "Order.ID: " << id << std::endl;
    order_date.serialize(os);
    os << "Order.ProductsCount: " << products.size() << std::endl;
    for (size_t i = 0; i < products.size(); ++i) {
        os << "Order.Product[" << i << "]: ---BEGIN---" << std::endl;
        products[i].serialize(os);
        os << "Order.Product[" << i << "]: ---END---" << std::endl;
    }
    os << "Order.TotalPrice: " << totalPrice << std::endl;
}

Order Order::deserialize(std::istream& is) {
    Order order;
    order.id = Utils::readIntValueAfterLabel(is, "Order.ID");
    order.order_date = Date::deserialize(is);

    size_t num_products = Utils::readIntValueAfterLabel(is, "Order.ProductsCount");
    order.products.reserve(num_products);
    for (size_t i = 0; i < num_products; ++i) {
        Utils::readStringValueAfterLabel(is, "Order.Product[" + std::to_string(i) + "]: ---BEGIN---");
        order.products.push_back(Product::deserialize(is));
        Utils::readStringValueAfterLabel(is, "Order.Product[" + std::to_string(i) + "]: ---END---");
    }
    order.totalPrice = Utils::readDoubleValueAfterLabel(is, "Order.TotalPrice");
    // Можна викликати order.calculateTotalPrice(); якщо не довіряємо збереженому totalPrice
    return order;
}