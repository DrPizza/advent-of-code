#include "stdafx.h"

#include "problem.hpp"
#include "assembunny.hpp"

#include <fstream>

struct advent_2016_23 : problem
{
	advent_2016_23() noexcept : problem(2016, 23) {
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
		cpu.registers['a'] = 7;
		cpu.run();
		return std::to_string(cpu.registers['a']);
	}

	void tidy_up() override {
		instructions.clear();
		for(const std::string& line : raw_instructions) {
			instructions.push_back(assembunny::parse_instruction(line));
		}
	}

	std::string part_2() override {
		virtualize_jumps(instructions);
		optimize(instructions);
		devirtualize_jumps(instructions);

		assembunny::processor cpu = { &instructions };
		std::vector<std::size_t> toggled_locations;
		cpu.registers['a'] = 12;
		for(;;) {
			bool toggled = false;
			const std::size_t toggle_pc = cpu.instruction_pointer;
			if(assembunny::tgl* t = dynamic_cast<assembunny::tgl*>((*cpu.instructions)[cpu.instruction_pointer].get()); t != nullptr) {
				const assembunny::reg control = std::get<assembunny::reg>(t->op1);
				if(0 <= gsl::narrow_cast<std::ptrdiff_t>(toggle_pc) + cpu.registers[control]
				&&      gsl::narrow_cast<std::ptrdiff_t>(toggle_pc) + cpu.registers[control] < gsl::narrow_cast<std::ptrdiff_t>(cpu.instructions->size())) {
					toggled_locations.push_back(toggle_pc + gsl::narrow_cast<std::size_t>(cpu.registers[control]));
				}
				toggled = true;
				instructions.clear();
				for(const std::string& line : raw_instructions) {
					instructions.push_back(assembunny::parse_instruction(line));
				}
			}
			if(!cpu.single_step()) {
				break;
			}
			if(toggled) {
				assembunny::instruction_ptr toggle = std::make_unique<assembunny::tgl>(assembunny::reg{ 'z' });
				const std::size_t true_pc = cpu.instruction_pointer;
				cpu.instruction_pointer = toggle_pc;
				for(std::size_t i = 0; !toggled_locations.empty() && i < toggled_locations.size() - 1; ++i) {
					const std::size_t target = toggled_locations[i];
					cpu.registers['z'] = gsl::narrow_cast<std::ptrdiff_t>(target) - gsl::narrow_cast<std::ptrdiff_t>(cpu.instruction_pointer);

					toggle->execute(cpu);
				}
				cpu.registers.erase('z');
				cpu.instruction_pointer = true_pc;

				virtualize_jumps(instructions);
				optimize(instructions);
				devirtualize_jumps(instructions);
			}
		}
		return std::to_string(cpu.registers['a']);
	}
};

REGISTER_SOLVER(2016, 23);
