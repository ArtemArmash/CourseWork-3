#include "../include/DataManager.h"
#include <fstream>
#include <vector>
#include <algorithm> // Для std::max
#include <iostream>  // Для std::cerr
#include <limits>    // Для std::numeric_limits

// #include <filesystem> // Для C++17 std::filesystem::create_directories
                      // Потрібно буде налаштувати проект для C++17

// --- Допоміжні функції для запису/читання векторів ---
namespace DataManager { // Поміщаємо їх в той же namespace

    template<typename T>
    void writeVector(std::ostream& os, const std::vector<T>& vec) {
        os << vec.size() << std::endl;
        for (const auto& item : vec) {
            item.serialize(os); // Припускаємо, що T має метод serialize
        }
    }

    template<typename T>
    std::vector<T> readVector(std::istream& is) {
        size_t size = 0; // Ініціалізуємо нулем
        if (!(is >> size)) { // Перевірка на помилку читання розміру
            if (is.eof() && size == 0) { // Якщо кінець файлу і розмір не прочитаний (або 0) - порожній вектор
                return {};
            }
            std::cerr << "DataManager Error: Failed to read vector size." << std::endl;
            // Можна кинути виключення або повернути порожній вектор
            return {}; // Повертаємо порожній вектор у випадку помилки
        }
        is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::vector<T> vec;
        if (size == 0) return vec; // Якщо розмір 0, повертаємо порожній вектор

        vec.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            if (is.peek() == EOF && i < size) { // Перевірка на несподіваний кінець файлу
                std::cerr << "DataManager Error: Unexpected EOF while reading vector item " << i + 1 << " of " << size << std::endl;
                break; // Виходимо, якщо файл закінчився раніше
            }
            vec.push_back(T::deserialize(is)); // Припускаємо, що T має статичний метод deserialize
        }
        return vec;
    }
    // --- Кінець допоміжних функцій ---


    void saveData(const Menu& menu, const std::string& basePath) {
        // Спроба створити директорію (потребує C++17 та <filesystem>)
        /*
        try {
            if (!std::filesystem::exists(basePath)) {
                std::filesystem::create_directories(basePath);
                std::cout << "Directory created: " << basePath << std::endl;
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Filesystem error: " << e.what() << std::endl;
            // Можливо, варто не продовжувати, якщо директорію не вдалося створити
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
            // Додати інші лічильники ID, якщо вони з'являться (наприклад, для Sale)
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

        // Відновлення зв'язків для об'єктів Sale всередині кожного Seller
        // Важливо: menu.getSellers() та menu.getBuyers() тут мають повертати вже завантажені дані
        for (auto& seller : menu.getSellersEditable()) { // Потрібен доступ до не-const вектора
            for (auto& sale : seller.getSalesEditable()) { // Потрібен доступ до не-const вектора
                sale.linkReferences(menu.getSellers(), menu.getBuyers());
            }
        }
        // Аналогічно, якщо Store зберігає ID продавців, тут було б зв'язування для них.

        std::ifstream idCountersFile(basePath + ID_COUNTERS_FILE);
        if (idCountersFile.is_open()) {
            int nextOrderId = 1, nextSellerId = 1; // Значення за замовчуванням, якщо читання не вдасться
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
            // Якщо файл лічильників не знайдено (перший запуск або помилка),
            // встановлюємо лічильники на основі максимальних ID серед завантажених даних.
            int maxOrderId = 0;
            for (const auto& buyer : menu.getBuyers()) { // Використовуємо const-версію для читання
                for (const auto& order : buyer.getOrders()) {
                    if (order.getId() > maxOrderId) maxOrderId = order.getId();
                }
            }
            Order::setNextId(maxOrderId + 1);

            int maxSellerId = 0;
            for (const auto& seller : menu.getSellers()) { // Використовуємо const-версію для читання
                if (seller.getId() > maxSellerId) maxSellerId = seller.getId();
            }
            Seller::setNextId(maxSellerId + 1);
        }
    }

} // namespace DataManager