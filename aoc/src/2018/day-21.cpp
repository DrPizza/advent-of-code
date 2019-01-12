#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <array>
#include <map>

namespace
{
	using reg_t = std::uintmax_t;
	using computer_t = std::array<reg_t, 6>;

	using operation_t = void(*)(computer_t& computer, std::uintmax_t a, std::uintmax_t b, std::uintmax_t c);

	struct instruction_t
	{
		operation_t op;
		std::uintmax_t a;
		std::uintmax_t b;
		std::uintmax_t c;
	};

	void addr(computer_t& computer, std::uintmax_t a, std::uintmax_t b, std::uintmax_t c) noexcept {
		computer[c] = computer[a] + computer[b];
	}
	void addi(computer_t& computer, std::uintmax_t a, std::uintmax_t b, std::uintmax_t c) noexcept {
		computer[c] = computer[a] + b;
	}
	void mulr(computer_t& computer, std::uintmax_t a, std::uintmax_t b, std::uintmax_t c) noexcept {
		computer[c] = computer[a] * computer[b];
	}
	void muli(computer_t& computer, std::uintmax_t a, std::uintmax_t b, std::uintmax_t c) noexcept {
		computer[c] = computer[a] * b;
	}
	void banr(computer_t& computer, std::uintmax_t a, std::uintmax_t b, std::uintmax_t c) noexcept {
		computer[c] = computer[a] & computer[b];
	}
	void bani(computer_t& computer, std::uintmax_t a, std::uintmax_t b, std::uintmax_t c) noexcept {
		computer[c] = computer[a] & b;
	}
	void borr(computer_t& computer, std::uintmax_t a, std::uintmax_t b, std::uintmax_t c) noexcept {
		computer[c] = computer[a] | computer[b];
	}
	void bori(computer_t& computer, std::uintmax_t a, std::uintmax_t b, std::uintmax_t c) noexcept {
		computer[c] = computer[a] | b;
	}
	void setr(computer_t& computer, std::uintmax_t a, std::uintmax_t, std::uintmax_t c) noexcept {
		computer[c] = computer[a];
	}
	void seti(computer_t& computer, std::uintmax_t a, std::uintmax_t, std::uintmax_t c) noexcept {
		computer[c] = a;
	}
	void gtir(computer_t& computer, std::uintmax_t a, std::uintmax_t b, std::uintmax_t c) noexcept {
		computer[c] = a > computer[b] ? 1ui64 : 0ui64;
	}
	void gtri(computer_t& computer, std::uintmax_t a, std::uintmax_t b, std::uintmax_t c) noexcept {
		computer[c] = computer[a] > b ? 1ui64 : 0ui64;
	}
	void gtrr(computer_t& computer, std::uintmax_t a, std::uintmax_t b, std::uintmax_t c) noexcept {
		computer[c] = computer[a] > computer[b] ? 1ui64 : 0ui64;
	}
	void eqir(computer_t& computer, std::uintmax_t a, std::uintmax_t b, std::uintmax_t c) noexcept {
		computer[c] = a == computer[b] ? 1ui64 : 0ui64;
	}
	void eqri(computer_t& computer, std::uintmax_t a, std::uintmax_t b, std::uintmax_t c) noexcept {
		computer[c] = computer[a] == b ? 1ui64 : 0ui64;
	}
	void eqrr(computer_t& computer, std::uintmax_t a, std::uintmax_t b, std::uintmax_t c) noexcept {
		computer[c] = computer[a] == computer[b] ? 1ui64 : 0ui64;
	}

	std::map<std::string, operation_t> opcodes = {
		{ "addr", addr },
		{ "addi", addi },
		{ "mulr", mulr },
		{ "muli", muli },
		{ "banr", banr },
		{ "bani", bani },
		{ "borr", borr },
		{ "bori", bori },
		{ "setr", setr },
		{ "seti", seti },
		{ "gtir", gtir },
		{ "gtri", gtri },
		{ "gtrr", gtrr },
		{ "eqir", eqir },
		{ "eqri", eqri },
		{ "eqrr", eqrr },
	};

}

struct advent_2018_21 : problem
{
	advent_2018_21() noexcept : problem(2018, 21) {
	}

protected:
	std::size_t ip_register;
	std::vector<instruction_t> program;

	void prepare_input(std::ifstream& fin) override {
		const std::regex ip_pattern(R"(#ip ([[:digit:]]+))");
		const std::regex instr_pattern(R"((....) ([[:digit:]]+) ([[:digit:]]+) ([[:digit:]]+))");
		std::smatch m;
		for(std::string line; std::getline(fin, line); ) {
			if(std::regex_match(line, m, ip_pattern)) {
				ip_register = std::stoull(m[1]);
			} else if(std::regex_match(line, m, instr_pattern)) {
				program.push_back({ opcodes[m[1].str()], std::stoull(m[2]), std::stoull(m[3]), std::stoull(m[4])});
			}
		}
	}

	std::string part_1() override {
		computer_t computer = {};
		for(;;) {
			if(computer[ip_register] > program.size()) {
				break;
			}
			if(computer[ip_register] == 28ui64) {
				return std::to_string(computer[4]);
			}
			const instruction_t& ins = program[computer[ip_register]];
			ins.op(computer, ins.a, ins.b, ins.c);

			++computer[ip_register];
		}
		__assume(0);
	}

	std::string part_2() override {
		std::vector<std::size_t> observed_values;
		computer_t computer = {};
		for(;;) {
			if(computer[ip_register] > program.size()) {
				break;
			}
			if(computer[ip_register] == 28ui64) {
				if(std::find(std::begin(observed_values), std::end(observed_values), computer[4]) != std::end(observed_values)) {
					return std::to_string(observed_values.back());
				} else {
					observed_values.push_back(computer[4]);
				}
			}
			const instruction_t& ins = program[computer[ip_register]];
			ins.op(computer, ins.a, ins.b, ins.c);

			++computer[ip_register];
		}
		__assume(0);
	}
};

template<>
void solve<advent_year::year_2018, advent_day::day_21>() {
	advent_2018_21 a;
	a.solve();
}
