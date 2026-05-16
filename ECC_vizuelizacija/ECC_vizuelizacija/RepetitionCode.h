#include <stdio.h>
#include <vector>
#pragma once
class RepetitionCode
{
public:
	std::vector<int> encode(const std::vector<int>& data, int rep);
	std::vector<int> decode(const std::vector<int>& data, int rep);
	int introduceError(std::vector<int>& data);
};