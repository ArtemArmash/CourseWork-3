#pragma once
#include <vector>
#include <string>
#include "Store.h"

class Manager {
private:
    std::string name;
    int id;
    std::vector<Store> stores;

public:
    Manager(const std::string& name, int id);
    void addStore(const Store& store);
    void displayManagerInfo() const;
};
