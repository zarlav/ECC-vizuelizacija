#include "CRC.h"

std::vector<int> CRC::send(std::vector<int>& data)
{
	std::vector<int> ostatak;
	int n = generator.size() - 1;
	for (int i = 0; i < n; i++)
		data.push_back(0);
	ostatak = xorDivision(data);
	data.insert(data.end(), ostatak.begin(), ostatak.end());
	return data;


}

std::vector<int> CRC::xorDivision(std::vector<int>& bits)
{
	std::vector<int> deljenik;
	std::vector<int> result;
	int pos = generator.size();
	deljenik.assign(bits.begin(), bits.begin() + generator.size());

	for (int i = 0; i < bits.size() - generator.size(); i++)
	{
		for (int j = 0; j < generator.size(); j++)
		{
			result.push_back(deljenik[j] ^ generator[j]);
		}
		if (result[1] == 0)  // rezultat pocinje sa 2 nule
		{
			for (int p = 0; p < 2; p++)
			{
				result.erase(result.begin());
				result.push_back(bits[pos]);
				pos++;
			}
			i++;
		}
		else
		{
			result.erase(result.begin());
			result.push_back(bits[pos]);
			pos++;
		}
		deljenik.assign(result.begin(), result.end());
		result.clear();
	}
	result = deljenik;
	result.erase(result.begin());
	return result;
}
void CRC::receive(std::vector<int>& bits)
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
}
