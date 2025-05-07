#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Request.h"
#include "Sale.h"
#include "Order.h"

class QueryProcessor {
private:
    std::vector<Request> requests;
    std::vector<Sale> sales;
    std::vector<Order> orders;

public:
    QueryProcessor() {}

    void processSale(const Sale& sale) {
        sales.push_back(sale);
    }

    void analyzeRequests() {
        std::cout << "Обробка запитів..." << std::endl;
    }
};