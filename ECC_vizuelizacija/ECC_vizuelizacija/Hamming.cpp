#include "Hamming.h"
#include <vector>
#include <bitset>
#include <random>
Haming::Haming()
{

}
std::vector<int> Haming::send(std::vector<int>& data)
{
	std::string str;
	int r = getParityBits(data.size() - 1); // broj redudantnih bitova
	senderSteps.clear();
	str = "Broj redudantnih bitova je: " + std::to_string(r);
	senderSteps.push_back(str);
	data.insert(data.begin(), 0); // ovde unacujem lazni prvi bit 0, da bi hamingovi krenuli od pozicije 1
	for (int i = 0, p = 1; i < r ; i++)
	{
		data.insert(data.begin() + p, -1);  // pocetne vrednosti za redudantne bitove
		if (p == 1)
			p++;
		else
			p *= 2;
	}
	str.clear();
	for (int i = 0, p=1; i < r; i++)   // dodajemo redudantne bitove 
	{
		std::bitset<3> x(p);
		std::vector<int> csb = checkSameBit(x , data, false);
		int oneConter = caluclateParityBit(csb);
		if (oneConter == 0)
			data[p] = 0;
		else
			data[p] = 1;
		str = "Redudantni bit na poziciji " + std::to_string(p) + " je dobio vrednost: " + std::to_string(oneConter);
		senderSteps.push_back(str);
		if (p == 1)
			p++;
		else
			p *= 2;
	}
	str.clear();
	std::vector<int> result(data.begin() + 1, data.end());
	for (int x : result)
	{
		str += std::to_string(x);
	}
	senderSteps.push_back(str);
	return data;
}

int Haming::getParityBits(int n)
{
	int r = 2;
	while (r + n + 1 > pow(2, r))
	{
		r++;
	}
	parityBits = r;
	return parityBits;
}

std::vector<int> Haming::checkSameBit(std::bitset<3> x, std::vector<int>& data, bool receive)
{
	int pos = 0;
	int size = data.size() -1 ;
	int decimal = x.to_ulong();
	std::vector<int> bitsPositions;
	for (int i = 0; i < 3; i++)
	{
		if (x[i] == 1)
		{
			pos = i;
			break;
		}
	}


	for (int i = 1; i <= size; i++)
	{
		decCode.first = i;
		decCode.second = std::bitset<3>(i);
		std::bitset<3> bitWord = decCode.second;
		for (int j = 0; j < 3; j++)
		{
			if (bitWord[j] == 1 && pos == j)
			{
				if (receive && decimal == i)
					continue;
				else	
					bitsPositions.insert(bitsPositions.begin(), data[i]);
				break;
			}
		}
	}
	return bitsPositions;
}

int Haming::caluclateParityBit(std::vector<int>& data)
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

std::pair<std::vector<int>, bool > Haming::receive(std::vector<int>& data)
{
	std::vector<int> binaryNumber;
	//
	//
	//introduceError(data);
	//
	receiverSteps.clear();
	std::string str;
	for (int i = 0, p = 1; i < parityBits; i++)   // proveravamo redudantne bitove
	{
		std::bitset<3> x(p);
		std::vector<int> csb = checkSameBit(x, data, true);
		int oneCounter = caluclateParityBit(csb);
		str = "Za bit na poziciji " + std::to_string(p) + " se proverava da li je bit: "+ std::to_string(oneCounter);
		receiverSteps.push_back(str);
		if (oneCounter == 0 && data[p] != 0)
			detectedError = true;
		else if (oneCounter == 1 && data[p] != 1)
			detectedError = true;
		if (detectedError)
		{
			binaryNumber.insert(binaryNumber.begin(), oneCounter);
		}
		if (p == 1)
			p++;
		else
			p *= 2;
	}

	str.clear();

	for (int i = 1; i < data.size(); i++)
	{
		str += std::to_string(data[i]);
	}
	receiverSteps.push_back(str);

	if (detectedError) // ispravljanje greske
	{
		str = "Detektovana je greska!";
		receiverSteps.push_back(str);
		int decimalNumber = 0;
		int size = binaryNumber.size();
		str.clear();
		for (int x : binaryNumber)
			str += binaryNumber[x];
		receiverSteps.push_back(str);
		for (int i = 0, j = size-1 ; i < size ; i++, j--)
		{
			decimalNumber += binaryNumber[i] * pow(2, j);
		}
		str += "  :" + decimalNumber;
		receiverSteps.push_back(str);
		data[decimalNumber] ^= 1;
	}

	for (int i = 0, p= 1; i < parityBits; i++) // izbacivanje redudantnih 
	{
		data[p] = -1;
		if (p == 1)
			p++;
		else
			p *= 2;
	}

	for (int i = 1; i < data.size(); i++)
	{
		if (data[i] == -1)
		{
			data.erase(data.begin() + i);
			i--;
		}
	}
	if (detectedError)
		return { data, true };
	else
		return { data, false };
}

std::pair<std::vector<int>, int> Haming::introduceError(std::vector<int>& data)
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

std::vector<std::string> Haming::getSenderSteps()
{
	return senderSteps;
}

std::vector<std::string> Haming::getReceiverSteps()
{
	return receiverSteps;
}
