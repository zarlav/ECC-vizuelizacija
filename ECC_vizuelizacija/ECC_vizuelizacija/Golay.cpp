#include "Golay.h"
#include <bitset>
#include <string>
#include <algorithm>

std::vector<std::bitset<1>> Golay::odrediKodnuRec(std::bitset<12> data)
{
	std::bitset<1> xorRez = 0;
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			if (generatorskaMatrica[i][j] == 1)
			{
				codewords[i][j] = data[i];
				if (j > 0)
					xorRez[0] = xorRez[0] ^ codewords[i][j];
			}
			vector[i] = xorRez;
			xorRez = 0;
		}
	} 
	return vector;
}

void Golay::odrediCiklicnuMatricu(std::bitset<11> b1)
{
	std::bitset<11> temp;
	CiklicnaMatricaSteps.clear();
	for (size_t i = 0; i < temp.size(); ++i)
	{
		temp[i] = b1[temp.size() - 1 - i]; // reverse od b1
	}
	std::string str;
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			ciklicnaMatrica[i][j] = temp[j];
			str += std::to_string(ciklicnaMatrica[i][j]);
		}
		CiklicnaMatricaSteps.push_back(str.c_str());
		str.clear();
		temp = rotate_left(temp, 1);
	}
}

void Golay::odrediPodMatricuB()
{
	std::string str;
	PodMatricaSteps.clear();
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

			str += std::to_string(podMatricaB[i][j]);
		}
		//std::reverse(str.begin(), str.end());
		PodMatricaSteps.push_back(str.c_str());
		str.clear();
	}
}

void Golay::odrediGeneratorskuMatricu()
{
	std::string str;
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
	GeneratorskaMatricaSteps.clear();

	for (int i = 0; i < 12; i++)
	{
		std::string str;

		for (int j = 0; j < 24; j++)
		{
			str += std::to_string(generatorskaMatrica[i][j]);
		}

		GeneratorskaMatricaSteps.push_back(str);
	}
}

void Golay::odrediParityCheckMatricu()
{
	std::string str;
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

	ParityCheckMatricaSteps.clear();
	for (int i = 0; i < 12; i++)
	{
		std::string str;

		for (int j = 0; j < 24; j++)
		{
			str += std::to_string(parityCheckMatrica[i][j]);
		}

		ParityCheckMatricaSteps.push_back(str);
	}
}

std::bitset<11> Golay::rotate_left(const std::bitset<11>& b, int shift)
{
	shift = shift % 11;

	return (b >> shift) | (b << (11 - shift));
}

std::bitset<12> Golay::izracunajSindrom(std::bitset<24> received)
{
	for (int i = 0; i < 12; i++)
	{
		int bit = 0;
		for (int j = 0; j < 24; j++)
		{
			bit ^= (received[j] & parityCheckMatrica[i][j]);
		}
		sindrom[i] = bit;
	}
	return sindrom;
}

int Golay::odrediTezinuSindroma(std::bitset<12> s)
{
	return s.count();
}

std::bitset<24> Golay::korak3(std::bitset<12> syndrome)  // if wt(s+bi) <=2 -> u[s+bi,ei]
{
	std::bitset<24> errorVector;

	for (int i = 0; i < 12; i++)
	{
		std::bitset<12> redB;

		for (int j = 0; j < 12; j++)
			redB[j] = podMatricaB[i][j];

		std::bitset<12> temp = syndrome ^ redB;

		if (temp.count() <= 2)
		{
			// leva polovina = syndrome + b_i
			for (int j = 0; j < 12; j++)
				errorVector[j] = temp[j];

			// desna polovina = e_i
			errorVector[12 + i] = 1;
			return errorVector;
		}
	}
	// nije pronadjeno resenje
	return std::bitset<24>();
}

std::bitset<12> Golay::izracunajDrugiSindrom(std::bitset<12> syndrome)
{
	std::bitset<12> drugiSindrom = 0;
	for (int i = 0; i < 12; i++)
	{
		int bit = 0;
		for (int j = 0; j < 12; j++)
		{
			bit ^= (syndrome[j] & podMatricaB[i][j]);
		}
	}
	return drugiSindrom;
}

std::bitset<24> Golay::korak6(std::bitset<12> drugiSindrom)
{
	std::bitset<24> errorVector;

	for (int i = 0; i < 12; i++)
	{
		std::bitset<12> redB;

		for (int j = 0; j < 12; j++)
			redB[j] = podMatricaB[i][j];

		std::bitset<12> temp = drugiSindrom ^ redB;

		if (temp.count() <= 2)
		{
			for (int j = 12; j < 24; j++)
				errorVector[j] = temp[j];

			// leva polovina u[ei, sB+bi]
			errorVector[0 + i] = 1;
			return errorVector;
		}
	}
	return std::bitset<24>();
}

std::bitset<24> Golay::decode(std::bitset<24> received, bool& success)
{
	success = false;

	std::bitset<12> syndrome = izracunajSindrom(received);

	if (odrediTezinuSindroma(syndrome) <= 3)
	{
		std::bitset<24> errorVector = 0;

		for (int i = 0; i < 12; i++)
			errorVector[i] = syndrome[i];

		success = true;
		return received ^ errorVector;
	}

	std::bitset<24> errorVector = korak3(syndrome);

	if (!errorVector.any())
	{
		std::bitset<12> drugiSindrom = izracunajDrugiSindrom(syndrome);

		if (odrediTezinuSindroma(drugiSindrom) <= 3)
		{
			std::bitset<24> errorVector2 = 0;

			for (int i = 0; i < 12; i++)
				errorVector2[i] = drugiSindrom[i];

			success = true;
			return received ^ errorVector2;
		}

		errorVector = korak6(drugiSindrom);
	}

	if (!errorVector.any())
	{
		success = false;
		return std::bitset<24>();
	}

	std::bitset<24> corrected = received ^ errorVector;

	success = true;
	return corrected;
}

std::vector<std::string> Golay::VratiCiklicnaMatricaSteps()
{
	return CiklicnaMatricaSteps;
}

std::vector<std::string> Golay::VratiPodMatricuSteps()
{
	return PodMatricaSteps;
}

std::vector<std::string> Golay::VratiGeneratorskuMatricuSteps()
{
	return GeneratorskaMatricaSteps;
}

std::vector<std::string> Golay::VratiParityCheckMatricuSteps()
{
	return ParityCheckMatricaSteps;
}

