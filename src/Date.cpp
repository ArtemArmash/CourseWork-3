#include "../include/Date.h" // Адаптуй шлях
#include "../include/Utils.h" // Адаптуй шлях
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <limits>

// ... (конструктори та інші оператори залишаються без змін з попередньої версії) ...
Date::Date() : valid_(false) {}

Date::Date(const std::string& dateStr) : valid_(false) {
    if (dateStr.empty()) {
        return;
    }
    std::tm temp_tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&temp_tm, "%Y-%m-%d");
    if (ss.fail()) return;
    char remaining;
    if (ss >> remaining) return;
    std::tm check_tm = temp_tm;
    check_tm.tm_isdst = -1;
    time_t t = std::mktime(&check_tm);
    if (t == -1) return;
    if (temp_tm.tm_mday != check_tm.tm_mday ||
        temp_tm.tm_mon != check_tm.tm_mon ||
        temp_tm.tm_year != check_tm.tm_year) return;
    date = check_tm;
    valid_ = true;
}

bool Date::isValid() const { return valid_; }

std::string Date::toString() const {
    if (!valid_) return "Invalid Date";
    std::ostringstream oss;
    oss << std::put_time(&date, "%Y-%m-%d");
    return oss.str();
}
std::tm Date::getTimeStruct() const { return date; }

bool Date::operator<(const Date& other) const {
    if (!valid_ || !other.valid_) return false;
    time_t t1 = mktime(const_cast<std::tm*>(&date));
    time_t t2 = mktime(const_cast<std::tm*>(&other.date));
    return t1 < t2;
}
// ... (інші оператори порівняння аналогічно) ...
bool Date::operator<=(const Date& other) const { if (!valid_ || !other.valid_) return false; time_t t1 = mktime(const_cast<std::tm*>(&date)); time_t t2 = mktime(const_cast<std::tm*>(&other.date)); return t1 <= t2; }
bool Date::operator>(const Date& other) const { return !(*this <= other); }
bool Date::operator>=(const Date& other) const { return !(*this < other); }
bool Date::operator==(const Date& other) const { if (valid_ != other.valid_) return false; if (!valid_) return true; time_t t1 = mktime(const_cast<std::tm*>(&date)); time_t t2 = mktime(const_cast<std::tm*>(&other.date)); return t1 == t2; }
bool Date::operator!=(const Date& other) const { return !(*this == other); }


void Date::serialize(std::ostream& os) const {
    os << "Date.Valid: " << valid_ << std::endl;
    if (valid_) {
        os << "Date.Value: " << toString() << std::endl;
    }
}

Date Date::deserialize(std::istream& is) {
    bool valid_status = Utils::readBoolValueAfterLabel(is, "Date.Valid");
    if (valid_status) {
        std::string dateStr = Utils::readStringValueAfterLabel(is, "Date.Value");
        return Date(dateStr);
    }
    return Date(); // Повертаємо невалідну дату
}