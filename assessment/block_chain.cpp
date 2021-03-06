#include "block_chain.h"
#include "sha256.h"
#include "Verification.h"

#include <iostream>
#include <sstream>
#include <chrono>
#include <fstream>
#include <queue>
#include <future>

using namespace std;
using namespace std::chrono;


// Note that _time would normally be set to the time of the block's creation.
// This is part of the audit a block chain.  To enable consistent results
// from parallelisation we will just use the index value, so time increments
// by one each time: 1, 2, 3, etc.
block::block(uint32_t index, const string &data)
	: _index(index), _data(data), _nonce(0), _time(static_cast<long>(index)) {}

string block::mine_block(uint32_t difficulty) noexcept {
	string str(difficulty, '0');

	auto start = system_clock::now();
	queue<future<string>> futures;
	uint32_t threadCount = thread::hardware_concurrency();
	bool notDone = true;
	while (notDone) {
		// Make up to the maximum futures (main inclusive)
		while (futures.size() < threadCount - (threadCount > 1 ? 1 : 0)) {
			futures.push(async(launch::async, &block::calculate_hash, this, ++_nonce));
		}
		while (notDone) {
			// Break if future queue has space and next entry isn't ready
			if (futures.size() < threadCount - 1) {
				if (futures.front().wait_for(chrono::seconds(0)) != future_status::ready) {
					break;
				}
			}
			// Get a future
			future<string> fu = move(futures.front());
			// Remove it from queue
			futures.pop();
			// Wait for future to be ready
			if (fu.wait_for(chrono::seconds(0)) != future_status::ready) {
				fu.wait();
			}
			// write the future return value to _hash
			_hash = move(fu.get());
			// if the right hash is found exit loops
			if (_hash.substr(0, difficulty) == str) {
				notDone = false;
			}
			// if no more futures left
			if (futures.size() == 0) {
				break;
			}
		}
	}

	// have 3 threads doing calculate_hash()
	// have main check them in order via queue
	// if criteria met continue


	auto end = system_clock::now();
	duration<double> diff = end - start;

	cout << "Block mined: " << _hash << " in " << diff.count() << " seconds" << endl;
	return to_string(diff.count());
}

std::string block::calculate_hash(const uint64_t nonce) const noexcept {
	stringstream ss;
	ss << _index << _time << _data << nonce << prev_hash;
	return sha256(ss.str());
}

block_chain::block_chain() {
	_chain.emplace_back(block(0, "Genesis Block"));
	_difficulty = 2;
}

string block_chain::add_block(block &&new_block) noexcept {
	new_block.prev_hash = get_last_block().get_hash();
	string time = new_block.mine_block(_difficulty);
	_chain.push_back(new_block);

	// Verify the found hash
	Verification::VerifyHash(new_block.get_hash());
	// Print out the new hash (primarily for finding initial hash values)
	static ofstream hashOut("test_hashes.txt", ios_base::out | ios_base::trunc);
	hashOut << new_block.get_hash() << endl;

	return time;
}