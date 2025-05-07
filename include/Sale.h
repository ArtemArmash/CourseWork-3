#pragma once
#include <vector>
#include <string>
#include <iostream> // Для ostream, istream
#include "Product.h"
#include "Date.h"

class Seller; // Попереднє оголошення
class Buyer;  // Попереднє оголошення

class Sale {
private:
    int id;
    // Для серіалізації/десеріалізації
    int sellerId_serial;
    std::string buyerEmail_serial;

    // Для роботи в пам'яті після завантаження та зв'язування
    Seller* seller_ptr = nullptr;
    Buyer* buyer_ptr = nullptr;

    std::vector<Product> products;
    double totalPrice;
    Date saleDate;
    std::string storeName_serial; // Ім'я магазину

    void calculateTotalPrice();

public:
    Sale(int id, Seller* seller, Buyer* buyer, const std::vector<Product>& products,
        const Date& saleDate, const std::string& storeName);
    Sale(); // Конструктор за замовчуванням для десеріалізації

    int getId() const;
    Seller* getSeller() const; // Повертає зв'язаний вказівник
    Buyer* getBuyer() const;   // Повертає зв'язаний вказівник
    const std::vector<Product>& getProducts() const;
    double getTotalPrice() const;
    Date getDate() const;
    std::string getStoreName() const;

    void linkReferences(const std::vector<Seller>& allSellers, const std::vector<Buyer>& allBuyers);

    void serialize(std::ostream& os) const;
    static Sale deserialize(std::istream& is);
};