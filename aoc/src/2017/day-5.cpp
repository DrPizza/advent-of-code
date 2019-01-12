#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>

struct advent_2017_5 : problem
{
	advent_2017_5() noexcept : problem(2017, 5) {
	}

protected:
	std::vector<std::ptrdiff_t> instructions;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line);) {
			instructions.push_back(std::stoll(line));
		}
	}

	std::string part_1() override {
		std::vector<std::ptrdiff_t> my_instructions(instructions);
		std::size_t instruction_count = 0;
		for(auto it = std::begin(my_instructions); it != std::end(my_instructions);) {
			const std::ptrdiff_t jump = *it;
			*it += 1;
			it += jump;
			++instruction_count;
		}
		return std::to_string(instruction_count);
	}

	std::string part_2() override {
		std::vector<std::ptrdiff_t> my_instructions(instructions);
		std::size_t instruction_count = 0;
		for(auto it = std::begin(my_instructions); it != std::end(my_instructions);) {
			const std::ptrdiff_t jump = *it;
			if(jump >= 3) {
				*it -= 1;
			} else {
				*it += 1;
			}
			it += jump;
			++instruction_count;
		}
		return std::to_string(instruction_count);
	}
};

REGISTER_SOLVER(2017, 5);
