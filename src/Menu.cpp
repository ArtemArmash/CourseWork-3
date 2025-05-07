#include "../include/Menu.h"
#include <iostream>
#include <map>
#include <algorithm>
#include <limits>
#include <optional>
#include <iomanip>

// Функція для порівняння рядків без урахування регістру (можна винести в утиліти)
bool containsIgnoreCaseLocal(const std::string& text, const std::string& pattern) {
    if (pattern.empty()) return true;
    if (text.empty()) return false;
    return std::search(
        text.begin(), text.end(),
        pattern.begin(), pattern.end(),
        [](unsigned char ch1, unsigned char ch2) { return std::tolower(ch1) == std::tolower(ch2); }
    ) != text.end();
}


// --- Постачальники ---
void Menu::addSupplier(const std::string& name, const std::string& contactInfo) {
    suppliers.emplace_back(name, contactInfo); // Використовуємо emplace_back для ефективності
    std::cout << "Постачальник '" << name << "' доданий.\n";
}

void Menu::addProductToSupplier(const std::string& supplierName, const std::string& productName,
    int quantity, const Date& supplyDate, double price) {
    for (auto& supplier : suppliers) {
        if (containsIgnoreCaseLocal(supplier.getName(), supplierName)) {
            supplier.addSuppliedProduct(Product(productName, quantity, supplyDate, price));
            std::cout << "Продукт '" << productName << "' доданий до постачальника '" << supplier.getName() << "'.\n";
            return;
        }
    }
    std::cout << "Помилка: Постачальник з ім'ям схожим на '" << supplierName << "' не знайдений.\n";
}

void Menu::querySuppliersForProduct(const std::string& productName, int minQuantity,
    const Date& startDate, const Date& endDate) {
    int supplierCount = 0;
    std::cout << "\n--- Результати пошуку постачальників ---\n";
    std::cout << "Товар: " << productName
        << ", Мін. кількість: " << (minQuantity > 0 ? std::to_string(minQuantity) : "не враховується")
        << ", Період: ";
    if (startDate.isValid() && endDate.isValid()) {
        std::cout << startDate.toString() << " - " << endDate.toString() << "\n";
    }
    else if (startDate.isValid()) {
        std::cout << "з " << startDate.toString() << "\n";
    }
    else if (endDate.isValid()) {
        std::cout << "до " << endDate.toString() << "\n";
    }
    else {
        std::cout << "Весь час\n";
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
                bool dateMatch = true; // За замовчуванням дата підходить
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
            std::cout << "Постачальник: " << supplier.getName()
                << ", Загальна кількість за критеріями: " << totalQuantityMatchingCriteria << "\n";
            supplierCount++;
            anySupplierFound = true;
        }
        else if (suppliesThisProductType && minQuantity <= 0 && !startDate.isValid() && !endDate.isValid()) {
            // Якщо постачальник просто постачає товар, і немає фільтрів по даті/кількості
            std::cout << "Постачальник: " << supplier.getName() << " (постачає '" << productName << "')\n";
            supplierCount++;
            anySupplierFound = true;
        }
    }

    if (!anySupplierFound) {
        std::cout << "Постачальники, що відповідають критеріям, не знайдені.\n";
    }
    std::cout << "--------------------------------------\n";
    std::cout << "Всього знайдено постачальників: " << supplierCount << "\n";
}


// --- Покупці та Замовлення ---
void Menu::addBuyer(const Buyer& buyer) {
    // Перевірка на унікальність email перед додаванням
    for (const auto& b : buyers) {
        if (b.getEmail() == buyer.getEmail()) {
            std::cout << "Помилка: Покупець з email '" << buyer.getEmail() << "' вже існує.\n";
            return;
        }
    }
    buyers.push_back(buyer);
    std::cout << "Покупець '" << buyer.getName() << "' доданий.\n";
}

Buyer* Menu::findBuyerByEmail(const std::string& email) {
    for (auto& b : buyers) {
        if (b.getEmail() == email) {
            return &b;
        }
    }
    return nullptr;
}

