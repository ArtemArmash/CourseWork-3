#pragma once
#include <string>
#include <ctime>
#include <iostream> // Äëÿ ostream, istream

class Date {
private:
    std::tm date{};
    bool valid_ = false;

public:
    Date();
    explicit Date(const std::string& dateStr);

    bool isValid() const;
    std::string toString() const;
    std::tm getTimeStruct() const;

    bool operator<(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator>=(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;

    void serialize(std::ostream& os) const;
    static Date deserialize(std::istream& is);
};