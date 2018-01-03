#include "stdafx.h"

#include "problem.hpp"
#include "assembler.hpp"

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

	assembler::program instructions;

	void precompute() override {
		for(const std::string& line : raw_instructions) {
			instructions.push_back(assembler::parse_instruction(line));
		}
	}

	std::string part_1() override {
		virtualize_jumps(instructions);
		optimize(instructions);
		devirtualize_jumps(instructions);

		for(std::ptrdiff_t i = 0; ; ++i) {
			assembler::processor cpu = { &instructions };
			std::vector<std::ptrdiff_t> output;
			cpu.output = &output;
			cpu.registers['a'] = i;
			bool output_valid = true;
			std::size_t output_size = 0;
			do {
				if(output.size() != output_size) {
					if(output[0] != 0) {
						output_valid = false;
					} else {
						for(std::size_t j = 1; output.size() > 2 && j < output.size(); ++j) {
							if(output[j - 1] != 1 - output[j]) {
								output_valid = false;
								break;
							}
						}
					}
					output_size = output.size();
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

template<>
void solve<advent_year::year_2016, advent_day::day_25>() {
	advent_2016_25 a;
	a.solve();
}
