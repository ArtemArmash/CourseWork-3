#include <iostream>
#include <vector>
#include <string>
#include <clocale>      // ��� setlocale
#include <iomanip>      // ��� std::fixed, std::setprecision
// #include <limits>    // ���� �� ����������� ��� ������� (��� ��������������� � Utils �� ����� .cpp)

#include "include/Menu.h"    // �������� ����
#include "include/Date.h"    // ���� ��� ������ � ������
#include "include/Product.h" // ���� Product
#include "include/Buyer.h"   // ���� Buyer
#include "include/Store.h"   // ���� Store
#include "include/Seller.h"  // ���� Seller
#include "include/Supplier.h"// ���� Supplier

#include "include/DataManager.h" // ��� �������� �����
#include "include/Utils.h"       // ���� ���� ������ ����� ������ ������� � main (����� �)

// �������� ������� ��� ������ ���������� ��� ��������
void printBuyersList(const std::string& title, const std::vector<Buyer>& buyers_list) {
    std::cout << "\n--- " << title << " --- (" << buyers_list.size() << " ��������)\n";
    if (buyers_list.empty()) {
        std::cout << "���� ��������, �� ���������� ��������.\n";
    }
    else {
        for (const auto& buyer_item : buyers_list) {
            buyer_item.displayBuyerInfo();
            std::cout << "  ���������� �������:\n";
            if (buyer_item.getOrders().empty()) {
                std::cout << "    (���� ���������)\n";
            }
            else {
                for (const auto& order : buyer_item.getOrders()) {
                    std::cout << "    - ���������� ID: " << order.getId() << ", ����: " << order.getOrderDate().toString()
                        << ", ����: " << order.getTotalPrice() << "\n";
                    for (const auto& p : order.getProducts()) {
                        std::cout << "      -- " << p.getName() << " (�-���: " << p.getQuantity() << ", ֳ��: " << p.getPrice() << ")\n";
                    }
                }
            }
            std::cout << "----------------------\n";
        }
    }
}

// �������� ������� ��� ������ ������������� (����� ���������, ���� �����)
void printSuppliersBrief(const std::vector<Supplier>& suppliers_list) {
    if (suppliers_list.empty()) {
        std::cout << "������������� �� �������.\n";
        return;
    }
    for (const auto& sup : suppliers_list) {
        std::cout << "������������: " << sup.getName() << " (" << sup.getContactInfo() << ")\n";
    }
}


