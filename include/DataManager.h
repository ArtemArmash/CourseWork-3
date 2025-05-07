#pragma once
#include <string>
#include "../include/Menu.h" // Щоб знати структуру Menu та мати доступ до класів

namespace DataManager {
    // Шляхи до файлів (можна зробити константами)
    const std::string SUPPLIERS_FILE = "suppliers.dat";
    const std::string BUYERS_FILE = "buyers.dat";
    // Замовлення зберігаються всередині Buyer, тому окремий файл для Order не обов'язковий,
    // але якщо їх багато або є складні зв'язки, можна винести.
    // const std::string ORDERS_FILE = "orders.dat";
    const std::string STORES_FILE = "stores.dat";
    const std::string SELLERS_FILE = "sellers.dat";
    // Продажі зберігаються всередині Seller.
    // const std::string SALES_FILE = "sales.dat";
    const std::string ID_COUNTERS_FILE = "id_counters.dat";

    // Функції для збереження та завантаження всіх даних
    void saveData(const Menu& menu, const std::string& basePath = "data/");
    void loadData(Menu& menu, const std::string& basePath = "data/");
}