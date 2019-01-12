#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2017_1 : problem
{
	advent_2017_1() noexcept : problem(2017, 1) {
	}

protected:
	std::string code;

	void prepare_input(std::ifstream& fin) override {
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

REGISTER_SOLVER(2017, 1);
