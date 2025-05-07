#pragma once
#include <string>
#include <iostream> // Для istream
#include <vector>   // Для readStringVector / writeStringVector

namespace Utils {
    // Читає рядок до першого символу новой строки або до кінця потоку
    // та видаляє частину рядка до першого входження ':', а потім пробіл (якщо є)
    // Повертає значення після мітки.
    std::string readStringValueAfterLabel(std::istream& is, const std::string& expectedLabelPrefix = "");
    int readIntValueAfterLabel(std::istream& is, const std::string& expectedLabelPrefix = "");
    double readDoubleValueAfterLabel(std::istream& is, const std::string& expectedLabelPrefix = "");
    bool readBoolValueAfterLabel(std::istream& is, const std::string& expectedLabelPrefix = "");

    // Для серіалізації/десеріалізації простих векторів рядків (якщо знадобиться)
    void writeStringVector(std::ostream& os, const std::vector<std::string>& vec, const std::string& listLabel);
    std::vector<std::string> readStringVector(std::istream& is, const std::string& listLabel);
    bool containsIgnoreCase(const std::string& text, const std::string& pattern);
}