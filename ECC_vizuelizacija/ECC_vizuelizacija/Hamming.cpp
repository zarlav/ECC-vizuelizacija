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
	int r = getParityBits(data.size()); // broj redudantnih bitova
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
		std::bitset<4> x(p);
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
	int r = 0;

	while ((1 << r) < (n + r + 1))
	{
		r++;
	}

	parityBits = r;
	return r;
}

std::vector<int> Haming::checkSameBit(std::bitset<4> x, std::vector<int>& data, bool receive)
{
	int size = data.size() - 1;
	int decimal = x.to_ulong();
	std::vector<int> bitsPositions;

	for (int i = 1; i <= size; i++)
	{
		if ((i & decimal) != 0)
		{
			if (receive && decimal == i)
				continue;
			else
				bitsPositions.push_back(data[i]);
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
	receiverSteps.clear();
	std::string decimalniBroj;
	std::string str;
	for (int i = 0, p = 1; i < parityBits; i++)
	{
		std::vector<int> csb = checkSameBit(p, data, false);

		int parity = caluclateParityBit(csb) % 2;

		decimalniBroj += char('0' + parity);

		if (parity == 1)
			detectedError = true;

		if (detectedError)
			receiverSteps.push_back(
				"R" + std::to_string(p) + " parity = " + std::to_string(parity));
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
		std::reverse(decimalniBroj.begin(), decimalniBroj.end());
		int pozicijaGreske = std::stoi(decimalniBroj, nullptr, 2);


		str.clear();
		str = decimalniBroj + "(binarno)  :" + std::to_string(pozicijaGreske);
		receiverSteps.push_back(str);
		data[pozicijaGreske] ^= 1;
		str = "Ispravljeni podatak: ";
		receiverSteps.push_back(str);
		str.clear();
		for (int i = 1; i < data.size(); i++)
		{
			str += std::to_string(data[i]);
		}
		receiverSteps.push_back(str);
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
	str = "Originalni podatak: ";
	receiverSteps.push_back(str);
	str.clear();
	for (int i = 1; i < data.size(); i++)
	{
		str += std::to_string(data[i]);
	}
	receiverSteps.push_back(str);
	if (detectedError)
		return { data, true };
	else
		return { data, false };
}

std::pair<std::vector<int>, int> Haming::introduceError(std::vector<int>& data)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, data.size() - 1);
	int rPos = distrib(gen);
	data[rPos] ^= 1;
	return { data, rPos };
}

std::vector<std::string> Haming::getSenderSteps()
{
	return senderSteps;
}

std::vector<std::string> Haming::getReceiverSteps()
{
	return receiverSteps;
}
