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
    std::vector<int> xorDivision(std::vector<int>& bits);
    std::vector<std::string> getSteps();
    int getGeneratorSize();

private:
//    std::vector<int> generator = {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1};	//generator: 
    std::vector<int> generator = { 1,1,0,1,1 };
    std::vector<std::string> steps;
};