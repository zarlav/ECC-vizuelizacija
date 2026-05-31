#pragma once
#include <bitset>
#include <vector>
class Golay
{
public:
	uint8_t ciklicnaMatrica[11][11] = { 0 };
	uint8_t podMatricaB[12][12] = { 0 };
	uint8_t generatorskaMatrica[12][24] = { 0 };
	uint8_t parityCheckMatrica[12][24] = { 0 };
	std::vector<std::bitset<24>> codewords;
	std::vector<std::bitset<1>> vector;

	std::vector < std::bitset<1>> decode(std::bitset<12> data);
	void odrediCiklicnuMatricu(std::bitset<11> b1);
	void odrediPodMatricuB();
	void odrediGeneratorskuMatricu();
	void odrediParityCheckMatricu();
	std::bitset<11> rotate_left(const std::bitset<11>& b, int shift);
private:

};