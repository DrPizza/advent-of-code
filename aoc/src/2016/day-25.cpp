#include "stdafx.h"

#include "problem.hpp"
#include "assembunny.hpp"

#include <fstream>

struct advent_2016_25 : problem
{
	advent_2016_25() noexcept : problem(2016, 25) {
	}

protected:
	std::vector<std::string> raw_instructions;

	void prepare_input(std::ifstream& fin) override {
		std::vector<std::string> lines;
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
		virtualize_jumps(instructions);
		optimize(instructions);
		devirtualize_jumps(instructions);

		for(std::ptrdiff_t i = 0; ; ++i) {
			assembunny::processor cpu = { &instructions };
			cpu.registers['a'] = i;
			bool output_valid = true;
			std::size_t output_size = 0;
			do {
				if(cpu.output.size() != output_size) {
					output_size = cpu.output.size();
					if(cpu.output[0] != 0) {
						output_valid = false;
					} else {
						for(std::size_t j = 1; cpu.output.size() > 2 && j < cpu.output.size(); ++j) {
							if(cpu.output[j - 1] != 1 - cpu.output[j]) {
								output_valid = false;
								break;
							}
						}
					}
				}
				if(output_size == 100) {
					return std::to_string(i);
				}
			} while(output_valid && cpu.single_step());
		}
	}

	std::string part_2() override {
		return "*";
	}
};

REGISTER_SOLVER(2016, 25);
