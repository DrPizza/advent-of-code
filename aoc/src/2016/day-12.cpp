#include "stdafx.h"

#include "problem.hpp"
#include "assembunny.hpp"

#include <fstream>

struct advent_2016_12 : problem
{
	advent_2016_12() noexcept : problem(2016, 12) {
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
		return std::to_string(cpu.registers['a']);
	}

	std::string part_2() override {
		assembunny::processor cpu = { &instructions };
		cpu.registers['c'] = 1;
		cpu.run();
		return std::to_string(cpu.registers['a']);
	}
};

REGISTER_SOLVER(2016, 12);
