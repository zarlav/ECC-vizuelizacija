#include "CRC.h"
#include <random>
#include "BitUtils.h"

std::vector<int> CRC::send(std::vector<int>& data)
{
	std::vector<int> ostatak;
	int n = generator.size() - 1;
	for (int i = 0; i < n; i++)
		data.push_back(0);
	ostatak = xorDivision(data, true);
	data.erase(data.end() - ostatak.size(), data.end());
	data.insert(data.end(), ostatak.begin(), ostatak.end());
	return data;


}

std::vector<int> CRC::xorDivision(std::vector<int>& bits, bool sender)
{
    std::vector<int> work = bits;
    steps.clear();

    for (size_t i = 0; i <= work.size() - generator.size(); i++)
    {
        if (work[i] == 1)
        {
            std::vector<int> currentDividend(
                work.begin() + i,
                work.end());

            std::string dividendStr =
                BitUtils::BitsToString(currentDividend);

            std::string generatorStr =
                BitUtils::BitsToString(generator);

            steps.push_back(dividendStr);
            steps.push_back(generatorStr);

            steps.push_back(
                std::string(generatorStr.size(), '-')
            );

            for (size_t j = 0; j < generator.size(); j++)
                work[i + j] ^= generator[j];

            std::vector<int> currentResult(
                work.begin() + i,
                work.end());

            steps.push_back(
                BitUtils::BitsToString(currentResult)
            );

            steps.push_back("");
        }
    }

    std::vector<int> remainder(
        work.end() - (generator.size() - 1),
        work.end());

    steps.push_back("CRC:");
    //std::reverse(remainder.begin(), remainder.end());
    steps.push_back(
        BitUtils::BitsToString(remainder)
    );

    if (sender)
        stepsSender = steps;
    else
        stepsReceiver = steps;

    return remainder;
}
std::vector<std::string> CRC::getStepSender()
{
	return stepsSender;
}
std::vector<std::string> CRC::getStepReceiver()
{
	return stepsReceiver;
}
int CRC::getGeneratorSize()
{
	return generator.size();
}
std::vector<int> CRC::getGenerator()
{
	return generator;
}
CRCresult CRC::receive(std::vector<int>& bits)
{
	CRCresult result;
	std::vector<int> ostatak = xorDivision(bits, false);
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
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, data.size() - 1);
	int rPos = distrib(gen);
	data[rPos] ^= 1;
	return { data, rPos };
}