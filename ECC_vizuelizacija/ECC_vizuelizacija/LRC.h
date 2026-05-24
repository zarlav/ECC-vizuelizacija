#pragma once
#include <vector>
#include <bitset>
class LRC
{
public:
	std::vector<std::bitset<8>> send(std::vector<std::bitset<8>>& data);
	std::pair<std::vector<std::bitset<8>>, bool> receive(std::vector<std::bitset<8>>& data);
	int calculateParity(std::vector<int>& data);
	std::pair<std::vector<std::bitset<8>>, int> introduceError(std::vector<std::bitset<8>>& data);
};