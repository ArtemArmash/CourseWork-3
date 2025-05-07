#include "../include/Menu.h"
#include <iostream>
#include <map>
#include <algorithm>
#include <limits>
#include <optional>
#include <iomanip>

// ������� ��� ��������� ����� ��� ���������� ������� (����� ������� � ������)
bool containsIgnoreCaseLocal(const std::string& text, const std::string& pattern) {
    if (pattern.empty()) return true;
    if (text.empty()) return false;
    return std::search(
        text.begin(), text.end(),
        pattern.begin(), pattern.end(),
        [](unsigned char ch1, unsigned char ch2) { return std::tolower(ch1) == std::tolower(ch2); }
    ) != text.end();
}


// --- ������������� ---
void Menu::addSupplier(const std::string& name, const std::string& contactInfo) {
    suppliers.emplace_back(name, contactInfo); // ������������� emplace_back ��� �����������
    std::cout << "������������ '" << name << "' �������.\n";
}

void Menu::addProductToSupplier(const std::string& supplierName, const std::string& productName,
    int quantity, const Date& supplyDate, double price) {
    for (auto& supplier : suppliers) {
        if (containsIgnoreCaseLocal(supplier.getName(), supplierName)) {
            supplier.addSuppliedProduct(Product(productName, quantity, supplyDate, price));
            std::cout << "������� '" << productName << "' ������� �� ������������� '" << supplier.getName() << "'.\n";
            return;
        }
    }
    std::cout << "�������: ������������ � ��'�� ������ �� '" << supplierName << "' �� ���������.\n";
}

void Menu::querySuppliersForProduct(const std::string& productName, int minQuantity,
    const Date& startDate, const Date& endDate) {
    int supplierCount = 0;
    std::cout << "\n--- ���������� ������ ������������� ---\n";
    std::cout << "�����: " << productName
        << ", ̳�. �������: " << (minQuantity > 0 ? std::to_string(minQuantity) : "�� �����������")
        << ", �����: ";
    if (startDate.isValid() && endDate.isValid()) {
        std::cout << startDate.toString() << " - " << endDate.toString() << "\n";
    }
    else if (startDate.isValid()) {
        std::cout << "� " << startDate.toString() << "\n";
    }
    else if (endDate.isValid()) {
        std::cout << "�� " << endDate.toString() << "\n";
    }
    else {
        std::cout << "���� ���\n";
    }
    std::cout << "--------------------------------------\n";

    bool anySupplierFound = false;
    for (const auto& supplier : suppliers) {
        int totalQuantityMatchingCriteria = 0;
        bool suppliesThisProductType = false;

        for (const auto& product : supplier.getSuppliedProducts()) {
            if (containsIgnoreCaseLocal(product.getName(), productName)) {
                suppliesThisProductType = true;
                bool quantityMatch = (minQuantity <= 0) || (product.getQuantity() >= minQuantity);
                bool dateMatch = true; // �� ������������� ���� ��������
                if (startDate.isValid() && endDate.isValid()) {
                    dateMatch = product.getSupplyDate().isValid() && product.getSupplyDate() >= startDate && product.getSupplyDate() <= endDate;
                }
                else if (startDate.isValid()) {
                    dateMatch = product.getSupplyDate().isValid() && product.getSupplyDate() >= startDate;
                }
                else if (endDate.isValid()) {
                    dateMatch = product.getSupplyDate().isValid() && product.getSupplyDate() <= endDate;
                }

                if (quantityMatch && dateMatch) {
                    totalQuantityMatchingCriteria += product.getQuantity();
                }
            }
        }

        if (suppliesThisProductType && totalQuantityMatchingCriteria > 0) {
            std::cout << "������������: " << supplier.getName()
                << ", �������� ������� �� ���������: " << totalQuantityMatchingCriteria << "\n";
            supplierCount++;
            anySupplierFound = true;
        }
        else if (suppliesThisProductType && minQuantity <= 0 && !startDate.isValid() && !endDate.isValid()) {
            // ���� ������������ ������ ������� �����, � ���� ������� �� ���/�������
            std::cout << "������������: " << supplier.getName() << " (������� '" << productName << "')\n";
            supplierCount++;
            anySupplierFound = true;
        }
    }

    if (!anySupplierFound) {
        std::cout << "�������������, �� ���������� ��������, �� �������.\n";
    }
    std::cout << "--------------------------------------\n";
    std::cout << "������ �������� �������������: " << supplierCount << "\n";
}


