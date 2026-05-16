#include "LRC.h"
#include <random>

std::vector<std::bitset<8>> LRC::send(std::vector<std::bitset<8>>& data)
{
	std::vector<int> bitoviZaProveru;
	std::bitset<8> LRC;
	int brojRedova = data.size();
	int brojKolona = data[0].size();
	int posLRC = brojKolona * brojRedova;

	for (int i = 0,p= brojKolona-1; i < brojKolona; i++)
	{
		for (int j = 0, pos = 0; j < brojRedova; j++)
		{
			bitoviZaProveru.insert(bitoviZaProveru.begin(), data[j][i]);
		}
		LRC[p--] = calculateParity(bitoviZaProveru);
		bitoviZaProveru.clear();
	}
	data.push_back(LRC);
	return data;
}
std::pair<std::vector<std::bitset<8>>, bool> LRC::receive(std::vector<std::bitset<8>>& data)
{
	std::vector<int> bitoviZaProveru;
	int brojRedova = data.size() - 1;
	int brojKolona = data[0].size();
	bool greska = false;
	for (int i = 0; i < brojKolona; i++)
	{
		for (int j = 0; j < brojRedova; j++)
		{
			bitoviZaProveru.insert(bitoviZaProveru.begin(), data[i][j]);
		}
		if (calculateParity(bitoviZaProveru) != data[i][brojRedova])
			greska = true;
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

std::vector<std::bitset<8>> LRC::introduceError(std::vector<std::bitset<8>>& data)
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