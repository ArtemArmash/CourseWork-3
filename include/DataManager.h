#pragma once
#include <string>
#include "../include/Menu.h" // ��� ����� ��������� Menu �� ���� ������ �� �����

namespace DataManager {
    // ����� �� ����� (����� ������� �����������)
    const std::string SUPPLIERS_FILE = "suppliers.dat";
    const std::string BUYERS_FILE = "buyers.dat";
    // ���������� ����������� �������� Buyer, ���� ������� ���� ��� Order �� ����'�������,
    // ��� ���� �� ������ ��� � ������ ��'����, ����� �������.
    // const std::string ORDERS_FILE = "orders.dat";
    const std::string STORES_FILE = "stores.dat";
    const std::string SELLERS_FILE = "sellers.dat";
    // ������ ����������� �������� Seller.
    // const std::string SALES_FILE = "sales.dat";
    const std::string ID_COUNTERS_FILE = "id_counters.dat";

    // ������� ��� ���������� �� ������������ ��� �����
    void saveData(const Menu& menu, const std::string& basePath = "data/");
    void loadData(Menu& menu, const std::string& basePath = "data/");
}