// --- ������� �� ���������� ---
void Menu::addBuyer(const Buyer& buyer) {
    // �������� �� ���������� email ����� ����������
    for (const auto& b : buyers) {
        if (b.getEmail() == buyer.getEmail()) {
            std::cout << "�������: �������� � email '" << buyer.getEmail() << "' ��� ����.\n";
            return;
        }
    }
    buyers.push_back(buyer);
    std::cout << "�������� '" << buyer.getName() << "' �������.\n";
}

Buyer* Menu::findBuyerByEmail(const std::string& email) {
    for (auto& b : buyers) {
        if (b.getEmail() == email) {
            return &b;
        }
    }
    return nullptr;
}

// ������� �����
bool Menu::addOrderToBuyer(const std::string& buyerEmail, const std::vector<Product>& products, const Date& orderDate) {
    Buyer* buyer = findBuyerByEmail(buyerEmail);
    if (!buyer) {
        std::cout << "�������: �������� � email '" << buyerEmail << "' �� ���������.\n";
        return false;
    }
    if (products.empty()) {
        std::cout << "�������: ���������� �� ���� ���� ��� ������.\n";
        return false;
    }
    int orderId = Order::generateOrderId();
    Order newOrder(orderId, products, orderDate);
    buyer->addOrder(newOrder);
    std::cout << "���������� (ID: " << orderId << ") ������ ������ ��� ������� '" << buyer->getName() << "'.\n";
    return true;
}

std::vector<Buyer> Menu::getBuyersByProductCriteria(const std::string& productName,
    const Date& startDate, const Date& endDate,
    int minQuantity) {
    std::vector<Buyer> result;
    std::cout << "\n--- ���������� ������ �������� ---\n";
    std::cout << "�����: " << (productName.empty() ? "����-����" : productName)
        << ", ̳�. ������� � ������ ���������: " << (minQuantity > 0 ? std::to_string(minQuantity) : "�� �����������")
        << ", �����: ";
    if (startDate.isValid() && endDate.isValid()) {
        std::cout << startDate.toString() << " - " << endDate.toString() << "\n";
    }
    else if (startDate.isValid()) {
        std::cout << "� " << startDate.toString() << "\n";
    }
    else if (endDate.isValid()) {
        std::cout << "�� " << endDate.toString() << "\n";
    }
    else {
        std::cout << "���� ���\n";
    }
    std::cout << "-----------------------------------\n";

    for (const auto& buyer : buyers) {
        // ������������� ��������� Buyer::hasRelevantOrder
        if (buyer.hasRelevantOrder(productName, startDate, endDate, minQuantity)) {
            result.push_back(buyer);
        }
    }
    return result;
}

// --- �������� ---
void Menu::addStore(const Store& store) {
    for (const auto& s : stores) {
        if (s.getName() == store.getName()) {
            std::cout << "�������: ������� � ��'�� '" << store.getName() << "' ��� ����.\n";
            return;
        }
    }
    stores.push_back(store);
    std::cout << "������� '" << store.getName() << "' �������.\n";
}

Store* Menu::findStoreByName(const std::string& name) {
    for (auto& store : stores) {
        if (containsIgnoreCaseLocal(store.getName(), name)) {
            return &store;
        }
    }
    return nullptr;
}

void Menu::addProductToStore(const std::string& storeName, const std::string& productName, int quantity, double price) {
    Store* store = findStoreByName(storeName);
    if (store) {
        // �������, ��� ������� ����� ��� ��������� ��������� ������,
        // � �� ������ ��������� ������ Product, ���� ����� �������.
        // ���� �� ������ ������.
        store->addProduct(Product(productName, quantity, price)); // ��������� ������� ��� ���� ��������
        std::cout << "������� '" << productName << "' ������� �� �������� '" << store->getName() << "'.\n";
    }
    else {
        std::cout << "�������: ������� � ��'�� ������ �� '" << storeName << "' �� ���������.\n";
    }
}

