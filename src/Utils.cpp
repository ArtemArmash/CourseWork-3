#include "../include/Utils.h" // Адаптуй шлях
#include <sstream>
#include <limits>   // std::numeric_limits
#include <stdexcept> // std::invalid_argument, std::out_of_range
#include <algorithm> // Для std::search
#include <cctype>

std::string Utils::readStringValueAfterLabel(std::istream& is, const std::string& expectedLabelPrefix) {
    std::string line;
    if (!std::getline(is, line)) {
        // std::cerr << "Utils Error: Failed to read line for label '" << expectedLabelPrefix << "'." << std::endl;
        return ""; // Не вдалося прочитати рядок
    }

    size_t colonPos = line.find(':');
    if (colonPos == std::string::npos) {
        // std::cerr << "Utils Error: Colon not found for label '" << expectedLabelPrefix << "'. Line: " << line << std::endl;
        // Якщо мітка не обов'язкова, а ми просто читаємо значення, то можемо повернути весь рядок
        // Але якщо ми очікуємо мітку, це помилка.
        // Для універсальності, припустимо, що мітка може бути відсутня, і ми читаємо рядок "як є", якщо мітки немає
        // Проте, якщо expectedLabelPrefix не порожній, це свідчить про помилку формату.
        if (!expectedLabelPrefix.empty()) {
            std::cerr << "Utils Warning: Expected label prefix '" << expectedLabelPrefix << "' but no colon found. Line: '" << line << "'" << std::endl;
        }
        // Повертаємо весь рядок, якщо немає двокрапки (або очищений від пробілів)
        size_t first_char = line.find_first_not_of(" \t\n\r\f\v");
        if (std::string::npos == first_char) return "";
        size_t last_char = line.find_last_not_of(" \t\n\r\f\v");
        return line.substr(first_char, (last_char - first_char + 1));

    }

    std::string labelFromFile = line.substr(0, colonPos);
    if (!expectedLabelPrefix.empty() && labelFromFile.rfind(expectedLabelPrefix, 0) != 0) { // rfind(prefix, 0) == 0 means starts_with
        std::cerr << "Utils Warning: Label mismatch. Expected prefix '" << expectedLabelPrefix
            << "', got '" << labelFromFile << "'. Line: " << line << std::endl;
    }

    size_t valueStart = colonPos + 1;
    // Пропускаємо пробіли після двокрапки
    while (valueStart < line.length() && std::isspace(static_cast<unsigned char>(line[valueStart]))) {
        valueStart++;
    }
    return line.substr(valueStart);
}

int Utils::readIntValueAfterLabel(std::istream& is, const std::string& expectedLabelPrefix) {
    std::string str_val = readStringValueAfterLabel(is, expectedLabelPrefix);
    try {
        return std::stoi(str_val);
    }
    catch (const std::invalid_argument& ia) {
        std::cerr << "Utils Error: Invalid argument for stoi with label '" << expectedLabelPrefix << "'. Value: '" << str_val << "'. " << ia.what() << std::endl;
    }
    catch (const std::out_of_range& oor) {
        std::cerr << "Utils Error: Out of range for stoi with label '" << expectedLabelPrefix << "'. Value: '" << str_val << "'. " << oor.what() << std::endl;
    }
    return 0; // Значення за замовчуванням у разі помилки
}

double Utils::readDoubleValueAfterLabel(std::istream& is, const std::string& expectedLabelPrefix) {
    std::string str_val = readStringValueAfterLabel(is, expectedLabelPrefix);
    try {
        return std::stod(str_val);
    }
    catch (const std::invalid_argument& ia) {
        std::cerr << "Utils Error: Invalid argument for stod with label '" << expectedLabelPrefix << "'. Value: '" << str_val << "'. " << ia.what() << std::endl;
    }
    catch (const std::out_of_range& oor) {
        std::cerr << "Utils Error: Out of range for stod with label '" << expectedLabelPrefix << "'. Value: '" << str_val << "'. " << oor.what() << std::endl;
    }
    return 0.0; // Значення за замовчуванням у разі помилки
}

bool Utils::readBoolValueAfterLabel(std::istream& is, const std::string& expectedLabelPrefix) {
    std::string str_val = readStringValueAfterLabel(is, expectedLabelPrefix);
    if (str_val == "1" || str_val == "true" || str_val == "True") return true;
    if (str_val == "0" || str_val == "false" || str_val == "False") return false;

    std::cerr << "Utils Warning: Could not parse bool for label '" << expectedLabelPrefix << "'. Value: '" << str_val << "'. Defaulting to false." << std::endl;
    return false; // Значення за замовчуванням
}

void Utils::writeStringVector(std::ostream& os, const std::vector<std::string>& vec, const std::string& listLabel) {
    os << listLabel << "_Count: " << vec.size() << std::endl;
    for (size_t i = 0; i < vec.size(); ++i) {
        os << listLabel << "[" << i << "]: " << vec[i] << std::endl;
    }
}

std::vector<std::string> Utils::readStringVector(std::istream& is, const std::string& listLabel) {
    std::vector<std::string> vec;
    size_t count = Utils::readIntValueAfterLabel(is, listLabel + "_Count");
    vec.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        vec.push_back(Utils::readStringValueAfterLabel(is, listLabel + "[" + std::to_string(i) + "]"));
    }
    return vec;
}
bool Utils::containsIgnoreCase(const std::string& text, const std::string& pattern) {
    if (pattern.empty()) return true; // Порожній шаблон завжди "міститься"
    if (text.empty()) return false;  // Якщо текст порожній, а шаблон ні, то не міститься

    // Використовуємо std::search з кастомним предикатом для порівняння символів без урахування регістру
    auto it = std::search(
        text.begin(), text.end(),
        pattern.begin(), pattern.end(),
        [](unsigned char ch1, unsigned char ch2) {
            return std::tolower(ch1) == std::tolower(ch2);
        }
    );
    return (it != text.end()); // Якщо ітератор не вказує на кінець тексту, значить підрядок знайдено
}