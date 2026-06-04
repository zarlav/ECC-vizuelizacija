#include "Golay.h"
#include <bitset>
#include <string>
#include <algorithm>

Golay::Golay()
{
	this->polinomb = std::bitset<11>("11011100010");
}

std::bitset<24> Golay::odrediKodnuRec(std::bitset<12> data)
{
	KodiranjeSteps.clear();

	for (int j = 0; j < 24; j++)
	{
		bool xorRez = 0;
		std::string step = "v" + std::to_string(j) + " = ";

		bool first = true;

		for (int i = 0; i < 12; i++)
		{
			if (generatorskaMatrica[i][j] == 1)
			{
				xorRez ^= data[i];

				if (!first)
					step += " XOR ";

				step += "i" + std::to_string(i + 1);

				first = false;
			}
		}

		vector[j] = xorRez;
		KodiranjeSteps.push_back(step);
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
	SindromSteps.clear();

	for (int i = 0; i < 12; i++)
	{
		int bit = 0;
		std::string step = "s" + std::to_string(i) + " = ";

		bool first = true;

		for (int j = 0; j < 24; j++)
		{
			if (parityCheckMatrica[i][j] == 1)
			{
				bit ^= received[j];

				if (!first)
					step += " XOR ";

				step += "r" + std::to_string(j);

				first = false;
			}
		}

		sindrom[i] = bit;
		SindromSteps.push_back(step);
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
		drugiSindrom[i] = bit;
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
	std::string str;
	std::bitset<24> rez;
	DekodiranjeSteps.clear();
	if (odrediTezinuSindroma(syndrome) <= 3)
	{
		std::bitset<24> errorVector = 0;
		str.clear();
		for (int i = 0; i < 12; i++)
			errorVector[i] = syndrome[i];
		str = errorVector.to_string();
		DekodiranjeSteps.push_back(str);
		str = received.to_string();
		DekodiranjeSteps.push_back(str);
		rez = received ^ errorVector;
		DekodiranjeSteps.push_back("XOR");
		DekodiranjeSteps.push_back(rez.to_string());
		success = true;
		return rez;
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

std::bitset<24> Golay::VratiVector()
{
	return vector;
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

std::vector<std::string> Golay::VratiKodiranjeSteps()
{
	return KodiranjeSteps;
}

std::vector<std::string> Golay::VratiSindromSteps()
{
	return SindromSteps;
}

std::vector<std::string> Golay::VratiDekodiranjeSteps()
{
	return DekodiranjeSteps;
}

std::bitset<11> Golay::VratiPolinomB()
{
	return polinomb;
}

std::bitset<12> Golay::VratiPrviSindrom()
{
	return sindrom;
}

