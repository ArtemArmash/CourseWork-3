#pragma once
#include <string>
#include <iostream> // ��� istream
#include <vector>   // ��� readStringVector / writeStringVector

namespace Utils {
    // ���� ����� �� ������� ������� ����� ������ ��� �� ���� ������
    // �� ������� ������� ����� �� ������� ��������� ':', � ���� ����� (���� �)
    // ������� �������� ���� ����.
    std::string readStringValueAfterLabel(std::istream& is, const std::string& expectedLabelPrefix = "");
    int readIntValueAfterLabel(std::istream& is, const std::string& expectedLabelPrefix = "");
    double readDoubleValueAfterLabel(std::istream& is, const std::string& expectedLabelPrefix = "");
    bool readBoolValueAfterLabel(std::istream& is, const std::string& expectedLabelPrefix = "");

    // ��� ����������/������������ ������� ������� ����� (���� �����������)
    void writeStringVector(std::ostream& os, const std::vector<std::string>& vec, const std::string& listLabel);
    std::vector<std::string> readStringVector(std::istream& is, const std::string& listLabel);
    bool containsIgnoreCase(const std::string& text, const std::string& pattern);
}