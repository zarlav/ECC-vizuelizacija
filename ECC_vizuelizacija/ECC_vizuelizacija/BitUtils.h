#pragma once
#include <vector>
#include <string>

namespace BitUtils
{
    std::vector<int> StringToBits(const std::string& input);
    std::string BitsToString(const std::vector<int>& bits);
}