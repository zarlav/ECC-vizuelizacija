#include <vector>
#include "ParityBit.h"
#include <time.h>
#include <random>
#include "Hamming.h"
std::vector<int> ParityBit::send(std::vector<int>& data, bool parnaParnost)
{
	int parityBit = calculateParity(data);
	if (!parnaParnost)
		parityBit ^= 1;
	data.push_back(parityBit);
	return data;
}

int ParityBit::calculateParity(const std::vector<int>& data)
{
	int cnt = 0;
	int size = data.size();
	for (int i = 0; i < size; i++)
	{
		if (data[i] == 1)
			cnt++;
	}
	return (cnt % 2)==0 ? 0 : 1;
}

std::pair<std::vector<int>, bool> ParityBit::receive(const std::vector<int>& data)
{
	int parityBit = calculateParity(data);
	int lastBit = data.back();
	if (lastBit == parityBit)
		return { data,false };
	else
		return { data, true };
}



std::vector<int> ParityBit::introduceError(std::vector<int>& data)
{
	if (!data.empty())
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrib(0, data.size() - 2);
		int rPos = distrib(gen);
		data[rPos] ^= 1;
		return data;
	}
}