void Menu::viewStoreProducts(const std::string& storeName) {
    Store* store = findStoreByName(storeName);
    if (store) {
        std::cout << "\n--- ������ � �������: " << store->getName() << " ---\n";
        store->displayProductCatalog();
        std::cout << "---------------------------------\n";
    }
    else {
        std::cout << "������� � ��'�� ������ �� '" << storeName << "' �� ���������.\n";
    }
}

void Menu::printProductInfoInStore(const Store& store, const std::string& productName) {
    std::cout << "\n�������: " << store.getName() << " (���: " << store.getType() << ", �������: " << store.getLocation() << ")" << std::endl;
    bool found = false;
    for (const auto& product : store.getProducts()) {
        if (containsIgnoreCaseLocal(product.getName(), productName)) {
            std::cout << "  - �����: " << product.getName()
                << " | ʳ������: " << product.getQuantity()
                << " | ֳ��: " << product.getPrice() << std::endl;
            found = true;
        }
    }
    if (!found) {
        std::cout << "  ����� '" << productName << "' �� �������� � ����� �������.\n";
    }
}

void Menu::getProductInfoAllStores(const std::string& productName) {
    std::cout << "\n--- ���������� ��� ����� '" << productName << "' � ��� ��������� ---";
    if (stores.empty()) {
        std::cout << "\n���� ������� �������� � ������.\n";
        return;
    }
    bool foundAnywhere = false;
    for (const auto& store : stores) {
        // ���������, �� � ����� � �������, ����� ��� �� ��������� printProductInfoInStore
        // ��� �������� ������ "����� �� ��������" ��� ������� ��������.
        bool presentInThisStore = false;
        for (const auto& product : store.getProducts()) {
            if (containsIgnoreCaseLocal(product.getName(), productName)) {
                presentInThisStore = true;
                foundAnywhere = true;
                break;
            }
        }
        if (presentInThisStore) {
            printProductInfoInStore(store, productName);
        }
    }
    if (!foundAnywhere) {
        std::cout << "\n����� '" << productName << "' �� �������� � ������� �������.\n";
    }
    std::cout << "-----------------------------------------------------\n";
}

void Menu::getProductInfoByStoreType(const std::string& productName, const std::string& storeType) {
    std::cout << "\n--- ���������� ��� ����� '" << productName << "' � ��������� ���� '" << storeType << "' ---";
    bool anyStoreOfTypeFound = false;
    bool productFoundInType = false;
    for (const auto& store : stores) {
        if (containsIgnoreCaseLocal(store.getType(), storeType)) {
            anyStoreOfTypeFound = true;
            bool presentInThisStore = false;
            for (const auto& product : store.getProducts()) {
                if (containsIgnoreCaseLocal(product.getName(), productName)) {
                    presentInThisStore = true;
                    productFoundInType = true;
                    break;
                }
            }
            if (presentInThisStore) {
                printProductInfoInStore(store, productName);
            }
        }
    }
    if (!anyStoreOfTypeFound) {
        std::cout << "\n�� �������� �������� ���� '" << storeType << "'.\n";
    }
    else if (!productFoundInType) {
        std::cout << "\n����� '" << productName << "' �� �������� � ��������� ���� '" << storeType << "'.\n";
    }
    std::cout << "-----------------------------------------------------------------\n";
}

void Menu::getProductInfoByStoreName(const std::string& productName, const std::string& storeName) {
    std::cout << "\n--- ���������� ��� ����� '" << productName << "' � ������� '" << storeName << "' ---";
    Store* store = findStoreByName(storeName);
    if (store) {
        printProductInfoInStore(*store, productName);
    }
    else {
        std::cout << "\n������� � ��'�� ������ �� '" << storeName << "' �� ��������.\n";
    }
    std::cout << "------------------------------------------------------------\n";
}


