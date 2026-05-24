#pragma once
#include <vector>
class ParityBit
{
public:
	std::vector<int> send(std::vector<int>& data, bool parnaParnost);
	std::pair<std::vector<int>, bool> receive(const std::vector<int>& data);
	int calculateParity(const std::vector<int>& data);
	std::pair<std::vector<int>, int> introduceError(std::vector<int>& data);
};