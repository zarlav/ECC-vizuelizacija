#include "LRC.h"
#include <random>

std::vector<std::bitset<8>> LRC::send(std::vector<std::bitset<8>>& data, bool parnaParnost)
{
	std::vector<int> bitoviZaProveru;
	std::bitset<8> LRC;
	int brojRedova = data.size();
	int brojKolona = data[0].size();
	int posLRC = brojKolona * brojRedova;

	for (int i = 0; i < brojKolona; i++)
	{
		for (int j = 0, pos = 0; j < brojRedova; j++)
		{
			bitoviZaProveru.insert(bitoviZaProveru.begin(), data[j][i]);
		}
		if(parnaParnost)
			LRC[i] = calculateParity(bitoviZaProveru);
		else
			LRC[i] = calculateParity(bitoviZaProveru) ^ 1;
		bitoviZaProveru.clear();
	}
	data.push_back(LRC);
	return data;
}
std::pair<std::vector<std::bitset<8>>, bool> LRC::receive(std::vector<std::bitset<8>>& data, bool parnaParnost)
{
	std::vector<int> bitoviZaProveru;
	int brojRedova = data.size() - 1;
	int brojKolona = data[0].size();
	bool greska = false;
	int parityBit;
	for (int i = 0; i < brojKolona; i++)
	{
		for (int j = 0; j < brojRedova; j++)
		{
			bitoviZaProveru.insert(bitoviZaProveru.begin(), data[j][i]);
		}
		if (parnaParnost)
			parityBit = calculateParity(bitoviZaProveru);
		else
			parityBit = calculateParity(bitoviZaProveru) ^ 1;

		if (parityBit != data[brojRedova][i])
			greska = true;
		bitoviZaProveru.clear();
	}
	return { data, greska };
}
int LRC::calculateParity(std::vector<int>& data)
{
	int cnt = 0;
	int size = data.size();
	for (int i = 0; i < size; i++)
	{
		if (data[i] == 1)
			cnt++;
	}
	return (cnt % 2) == 0 ? 0 : 1;
}

std::pair<std::vector<std::bitset<8>>, int> LRC::introduceError(std::vector<std::bitset<8>>&data)
{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrib(0, data.size() - 2);
		int rPos = distrib(gen);
		data[rPos] ^= 1;
		return { data, rPos };
}