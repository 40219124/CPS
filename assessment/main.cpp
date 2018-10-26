#include <iostream>
#include <string>
#include "block_chain.h"
#include <fstream>

using namespace std;

int main()
{
	bool printTimes = true;
	ofstream outFile;
	if (printTimes) {
		outFile.open("cps_data.csv", ios_base::out | ios_base::trunc);
	}

	uint32_t blocklimit = 20u;
	uint32_t replimit = 25u;
	uint32_t difflimit = 5u;

	for (uint32_t diff = 0; diff <= difflimit; ++diff) {
		// Create the header for the tables, stating difficulty and block index
		if (printTimes) {
			outFile << "difficulty " << diff << endl;
			for (uint32_t i = 1; i < blocklimit; ++i) {
				outFile << "block " << i << ",";
			}
			outFile << endl;
		}
		for (uint32_t repeat = 0; repeat < replimit; ++repeat) {

			for (uint32_t blocks = 1; blocks <= blocklimit; ++blocks)
			{
				block_chain bchain;
				bchain.change_difficulty(diff);
				cout << "Mining block " << blocks << "..." << endl;
				// Print the time the block took, and seperate from the next with comma
				string timeTaken = bchain.add_block(block(blocks, string("Block ") + to_string(blocks) + string(" Data")));
				if (printTimes) {
					outFile << timeTaken << ",";
				}
			}
			// New line before doing blocks again
			if (printTimes) {
				outFile << endl;
			}
		}
		// Spacing line between difficulties
		if (printTimes) {
			outFile << endl;
		}
	}
	// New line at end of file
	if (printTimes) {
		outFile << endl;
	}


	return 0;
}