#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>

struct advent_2018_1 : problem
{
	advent_2018_1() noexcept : problem(2018, 1) {
	}

protected:
	std::vector<std::ptrdiff_t> changes;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line);) {
			changes.push_back(std::stoll(line));
		}
	}

	std::string part_1() override {
		std::ptrdiff_t frequency = 0;
		for(const std::ptrdiff_t delta : changes) {
			frequency += delta;
		}
		return std::to_string(frequency);
	}

	std::string part_2() override {
		std::ptrdiff_t frequency = 0;
		std::unordered_set<std::ptrdiff_t> previous_frequencies;
		for(;;) {
			for(const std::ptrdiff_t delta : changes) {
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

template<>
void solve<advent_year::year_2018, advent_day::day_1>() {
	advent_2018_1 a;
	a.solve();
}
