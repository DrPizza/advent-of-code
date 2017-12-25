#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <string>
#include <variant>
#include <unordered_map>
#include <list>

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

	struct instruction;

	using reg           = char;
	using operand       = std::variant<reg, std::ptrdiff_t, instruction*>;
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
		switch(r) {
		case 'f':
			return "is_prime";
		case 'h':
			return "total_composite";
		default:
			return std::string(1, r);
		}
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

		std::ptrdiff_t abs_addr = 0;

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

	struct ternary_instruction : instruction
	{
		reg op1;
		operand op2;
		operand op3;

		ternary_instruction(reg op1_, operand op2_, operand op3_) noexcept : op1(op1_), op2(op2_), op3(op3_) {
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

	struct add : binary_instruction
	{
		add(reg op1_, operand op2_) noexcept : binary_instruction(op1_, op2_) {
		}

		std::ptrdiff_t execute(register_file& registers) override {
			registers[op1] -= resolve_operand(op2, registers);
			return 1;
		}

		std::string emit_code(std::ptrdiff_t, std::ptrdiff_t) override {
			return print_register(op1) + " += " + print_operand(op2) + ";";
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
			std::ptrdiff_t absolute_address = 0;
			if(std::holds_alternative<std::ptrdiff_t>(op2)) {
				absolute_address = addr + std::get<std::ptrdiff_t>(op2);
			} else if(std::holds_alternative<instruction*>(op2)) {
				const instruction* const ins = std::get<instruction*>(op2);
				absolute_address = ins != nullptr ? std::get<instruction*>(op2)->abs_addr : std::numeric_limits<std::ptrdiff_t>::max();
			}
			const std::string destination = (absolute_address < highest_addr) ? "case_" + std::to_string(absolute_address)
			                                                                  : "case_end";
			if(std::holds_alternative<std::ptrdiff_t>(op1)) {
				return "goto " + destination + ";";
			} else {
				return "if(" + print_register(std::get<reg>(op1)) + " != 0) { goto " + destination + "; }";
			}
		}
	};

	struct tern_add : ternary_instruction
	{
		tern_add(reg op1_, operand op2_, operand op3_) noexcept : ternary_instruction(op1_, op2_, op3_) {
		}

		std::ptrdiff_t execute(register_file& registers) override {
			registers[op1] = resolve_operand(op2, registers) + resolve_operand(op3, registers);
			return 1;
		}

		std::string emit_code(std::ptrdiff_t, std::ptrdiff_t) override {
			return print_register(op1) + " = " + print_operand(op2) + " + " + print_operand(op3) + ";";
		}
	};

	struct tern_sub : ternary_instruction
	{
		tern_sub(reg op1_, operand op2_, operand op3_) noexcept : ternary_instruction(op1_, op2_, op3_) {
		}

		std::ptrdiff_t execute(register_file& registers) override {
			registers[op1] = resolve_operand(op2, registers) + resolve_operand(op3, registers);
			return 1;
		}

		std::string emit_code(std::ptrdiff_t, std::ptrdiff_t) override {
			return print_register(op1) + " = " + print_operand(op2) + " - " + print_operand(op3) + ";";
		}
	};

	struct tern_mul : ternary_instruction
	{
		tern_mul(reg op1_, operand op2_, operand op3_) noexcept : ternary_instruction(op1_, op2_, op3_) {
		}

		std::ptrdiff_t execute(register_file& registers) override {
			registers[op1] = resolve_operand(op2, registers) + resolve_operand(op3, registers);
			return 1;
		}

		std::string emit_code(std::ptrdiff_t, std::ptrdiff_t) override {
			return print_register(op1) + " = " + print_operand(op2) + " * " + print_operand(op3) + ";";
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

	void optimize(std::vector<instruction_ptr>& insns) {
		const auto fix_gotos = [&](instruction* first, instruction* second, instruction* replacement) -> bool {
			// gotos pointing at second should return false and disable the replacement entirely
			if(second != nullptr) {
				for(std::size_t i = 0; i < instructions.size(); ++i) {
					const instruction_ptr& ins = instructions[i];
					if(jnz* const j = dynamic_cast<jnz*>(ins.get()); j != nullptr) {
						if(std::get<instruction*>(j->op2) == second) {
							return false;
						}
					}
				}
			}
			// gotos pointing at first should be redirected to replacement
			for(std::size_t i = 0; i < instructions.size(); ++i) {
				const instruction_ptr& ins = instructions[i];
				if(jnz* const j = dynamic_cast<jnz*>(ins.get()); j != nullptr) {
					if(std::get<instruction*>(j->op2) == first) {
						std::get<instruction*>(j->op2) = replacement;
					}
				}
			}
			return true;
		};

		const auto set_sub_neg_to_add = [&]() -> bool {
			for(std::size_t i = 0; i < insns.size(); ++i) {
				if(sub* ins1 = dynamic_cast<sub*>(insns[i].get()); ins1 != nullptr) {
					if(std::holds_alternative<std::ptrdiff_t>(ins1->op2)) {
						const std::ptrdiff_t value = std::get<std::ptrdiff_t>(ins1->op2);
						if(value < 0) {
							instruction_ptr replacement = std::make_unique<add>(ins1->op1, operand{ -value });
							if(fix_gotos(ins1, nullptr, replacement.get())) {
								instructions[i] = std::move(replacement);
							}
						}
					}
				}
			}
			return false;
		};

		const auto set_arith_to_tern_arith = [&]() -> bool {
			for(std::size_t i = 0; i < insns.size() - 1; ++i) {
				if(set* ins1 = dynamic_cast<set*>(insns[i].get()); ins1 != nullptr) {
					instruction_ptr replacement = nullptr;
					binary_instruction* ins2 = nullptr;
					if(sub* i2 = dynamic_cast<sub*>(insns[i + 1].get()); i2 != nullptr) {
						ins2 = i2;
						replacement = std::make_unique<tern_sub>(ins1->op1, ins1->op2, ins2->op2);
					} else if(mul* i2 = dynamic_cast<mul*>(insns[i + 1].get()); i2 != nullptr) { 
						ins2 = i2;
						replacement = std::make_unique<tern_mul>(ins1->op1, ins1->op2, ins2->op2);
					}
					if(ins2 && ins1->op1 == ins2->op1) {
						if(fix_gotos(ins1, ins2, replacement.get())) {
							instructions.erase(instructions.begin() + gsl::narrow_cast<std::ptrdiff_t>(i), instructions.begin() + gsl::narrow_cast<std::ptrdiff_t>(i) + 2);
							instructions.insert(instructions.begin() + gsl::narrow_cast<std::ptrdiff_t>(i), std::move(replacement));
							return true;
						}
					}
				}
			}
			return false;
		};

		bool did_work = false;
		do {
			did_work = set_sub_neg_to_add();
			did_work = did_work || set_arith_to_tern_arith();
		} while(did_work);
	}

	std::string part_2() override {
		for(std::size_t i = 0; i < instructions.size(); ++i) {
			const instruction_ptr& ins = instructions[i];
			jnz* const j = dynamic_cast<jnz*>(ins.get());
			if(j) {
				const std::ptrdiff_t relative_address = std::get<ptrdiff_t>(j->op2);
				const std::ptrdiff_t absolute_address = gsl::narrow_cast<std::ptrdiff_t>(i) + relative_address;
				if(absolute_address >= gsl::narrow_cast<std::ptrdiff_t>(instructions.size())) {
					instruction* const ptr = nullptr;
					j->op2 = { ptr };
				} else {
					j->op2 = { instructions[gsl::narrow_cast<std::size_t>(absolute_address)].get() };
				}
			}
		}

		optimize(instructions);

		// bake addresses
		for(std::size_t i = 0; i < instructions.size(); ++i) {
			instructions[i]->abs_addr = gsl::narrow_cast<std::ptrdiff_t>(i);
		}

		std::cout <<
R"(#include <iostream>
#include <cstddef>

int main() {
	const std::int32_t a = 1;
	std::int32_t b = 0, c = 0, d = 0, e = 0, is_prime = 0, g = 0, total_composite = 0;
	{
)";
		for(std::size_t i = 0; i < instructions.size(); ++i) {
			std::cout << "\tcase_" << i << ": \n";
			std::cout << "\t\t\t" << instructions[i]->emit_code(gsl::narrow_cast<std::ptrdiff_t>(i), gsl::narrow_cast<std::ptrdiff_t>(instructions.size())) << "\n";
		}
		std::cout <<
R"(
	case_end:
		;
	}
	return total_composite;
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