// --- �������� �� ������ ---
void Menu::addSeller(const std::string& name, double salary) {
    for (const auto& s : sellers) {
        // ����� ������ �������� �� ���������� ���� ��� ID, ���� �������
        if (s.getName() == name) { // ������ �������� �� ��'�
            std::cout << "�������: ��������� � ��'�� '" << name << "' ��� ����.\n";
            return;
        }
    }
    sellers.emplace_back(name, salary);
    std::cout << "��������� '" << name << "' ������� (ID: " << sellers.back().getId() << ").\n";
}

Seller* Menu::findSellerByName(const std::string& name) {
    for (auto& seller : sellers) { // ������ � ����������� ������ sellers
        if (containsIgnoreCaseLocal(seller.getName(), name)) {
            return &seller;
        }
    }
    return nullptr;
}
Seller* Menu::findSellerById(int id) {
    for (auto& seller : sellers) {
        if (seller.getId() == id) {
            return &seller;
        }
    }
    return nullptr;
}


bool Menu::addSale(const std::string& sellerName, const std::string& buyerEmail,
    const std::string& storeName, const std::vector<Product>& products,
    const Date& saleDate) {
    Seller* seller = findSellerByName(sellerName); // ������ � ����������� ������
    if (!seller) {
        std::cerr << "�������: ��������� '" << sellerName << "' �� ��������� � ������.\n";
        return false;
    }

    Buyer* buyer = findBuyerByEmail(buyerEmail);
    if (!buyer && !buyerEmail.empty()) { // ���������� ��������� �������, ���� email �������
        std::cerr << "�������: �������� � email '" << buyerEmail << "' �� ���������.\n";
        // ���� �������, ����� �������� "�����������" ��� "���������" ������� ���,
        // ��� ��������, ��� �������� ������ �������. ��� ��, �� � ������ ���� ����� �������,
        // �� ���� ���� ��������. ���� �� �������� ���������, ���� email ��������.
        return false;
    }
    // ���� buyerEmail �������, buyer ���������� nullptr, � �� ����� ���� ��������� � Sale.

    Store* store = findStoreByName(storeName);
    if (!store) {
        std::cerr << "�������: ������� '" << storeName << "' �� ���������.\n";
        return false;
    }
    if (products.empty()) {
        std::cerr << "�������: ������ �� ���� ���� ��� ������.\n";
        return false;
    }

    // ��� ����� �������� �������� ������ � ������� ����� ��������
    // �� ��������� �� �������. �� ������� �������, ��� ����� ����.
    // For now, assume products are available.

    static int nextSaleId = 1; // TODO: ��� �������� ID ��� ����� ��������/�����������
    int saleId = nextSaleId++;

    Sale newSale(saleId, seller, buyer, products, saleDate, store->getName());
    seller->addSale(newSale); // ������ ������ �� ����������� ��'���� ��������

    std::cout << "������ (ID: " << saleId << ") ������ ������������ ��� �������� '" << seller->getName()
        << (buyer ? std::string(" ������� '") + buyer->getName() + "'" : " (�������� ��������)")
        << " � ������� '" << store->getName() << "'.\n";
    return true;
}


