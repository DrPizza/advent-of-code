#include "stdafx.h"

#include "problem.hpp"
#include "assembunny.hpp"

#include <fstream>
#include <string>

struct advent_2015_23 : problem
{
	advent_2015_23() noexcept : problem(2015, 23) {
	}

protected:
	std::vector<std::string> raw_instructions;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line); ) {
			raw_instructions.push_back(line);
		}
	}

	assembunny::program instructions;

	void precompute() override {
		for(const std::string& line : raw_instructions) {
			instructions.push_back(assembunny::parse_instruction(line));
		}
	}

	std::string part_1() override {
		assembunny::processor cpu = { &instructions };
		cpu.run();
		return std::to_string(cpu.registers['b']);
	}

	std::string part_2() override {
		assembunny::processor cpu = { &instructions };
		cpu.registers['a'] = 1;
		cpu.run();
		return std::to_string(cpu.registers['b']);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_23>() {
	advent_2015_23 a;
	a.solve();
}
