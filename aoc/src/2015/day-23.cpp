#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

struct advent_2015_23 : problem
{
	advent_2015_23() noexcept : problem(2015, 23) {
	}

	using reg           = char;
	using register_file = std::unordered_map<reg, uint64_t>;

	struct instruction
	{
		virtual std::ptrdiff_t execute(register_file& registers) = 0;

		virtual ~instruction() = default;
		instruction() = default;
		instruction(const instruction&) = default;
		instruction(instruction&&) = default;
		instruction& operator=(const instruction&) = default;
		instruction& operator=(instruction&&) = default;
	};

	struct unary_instruction : instruction
	{
		unary_instruction(reg target_) noexcept : target(target_) {
		}

		reg target;
	};

	struct binary_instruction : instruction
	{
		binary_instruction(reg target_, std::ptrdiff_t offset_) noexcept : target(target_), offset(offset_) {
		}

		reg target;
		std::ptrdiff_t offset;
	};

	struct hlf : unary_instruction
	{
		hlf(reg target_) noexcept : unary_instruction(target_) {
		}

		std::ptrdiff_t execute(register_file& registers) override {
			registers[target] /= 2;
			return 1;
		}
	};

	struct tpl : unary_instruction
	{
		tpl(reg target_) noexcept : unary_instruction(target_) {
		}

		std::ptrdiff_t execute(register_file& registers) override {
			registers[target] *= 3;
			return 1;
		}
	};

	struct inc : unary_instruction
	{
		inc(reg target_) noexcept : unary_instruction(target_) {
		}

		std::ptrdiff_t execute(register_file& registers) override {
			registers[target] += 1;
			return 1;
		}
	};

	struct jmp : instruction
	{
		jmp(std::ptrdiff_t offset_) noexcept : offset(offset_) {
		}

		std::ptrdiff_t execute(register_file&) noexcept override {
			return offset;
		}

		std::ptrdiff_t offset;
	};

	struct jie : binary_instruction
	{
		jie(reg target_, std::ptrdiff_t offset_) noexcept : binary_instruction(target_, offset_) {
		}

		std::ptrdiff_t execute(register_file& registers) override {
			if(0 == registers[target] % 2) {
				return offset;
			}
			return 1;
		}
	};

	struct jio : binary_instruction
	{
		jio(reg target_, std::ptrdiff_t offset_) noexcept : binary_instruction(target_, offset_) {
		}

		std::ptrdiff_t execute(register_file& registers) override {
			if(1 == registers[target]) {
				return offset;
			}
			return 1;
		}
	};

	using instruction_ptr = std::unique_ptr<instruction>;

	reg parse_register(const std::string& ins) {
		return ins[0];
	}

	std::ptrdiff_t parse_offset(const std::string& ins) {
		return std::stoll(ins);
	}

	instruction_ptr parse_hlf(const std::string& ins) {
		reg r = parse_register(ins);
		return std::make_unique<hlf>(r);
	}

	instruction_ptr parse_tpl(const std::string& ins) {
		reg r = parse_register(ins);
		return std::make_unique<tpl>(r);
	}

	instruction_ptr parse_inc(const std::string& ins) {
		reg r = parse_register(ins);
		return std::make_unique<inc>(r);
	}

	instruction_ptr parse_jmp(const std::string& ins) {
		std::ptrdiff_t offset = parse_offset(ins);
		return std::make_unique<jmp>(offset);
	}

	instruction_ptr parse_jie(const std::string& ins) {
		reg r = parse_register(ins);
		std::ptrdiff_t offset = parse_offset(ins.substr(2));
		return std::make_unique<jie>(r, offset);
	}

	instruction_ptr parse_jio(const std::string& ins) {
		reg r = parse_register(ins);
		std::ptrdiff_t offset = parse_offset(ins.substr(2));
		return std::make_unique<jio>(r, offset);
	}

	instruction_ptr parse_instruction(const std::string ins) {
		const std::string opcode = ins.substr(0, 3);
		if(opcode == "hlf") {
			return parse_hlf(ins.substr(4));
		} else if(opcode == "tpl") {
			return parse_tpl(ins.substr(4));
		} else if(opcode == "inc") {
			return parse_inc(ins.substr(4));
		} else if(opcode == "jmp") {
			return parse_jmp(ins.substr(4));
		} else if(opcode == "jie") {
			return parse_jie(ins.substr(4));
		} else if(opcode == "jio") {
			return parse_jio(ins.substr(4));
		}
		__assume(0);
	}

	using program = std::vector<instruction_ptr>;

protected:
	program instructions;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line);) {
			instructions.push_back(parse_instruction(line));
		}
	}
	
	std::string part_1() override {
		std::ptrdiff_t instruction_pointer = 0;
		register_file registers;
		do {
			instruction_pointer += instructions[gsl::narrow_cast<std::size_t>(instruction_pointer)]->execute(registers);
		} while(gsl::narrow_cast<std::size_t>(instruction_pointer) < instructions.size());
		return std::to_string(registers['b']);
	}

	std::string part_2() override {
		std::ptrdiff_t instruction_pointer = 0;
		register_file registers;
		registers['a'] = 1;
		do {
			instruction_pointer += instructions[gsl::narrow_cast<std::size_t>(instruction_pointer)]->execute(registers);
		} while(gsl::narrow_cast<std::size_t>(instruction_pointer) < instructions.size());
		return std::to_string(registers['b']);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_23>() {
	advent_2015_23 a;
	a.solve();
}
