#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <array>
#include <map>

using reg_t = std::size_t;
using computer_t = std::array<reg_t, 16>;

computer_t the_computer = { 0 };

struct instruction_t
{
	std::size_t opcode;
	std::size_t a;
	std::size_t b;
	std::size_t c;
};

using operation_t = void(*)(computer_t& computer, std::size_t a, std::size_t b, std::size_t c);

void addr(computer_t& computer, std::size_t a, std::size_t b, std::size_t c) noexcept {
	computer[c] = computer[a] + computer[b];
}
void addi(computer_t& computer, std::size_t a, std::size_t b, std::size_t c) noexcept {
	computer[c] = computer[a] + b;
}
void mulr(computer_t& computer, std::size_t a, std::size_t b, std::size_t c) noexcept {
	computer[c] = computer[a] * computer[b];
}
void muli(computer_t& computer, std::size_t a, std::size_t b, std::size_t c) noexcept {
	computer[c] = computer[a] * b;
}
void banr(computer_t& computer, std::size_t a, std::size_t b, std::size_t c) noexcept {
	computer[c] = computer[a] & computer[b];
}
void bani(computer_t& computer, std::size_t a, std::size_t b, std::size_t c) noexcept {
	computer[c] = computer[a] & b;
}
void borr(computer_t& computer, std::size_t a, std::size_t b, std::size_t c) noexcept {
	computer[c] = computer[a] | computer[b];
}
void bori(computer_t& computer, std::size_t a, std::size_t b, std::size_t c) noexcept {
	computer[c] = computer[a] | b;
}
void setr(computer_t& computer, std::size_t a, std::size_t, std::size_t c) noexcept {
	computer[c] = computer[a];
}
void seti(computer_t& computer, std::size_t a, std::size_t, std::size_t c) noexcept {
	computer[c] = a;
}
void gtir(computer_t& computer, std::size_t a, std::size_t b, std::size_t c) noexcept {
	computer[c] = a > computer[b] ? 1ui64 : 0ui64;
}
void gtri(computer_t& computer, std::size_t a, std::size_t b, std::size_t c) noexcept {
	computer[c] = computer[a] > b ? 1ui64 : 0ui64;
}
void gtrr(computer_t& computer, std::size_t a, std::size_t b, std::size_t c) noexcept {
	computer[c] = computer[a] > computer[b] ? 1ui64 : 0ui64;
}
void eqir(computer_t& computer, std::size_t a, std::size_t b, std::size_t c) noexcept {
	computer[c] = a == computer[b] ? 1ui64 : 0ui64;
}
void eqri(computer_t& computer, std::size_t a, std::size_t b, std::size_t c) noexcept {
	computer[c] = computer[a] == b ? 1ui64 : 0ui64;
}
void eqrr(computer_t& computer, std::size_t a, std::size_t b, std::size_t c) noexcept {
	computer[c] = computer[a] == computer[b] ? 1ui64 : 0ui64;
}

const operation_t op_table[] = {
	addr,
	addi,
	mulr,
	muli,
	banr,
	bani,
	borr,
	bori,
	setr,
	seti,
	gtir,
	gtri,
	gtrr,
	eqir,
	eqri,
	eqrr
};

struct sample
{
	reg_t initial[4];
	instruction_t op;
	reg_t result[4];
};

struct advent_2018_16 : problem
{
	std::vector<sample> samples;
	std::vector<instruction_t> program;

	advent_2018_16() noexcept : problem(2018, 16) {
	}

protected:
	void prepare_input(std::ifstream& fin) override {
		const std::regex opcode_pattern(
R"(Before: \[([[:digit:]]+), ([[:digit:]]+), ([[:digit:]]+), ([[:digit:]]+)\]
([[:digit:]]+) ([[:digit:]]+) ([[:digit:]]+) ([[:digit:]]+)
After:  \[([[:digit:]]+), ([[:digit:]]+), ([[:digit:]]+), ([[:digit:]]+)\])");
		
		std::string file;
		for(std::string line; std::getline(fin, line);) {
			file += line;
			file += "\n";
		}
		std::smatch m;
		for(; std::regex_search(file, m, opcode_pattern); ) {
			samples.push_back({
				{ std::stoull(m[1]), std::stoull(m[2]), std::stoull(m[3]), std::stoull(m[4]) },
				{ std::stoull(m[5]), std::stoull(m[6]), std::stoull(m[7]), std::stoull(m[8]) },
				{ std::stoull(m[9]), std::stoull(m[10]), std::stoull(m[11]), std::stoull(m[12]) }
			});
			file = m.suffix();
		}
		
		const std::regex program_pattern(R"(([[:digit:]]+) ([[:digit:]]+) ([[:digit:]]+) ([[:digit:]]+))");
		for(; std::regex_search(file, m, program_pattern); ) {
			program.push_back({
				std::stoull(m[1]), std::stoull(m[2]), std::stoull(m[3]), std::stoull(m[4])
			});
			file = m.suffix();
		}
	}

	std::string part_1() override {
		std::size_t ambiguous_opcodes = 0ui64;
		for(const sample& s : samples) {
			std::size_t possible_opcodes = 0ui64;
			for(const operation_t& op : op_table) {
				computer_t computer = { s.initial[0], s.initial[1], s.initial[2], s.initial[3] };
				op(computer, s.op.a, s.op.b, s.op.c);
				if(computer[0] == s.result[0]
				&& computer[1] == s.result[1]
				&& computer[2] == s.result[2]
				&& computer[3] == s.result[3]) {
					++possible_opcodes;
				}
			}
			if(possible_opcodes >= 3ui64) {
				++ambiguous_opcodes;
			}
		}
		return std::to_string(ambiguous_opcodes);
	}

	std::string part_2() override {
		std::map<operation_t, std::set<std::size_t>> candidate_mappings;
		for(const sample& s : samples) {
			for(const operation_t& op : op_table) {
				computer_t computer = { s.initial[0], s.initial[1], s.initial[2], s.initial[3] };
				op(computer, s.op.a, s.op.b, s.op.c);
				if(computer[0] == s.result[0]
				&& computer[1] == s.result[1]
				&& computer[2] == s.result[2]
				&& computer[3] == s.result[3]) {
					candidate_mappings[op].insert(s.op.opcode);
				}
			}
		}
		bool made_change = false;
		do {
			made_change = false;
			for(const auto& p : candidate_mappings) {
				if(p.second.size() == 1ui64) {
					for(auto& q : candidate_mappings) {
						if(&p != &q) {
							if(q.second.erase(*p.second.begin()) > 0ui64) {
								made_change = true;
							}
							
						}
					}
				}
			}
		} while(made_change);
		std::map<std::size_t, operation_t> opcode_mapping;
		for(const auto& p : candidate_mappings) {
			opcode_mapping[*p.second.begin()] = p.first;
		}
		computer_t computer = {};
		for(const instruction_t& i : program) {
			opcode_mapping[i.opcode](computer, i.a, i.b, i.c);
		}
		return std::to_string(computer[0]);
	}
};

template<>
void solve<advent_year::year_2018, advent_day::day_16>() {
	advent_2018_16 a;
	a.solve();
}
