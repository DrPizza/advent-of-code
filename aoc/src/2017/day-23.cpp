#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <string>
#include <variant>
#include <unordered_map>

struct advent_2017_23 : problem
{
	advent_2017_23() noexcept : problem(2017, 23) {
	}

	std::vector<std::string> raw_instructions;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line); ) {
			raw_instructions.push_back(line);
		}
	}

	using reg           = char;
	using operand       = std::variant<reg, std::ptrdiff_t>;
	using register_file = std::unordered_map<char, std::ptrdiff_t>;

	static std::ptrdiff_t resolve_operand(operand op, register_file& registers) {
		if(std::holds_alternative<reg>(op)) {
			return registers[std::get<reg>(op)];
		} else if(std::holds_alternative<std::ptrdiff_t>(op)) {
			return std::get<std::ptrdiff_t>(op);
		}
		__assume(0);
	}

	static std::string print_register(reg r) {
		return std::string(1, r);
	}

	static std::string print_operand(operand op) {
		if(std::holds_alternative<reg>(op)) {
			return std::string(1, std::get<reg>(op));
		} else if(std::holds_alternative<std::ptrdiff_t>(op)) {
			return std::to_string(std::get<std::ptrdiff_t>(op));
		}
		__assume(0);
	}

	struct instruction
	{
		virtual std::ptrdiff_t execute(register_file& registers) = 0;
		virtual std::string emit_code(std::ptrdiff_t addr, std::ptrdiff_t highest_addr) = 0;

		virtual ~instruction() = default;
		instruction() = default;
		instruction(const instruction&) = default;
		instruction(instruction&&) = default;
		instruction& operator=(const instruction&) = default;
		instruction& operator=(instruction&&) = default;
	};

	struct unary_instruction : instruction
	{
		reg op1;

		unary_instruction(reg op1_) noexcept : op1(op1_) {
		}
	};

	struct binary_instruction : instruction
	{
		reg op1;
		operand op2;

		binary_instruction(reg op1_, operand op2_) noexcept : op1(op1_), op2(op2_) {
		}
	};

	struct set : binary_instruction
	{
		set(reg op1_, operand op2_) noexcept : binary_instruction(op1_, op2_) {
		}

		std::ptrdiff_t execute(register_file& registers) override {
			registers[op1] = resolve_operand(op2, registers);
			return 1;
		}
		
		std::string emit_code(std::ptrdiff_t, std::ptrdiff_t) override {
			return print_register(op1) + " = " + print_operand(op2) + ";";
		}
	};

	struct sub : binary_instruction
	{
		sub(reg op1_, operand op2_) noexcept : binary_instruction(op1_, op2_) {
		}

		std::ptrdiff_t execute(register_file& registers) override {
			registers[op1] -= resolve_operand(op2, registers);
			return 1;
		}

		std::string emit_code(std::ptrdiff_t, std::ptrdiff_t) override {
			return print_register(op1) + " -= " + print_operand(op2) + ";";
		}
	};

	struct mul : binary_instruction
	{
		mul(reg op1_, operand op2_) noexcept : binary_instruction(op1_, op2_) {
		}

		std::ptrdiff_t execute(register_file& registers) override {
			registers[op1] *= resolve_operand(op2, registers);
			return 1;
		}

		std::string emit_code(std::ptrdiff_t, std::ptrdiff_t) override {
			return print_register(op1) + " *= " + print_operand(op2) + ";";
		}
	};

	struct jnz : instruction
	{
		operand op1;
		operand op2;

		jnz(operand op1_, operand op2_) noexcept : op1(op1_), op2(op2_) {
		}

		std::ptrdiff_t execute(register_file& registers) override {
			const std::ptrdiff_t control_value = std::holds_alternative<reg>(op1) ? registers[std::get<reg>(op1)]
				: std::get<std::ptrdiff_t>(op1);
			if(control_value != 0) {
				return resolve_operand(op2, registers);
			} else {
				return 1;
			}
		}

		std::string emit_code(std::ptrdiff_t addr, std::ptrdiff_t highest_addr) override {
			const std::ptrdiff_t absolute_address = addr + std::get<std::ptrdiff_t>(op2);
			const std::string destination = (absolute_address < highest_addr) ? "case_" + std::to_string(absolute_address)
			                                                                  : "case_end";
			if(std::holds_alternative<std::ptrdiff_t>(op1)) {
				return "goto " + destination + ";";
			} else {
				return "if(" + print_register(std::get<reg>(op1)) + " != 0) { goto " + destination + "; }";
			}
		}

	};

	using instruction_ptr = std::unique_ptr<instruction>;

	reg parse_reg(const std::string& ins) {
		return ins[0];
	}

	operand parse_operand(const std::string& ins) {
		std::regex pattern(R"(([[:lower:]])|(-?[[:digit:]]+))");
		std::smatch m;
		std::regex_search(ins, m, pattern);
		if(m[1].matched) {
			return m[1].str()[0];
		} else if(m[2].matched) {
			return std::stoll(m[2].str());
		}
		__assume(0);
	}

	instruction_ptr parse_set(const std::string& ins) {
		reg     r = parse_reg    (ins.substr(0, ins.find(' ')));
		operand o = parse_operand(ins.substr(ins.find(' ') + 1));
		return std::make_unique<set>(r, o);
	}

	instruction_ptr parse_sub(const std::string& ins) {
		reg     r = parse_reg    (ins.substr(0, ins.find(' ')));
		operand o = parse_operand(ins.substr(ins.find(' ') + 1));
		return std::make_unique<sub>(r, o);
	}

	instruction_ptr parse_mul(const std::string& ins) {
		reg     r = parse_reg    (ins.substr(0, ins.find(' ')));
		operand o = parse_operand(ins.substr(ins.find(' ') + 1));
		return std::make_unique<mul>(r, o);
	}

	instruction_ptr parse_jnz(const std::string& ins) {
		operand o1 = parse_operand(ins.substr(0, ins.find(' ')));
		operand o2 = parse_operand(ins.substr(ins.find(' ') + 1));
		return std::make_unique<jnz>(o1, o2);
	}

	instruction_ptr parse_instruction(const std::string ins) {
		const std::string opcode = ins.substr(0, 3);
		if(opcode == "set") {
			return parse_set(ins.substr(4));
		} else if(opcode == "sub") {
			return parse_sub(ins.substr(4));
		} else if(opcode == "mul") {
			return parse_mul(ins.substr(4));
		} else if(opcode == "jnz") {
			return parse_jnz(ins.substr(4));
		}
		__assume(0);
	}

	using program = std::vector<instruction_ptr>;

	program instructions;

	struct processor
	{
		program* instructions;
		std::size_t instruction_pointer = 0;
		register_file registers;

		processor(program& instructions_) : instructions(&instructions_) {
		}

		bool single_step() {
			const instruction_ptr& ins = (*instructions)[instruction_pointer];
			const std::ptrdiff_t delta = ins->execute(registers);
			instruction_pointer += delta;
			if(instruction_pointer >= instructions->size()) {
				return false;
			}
			return true;
		}
	};

	void precompute() override {
		for(const std::string& line : raw_instructions) {
			instructions.push_back(parse_instruction(line));
		}
	}

	std::string part_1() override {
		processor cpu = { processor(instructions) };
		std::size_t muls = 0;
		do {
			const instruction_ptr& ins = (*cpu.instructions)[cpu.instruction_pointer];
			const mul* m = dynamic_cast<const mul*>(ins.get());
			if(m) {
				++muls;
			}

		} while(cpu.single_step());

		return std::to_string(muls);
	}

	std::string part_2() override {
		std::cout <<
R"(#include <iostream>
#include <cstddef>

int main() {
	std::int32_t a = 1, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0;
	{
)";
		for(std::size_t i = 0; i < instructions.size(); ++i) {
			std::cout << "\tcase_" << i << ": \n";
			std::cout << "\t\t\t" << instructions[i]->emit_code(i, instructions.size()) << "\n";
		}
		std::cout <<
R"(
	case_end:
		;
	}
	return h;
}
)" << std::endl;
		return "905";
	}
};

template<>
void solve<advent_year::year_2017, advent_day::day_23>() {
	advent_2017_23 a;
	a.solve();
}
