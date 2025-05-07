#include <iostream>
#include <vector>
#include <string>
#include <clocale>      // Для setlocale
#include <iomanip>      // Для std::fixed, std::setprecision
// #include <limits>    // Може не знадобитися тут напряму (вже використовується в Utils та інших .cpp)

#include "include/Menu.h"    // Головний клас
#include "include/Date.h"    // Клас для роботи з датами
#include "include/Product.h" // Клас Product
#include "include/Buyer.h"   // Клас Buyer
#include "include/Store.h"   // Клас Store
#include "include/Seller.h"  // Клас Seller
#include "include/Supplier.h"// Клас Supplier

#include "include/DataManager.h" // Наш менеджер даних
#include "include/Utils.h"       // Якщо якісь утиліти звідти потрібні напряму в main (зараз ні)

// Допоміжна функція для виводу інформації про покупців
void printBuyersList(const std::string& title, const std::vector<Buyer>& buyers_list) {
    std::cout << "\n--- " << title << " --- (" << buyers_list.size() << " знайдено)\n";
    if (buyers_list.empty()) {
        std::cout << "Немає покупців, що відповідають критеріям.\n";
    }
    else {
        for (const auto& buyer_item : buyers_list) {
            buyer_item.displayBuyerInfo();
            std::cout << "  Замовлення покупця:\n";
            if (buyer_item.getOrders().empty()) {
                std::cout << "    (немає замовлень)\n";
            }
            else {
                for (const auto& order : buyer_item.getOrders()) {
                    std::cout << "    - Замовлення ID: " << order.getId() << ", Дата: " << order.getOrderDate().toString()
                        << ", Сума: " << order.getTotalPrice() << "\n";
                    for (const auto& p : order.getProducts()) {
                        std::cout << "      -- " << p.getName() << " (К-сть: " << p.getQuantity() << ", Ціна: " << p.getPrice() << ")\n";
                    }
                }
            }
            std::cout << "----------------------\n";
        }
    }
}

// Допоміжна функція для виводу постачальників (можна розширити, якщо треба)
void printSuppliersBrief(const std::vector<Supplier>& suppliers_list) {
    if (suppliers_list.empty()) {
        std::cout << "Постачальники не знайдені.\n";
        return;
    }
    for (const auto& sup : suppliers_list) {
        std::cout << "Постачальник: " << sup.getName() << " (" << sup.getContactInfo() << ")\n";
    }
}


