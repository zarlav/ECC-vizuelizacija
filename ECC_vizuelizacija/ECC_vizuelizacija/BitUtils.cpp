#include "BitUtils.h"

std::vector<int> BitUtils::StringToBits(const std::string& input)
{
    std::vector<int> bits;

    for (char c : input)
        bits.push_back(c - '0');

    return bits;
}

std::string BitUtils::BitsToString(const std::vector<int>& bits)
{
    std::string text;

    for (int bit : bits)
        text += std::to_string(bit);

    return text;
}