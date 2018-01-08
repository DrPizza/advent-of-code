#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"
#include "assembler.hpp"

#include <fstream>
#include <string>
#include <variant>

struct advent_2017_23 : problem
{
	advent_2017_23() noexcept : problem(2017, 23) {
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
		assembunny::processor cpu = { &instructions };
		std::size_t muls = 0;
		do {
			const assembunny::instruction_ptr& ins = (*cpu.instructions)[cpu.instruction_pointer];
			const assembunny::mul* m = dynamic_cast<const assembunny::mul*>(ins.get());
			if(m) {
				++muls;
			}

		} while(cpu.single_step());

		return std::to_string(muls);
	}

	std::string part_2() override {
		virtualize_jumps(instructions);
		optimize(instructions);
		devirtualize_jumps(instructions);

		assembunny::print_asm(instructions);

		assembunny::processor cpu = { &instructions };
		cpu.registers['a'] = 1;
		while(cpu.single_step()) {
			;
		}
		return std::to_string(cpu.registers['h']);
	}
};

template<>
void solve<advent_year::year_2017, advent_day::day_23>() {
	advent_2017_23 a;
	a.solve();
}
