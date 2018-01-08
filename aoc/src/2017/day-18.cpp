#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"
#include "assembunny.hpp"

#include <fstream>

struct advent_2017_18 : problem
{
	advent_2017_18() noexcept : problem(2017, 18) {
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

		do {
			const assembunny::rcv* r = dynamic_cast<const assembunny::rcv*>((*cpu.instructions)[cpu.instruction_pointer].get());
			if(r != nullptr && cpu.registers[std::get<assembunny::reg>(r->op1)] != 0) {
				return std::to_string(cpu.output[0]);
			}

		} while(cpu.single_step());

		__assume(0);
	}

	std::string part_2() override {
		assembunny::processor cpu[2] = { &instructions, &instructions };
		cpu[0].input = &cpu[1].output;
		cpu[0].registers['p'] = 0;
		cpu[1].input = &cpu[0].output;
		cpu[1].registers['p'] = 1;

		std::size_t send_count = 0;

		do {
			while(cpu[0].single_step()) {
				;
			}

			do {
				const assembunny::snd* s = dynamic_cast<const assembunny::snd*>((*cpu[1].instructions)[cpu[1].instruction_pointer].get());
				if(s != nullptr) {
					++send_count;
				}
			} while(cpu[1].single_step());

		} while(cpu[0].single_step() || cpu[1].single_step());

		return std::to_string(send_count);
	}
};

template<>
void solve<advent_year::year_2017, advent_day::day_18>() {
	advent_2017_18 a;
	a.solve();
}
