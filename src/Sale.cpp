#include "../include/Sale.h"   // Адаптуй шлях
#include "../include/Seller.h" // Потрібен для seller->getId()
#include "../include/Buyer.h"  // Потрібен для buyer->getEmail()
#include "../include/Utils.h"  // Адаптуй шлях
#include <numeric>
#include <limits>

// ... (конструктори, calculateTotalPrice, геттери, linkReferences - без змін) ...
Sale::Sale(int id, Seller* seller_param, Buyer* buyer_param, const std::vector<Product>& products_param, const Date& saleDate_param, const std::string& storeName_param)
    : id(id), products(products_param), totalPrice(0.0), saleDate(saleDate_param), storeName_serial(storeName_param),
    seller_ptr(seller_param), buyer_ptr(buyer_param) {
    if (seller_param) sellerId_serial = seller_param->getId(); else sellerId_serial = 0; // 0 - як ознака відсутності
    if (buyer_param) buyerEmail_serial = buyer_param->getEmail(); else buyerEmail_serial = "";
    calculateTotalPrice();
}
Sale::Sale() : id(0), sellerId_serial(0), buyerEmail_serial(""), totalPrice(0.0), seller_ptr(nullptr), buyer_ptr(nullptr) {}
void Sale::calculateTotalPrice() { totalPrice = std::accumulate(products.begin(), products.end(), 0.0, [](double sum, const Product& p) { return sum + (p.getPrice() * p.getQuantity()); }); }
int Sale::getId() const { return id; }
Seller* Sale::getSeller() const { return seller_ptr; }
Buyer* Sale::getBuyer() const { return buyer_ptr; }
const std::vector<Product>& Sale::getProducts() const { return products; }
double Sale::getTotalPrice() const { return totalPrice; }
Date Sale::getDate() const { return saleDate; }
std::string Sale::getStoreName() const { return storeName_serial; }
void Sale::linkReferences(const std::vector<Seller>& allSellers, const std::vector<Buyer>& allBuyers) {
    seller_ptr = nullptr;
    if (sellerId_serial != 0) { // Шукаємо, тільки якщо ID не нульовий
        for (const auto& s : allSellers) {
            if (s.getId() == this->sellerId_serial) {
                this->seller_ptr = const_cast<Seller*>(&s); break;
            }
        }
    }
    buyer_ptr = nullptr;
    if (!buyerEmail_serial.empty()) { // Шукаємо, тільки якщо email не порожній
        for (const auto& b : allBuyers) {
            if (b.getEmail() == this->buyerEmail_serial) {
                this->buyer_ptr = const_cast<Buyer*>(&b); break;
            }
        }
    }
    if (sellerId_serial != 0 && !seller_ptr) { std::cerr << "Warning [Sale ID " << id << "]: Could not link seller with ID " << sellerId_serial << std::endl; }
    if (!buyerEmail_serial.empty() && !buyer_ptr) { std::cerr << "Warning [Sale ID " << id << "]: Could not link buyer with email " << buyerEmail_serial << std::endl; }
}


void Sale::serialize(std::ostream& os) const {
    os << "Sale.ID: " << id << std::endl;
    os << "Sale.SellerID: " << sellerId_serial << std::endl;       // Зберігаємо ID
    os << "Sale.BuyerEmail: " << buyerEmail_serial << std::endl; // Зберігаємо Email
    saleDate.serialize(os);
    os << "Sale.StoreName: " << storeName_serial << std::endl;
    os << "Sale.ProductsCount: " << products.size() << std::endl;
    for (size_t i = 0; i < products.size(); ++i) {
        os << "Sale.Product[" << i << "]: ---BEGIN---" << std::endl;
        products[i].serialize(os);
        os << "Sale.Product[" << i << "]: ---END---" << std::endl;
    }
    os << "Sale.TotalPrice: " << totalPrice << std::endl;
}

Sale Sale::deserialize(std::istream& is) {
    Sale sale;
    sale.id = Utils::readIntValueAfterLabel(is, "Sale.ID");
    sale.sellerId_serial = Utils::readIntValueAfterLabel(is, "Sale.SellerID");
    sale.buyerEmail_serial = Utils::readStringValueAfterLabel(is, "Sale.BuyerEmail");
    sale.saleDate = Date::deserialize(is);
    sale.storeName_serial = Utils::readStringValueAfterLabel(is, "Sale.StoreName");

    size_t num_products = Utils::readIntValueAfterLabel(is, "Sale.ProductsCount");
    sale.products.reserve(num_products);
    for (size_t i = 0; i < num_products; ++i) {
        Utils::readStringValueAfterLabel(is, "Sale.Product[" + std::to_string(i) + "]: ---BEGIN---");
        sale.products.push_back(Product::deserialize(is));
        Utils::readStringValueAfterLabel(is, "Sale.Product[" + std::to_string(i) + "]: ---END---");
    }
    sale.totalPrice = Utils::readDoubleValueAfterLabel(is, "Sale.TotalPrice");
    // seller_ptr та buyer_ptr будуть встановлені пізніше через linkReferences
    return sale;
}