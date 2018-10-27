#pragma once
#include <string>
#include <fstream>

class Verification
{
public:
	static void SetLimitDetails(uint32_t diffLim, uint32_t blockLim);
	static void SetBlockDetails(uint32_t difficulty, uint32_t blockIndex);
	static void VerifyHash(std::string hash);

private:
	Verification();
	~Verification();

	static std::string _answers[5][20];
	static std::ofstream _testResults;

	static uint32_t _difficulty;
	static uint32_t _blockIndex;

	static uint32_t _difficultyLimit;
	static uint32_t _blockIndexLimit;
};

