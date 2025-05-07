#pragma once
#include <vector>
#include <string>
#include <iostream> // ��� ostream, istream
#include "Product.h"
#include "Date.h"

class Seller; // �������� ����������
class Buyer;  // �������� ����������

class Sale {
private:
    int id;
    // ��� ����������/������������
    int sellerId_serial;
    std::string buyerEmail_serial;

    // ��� ������ � ���'�� ���� ������������ �� ��'��������
    Seller* seller_ptr = nullptr;
    Buyer* buyer_ptr = nullptr;

    std::vector<Product> products;
    double totalPrice;
    Date saleDate;
    std::string storeName_serial; // ��'� ��������

    void calculateTotalPrice();

public:
    Sale(int id, Seller* seller, Buyer* buyer, const std::vector<Product>& products,
        const Date& saleDate, const std::string& storeName);
    Sale(); // ����������� �� ������������� ��� ������������

    int getId() const;
    Seller* getSeller() const; // ������� ��'������ ��������
    Buyer* getBuyer() const;   // ������� ��'������ ��������
    const std::vector<Product>& getProducts() const;
    double getTotalPrice() const;
    Date getDate() const;
    std::string getStoreName() const;

    void linkReferences(const std::vector<Seller>& allSellers, const std::vector<Buyer>& allBuyers);

    void serialize(std::ostream& os) const;
    static Sale deserialize(std::istream& is);
};