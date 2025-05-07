#include "../include/Manager.h"

Manager::Manager(const std::string& name, int id)
    : name(name), id(id) {}

void Manager::addStore(const Store& store) {
    stores.push_back(store);
}

void Manager::displayManagerInfo() const {
    std::cout << "Manager Name: " << name << "\nID: " << id << std::endl;
}