// Змінений метод
bool Menu::addOrderToBuyer(const std::string& buyerEmail, const std::vector<Product>& products, const Date& orderDate) {
    Buyer* buyer = findBuyerByEmail(buyerEmail);
    if (!buyer) {
        std::cout << "Помилка: Покупець з email '" << buyerEmail << "' не знайдений.\n";
        return false;
    }
    if (products.empty()) {
        std::cout << "Помилка: Замовлення не може бути без товарів.\n";
        return false;
    }
    int orderId = Order::generateOrderId();
    Order newOrder(orderId, products, orderDate);
    buyer->addOrder(newOrder);
    std::cout << "Замовлення (ID: " << orderId << ") успішно додано для покупця '" << buyer->getName() << "'.\n";
    return true;
}

std::vector<Buyer> Menu::getBuyersByProductCriteria(const std::string& productName,
    const Date& startDate, const Date& endDate,
    int minQuantity) {
    std::vector<Buyer> result;
    std::cout << "\n--- Результати пошуку покупців ---\n";
    std::cout << "Товар: " << (productName.empty() ? "будь-який" : productName)
        << ", Мін. кількість в одному замовленні: " << (minQuantity > 0 ? std::to_string(minQuantity) : "не враховується")
        << ", Період: ";
    if (startDate.isValid() && endDate.isValid()) {
        std::cout << startDate.toString() << " - " << endDate.toString() << "\n";
    }
    else if (startDate.isValid()) {
        std::cout << "з " << startDate.toString() << "\n";
    }
    else if (endDate.isValid()) {
        std::cout << "до " << endDate.toString() << "\n";
    }
    else {
        std::cout << "Весь час\n";
    }
    std::cout << "-----------------------------------\n";

    for (const auto& buyer : buyers) {
        // Використовуємо оновлений Buyer::hasRelevantOrder
        if (buyer.hasRelevantOrder(productName, startDate, endDate, minQuantity)) {
            result.push_back(buyer);
        }
    }
    return result;
}

// --- Магазини ---
void Menu::addStore(const Store& store) {
    for (const auto& s : stores) {
        if (s.getName() == store.getName()) {
            std::cout << "Помилка: Магазин з ім'ям '" << store.getName() << "' вже існує.\n";
            return;
        }
    }
    stores.push_back(store);
    std::cout << "Магазин '" << store.getName() << "' доданий.\n";
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
        // Можливо, тут потрібна логіка для оновлення існуючого товару,
        // а не просто додавання нового Product, якщо назва співпадає.
        // Поки що просто додаємо.
        store->addProduct(Product(productName, quantity, price)); // Створюємо продукт без дати поставки
        std::cout << "Продукт '" << productName << "' доданий до магазину '" << store->getName() << "'.\n";
    }
    else {
        std::cout << "Помилка: Магазин з ім'ям схожим на '" << storeName << "' не знайдений.\n";
    }
}

void Menu::viewStoreProducts(const std::string& storeName) {
    Store* store = findStoreByName(storeName);
    if (store) {
        std::cout << "\n--- Товари в магазині: " << store->getName() << " ---\n";
        store->displayProductCatalog();
        std::cout << "---------------------------------\n";
    }
    else {
        std::cout << "Магазин з ім'ям схожим на '" << storeName << "' не знайдений.\n";
    }
}

void Menu::printProductInfoInStore(const Store& store, const std::string& productName) {
    std::cout << "\nМагазин: " << store.getName() << " (Тип: " << store.getType() << ", Локація: " << store.getLocation() << ")" << std::endl;
    bool found = false;
    for (const auto& product : store.getProducts()) {
        if (containsIgnoreCaseLocal(product.getName(), productName)) {
            std::cout << "  - Товар: " << product.getName()
                << " | Кількість: " << product.getQuantity()
                << " | Ціна: " << product.getPrice() << std::endl;
            found = true;
        }
    }
    if (!found) {
        std::cout << "  Товар '" << productName << "' не знайдено в цьому магазині.\n";
    }
}

