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

	//std::string part_2() override {
	//	std::ptrdiff_t floor           = 0;
	//	std::size_t instruction_number = 0;
	//	for(const char ch : floors) {
	//		++instruction_number;
	//		if(ch == '(') {
	//			++floor;
	//		} else if(ch == ')') {
	//			--floor;
	//		}
	//		if(floor == -1) {
	//			break;
	//		}
	//	}
	//	return std::to_string(instruction_number);
	//}
};

template<>
void solve<advent_year::year_2018, advent_day::day_1>() {
	advent_2018_1 a;
	a.solve();
}
