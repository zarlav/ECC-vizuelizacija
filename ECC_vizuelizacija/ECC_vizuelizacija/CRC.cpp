#include "CRC.h"
#include <random>
#include "BitUtils.h"

std::vector<int> CRC::send(std::vector<int>& data)
{
	std::vector<int> ostatak;
	int n = generator.size() - 1;
	for (int i = 0; i < n; i++)
		data.push_back(0);
	ostatak = xorDivision(data);
	data.erase(data.end() - ostatak.size(), data.end());
	data.insert(data.end(), ostatak.begin(), ostatak.end());
	return data;


}

std::vector<int> CRC::xorDivision(std::vector<int>& bits)
{
	std::vector<int> deljenik;
	std::vector<int> result;
	int pos = generator.size();
	deljenik.assign(bits.begin(), bits.begin() + generator.size());
	steps.clear();
	steps.push_back(BitUtils::BitsToString(bits));
	steps.push_back(BitUtils::BitsToString(generator));

	for (int i = 0; i < bits.size() - generator.size(); i++)
	{
		for (int j = 0; j < generator.size(); j++)
		{
			result.push_back(deljenik[j] ^ generator[j]); 
		}
		steps.push_back(BitUtils::BitsToString(result));
		if (result[1] == 0)  // rezultat pocinje sa 2 nule
		{
			for (int p = 0; p < 2; p++)
			{
				if (p == 1)
					steps.push_back(std::string(generator.size(), '0')); // nule
				result.erase(result.begin());
				steps.push_back(BitUtils::BitsToString(result));  // brisanje nule
				result.push_back(bits[pos]);
				steps.push_back(BitUtils::BitsToString(result)); // spustanje sledeceg bita iz data
				pos++;
			}
			i++;
		}
		else
		{
			result.erase(result.begin());
			result.push_back(bits[pos]);
			steps.push_back(BitUtils::BitsToString(result));
			steps.push_back(BitUtils::BitsToString(generator));
			pos++;
		}
		deljenik.assign(result.begin(), result.end());
		result.clear();
	}
	result = deljenik;
	result.erase(result.begin());
	steps.push_back(BitUtils::BitsToString(result));
	return result;
}
std::vector<std::string> CRC::getSteps()
{
	return steps;
}
int CRC::getGeneratorSize()
{
	return generator.size();
}
CRCresult CRC::receive(std::vector<int>& bits)
{
	CRCresult result;
	std::vector<int> ostatak = xorDivision(bits);
	bool sveNule = std::count(ostatak.begin(), ostatak.end(), 1) == 0;
	result.data = bits;
	result.remainder = ostatak;
	if (sveNule)
		result.error = false;
	else
		result.error = true;
	return result;
}
std::pair<std::vector<int>, int> CRC::introduceError(std::vector<int>& data)
{
	if (!data.empty())
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrib(1, data.size() - 1);
		int rPos = distrib(gen);
		data[rPos] ^= 1;
		return { data, rPos };
	}
}