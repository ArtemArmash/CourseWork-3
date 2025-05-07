#pragma once
#include <vector>
#include <string>
#include "Supplier.h"
#include "Buyer.h"   // Включає Order
#include "Store.h"   // Включає Seller (якщо Seller.h не включає Menu.h)
#include "Seller.h"  // Включає Sale (якщо Sale.h не включає Menu.h)
#include "Date.h"
#include "Sale.h"
// Попередні оголошення, якщо є циклічні залежності, яких не вдалося уникнути
// class Seller;
// class Store;

class Menu {
private:
    std::vector<Supplier> suppliers;
    std::vector<Buyer> buyers;
    std::vector<Store> stores;
    std::vector<Seller> sellers; // Основний список унікальних продавців системи

    Buyer* findBuyerByEmail(const std::string& email);
    Seller* findSellerByName(const std::string& name); // Шукає в sellers
    Seller* findSellerById(int id); // Додатково для зв'язування
    Store* findStoreByName(const std::string& name);
    void printProductInfoInStore(const Store& store, const std::string& productName);

public:
    Menu() = default; // Конструктор за замовчуванням

    // --- Постачальники ---
    void addSupplier(const std::string& name, const std::string& contactInfo);
    void addProductToSupplier(const std::string& supplierName, const std::string& productName,
        int quantity, const Date& supplyDate, double price);
    // Запрос 1
    void querySuppliersForProduct(const std::string& productName, int minQuantity,
        const Date& startDate, const Date& endDate);

    // --- Покупці та Замовлення ---
    void addBuyer(const Buyer& buyer);
    // Змінено: приймає дані для створення Order всередині
    bool addOrderToBuyer(const std::string& buyerEmail, const std::vector<Product>& products, const Date& orderDate);
    // Запрос 2
    std::vector<Buyer> getBuyersByProductCriteria(const std::string& productName,
        const Date& startDate,
        const Date& endDate,
        int minQuantity);
    const std::vector<Buyer>& getBuyers() const { return buyers; } // Геттер для DataManager та ін.

    // --- Магазини та Товари ---
    void addStore(const Store& store);
    void addProductToStore(const std::string& storeName, const std::string& productName,
        int quantity, double price);
    // Запрос 3
    void viewStoreProducts(const std::string& storeName);
    // Запрос 4
    void getProductInfoAllStores(const std::string& productName);
    void getProductInfoByStoreType(const std::string& productName, const std::string& storeType);
    void getProductInfoByStoreName(const std::string& productName, const std::string& storeName);

    // --- Продавці та Продажі ---
    void addSeller(const std::string& name, double salary); // Додає до глобального списку sellers
    // Продавець додається до магазину окремою логікою (наприклад, Store::addSellerById(int sellerId))
    // або при створенні магазину передаються ID продавців.
    // Поточний Menu::addSale буде шукати продавця в глобальному списку.

    bool addSale(const std::string& sellerName, // Ім'я продавця з глобального списку
        const std::string& buyerEmail,
        const std::string& storeName,
        const std::vector<Product>& products,
        const Date& saleDate);

    // --- Аналітика ---
    // Запрос 5
    void getSellerProductivity(const std::string& sellerName, const Date& startDate, const Date& endDate, const std::string& storeTypeFilter);

    // Методи доступу для DataManager
    std::vector<Supplier>& getSuppliersEditable() { return suppliers; }
    std::vector<Buyer>& getBuyersEditable() { return buyers; }
    std::vector<Store>& getStoresEditable() { return stores; }
    std::vector<Seller>& getSellersEditable() { return sellers; }

    const std::vector<Supplier>& getSuppliers() const { return suppliers; }
    // getBuyers() вже є
    const std::vector<Store>& getStores() const { return stores; }
    const std::vector<Seller>& getSellers() const { return sellers; } // Глобальний список продавців


    // Запит 6
    void getSellerProductivityInSpecificStore(const std::string& sellerName, const std::string& storeName,
        const Date& startDate, const Date& endDate);

};