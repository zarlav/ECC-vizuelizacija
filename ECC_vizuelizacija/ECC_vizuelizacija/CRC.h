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
    void receive(std::vector<int>& bits);
    std::pair<std::vector<int>, int> introduceError(std::vector<int>& bits);
    std::vector<int> xorDivision(std::vector<int>& bits);

private:
//    std::vector<int> generator = { 1,0,0,0,0,0,1,1,1 };	//generator: x^8+x^2+x+1
    std::vector<int> generator = { 1,1,0,1 };
};