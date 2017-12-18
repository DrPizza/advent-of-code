#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <iomanip>
#include <regex>
#include <utility>
#include <map>
#include <numeric>
#include <chrono>
#include <variant>
#include <deque>
#include <boost/algorithm/string.hpp>

struct advent_18 : problem
{
	advent_18() noexcept : problem(18) {
	}

	std::vector<std::string> raw_instructions;

	void prepare_input() override {
		std::ifstream fin("day-18.txt");
		for(std::string line; std::getline(fin, line); ) {
			raw_instructions.push_back(line);
		}
	}

	using reg     = char;
	using operand = std::variant<reg, std::ptrdiff_t>;
	using register_file = std::unordered_map<char, std::ptrdiff_t>;
	using speaker = std::deque<std::ptrdiff_t>;

	static std::ptrdiff_t resolve_operand(operand op, register_file& registers) {
		if(std::holds_alternative<reg>(op)) {
			return registers[std::get<reg>(op)];
		} else if(std::holds_alternative<std::ptrdiff_t>(op)) {
			return std::get<std::ptrdiff_t>(op);
		}
		__assume(0);
	}

	struct instruction
	{
		virtual std::ptrdiff_t execute(register_file& registers, speaker& send, speaker& receive) = 0;

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

	struct snd : unary_instruction
	{
		snd(reg op1_) noexcept : unary_instruction(op1_) {
		}

		std::ptrdiff_t execute(register_file& registers, speaker& send, speaker&) override {
			send.push_front(registers[op1]);
			return 1;
		}
	};

	struct set : binary_instruction
	{
		set(reg op1_, operand op2_) noexcept : binary_instruction(op1_, op2_) {
		}

		std::ptrdiff_t execute(register_file& registers, speaker&, speaker&) override {
			registers[op1] = resolve_operand(op2, registers);
			return 1;
		}
	};

	struct add : binary_instruction
	{
		add(reg op1_, operand op2_) noexcept : binary_instruction(op1_, op2_) {
		}

		std::ptrdiff_t execute(register_file& registers, speaker&, speaker&) override {
			registers[op1] += resolve_operand(op2, registers);
			return 1;
		}
	};

	struct mul : binary_instruction
	{
		mul(reg op1_, operand op2_) noexcept : binary_instruction(op1_, op2_) {
		}

		std::ptrdiff_t execute(register_file& registers, speaker&, speaker&) override {
			registers[op1] *= resolve_operand(op2, registers);
			return 1;
		}
	};

	struct mod : binary_instruction
	{
		mod(reg op1_, operand op2_) noexcept : binary_instruction(op1_, op2_) {
		}

		std::ptrdiff_t execute(register_file& registers, speaker&, speaker&) override {
			registers[op1] %= resolve_operand(op2, registers);
			return 1;
		}
	};

	struct rcv : unary_instruction
	{
		rcv(reg op1_) noexcept : unary_instruction(op1_) {
		}

		std::ptrdiff_t execute(register_file& registers, speaker&, speaker& receive) override {
			if(!receive.empty()) {
				registers[op1] = receive.back();
				receive.pop_back();
				return 1;
			} else {
				return 0;
			}
		}
	};

	struct jgz : instruction
	{
		operand op1;
		operand op2;

		jgz(operand op1_, operand op2_) noexcept : op1(op1_), op2(op2_) {
		}

		std::ptrdiff_t execute(register_file& registers, speaker&, speaker&) override {
			const std::ptrdiff_t control_value = std::holds_alternative<reg>(op1) ? registers[std::get<reg>(op1)]
			                                                                      : std::get<std::ptrdiff_t>(op1);
			if(control_value > 0) {
				return resolve_operand(op2, registers);
			} else {
				return 1;
			}
		}
	};

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

	std::unique_ptr<instruction> parse_snd(const std::string& ins) {
		reg r = parse_reg(ins);
		return std::make_unique<snd>(r);
	}

