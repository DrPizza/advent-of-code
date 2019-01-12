#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <string>
#include <vector>

struct advent_2015_17 : problem
{
	advent_2015_17() noexcept : problem(2015, 17) {
	}

protected:
	std::vector<std::size_t> containers;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line); ) {
			containers.push_back(std::stoull(line));
		}
	}

	std::size_t total_ways = 0;
	std::size_t minimal_ways = 0;

	void precompute() override {
		const std::size_t number_of_bits = containers.size();
		std::size_t fewest_bits = number_of_bits + 1;

		for(std::size_t i = 0; (i >> number_of_bits) == 0; ++i) {
			std::size_t eggnog = 0;
			for(std::size_t j = 0; j < number_of_bits; ++j) {
				const std::size_t mask = 1ui64 << j;
				if(0 != (i & mask)) {
					eggnog += containers[j];
				}
			}
			if(eggnog == 150) {
				++total_ways;

				const DWORD64 bits_used = __popcnt64(i);
				if(bits_used == fewest_bits) {
					++minimal_ways;
				}
				else if(bits_used < fewest_bits) {
					fewest_bits = bits_used;
					minimal_ways = 1;
				}
			}
		}
	}

	std::string part_1() override {
		return std::to_string(total_ways);
	}

	std::string part_2() override {
		return std::to_string(minimal_ways);
	}

};

REGISTER_SOLVER(2015, 17);
