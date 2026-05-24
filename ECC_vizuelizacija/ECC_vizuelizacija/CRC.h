#pragma once
#include <vector>
#include <bitset>
class CRC 
{
public:
    std::vector<int> send(std::vector<int>& bits);
    bool receive(std::vector<int>& bits);
    std::pair<std::vector<int>, int> introduceError(std::vector<int>& bits);

    std::vector<int> appendZeros(std::vector<int>& bits);
    std::vector<int> calculateCRC(std::vector<int>& bits);
    std::vector<int> xorDivision(std::vector<int>& bits);
    std::vector<int> getRemainder(std::vector<int>& bits);

    bool hasError(std::vector<int>& bits);

    void setGenerator(std::vector<int>& g);
};