	std::unique_ptr<instruction> parse_set(const std::string& ins) {
		reg r = parse_reg(ins.substr(0, 1));
		operand o = parse_operand(ins.substr(2));
		return std::make_unique<set>(r, o);
	}

	std::unique_ptr<instruction> parse_add(const std::string& ins) {
		reg r = parse_reg(ins.substr(0, 1));
		operand o = parse_operand(ins.substr(2));
		return std::make_unique<add>(r, o);
	}

	std::unique_ptr<instruction> parse_mul(const std::string& ins) {
		reg r = parse_reg(ins.substr(0, 1));
		operand o = parse_operand(ins.substr(2));
		return std::make_unique<mul>(r, o);
	}

	std::unique_ptr<instruction> parse_mod(const std::string& ins) {
		reg r = parse_reg(ins.substr(0, 1));
		operand o = parse_operand(ins.substr(2));
		return std::make_unique<mod>(r, o);
	}

	std::unique_ptr<instruction> parse_rcv(const std::string& ins) {
		reg r = parse_reg(ins);
		return std::make_unique<rcv>(r);
	}

	std::unique_ptr<instruction> parse_jgz(const std::string& ins) {
		operand o1 = parse_operand(ins.substr(0, ins.find(' ')));
		operand o2 = parse_operand(ins.substr(ins.find(' ') + 1));
		return std::make_unique<jgz>(o1, o2);
	}

	std::unique_ptr<instruction> parse_instruction(const std::string ins) {
		const std::string opcode = ins.substr(0, 3);
		if(opcode == "snd") {
			return parse_snd(ins.substr(4));
		} else if(opcode == "set") {
			return parse_set(ins.substr(4));
		} else if(opcode == "add") {
			return parse_add(ins.substr(4));
		} else if(opcode == "mul") {
			return parse_mul(ins.substr(4));
		} else if(opcode == "mod") {
			return parse_mod(ins.substr(4));
		} else if(opcode == "rcv") {
			return parse_rcv(ins.substr(4));
		} else if(opcode == "jgz") {
			return parse_jgz(ins.substr(4));
		}
		__assume(0);
	}

	std::vector<std::unique_ptr<instruction>> instructions;

	void precompute() override {
		for(const std::string& line : raw_instructions) {
			instructions.push_back(parse_instruction(line));
		}
	}

	std::string part_1() override {
		std::size_t instruction_pointer = 0;
		register_file registers;
		speaker spk;
		while(instruction_pointer < instructions.size()) {
			const std::unique_ptr<instruction>& ins = instructions[instruction_pointer];
			const rcv* r = dynamic_cast<const rcv*>(ins.get());
			if(r != nullptr && registers[r->op1] != 0) {
				return std::to_string(spk[0]);
			}
			instruction_pointer += ins->execute(registers, spk, spk);
		}
		__assume(0);
	}

	std::string part_2() override {
		std::size_t instruction_pointer[2] = { 0, 0 };
		register_file registers[2];
		registers[0]['p'] = 0;
		registers[1]['p'] = 1;
		speaker spk[2];

		std::size_t send_count[2] = { 0, 0 };

		const auto single_step = [&](std::size_t i) {
			if(instruction_pointer[i] >= instructions.size()) {
				return false;
			}
			const std::unique_ptr<instruction>& ins = instructions[instruction_pointer[i]];
			std::ptrdiff_t delta = ins->execute(registers[i], spk[1 - i], spk[i]);
			if(dynamic_cast<snd*>(ins.get()) != nullptr) {
				++send_count[i];
			}
			if(delta == 0) {
				return false;
			}
			instruction_pointer[i] += delta;
			return true;
		};

		while(single_step(0) || single_step(1)) {
			while(single_step(0)) {
				;
			}
			while(single_step(1)) {
				;
			}
		}
		return std::to_string(send_count[1]);
	}
};

template<>
void solve<advent_day::day_18>() {
	advent_18 a;
	a.solve();
}
