#pragma once
#include <vector>
#include <bitset>
struct CRCresult
{
    std::vector<int> data;
    std::vector<int> remainder;
    bool error = false;
};
class CRC 
{
public:
    std::vector<int> send(std::vector<int>& bits);
    CRCresult receive(std::vector<int>& bits);
    std::pair<std::vector<int>, int> introduceError(std::vector<int>& bits);
    std::vector<int> xorDivision(std::vector<int>& bits, bool sender);
    std::vector<std::string> getStepSender();
    std::vector<std::string> getStepReceiver();
    int getGeneratorSize();
    std::vector<int> getGenerator();

private:
    std::vector<int> generator = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1};
    //generator: P(x) = x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x^1 + 1
    std::vector<std::string> steps;
    std::vector<std::string> stepsSender;
    std::vector<std::string> stepsReceiver;
};