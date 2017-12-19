#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <unordered_map>

struct advent_2017_8 : problem
{
	advent_2017_8() noexcept : problem(2017, 8) {
	}

	enum operation
	{
		increment,
		decrement
	};

	operation str_to_op(const std::string& s) {
		if(s == "inc") {
			return increment;
		} else {
			return decrement;
		}
	}

	std::unordered_map<operation, std::function<std::ptrdiff_t(std::ptrdiff_t, std::ptrdiff_t)>> operations{
		{ increment, std::plus<void>{} },
		{ decrement, std::minus<void>{} },
	};

	enum comparison
	{
		lt,
		gt,
		lte,
		gte,
		eq,
		ne
	};

	comparison str_to_comp(const std::string& s) {
		if(s == "<") {
			return lt;
		} else if(s == ">") {
			return gt;
		} else if(s == "<=") {
			return lte;
		} else if(s == ">=") {
			return gte;
		} else if(s == "==") {
			return eq;
		} else if(s == "!=") {
			return ne;
		}
		__assume(0);
	}

	std::unordered_map<comparison, std::function<bool(std::ptrdiff_t, std::ptrdiff_t)>> comparisons{
		{ lt, std::less<void>{} },           { gt, std::greater<void>{} },  { lte, std::less_equal<void>{} },
		{ gte, std::greater_equal<void>{} }, { eq, std::equal_to<void>{} }, { ne, std::not_equal_to<void>{} }
	};

	struct instruction
	{
		std::string destination;
		operation op;
		std::ptrdiff_t operand;
		std::string guard;
		comparison cmp;
		std::ptrdiff_t comperand;
	};

	std::vector<instruction> instructions;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line);) {
			std::vector<std::string> fragments;
			boost::split(fragments, line, [](char c) { return c == ' '; });
			instructions.push_back(instruction{ fragments[0], str_to_op(fragments[1]), std::stoll(fragments[2]), fragments[4], str_to_comp(fragments[5]), stoll(fragments[6]) });
		}
	}

	std::ptrdiff_t highest_ever   = std::numeric_limits<std::ptrdiff_t>::min();
	std::ptrdiff_t highest_at_end = std::numeric_limits<std::ptrdiff_t>::min();

	void precompute() noexcept override {
		std::unordered_map<std::string, std::ptrdiff_t> registers;
		for(const instruction& i : instructions) {
			if(comparisons[i.cmp](registers[i.guard], i.comperand)) {
				registers[i.destination] = operations[i.op](registers[i.destination], i.operand);
			}
			if(registers[i.destination] > highest_ever) {
				highest_ever = registers[i.destination];
			}
		}
		for(auto x : registers) {
			if(x.second > highest_at_end) {
				highest_at_end = x.second;
			}
		}
	}

	std::string part_1() noexcept override {
		return std::to_string(highest_at_end);
	}

	std::string part_2() noexcept override {
		return std::to_string(highest_ever);
	}
};

template<>
void solve<advent_year::year_2017, advent_day::day_8>() {
	advent_2017_8 a;
	a.solve();
}
