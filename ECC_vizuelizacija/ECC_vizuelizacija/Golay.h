#pragma once
#include <bitset>
#include <vector>
#include <vector>
class Golay
{
public:
	std::vector < std::bitset<1>> odrediKodnuRec(std::bitset<12> data);
	void odrediCiklicnuMatricu(std::bitset<11> b1);
	void odrediPodMatricuB();
	void odrediGeneratorskuMatricu();
	void odrediParityCheckMatricu();    // H
	std::bitset<11> rotate_left(const std::bitset<11>& b, int shift);

	std::bitset<12> izracunajSindrom(std::bitset<24> received);  //s = w * H^T
	int odrediTezinuSindroma(std::bitset<12> s);
	std::bitset<24> korak3(std::bitset<12> syndrome);
	std::bitset<12> izracunajDrugiSindrom(std::bitset<12> syndrome); 
	std::bitset<24> korak6(std::bitset<12> syndrome);
	std::bitset<24> decode(std::bitset<24> received, bool& success);

	std::vector<std::string> VratiCiklicnaMatricaSteps();
	std::vector<std::string> VratiPodMatricuSteps();
	std::vector<std::string> VratiGeneratorskuMatricuSteps();
	std::vector<std::string> VratiParityCheckMatricuSteps();
private:
	uint8_t ciklicnaMatrica[11][11] = { 0 };
	uint8_t podMatricaB[12][12] = { 0 };
	uint8_t generatorskaMatrica[12][24] = { 0 };
	uint8_t parityCheckMatrica[12][24] = { 0 };
	std::bitset<12> sindrom;
	std::vector<std::bitset<24>> codewords;
	std::vector<std::bitset<1>> vector;

	std::vector<std::string> CiklicnaMatricaSteps;
	std::vector<std::string> PodMatricaSteps;
	std::vector<std::string> GeneratorskaMatricaSteps;
	std::vector<std::string> ParityCheckMatricaSteps;
};