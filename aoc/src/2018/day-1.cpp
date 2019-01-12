#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <cstddef>

struct advent_2018_1 : problem
{
	advent_2018_1() noexcept : problem(2018, 1) {
	}

protected:
	std::vector<std::intmax_t> changes;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line);) {
			changes.push_back(std::stoll(line));
		}
	}

	std::string part_1() override {
		std::intmax_t frequency = 0;
		for(const std::intmax_t delta : changes) {
			frequency += delta;
		}
		return std::to_string(frequency);
	}

	std::string part_2() override {
		std::intmax_t frequency = 0;
		std::unordered_set<std::intmax_t> previous_frequencies;
		for(;;) {
			for(const std::intmax_t delta : changes) {
				frequency += delta;
				if(previous_frequencies.find(frequency) != previous_frequencies.end()) {
					return std::to_string(frequency);
				} else {
					previous_frequencies.insert(frequency);
				}
			}
		}
		__assume(0);
	}
};

REGISTER_SOLVER(2018, 1);
