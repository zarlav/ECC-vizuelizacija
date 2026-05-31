#include "Golay.h"
#include <bitset>

std::vector<std::bitset<1>> Golay::decode(std::bitset<12> data)
{
	std::bitset<1> xorRez = 0;
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			if (generatorskaMatrica[i][j] == 1)
			{
				codewords[i][j] == data[i];
				if (j > 0)
					xorRez[0] = xorRez[0] ^ codewords[i][j];
			}
			vector[i] = xorRez;
		}
	} 
	return vector;
}

void Golay::odrediCiklicnuMatricu(std::bitset<11> b1)
{
	std::bitset<11> temp = b1;
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			ciklicnaMatrica[i][j] = temp[j];
		}
		temp = rotate_left(temp, 1);
	}
}

void Golay::odrediPodMatricuB()
{
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			if (i < 11 && j < 11)
				podMatricaB[i][j] = ciklicnaMatrica[i][j];
			else if (i == 11 && j == 11)
				podMatricaB[i][j] = 0;
			else
				podMatricaB[i][j] = 1;
		}
	}
}

void Golay::odrediGeneratorskuMatricu()
{
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			generatorskaMatrica[i][j] = podMatricaB[i][j];
		}
	}
	for (int i = 12, j = 0 ; i < 24; i++, j++)
	{
		generatorskaMatrica[j][i] = 1;
	}
}

void Golay::odrediParityCheckMatricu()
{
	for (int i = 0; i < 12; i++)
	{
		for (int j = 12,r = 0; j < 24; j++, r++)
		{
			parityCheckMatrica[i][j] = podMatricaB[i][r];
		}
	}
	for (int i = 0; i < 12; i++)
	{
		parityCheckMatrica[i][i] = 1;
	}
}

std::bitset<11> Golay::rotate_left(const std::bitset<11>& b, int shift)
{
	shift = shift % 11;

	return (b << shift) | (b >> (11 - shift));
}
