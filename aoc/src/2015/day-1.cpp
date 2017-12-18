#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2015_1 : problem
{
	advent_2015_1() noexcept : problem(2015, 1) {
	}

protected:
	std::string floors;

	void prepare_input() override {
		std::ifstream fin("input/2015/day-1.txt");
		std::getline(fin, floors);
	}

	std::string part_1() override {
		std::ptrdiff_t floor = 0;
		for(const char ch : floors) {
			if(ch == '(') {
				++floor;
			} else if(ch == ')') {
				--floor;
			}
		}
		return std::to_string(floor);
	}

	std::string part_2() override {
		std::ptrdiff_t floor = 0;
		std::size_t instruction_number = 0;
		for(const char ch : floors) {
			++instruction_number;
			if(ch == '(') {
				++floor;
			} else if(ch == ')') {
				--floor;
			}
			if(floor == -1) {
				break;
			}
		}
		return std::to_string(instruction_number);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_1>() {
	advent_2015_1 a;
	a.solve();
}