int main() {
    // ������������ ����� �� ������ ��� ������
    // ��� Windows ����� ���������� �� ��� ������� ����������� ���������� �������
    // #ifdef _WIN32
    // SetConsoleOutputCP(1251); // ��� CP_UTF8 (65001)
    // SetConsoleCP(1251);
    // #endif
    setlocale(LC_ALL, "ukr"); // ��� "Ukrainian" ��� ""
    std::cout << std::fixed << std::setprecision(2);

    Menu menu;
    const std::string dataPath = "trade_data/"; // �����������, �� �� ����� ����

    std::cout << "������ ����������� ��� � " << dataPath << "...\n";
    try {
        DataManager::loadData(menu, dataPath);
        std::cout << "��� ������ ����������� (��� ����� �� ��������/������).\n";

        // ... (������� main.cpp) ...

        if (menu.getSuppliers().empty() && menu.getBuyers().empty() && menu.getStores().empty() && menu.getSellers().empty()) {
            std::cout << "������� ������� ���� ������ ������������. ��������� ���������� �������� �����...\n";

            // --- �������² �����² ��Ͳ ---
            // �������������
            menu.addSupplier("���� ��� �����", "���, ���. ����������, 10");
            menu.addSupplier("��������-����� ������", "����, ���. ����������, 200");
            menu.addSupplier("����������� ���", "������, ���� ������, 5");

            // �����������: ����� ������ ��� �������� �����, ���� ���� �� �������
            menu.addProductToSupplier("���� ��� �����", "������ ����� 2.5%", 120, Date("2024-05-25"), 33.50);
            menu.addProductToSupplier("���� ��� �����", "��� ����������", 250, Date("2024-05-29"), 26.00);
            menu.addProductToSupplier("��������-����� ������", "��� �����", 60, Date("2024-05-20"), 280.00);
            menu.addProductToSupplier("��������-����� ������", "������ ����� 2.5%", 80, Date("2024-05-26"), 33.00);
            menu.addProductToSupplier("����������� ���", "������ ������", 500, Date("2024-05-22"), 25.00); // <--- ����������

            // �������
            menu.addBuyer(Buyer("����� ���������", "andrii.k@example.com"));
            menu.addBuyer(Buyer("����� �������", "olena.m@example.com"));
            menu.addBuyer(Buyer("����� ��������", "serhii.t@example.com"));

            // ���������� (����������� ���� ������)
            std::vector<Product> andrii_order1_prods = { Product("������ ����� 2.5%", 3, 38.00), Product("��� ����������", 1, 29.00) };
            menu.addOrderToBuyer("andrii.k@example.com", andrii_order1_prods, Date("2024-05-30"));
            std::vector<Product> olena_order1_prods = { Product("��� �����", 1, 300.00), Product("������ ������", 2, 30.00) }; // <--- ����������
            menu.addOrderToBuyer("olena.m@example.com", olena_order1_prods, Date("2024-05-28"));
            std::vector<Product> andrii_order2_prods = { Product("������ ������", 5, 30.00) }; // <--- ����������
            menu.addOrderToBuyer("andrii.k@example.com", andrii_order2_prods, Date("2024-05-25"));

            // ��������
            menu.addStore(Store("����������� ������ �����", "�����������", "�. ���, ���. ������, 15"));
            menu.addStore(Store("������� ���� ����", "�������", "�. ����, ���. �������, 3"));
            menu.addStore(Store("ʳ��� ��� ��������", "ʳ���", "�. ���, ���������� ������� �� �������"));

            // ������ � ��������� (����������� ���� ������)
            menu.addProductToStore("����������� ������ �����", "������ ����� 2.5%", 30, 39.00);
            menu.addProductToStore("����������� ������ �����", "��� ����������", 50, 30.00);
            menu.addProductToStore("����������� ������ �����", "��� �����", 15, 310.00);
            menu.addProductToStore("����������� ������ �����", "������ ������", 100, 32.00); // <--- ����������

            menu.addProductToStore("������� ���� ����", "������ ����� 2.5%", 10, 40.00);
            menu.addProductToStore("������� ���� ����", "��� ����������", 20, 31.00);

            menu.addProductToStore("ʳ��� ��� ��������", "������ ����� 2.5%", 5, 42.00);
            menu.addProductToStore("ʳ��� ��� ��������", "��� ����������", 10, 32.00);

            // ��������
            menu.addSeller("³����� ��������", 10000.00);
            menu.addSeller("������ ����������", 9800.00);
            menu.addSeller("������ �������", 9500.00);

            // ������ (����������� ���� ������)
            std::vector<Product> sale1_prods = { Product("������ ����� 2.5%", 1, 39.00), Product("��� ����������", 1, 30.00) };
            menu.addSale("³����� ��������", "andrii.k@example.com", "����������� ������ �����", sale1_prods, Date("2024-05-30"));

            std::vector<Product> sale2_prods = { Product("��� �����", 1, 310.00) };
            menu.addSale("������ ����������", "olena.m@example.com", "����������� ������ �����", sale2_prods, Date("2024-05-28"));

            std::vector<Product> sale3_prods = { Product("������ ����� 2.5%", 2, 40.00) };
            menu.addSale("������ �������", "serhii.t@example.com", "������� ���� ����", sale3_prods, Date("2024-05-31"));

            std::vector<Product> sale4_prods = { Product("��� ����������", 1, 32.00) };
            menu.addSale("³����� ��������", "", "ʳ��� ��� ��������", sale4_prods, Date("2024-05-29"));
        }

    }
    catch (const std::exception& e) {
        std::cerr << "�������� ������� �� ��� ������������ �����: " << e.what()
            << "\n������ �������� ���� ���������. �������� ����� ����� �: " << dataPath << std::endl;
        return 1; // ��������� ��������, ���� ������������ �������� �� �������
    }
    std::cout << "==================================================\n";
    std::cout << "\n���������ֲ� ������ ������� �� ����Ҳ� 1-6\n";
    std::cout << "==================================================\n";

    // --- ����� 1: ������������� ---
    std::cout << "\n--- [����� 1] ������������� ������ '������', ��. �-��� 70, �� ������� 2024 ---\n";
    menu.querySuppliersForProduct("������", 70, Date("2024-05-01"), Date("2024-05-31"));
    std::cout << "\n--- [����� 1] ������������� ������ '���' (��� �������� �� ���/�������) ---\n";
    menu.querySuppliersForProduct("���", 0, Date(), Date());

    // --- ����� 2: ������� ---
      // �����������: ������ �� ������ ��� �������� �����
    std::cout << "\n--- [����� 2] �������, �� ������ '������ ������', ��. 2 ��. � ���������, �� ������� 2024 ---\n";
    std::vector<Buyer> buyers_apples = menu.getBuyersByProductCriteria("������ ������", Date("2024-05-01"), Date("2024-05-31"), 2);
    printBuyersList("������� '������ ������' (��. 2 ��.) �� �������", buyers_apples);

    std::cout << "\n--- [����� 2] �������, �� ������ ����-�� ���������� 30.05.2024 ---\n";
    std::vector<Buyer> buyers_may30 = menu.getBuyersByProductCriteria("", Date("2024-05-30"), Date("2024-05-30"), 0);
    printBuyersList("������� � ������������ 30.05.2024", buyers_may30);

    // --- ����� 3: ������������ �� ����� ������ � ������� ---
    std::cout << "\n--- [����� 3] ������ � ������� '����������� ������ �����' ---\n";
    menu.viewStoreProducts("����������� ������ �����");
    std::cout << "\n--- [����� 3] ������ � ������� 'ʳ��� ��� ��������' ---\n";
    menu.viewStoreProducts("ʳ��� ��� ��������");

    // --- ����� 4: ³������ ��� ����� �� ������ ---
    // �����������: ������ �� ������� ��� �������� �����
    std::cout << "\n--- [����� 4] ���������� ��� ����� '������ ����� 2.5%' � ��� ��������� ---\n";
    menu.getProductInfoAllStores("������ ����� 2.5%");
    std::cout << "\n--- [����� 4] ���������� ��� ����� '��� ����������' � ��������� ���� '�������' ---\n";
    menu.getProductInfoByStoreType("��� ����������", "�������");
    std::cout << "\n--- [����� 4] ���������� ��� ����� '��� �����' � ������� '����������� ������ �����' ---\n";
    menu.getProductInfoByStoreName("��� �����", "����������� ������ �����");

    // ... (����� ������ 5 �� 6 ����������� � ������������ ������� ��������) ...
    // --- ����� 5: ������������� �������� �� ���� �������� ---
    std::cout << "\n--- [����� 5] ������������� �������� '³����� ��������' (�� ���� ���, ���: �����������) ---\n";
    menu.getSellerProductivity("³����� ��������", Date(), Date(), "�����������");
    std::cout << "\n--- [����� 5] ������������� �������� '������ ����������' (�� ������� 2024, �� ���� ��������) ---\n";
    menu.getSellerProductivity("������ ����������", Date("2024-05-01"), Date("2024-05-31"), "");

    // --- ����� 6: ������������� �������� � ����������� ������� ---
    std::cout << "\n--- [����� 6] ������������� '³����� ��������' � '����������� ������ �����' �� ������� 2024 ---\n";
    menu.getSellerProductivityInSpecificStore("³����� ��������", "����������� ������ �����", Date("2024-05-01"), Date("2024-05-31"));
    std::cout << "\n--- [����� 6] ������������� '������ �������' � '������� ���� ����' �� ���� ��� ---\n";
    menu.getSellerProductivityInSpecificStore("������ �������", "������� ���� ����", Date(), Date());


    std::cout << "\n\n--- ̲��� ��� ���������ֲ� ����Ҳ� 7-15 (���� �� ���������) ---\n";
    // ��� �� ����� �������� ������� ��� ����� ����Ҳ�
    // �������:
    // std::cout << "\n--- [����� 7] ����� ������� ������ '������ '�����' 2.5%' �� ������� �� ��� ��������� ---\n";
    // menu.getProductSalesVolume("������ '�����' 2.5%", Date("2024-05-01"), Date("2024-05-31"));
    // (������� ���� �������� ����� getProductSalesVolume � Menu)

    std::cout << "==================================================\n";
    std::cout << "\n������ �������� ��� �� " << dataPath << "...\n";
    try {
        DataManager::saveData(menu, dataPath);
        std::cout << "��� ������ ���������.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "������� �� ��� ���������� �����: " << e.what() << std::endl;
    }
    std::cout << "==================================================\n";

    std::cout << "���������� �������������� ��������.\n";
    // std::cout << "�������� Enter ��� ������...";
    // std::cin.get(); // ��� ������� �� ����������� ������, ���� �������� �� � IDE
    return 0;
}