void Menu::getProductInfoAllStores(const std::string& productName) {
    std::cout << "\n--- Інформація про товар '" << productName << "' у всіх магазинах ---";
    if (stores.empty()) {
        std::cout << "\nНемає жодного магазину в системі.\n";
        return;
    }
    bool foundAnywhere = false;
    for (const auto& store : stores) {
        // Перевіримо, чи є товар в магазині, перед тим як викликати printProductInfoInStore
        // щоб уникнути виводу "Товар не знайдено" для кожного магазину.
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
        std::cout << "\nТовар '" << productName << "' не знайдено в жодному магазині.\n";
    }
    std::cout << "-----------------------------------------------------\n";
}

void Menu::getProductInfoByStoreType(const std::string& productName, const std::string& storeType) {
    std::cout << "\n--- Інформація про товар '" << productName << "' в магазинах типу '" << storeType << "' ---";
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
        std::cout << "\nНе знайдено магазинів типу '" << storeType << "'.\n";
    }
    else if (!productFoundInType) {
        std::cout << "\nТовар '" << productName << "' не знайдено в магазинах типу '" << storeType << "'.\n";
    }
    std::cout << "-----------------------------------------------------------------\n";
}

void Menu::getProductInfoByStoreName(const std::string& productName, const std::string& storeName) {
    std::cout << "\n--- Інформація про товар '" << productName << "' в магазині '" << storeName << "' ---";
    Store* store = findStoreByName(storeName);
    if (store) {
        printProductInfoInStore(*store, productName);
    }
    else {
        std::cout << "\nМагазин з ім'ям схожим на '" << storeName << "' не знайдено.\n";
    }
    std::cout << "------------------------------------------------------------\n";
}


// --- Продавці та Продажі ---
void Menu::addSeller(const std::string& name, double salary) {
    for (const auto& s : sellers) {
        // Можна додати перевірку на унікальність імені або ID, якщо потрібно
        if (s.getName() == name) { // Проста перевірка на ім'я
            std::cout << "Помилка: Продавець з ім'ям '" << name << "' вже існує.\n";
            return;
        }
    }
    sellers.emplace_back(name, salary);
    std::cout << "Продавець '" << name << "' доданий (ID: " << sellers.back().getId() << ").\n";
}

