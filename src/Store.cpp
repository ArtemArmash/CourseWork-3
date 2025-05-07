#include "../include/Store.h" // Адаптуй шлях
#include "../include/Utils.h" // Адаптуй шлях
#include <limits>

// ... (конструктори, геттери, addProduct, addSeller, displayStoreInfo, displayProductCatalog - без змін) ...
Store::Store(const std::string& name, const std::string& type, const std::string& location) : name(name), type(type), location(location) {}
Store::Store() : name(""), type(""), location("") {}
void Store::addProduct(const Product& product) { products.push_back(product); }
void Store::addSeller(const Seller& seller) { sellers.push_back(seller); } // Додає копію
void Store::displayStoreInfo() const { std::cout << "Store Name: " << name << "\nType: " << type << "\nLocation: " << location << std::endl; }
void Store::displayProductCatalog() const { if (products.empty()) { std::cout << "No products available in this store.\n"; } else { std::cout << "Product catalog for " << name << ":\n"; for (const auto& product : products) { std::cout << " - " << product.getName() << " | Qty: " << product.getQuantity() << " | Price: " << product.getPrice() << std::endl; } } }
std::vector<Product>& Store::getProductsEditable() { return products; }
std::vector<Seller>& Store::getSellersEditable() { return sellers; }


void Store::serialize(std::ostream& os) const {
    os << "Store.Name: " << name << std::endl;
    os << "Store.Type: " << type << std::endl;
    os << "Store.Location: " << location << std::endl;

    os << "Store.ProductsCount: " << products.size() << std::endl;
    for (size_t i = 0; i < products.size(); ++i) {
        os << "Store.Product[" << i << "]: ---BEGIN---" << std::endl;
        products[i].serialize(os);
        os << "Store.Product[" << i << "]: ---END---" << std::endl;
    }

    os << "Store.SellersCount: " << sellers.size() << std::endl;
    for (size_t i = 0; i < sellers.size(); ++i) {
        os << "Store.Seller[" << i << "]: ---BEGIN---" << std::endl;
        // Зберігаємо копію продавця, як вона є в магазині
        sellers[i].serialize(os);
        os << "Store.Seller[" << i << "]: ---END---" << std::endl;
    }
}

Store Store::deserialize(std::istream& is) {
    Store store;
    store.name = Utils::readStringValueAfterLabel(is, "Store.Name");
    store.type = Utils::readStringValueAfterLabel(is, "Store.Type");
    store.location = Utils::readStringValueAfterLabel(is, "Store.Location");

    size_t num_products = Utils::readIntValueAfterLabel(is, "Store.ProductsCount");
    store.products.reserve(num_products);
    for (size_t i = 0; i < num_products; ++i) {
        Utils::readStringValueAfterLabel(is, "Store.Product[" + std::to_string(i) + "]: ---BEGIN---");
        store.products.push_back(Product::deserialize(is));
        Utils::readStringValueAfterLabel(is, "Store.Product[" + std::to_string(i) + "]: ---END---");
    }

    size_t num_sellers = Utils::readIntValueAfterLabel(is, "Store.SellersCount");
    store.sellers.reserve(num_sellers);
    for (size_t i = 0; i < num_sellers; ++i) {
        Utils::readStringValueAfterLabel(is, "Store.Seller[" + std::to_string(i) + "]: ---BEGIN---");
        store.sellers.push_back(Seller::deserialize(is));
        Utils::readStringValueAfterLabel(is, "Store.Seller[" + std::to_string(i) + "]: ---END---");
    }
    // Якщо продавці в магазині мали б бути посиланнями на глобальних продавців,
    // то тут би читалися їхні ID, а потім в DataManager::loadData відбувалося б зв'язування.
    // З поточним підходом (копії продавців у магазині), додаткове зв'язування не потрібне,
    // АЛЕ продажі цих продавців не будуть синхронізовані з "глобальним" об'єктом продавця,
    // якщо такий є. Це обмеження поточної моделі.
    return store;
}