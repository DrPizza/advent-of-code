#pragma once

#include "problem.hpp"

#include <fstream>

struct advent_1 : problem
{
	advent_1() noexcept : problem(1) {
	}

protected:
	std::string code;

	void prepare_input() {
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
				total += code[current] - '0';
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
