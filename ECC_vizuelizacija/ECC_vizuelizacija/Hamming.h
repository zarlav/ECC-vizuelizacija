#pragma once
#include <vector>
#include <bitset>
#include <string>
class Haming
{
public:
	Haming();
	std::vector<int> send(std::vector<int>& data);
	int getParityBits(int n);
	int caluclateParityBit(std::vector<int>& data);
	std::pair<std::vector<int>, bool> receive(std::vector<int>& data);
	std::vector<int> checkSameBit(std::bitset<3> i, std::vector<int>& data, bool receive);
	std::pair<std::vector<int>, int> introduceError(std::vector<int>& data);
	std::vector<std::string> getSenderSteps();
	std::vector<std::string> getReceiverSteps();
private:
	std::pair<int, std::bitset<3>> decCode;
	bool detectedError = false;
	int parityBits = 0;

	std::vector<std::string> senderSteps;
	std::vector<std::string> receiverSteps;
};