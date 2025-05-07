#include "../include/DataManager.h"
#include <fstream>
#include <vector>
#include <algorithm> // ��� std::max
#include <iostream>  // ��� std::cerr
#include <limits>    // ��� std::numeric_limits

// #include <filesystem> // ��� C++17 std::filesystem::create_directories
                      // ������� ���� ����������� ������ ��� C++17

// --- ������� ������� ��� ������/������� ������� ---
namespace DataManager { // ������� �� � ��� �� namespace

    template<typename T>
    void writeVector(std::ostream& os, const std::vector<T>& vec) {
        os << vec.size() << std::endl;
        for (const auto& item : vec) {
            item.serialize(os); // ����������, �� T �� ����� serialize
        }
    }

    template<typename T>
    std::vector<T> readVector(std::istream& is) {
        size_t size = 0; // ���������� �����
        if (!(is >> size)) { // �������� �� ������� ������� ������
            if (is.eof() && size == 0) { // ���� ����� ����� � ����� �� ���������� (��� 0) - ������� ������
                return {};
            }
            std::cerr << "DataManager Error: Failed to read vector size." << std::endl;
            // ����� ������ ���������� ��� ��������� ������� ������
            return {}; // ��������� ������� ������ � ������� �������
        }
        is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::vector<T> vec;
        if (size == 0) return vec; // ���� ����� 0, ��������� ������� ������

        vec.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            if (is.peek() == EOF && i < size) { // �������� �� ����������� ����� �����
                std::cerr << "DataManager Error: Unexpected EOF while reading vector item " << i + 1 << " of " << size << std::endl;
                break; // ��������, ���� ���� ��������� �����
            }
            vec.push_back(T::deserialize(is)); // ����������, �� T �� ��������� ����� deserialize
        }
        return vec;
    }
    // --- ʳ���� ��������� ������� ---


    void saveData(const Menu& menu, const std::string& basePath) {
        // ������ �������� ��������� (������� C++17 �� <filesystem>)
        /*
        try {
            if (!std::filesystem::exists(basePath)) {
                std::filesystem::create_directories(basePath);
                std::cout << "Directory created: " << basePath << std::endl;
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Filesystem error: " << e.what() << std::endl;
            // �������, ����� �� ������������, ���� ��������� �� ������� ��������
            return;
        }
        */

        std::ofstream suppliersFile(basePath + SUPPLIERS_FILE);
        if (suppliersFile.is_open()) {
            writeVector(suppliersFile, menu.getSuppliers());
            suppliersFile.close();
        }
        else {
            std::cerr << "Error: Could not open file for writing: " << basePath + SUPPLIERS_FILE << std::endl;
        }

        std::ofstream buyersFile(basePath + BUYERS_FILE);
        if (buyersFile.is_open()) {
            writeVector(buyersFile, menu.getBuyers());
            buyersFile.close();
        }
        else {
            std::cerr << "Error: Could not open file for writing: " << basePath + BUYERS_FILE << std::endl;
        }

        std::ofstream storesFile(basePath + STORES_FILE);
        if (storesFile.is_open()) {
            writeVector(storesFile, menu.getStores());
            storesFile.close();
        }
        else {
            std::cerr << "Error: Could not open file for writing: " << basePath + STORES_FILE << std::endl;
        }

        std::ofstream sellersFile(basePath + SELLERS_FILE);
        if (sellersFile.is_open()) {
            writeVector(sellersFile, menu.getSellers());
            sellersFile.close();
        }
        else {
            std::cerr << "Error: Could not open file for writing: " << basePath + SELLERS_FILE << std::endl;
        }

        std::ofstream idCountersFile(basePath + ID_COUNTERS_FILE);
        if (idCountersFile.is_open()) {
            idCountersFile << Order::getCurrentNextId() << std::endl;
            idCountersFile << Seller::getCurrentNextId() << std::endl;
            // ������ ���� ��������� ID, ���� ���� �'�������� (���������, ��� Sale)
            idCountersFile.close();
        }
        else {
            std::cerr << "Error: Could not open file for writing: " << basePath + ID_COUNTERS_FILE << std::endl;
        }
    }

    void loadData(Menu& menu, const std::string& basePath) {
        std::ifstream suppliersFile(basePath + SUPPLIERS_FILE);
        if (suppliersFile.is_open()) {
            menu.getSuppliersEditable() = readVector<Supplier>(suppliersFile);
            suppliersFile.close();
        }
        else {
            std::cerr << "Warning: Could not open file for reading: " << basePath + SUPPLIERS_FILE << ". Starting with no suppliers.\n";
        }

        std::ifstream buyersFile(basePath + BUYERS_FILE);
        if (buyersFile.is_open()) {
            menu.getBuyersEditable() = readVector<Buyer>(buyersFile);
            buyersFile.close();
        }
        else {
            std::cerr << "Warning: Could not open file for reading: " << basePath + BUYERS_FILE << ". Starting with no buyers.\n";
        }

        std::ifstream storesFile(basePath + STORES_FILE);
        if (storesFile.is_open()) {
            menu.getStoresEditable() = readVector<Store>(storesFile);
            storesFile.close();
        }
        else {
            std::cerr << "Warning: Could not open file for reading: " << basePath + STORES_FILE << ". Starting with no stores.\n";
        }

        std::ifstream sellersFile(basePath + SELLERS_FILE);
        if (sellersFile.is_open()) {
            menu.getSellersEditable() = readVector<Seller>(sellersFile);
            sellersFile.close();
        }
        else {
            std::cerr << "Warning: Could not open file for reading: " << basePath + SELLERS_FILE << ". Starting with no sellers.\n";
        }

        // ³��������� ��'���� ��� ��'���� Sale �������� ������� Seller
        // �������: menu.getSellers() �� menu.getBuyers() ��� ����� ��������� ��� ���������� ���
        for (auto& seller : menu.getSellersEditable()) { // ������� ������ �� ��-const �������
            for (auto& sale : seller.getSalesEditable()) { // ������� ������ �� ��-const �������
                sale.linkReferences(menu.getSellers(), menu.getBuyers());
            }
        }
        // ���������, ���� Store ������ ID ���������, ��� ���� � ��'�������� ��� ���.

        std::ifstream idCountersFile(basePath + ID_COUNTERS_FILE);
        if (idCountersFile.is_open()) {
            int nextOrderId = 1, nextSellerId = 1; // �������� �� �������������, ���� ������� �� ��������
            if (!(idCountersFile >> nextOrderId)) nextOrderId = 1;
            idCountersFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (!(idCountersFile >> nextSellerId)) nextSellerId = 1;
            idCountersFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            Order::setNextId(nextOrderId);
            Seller::setNextId(nextSellerId);
            idCountersFile.close();
        }
        else {
            std::cerr << "Warning: Could not open ID counters file: " << basePath + ID_COUNTERS_FILE << ". Setting IDs based on loaded data.\n";
            // ���� ���� ��������� �� �������� (������ ������ ��� �������),
            // ������������ ��������� �� ����� ������������ ID ����� ������������ �����.
            int maxOrderId = 0;
            for (const auto& buyer : menu.getBuyers()) { // ������������� const-����� ��� �������
                for (const auto& order : buyer.getOrders()) {
                    if (order.getId() > maxOrderId) maxOrderId = order.getId();
                }
            }
            Order::setNextId(maxOrderId + 1);

            int maxSellerId = 0;
            for (const auto& seller : menu.getSellers()) { // ������������� const-����� ��� �������
                if (seller.getId() > maxSellerId) maxSellerId = seller.getId();
            }
            Seller::setNextId(maxSellerId + 1);
        }
    }

} // namespace DataManager