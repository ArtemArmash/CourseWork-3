#pragma once
#include <vector>
#include <string>
#include "Supplier.h"
#include "Buyer.h"   // ������ Order
#include "Store.h"   // ������ Seller (���� Seller.h �� ������ Menu.h)
#include "Seller.h"  // ������ Sale (���� Sale.h �� ������ Menu.h)
#include "Date.h"
#include "Sale.h"
// �������� ����������, ���� � ������ ���������, ���� �� ������� ��������
// class Seller;
// class Store;

class Menu {
private:
    std::vector<Supplier> suppliers;
    std::vector<Buyer> buyers;
    std::vector<Store> stores;
    std::vector<Seller> sellers; // �������� ������ ��������� ��������� �������

    Buyer* findBuyerByEmail(const std::string& email);
    Seller* findSellerByName(const std::string& name); // ���� � sellers
    Seller* findSellerById(int id); // ��������� ��� ��'��������
    Store* findStoreByName(const std::string& name);
    void printProductInfoInStore(const Store& store, const std::string& productName);

public:
    Menu() = default; // ����������� �� �������������

    // --- ������������� ---
    void addSupplier(const std::string& name, const std::string& contactInfo);
    void addProductToSupplier(const std::string& supplierName, const std::string& productName,
        int quantity, const Date& supplyDate, double price);
    // ������ 1
    void querySuppliersForProduct(const std::string& productName, int minQuantity,
        const Date& startDate, const Date& endDate);

    // --- ������� �� ���������� ---
    void addBuyer(const Buyer& buyer);
    // ������: ������ ��� ��� ��������� Order ��������
    bool addOrderToBuyer(const std::string& buyerEmail, const std::vector<Product>& products, const Date& orderDate);
    // ������ 2
    std::vector<Buyer> getBuyersByProductCriteria(const std::string& productName,
        const Date& startDate,
        const Date& endDate,
        int minQuantity);
    const std::vector<Buyer>& getBuyers() const { return buyers; } // ������ ��� DataManager �� ��.

    // --- �������� �� ������ ---
    void addStore(const Store& store);
    void addProductToStore(const std::string& storeName, const std::string& productName,
        int quantity, double price);
    // ������ 3
    void viewStoreProducts(const std::string& storeName);
    // ������ 4
    void getProductInfoAllStores(const std::string& productName);
    void getProductInfoByStoreType(const std::string& productName, const std::string& storeType);
    void getProductInfoByStoreName(const std::string& productName, const std::string& storeName);

    // --- �������� �� ������ ---
    void addSeller(const std::string& name, double salary); // ���� �� ����������� ������ sellers
    // ��������� �������� �� �������� ������� ������ (���������, Store::addSellerById(int sellerId))
    // ��� ��� �������� �������� ����������� ID ���������.
    // �������� Menu::addSale ���� ������ �������� � ����������� ������.

    bool addSale(const std::string& sellerName, // ��'� �������� � ����������� ������
        const std::string& buyerEmail,
        const std::string& storeName,
        const std::vector<Product>& products,
        const Date& saleDate);

    // --- �������� ---
    // ������ 5
    void getSellerProductivity(const std::string& sellerName, const Date& startDate, const Date& endDate, const std::string& storeTypeFilter);

    // ������ ������� ��� DataManager
    std::vector<Supplier>& getSuppliersEditable() { return suppliers; }
    std::vector<Buyer>& getBuyersEditable() { return buyers; }
    std::vector<Store>& getStoresEditable() { return stores; }
    std::vector<Seller>& getSellersEditable() { return sellers; }

    const std::vector<Supplier>& getSuppliers() const { return suppliers; }
    // getBuyers() ��� �
    const std::vector<Store>& getStores() const { return stores; }
    const std::vector<Seller>& getSellers() const { return sellers; } // ���������� ������ ���������


    // ����� 6
    void getSellerProductivityInSpecificStore(const std::string& sellerName, const std::string& storeName,
        const Date& startDate, const Date& endDate);

};