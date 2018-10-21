#include <iostream>
#include <string>
#include "block_chain.h"
#include <fstream>

using namespace std;

int main()
{
	block_chain bchain;
	ofstream outFile("cps_data.csv", ios_base::out | ios_base::trunc);

	uint32_t blocklimit = 20u;
	uint32_t replimit = 25u;
	uint32_t difflimit = 5u;

	for (uint32_t diff = 0; diff <= difflimit; ++diff) {
		bchain.change_difficulty(diff);
		outFile << "difficulty " << diff << endl;
		for (uint32_t i = 1; i < blocklimit; ++i)
		{
			outFile << "block " << i << ",";
		}
		outFile << endl;
		for (uint32_t repeat = 0; repeat < replimit; ++repeat) {
			for (uint32_t blocks = 1; blocks < blocklimit; ++blocks)
			{
				cout << "Mining block " << blocks << "..." << endl;
				outFile << bchain.add_block(block(blocks, string("Block ") + to_string(blocks) + string(" Data"))) << ",";
			}
			outFile << endl;
		}
		outFile << endl;
	}
	outFile << endl;
	return 0;
}