Seller* Menu::findSellerByName(const std::string& name) {
    for (auto& seller : sellers) { // Шукаємо в глобальному списку sellers
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
    Seller* seller = findSellerByName(sellerName); // Шукаємо в глобальному списку
    if (!seller) {
        std::cerr << "Помилка: Продавець '" << sellerName << "' не знайдений у системі.\n";
        return false;
    }

    Buyer* buyer = findBuyerByEmail(buyerEmail);
    if (!buyer && !buyerEmail.empty()) { // Дозволяємо анонімного покупця, якщо email порожній
        std::cerr << "Помилка: Покупець з email '" << buyerEmail << "' не знайдений.\n";
        // Якщо потрібно, можна створити "тимчасового" або "анонімного" покупця тут,
        // або вимагати, щоб покупець завжди існував. Для ТЗ, де в кіосках немає даних покупця,
        // це може бути важливим. Поки що вимагаємо існуючого, якщо email вказаний.
        return false;
    }
    // Якщо buyerEmail порожній, buyer залишиться nullptr, і це треба буде врахувати в Sale.

    Store* store = findStoreByName(storeName);
    if (!store) {
        std::cerr << "Помилка: Магазин '" << storeName << "' не знайдений.\n";
        return false;
    }
    if (products.empty()) {
        std::cerr << "Помилка: Продаж не може бути без товарів.\n";
        return false;
    }

    // Тут логіка перевірки наявності товарів у магазині перед продажем
    // та зменшення їх кількості. Це важлива частина, якої зараз немає.
    // For now, assume products are available.

    static int nextSaleId = 1; // TODO: Цей лічильник ID теж треба зберігати/відновлювати
    int saleId = nextSaleId++;

    Sale newSale(saleId, seller, buyer, products, saleDate, store->getName());
    seller->addSale(newSale); // Додаємо продаж до глобального об'єкта продавця

    std::cout << "Продаж (ID: " << saleId << ") успішно зареєстровано для продавця '" << seller->getName()
        << (buyer ? std::string(" покупцю '") + buyer->getName() + "'" : " (анонімний покупець)")
        << " в магазині '" << store->getName() << "'.\n";
    return true;
}


void Menu::getSellerProductivity(const std::string& sellerName, const Date& startDate, const Date& endDate, const std::string& storeTypeFilter) {
    std::cout << "\n--- Продуктивність продавця '" << sellerName << "' --- \n";
    std::cout << "Період: ";
    if (startDate.isValid() && endDate.isValid()) {
        std::cout << startDate.toString() << " - " << endDate.toString();
    }
    else if (startDate.isValid()) {
        std::cout << "з " << startDate.toString();
    }
    else if (endDate.isValid()) {
        std::cout << "до " << endDate.toString();
    }
    else {
        std::cout << "Весь час";
    }

    if (!storeTypeFilter.empty()) {
        std::cout << ", Тип магазину: " << storeTypeFilter << "\n";
    }
    else {
        std::cout << ", Всі типи магазинів\n";
    }
    std::cout << "------------------------------------------------\n";

    Seller* seller = findSellerByName(sellerName); // Шукаємо в глобальному списку
    if (!seller) {
        std::cout << "Продавець '" << sellerName << "' не знайдений.\n";
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
            storeMatch = true; // Якщо фільтр типу магазину порожній, будь-який магазин підходить
        }
        else {
            // Потрібно знайти магазин, де відбувся продаж, і перевірити його тип
            Store* saleStore = findStoreByName(sale.getStoreName()); // sale.getStoreName() має повертати ім'я магазину
            if (saleStore && containsIgnoreCaseLocal(saleStore->getType(), storeTypeFilter)) {
                storeMatch = true;
            }
        }

        if (storeMatch) {
            totalProductivity += sale.getTotalPrice();
            salesCount++;
        }
    }

    std::cout << "Загальна сума продажів за критеріями: " << std::fixed << std::setprecision(2) << totalProductivity << "\n";
    std::cout << "Кількість продажів за критеріями: " << salesCount << "\n";
    std::cout << "------------------------------------------------\n";
}

///////////////// 6// / // / / /
void Menu::getSellerProductivityInSpecificStore(const std::string& sellerName, const std::string& storeName,
    const Date& startDate, const Date& endDate) {
    std::cout << "\n--- Продуктивність продавця '" << sellerName << "' в магазині '" << storeName << "' ---\n";
    std::cout << "Період: ";
    if (startDate.isValid() && endDate.isValid()) {
        std::cout << startDate.toString() << " - " << endDate.toString() << "\n";
    }
    else if (startDate.isValid()) {
        std::cout << "з " << startDate.toString() << "\n";
    }
    else if (endDate.isValid()) {
        std::cout << "до " << endDate.toString() << "\n";
    }
    else {
        std::cout << "Весь час\n";
    }
    std::cout << "------------------------------------------------------------------\n";

    Seller* seller = findSellerByName(sellerName); // Шукаємо в глобальному списку продавців
    if (!seller) {
        std::cout << "Продавець '" << sellerName << "' не знайдений.\n";
        std::cout << "------------------------------------------------------------------\n";
        return;
    }

    Store* store = findStoreByName(storeName);
    if (!store) {
        std::cout << "Магазин '" << storeName << "' не знайдений.\n";
        std::cout << "------------------------------------------------------------------\n";
        return;
    }

    double totalProductivity = 0.0;
    int salesCount = 0;

    for (const auto& sale : seller->getSales()) {
        // Перевірка, чи продаж відбувся в потрібному магазині
        if (sale.getStoreName() != store->getName()) { // Порівнюємо імена магазинів
            continue;
        }

        // Перевірка дати (аналогічно до getSellerProductivity)
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

    std::cout << "Загальна сума продажів за критеріями: " << std::fixed << std::setprecision(2) << totalProductivity << "\n";
    std::cout << "Кількість продажів за критеріями: " << salesCount << "\n";
    std::cout << "------------------------------------------------------------------\n";
}

