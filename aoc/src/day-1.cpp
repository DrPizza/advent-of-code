#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_1 : problem
{
	advent_1() noexcept : problem(1) {
	}

protected:
	std::string code;

	void prepare_input() override {
		std::ifstream fin("day-1.txt");
		std::getline(fin, code);
	}

	size_t core(size_t offset) {
		size_t total          = 0;
		const std::size_t end = code.size();
		for(size_t current = 0, next = offset; current != end; ++current, ++next) {
			if(next == end) {
				next = 0;
			}
			if(code[current] == code[next]) {
				total += gsl::narrow<std::size_t>(code[current]) - gsl::narrow<std::size_t>('0');
			}
		}
		return total;
	}

	std::string part_1() override {
		return std::to_string(core(1));
	}

	std::string part_2() override {
		return std::to_string(core(code.size() / 2));
	}
};

template<>
void solve<advent_day::day_1>() {
	advent_1 a;
	a.solve();
}
