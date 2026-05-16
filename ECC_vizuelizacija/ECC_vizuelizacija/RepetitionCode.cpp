#include "RepetitionCode.h"
#include <random>

std::vector<int> RepetitionCode::encode(const std::vector<int>& data, int rep)
{
	int dataSize = data.size();
	std::vector<int> encoded;
	encoded.reserve(dataSize * rep);

	for (int i = 0; i < dataSize; i++)
	{
		for (int r = 0; r < rep; r++)
		{
			encoded.push_back(data[i]);
		}
	}

	return encoded;
}

std::vector<int> RepetitionCode::decode(const std::vector<int>& data, int rep)
{
	std::vector<int> decoded;
	int dataSize = data.size();
	for (int i = 0; i < dataSize; i+=rep)
	{
		int zeroCnt = 0;
		int oneCnt = 0;
		for (int j = i; j < i + rep; j++)
		{
			if (data[j] == 1)
				oneCnt++;
			else
				zeroCnt++;
		}
		decoded.push_back(oneCnt > zeroCnt ? 1 : 0);
	}
	return decoded;
}


int RepetitionCode::introduceError(std::vector<int>& data)
{
	if (!data.empty())
	{
		srand(time(0));
		int rPos = rand() % data.size();
		data[rPos] ^= 1;
		return rPos;
	}
}
