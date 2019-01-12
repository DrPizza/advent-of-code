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
	void jmpr(computer_t& computer, std::uintmax_t a, std::uintmax_t b, std::uintmax_t c) noexcept {
		computer[c] = computer[a] + b;
	}
	void jmpa(computer_t& computer, std::uintmax_t a, std::uintmax_t, std::uintmax_t c) noexcept {
		computer[c] = computer[a];
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
		{ "jmpr", jmpr },
		{ "jmpa", jmpa }
	};

	std::map<operation_t, std::string> opcode_names;

}

struct advent_2018_19 : problem
{
	advent_2018_19() noexcept : problem(2018, 19) {
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
		for(const auto& p : opcodes) {
			opcode_names[p.second] = p.first;
		}
	}

	std::string part_1() override {
		computer_t computer = {};
		for(;;) {
			if(computer[ip_register] > program.size()) {
				break;
			}
			const instruction_t& ins = program[computer[ip_register]];
			ins.op(computer, ins.a, ins.b, ins.c);

			++computer[ip_register];
		}
		return std::to_string(computer[0]);
	}

	std::string part_2() override {
		computer_t computer = { 0 };
		computer[4] = 2;
		computer[4] *= computer[4];
		computer[4] *= 19;
		computer[4] *= 11;
		computer[3] = 1;
		computer[3] *= 22;
		computer[3] += 3;
		computer[4] += computer[3];
		computer[3] = 27;
		computer[3] *= 28;
		computer[3] += 29;
		computer[3] *= 30;
		computer[3] *= 14;
		computer[3] *= 32;
		computer[4] += computer[3]; // 10551261;

		for(computer[5] = 1; computer[5] <= computer[4]; ++computer[5]) {
			if(computer[4] % computer[5] == 0) {
				computer[0] += computer[5];
			}
		}

		return std::to_string(computer[0]);
	}
};

template<>
void solve<advent_year::year_2018, advent_day::day_19>() {
	advent_2018_19 a;
	a.solve();
}
