#pragma once
#include <vector>
#include <bitset>
struct CRCresult
{
    std::vector<int> data;
    std::vector<int> remainder;
    bool error;
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
    std::vector<int> generator = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1};	//generator: 
    //std::vector<int> generator = { 1,1,0,1,1 };
    std::vector<std::string> steps;
    std::vector<std::string> stepsSender;
    std::vector<std::string> stepsReceiver;
};