void Menu::getSellerProductivity(const std::string& sellerName, const Date& startDate, const Date& endDate, const std::string& storeTypeFilter) {
    std::cout << "\n--- ������������� �������� '" << sellerName << "' --- \n";
    std::cout << "�����: ";
    if (startDate.isValid() && endDate.isValid()) {
        std::cout << startDate.toString() << " - " << endDate.toString();
    }
    else if (startDate.isValid()) {
        std::cout << "� " << startDate.toString();
    }
    else if (endDate.isValid()) {
        std::cout << "�� " << endDate.toString();
    }
    else {
        std::cout << "���� ���";
    }

    if (!storeTypeFilter.empty()) {
        std::cout << ", ��� ��������: " << storeTypeFilter << "\n";
    }
    else {
        std::cout << ", �� ���� ��������\n";
    }
    std::cout << "------------------------------------------------\n";

    Seller* seller = findSellerByName(sellerName); // ������ � ����������� ������
    if (!seller) {
        std::cout << "��������� '" << sellerName << "' �� ���������.\n";
        std::cout << "------------------------------------------------\n";
        return;
    }

    double totalProductivity = 0.0;
    int salesCount = 0;

    for (const auto& sale : seller->getSales()) {
        bool dateMatch = true;
        if (startDate.isValid() && endDate.isValid()) {
            dateMatch = sale.getDate().isValid() && sale.getDate() >= startDate && sale.getDate() <= endDate;
        }
        else if (startDate.isValid()) {
            dateMatch = sale.getDate().isValid() && sale.getDate() >= startDate;
        }
        else if (endDate.isValid()) {
            dateMatch = sale.getDate().isValid() && sale.getDate() <= endDate;
        }

        if (!dateMatch) {
            continue;
        }

        bool storeMatch = false;
        if (storeTypeFilter.empty()) {
            storeMatch = true; // ���� ������ ���� �������� �������, ����-���� ������� ��������
        }
        else {
            // ������� ������ �������, �� ������� ������, � ��������� ���� ���
            Store* saleStore = findStoreByName(sale.getStoreName()); // sale.getStoreName() �� ��������� ��'� ��������
            if (saleStore && containsIgnoreCaseLocal(saleStore->getType(), storeTypeFilter)) {
                storeMatch = true;
            }
        }

        if (storeMatch) {
            totalProductivity += sale.getTotalPrice();
            salesCount++;
        }
    }

    std::cout << "�������� ���� ������� �� ���������: " << std::fixed << std::setprecision(2) << totalProductivity << "\n";
    std::cout << "ʳ������ ������� �� ���������: " << salesCount << "\n";
    std::cout << "------------------------------------------------\n";
}

///////////////// 6// / // / / /
void Menu::getSellerProductivityInSpecificStore(const std::string& sellerName, const std::string& storeName,
    const Date& startDate, const Date& endDate) {
    std::cout << "\n--- ������������� �������� '" << sellerName << "' � ������� '" << storeName << "' ---\n";
    std::cout << "�����: ";
    if (startDate.isValid() && endDate.isValid()) {
        std::cout << startDate.toString() << " - " << endDate.toString() << "\n";
    }
    else if (startDate.isValid()) {
        std::cout << "� " << startDate.toString() << "\n";
    }
    else if (endDate.isValid()) {
        std::cout << "�� " << endDate.toString() << "\n";
    }
    else {
        std::cout << "���� ���\n";
    }
    std::cout << "------------------------------------------------------------------\n";

    Seller* seller = findSellerByName(sellerName); // ������ � ����������� ������ ���������
    if (!seller) {
        std::cout << "��������� '" << sellerName << "' �� ���������.\n";
        std::cout << "------------------------------------------------------------------\n";
        return;
    }

    Store* store = findStoreByName(storeName);
    if (!store) {
        std::cout << "������� '" << storeName << "' �� ���������.\n";
        std::cout << "------------------------------------------------------------------\n";
        return;
    }

    double totalProductivity = 0.0;
    int salesCount = 0;

    for (const auto& sale : seller->getSales()) {
        // ��������, �� ������ ������� � ��������� �������
        if (sale.getStoreName() != store->getName()) { // ��������� ����� ��������
            continue;
        }

        // �������� ���� (��������� �� getSellerProductivity)
        bool dateMatch = true;
        if (startDate.isValid() && endDate.isValid()) {
            dateMatch = sale.getDate().isValid() && sale.getDate() >= startDate && sale.getDate() <= endDate;
        }
        else if (startDate.isValid()) {
            dateMatch = sale.getDate().isValid() && sale.getDate() >= startDate;
        }
        else if (endDate.isValid()) {
            dateMatch = sale.getDate().isValid() && sale.getDate() <= endDate;
        }

        if (dateMatch) {
            totalProductivity += sale.getTotalPrice();
            salesCount++;
        }
    }

    std::cout << "�������� ���� ������� �� ���������: " << std::fixed << std::setprecision(2) << totalProductivity << "\n";
    std::cout << "ʳ������ ������� �� ���������: " << salesCount << "\n";
    std::cout << "------------------------------------------------------------------\n";
}