int main() {
    // Налаштування локалі та виводу для консолі
    // Для Windows можна спробувати це для кращого відображення українських символів
    // #ifdef _WIN32
    // SetConsoleOutputCP(1251); // або CP_UTF8 (65001)
    // SetConsoleCP(1251);
    // #endif
    setlocale(LC_ALL, "ukr"); // або "Ukrainian" або ""
    std::cout << std::fixed << std::setprecision(2);

    Menu menu;
    const std::string dataPath = "trade_data/"; // Переконайся, що ця папка існує

    std::cout << "Спроба завантажити дані з " << dataPath << "...\n";
    try {
        DataManager::loadData(menu, dataPath);
        std::cout << "Дані успішно завантажено (або файли не знайдено/порожні).\n";

        // ... (початок main.cpp) ...

        if (menu.getSuppliers().empty() && menu.getBuyers().empty() && menu.getStores().empty() && menu.getSellers().empty()) {
            std::cout << "Система порожня після спроби завантаження. Додавання початкових тестових даних...\n";

            // --- ПОЧАТКОВІ ТЕСТОВІ ДАНІ ---
            // Постачальники
            menu.addSupplier("Фоззі Фуд Сервіс", "Київ, вул. Промислова, 10");
            menu.addSupplier("Галичина-Захід Постач", "Львів, вул. Городоцька, 200");
            menu.addSupplier("ОвочіФрукти Опт", "Херсон, шосе Оптове, 5");

            // ВИПРАВЛЕННЯ: назви товарів без внутрішніх лапок, якщо вони не критичні
            menu.addProductToSupplier("Фоззі Фуд Сервіс", "Молоко Ферма 2.5%", 120, Date("2024-05-25"), 33.50);
            menu.addProductToSupplier("Фоззі Фуд Сервіс", "Хліб Український", 250, Date("2024-05-29"), 26.00);
            menu.addProductToSupplier("Галичина-Захід Постач", "Сир Гауда", 60, Date("2024-05-20"), 280.00);
            menu.addProductToSupplier("Галичина-Захід Постач", "Молоко Ферма 2.5%", 80, Date("2024-05-26"), 33.00);
            menu.addProductToSupplier("ОвочіФрукти Опт", "Яблука Голден", 500, Date("2024-05-22"), 25.00); // <--- ВИПРАВЛЕНО

            // Покупці
            menu.addBuyer(Buyer("Андрій Ковальчук", "andrii.k@example.com"));
            menu.addBuyer(Buyer("Олена Мельник", "olena.m@example.com"));
            menu.addBuyer(Buyer("Сергій Ткаченко", "serhii.t@example.com"));

            // Замовлення (ВИПРАВЛЕННЯ назв товарів)
            std::vector<Product> andrii_order1_prods = { Product("Молоко Ферма 2.5%", 3, 38.00), Product("Хліб Український", 1, 29.00) };
            menu.addOrderToBuyer("andrii.k@example.com", andrii_order1_prods, Date("2024-05-30"));
            std::vector<Product> olena_order1_prods = { Product("Сир Гауда", 1, 300.00), Product("Яблука Голден", 2, 30.00) }; // <--- ВИПРАВЛЕНО
            menu.addOrderToBuyer("olena.m@example.com", olena_order1_prods, Date("2024-05-28"));
            std::vector<Product> andrii_order2_prods = { Product("Яблука Голден", 5, 30.00) }; // <--- ВИПРАВЛЕНО
            menu.addOrderToBuyer("andrii.k@example.com", andrii_order2_prods, Date("2024-05-25"));

            // Магазини
            menu.addStore(Store("Супермаркет Добрий Кошик", "Супермаркет", "м. Київ, вул. Велика, 15"));
            menu.addStore(Store("Магазин Біля Дому", "Магазин", "м. Львів, вул. Затишна, 3"));
            menu.addStore(Store("Кіоск Свіжі Продукти", "Кіоск", "м. Київ, перехрестя Головної та Паркової"));

            // Товари в магазинах (ВИПРАВЛЕННЯ назв товарів)
            menu.addProductToStore("Супермаркет Добрий Кошик", "Молоко Ферма 2.5%", 30, 39.00);
            menu.addProductToStore("Супермаркет Добрий Кошик", "Хліб Український", 50, 30.00);
            menu.addProductToStore("Супермаркет Добрий Кошик", "Сир Гауда", 15, 310.00);
            menu.addProductToStore("Супермаркет Добрий Кошик", "Яблука Голден", 100, 32.00); // <--- ВИПРАВЛЕНО

            menu.addProductToStore("Магазин Біля Дому", "Молоко Ферма 2.5%", 10, 40.00);
            menu.addProductToStore("Магазин Біля Дому", "Хліб Український", 20, 31.00);

            menu.addProductToStore("Кіоск Свіжі Продукти", "Молоко Ферма 2.5%", 5, 42.00);
            menu.addProductToStore("Кіоск Свіжі Продукти", "Хліб Український", 10, 32.00);

            // Продавці
            menu.addSeller("Вікторія Павленко", 10000.00);
            menu.addSeller("Максим Григоренко", 9800.00);
            menu.addSeller("Наталія Лисенко", 9500.00);

            // Продажі (ВИПРАВЛЕННЯ назв товарів)
            std::vector<Product> sale1_prods = { Product("Молоко Ферма 2.5%", 1, 39.00), Product("Хліб Український", 1, 30.00) };
            menu.addSale("Вікторія Павленко", "andrii.k@example.com", "Супермаркет Добрий Кошик", sale1_prods, Date("2024-05-30"));

            std::vector<Product> sale2_prods = { Product("Сир Гауда", 1, 310.00) };
            menu.addSale("Максим Григоренко", "olena.m@example.com", "Супермаркет Добрий Кошик", sale2_prods, Date("2024-05-28"));

            std::vector<Product> sale3_prods = { Product("Молоко Ферма 2.5%", 2, 40.00) };
            menu.addSale("Наталія Лисенко", "serhii.t@example.com", "Магазин Біля Дому", sale3_prods, Date("2024-05-31"));

            std::vector<Product> sale4_prods = { Product("Хліб Український", 1, 32.00) };
            menu.addSale("Вікторія Павленко", "", "Кіоск Свіжі Продукти", sale4_prods, Date("2024-05-29"));
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Критична помилка під час завантаження даних: " << e.what()
            << "\nРоботу програми буде припинено. Перевірте файли даних у: " << dataPath << std::endl;
        return 1; // Завершуємо програму, якщо завантаження критично не вдалося
    }
    std::cout << "==================================================\n";
    std::cout << "\nДЕМОНСТРАЦІЯ РОБОТИ СИСТЕМИ ТА ЗАПИТІВ 1-6\n";
    std::cout << "==================================================\n";

    // --- Запит 1: Постачальники ---
    std::cout << "\n--- [ЗАПИТ 1] Постачальники товару 'Молоко', мін. к-сть 70, за травень 2024 ---\n";
    menu.querySuppliersForProduct("Молоко", 70, Date("2024-05-01"), Date("2024-05-31"));
    std::cout << "\n--- [ЗАПИТ 1] Постачальники товару 'Хліб' (без обмежень по даті/кількості) ---\n";
    menu.querySuppliersForProduct("Хліб", 0, Date(), Date());

    // --- Запит 2: Покупці ---
      // ВИПРАВЛЕННЯ: шукаємо за назвою без внутрішніх лапок
    std::cout << "\n--- [ЗАПИТ 2] Покупці, що купили 'Яблука Голден', мін. 2 од. в замовленні, за травень 2024 ---\n";
    std::vector<Buyer> buyers_apples = menu.getBuyersByProductCriteria("Яблука Голден", Date("2024-05-01"), Date("2024-05-31"), 2);
    printBuyersList("Покупці 'Яблука Голден' (мін. 2 од.) за травень", buyers_apples);

    std::cout << "\n--- [ЗАПИТ 2] Покупці, що робили будь-які замовлення 30.05.2024 ---\n";
    std::vector<Buyer> buyers_may30 = menu.getBuyersByProductCriteria("", Date("2024-05-30"), Date("2024-05-30"), 0);
    printBuyersList("Покупці з замовленнями 30.05.2024", buyers_may30);

    // --- Запит 3: Номенклатура та обсяг товарів у магазині ---
    std::cout << "\n--- [ЗАПИТ 3] Товари в магазині 'Супермаркет Добрий Кошик' ---\n";
    menu.viewStoreProducts("Супермаркет Добрий Кошик");
    std::cout << "\n--- [ЗАПИТ 3] Товари в магазині 'Кіоск Свіжі Продукти' ---\n";
    menu.viewStoreProducts("Кіоск Свіжі Продукти");

    // --- Запит 4: Відомості про товар по точках ---
    // ВИПРАВЛЕННЯ: шукаємо за назвами без внутрішніх лапок
    std::cout << "\n--- [ЗАПИТ 4] Інформація про товар 'Молоко Ферма 2.5%' у всіх магазинах ---\n";
    menu.getProductInfoAllStores("Молоко Ферма 2.5%");
    std::cout << "\n--- [ЗАПИТ 4] Інформація про товар 'Хліб Український' в магазинах типу 'Магазин' ---\n";
    menu.getProductInfoByStoreType("Хліб Український", "Магазин");
    std::cout << "\n--- [ЗАПИТ 4] Інформація про товар 'Сир Гауда' в магазині 'Супермаркет Добрий Кошик' ---\n";
    menu.getProductInfoByStoreName("Сир Гауда", "Супермаркет Добрий Кошик");

    // ... (решта запитів 5 та 6 залишаються з виправленими іменами магазинів) ...
    // --- Запит 5: Продуктивність продавця по типу магазинів ---
    std::cout << "\n--- [ЗАПИТ 5] Продуктивність продавця 'Вікторія Павленко' (за весь час, тип: Супермаркет) ---\n";
    menu.getSellerProductivity("Вікторія Павленко", Date(), Date(), "Супермаркет");
    std::cout << "\n--- [ЗАПИТ 5] Продуктивність продавця 'Максим Григоренко' (за травень 2024, всі типи магазинів) ---\n";
    menu.getSellerProductivity("Максим Григоренко", Date("2024-05-01"), Date("2024-05-31"), "");

    // --- Запит 6: Продуктивність продавця в конкретному магазині ---
    std::cout << "\n--- [ЗАПИТ 6] Продуктивність 'Вікторія Павленко' в 'Супермаркет Добрий Кошик' за травень 2024 ---\n";
    menu.getSellerProductivityInSpecificStore("Вікторія Павленко", "Супермаркет Добрий Кошик", Date("2024-05-01"), Date("2024-05-31"));
    std::cout << "\n--- [ЗАПИТ 6] Продуктивність 'Наталія Лисенко' в 'Магазин Біля Дому' за весь час ---\n";
    menu.getSellerProductivityInSpecificStore("Наталія Лисенко", "Магазин Біля Дому", Date(), Date());


    std::cout << "\n\n--- МІСЦЕ ДЛЯ ДЕМОНСТРАЦІЇ ЗАПИТІВ 7-15 (після їх реалізації) ---\n";
    // ТУТ ТИ БУДЕШ ДОДАВАТИ ВИКЛИКИ ДЛЯ РЕШТИ ЗАПИТІВ
    // Приклад:
    // std::cout << "\n--- [ЗАПИТ 7] Обсяг продажів товару 'Молоко 'Ферма' 2.5%' за травень по всіх магазинах ---\n";
    // menu.getProductSalesVolume("Молоко 'Ферма' 2.5%", Date("2024-05-01"), Date("2024-05-31"));
    // (потрібно буде створити метод getProductSalesVolume в Menu)

    std::cout << "==================================================\n";
    std::cout << "\nСпроба зберегти дані до " << dataPath << "...\n";
    try {
        DataManager::saveData(menu, dataPath);
        std::cout << "Дані успішно збережено.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Помилка під час збереження даних: " << e.what() << std::endl;
    }
    std::cout << "==================================================\n";

    std::cout << "Завершення демонстраційної програми.\n";
    // std::cout << "Натисніть Enter для виходу...";
    // std::cin.get(); // Щоб консоль не закривалася одразу, якщо запускаєш не з IDE
    return 0;
}