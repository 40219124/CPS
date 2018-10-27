#include "Verification.h"

using namespace std;

string Verification::_answers[5][20];
ofstream Verification::_testResults;

uint32_t Verification::_difficulty = -1;
uint32_t Verification::_blockIndex;

uint32_t Verification::_difficultyLimit;
uint32_t Verification::_blockIndexLimit;

Verification::Verification() {
}

Verification::~Verification() {
}

void Verification::SetLimitDetails(uint32_t diffLim, uint32_t blockLim) {
	_difficultyLimit = diffLim;
	_blockIndexLimit = blockLim;

	_testResults.open("test_results.csv", ios_base::out | ios_base::trunc);

	ifstream answerSheet("true_hashes.txt");
	for (uint32_t diffI = 0; diffI < _difficultyLimit; ++diffI) {
		for (uint32_t blockI = 0; blockI < _blockIndexLimit; ++blockI) {
			answerSheet >> _answers[diffI][blockI];
		}
	}
	answerSheet.close();
}

void Verification::SetBlockDetails(uint32_t difficulty, uint32_t blockIndex) {
	// If new difficulty, perform file formatting operations
	if (difficulty - 1 != _difficulty) {
		// Set difficulty
		_difficulty = difficulty - 1;
		// Space between result sets
		_testResults << "\n" << "\n" << endl;
		// Write difficulty
		_testResults << "Difficulty " << _difficulty + 1 << endl;
		// Write block headings
		for (uint32_t i = 1; i <= _blockIndexLimit; ++i) {
			_testResults << "Block " << i << ",";
		}
		// New line after headings
		_testResults << endl;
	}
	// Set block index
	_blockIndex = blockIndex - 1;
}

void Verification::VerifyHash(string hash) {
	// Get result of hash comparison
	bool result = (hash == _answers[_difficulty][_blockIndex]);
	// Print to file
	_testResults << result << ",";
	// If final block, make a new line
	if (_blockIndex + 1 == _blockIndexLimit) {
		_testResults << endl;
	}
}
