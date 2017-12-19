#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <regex>
#include <variant>
#include <utility>

struct advent_2015_7 : problem
{
	advent_2015_7() noexcept : problem(2015, 7) {
	}

protected:
	std::vector<std::string> raw_instructions;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line); ) {
			raw_instructions.push_back(line);
		}
	}

	using wire = std::string;
	using operand = std::variant<wire, uint16_t>;

	using circuit = std::unordered_map<wire, uint16_t>;

	static uint16_t resolve_operand(operand op, circuit& c) {
		if(std::holds_alternative<wire>(op)) {
			return c[std::get<wire>(op)];
		} else if(std::holds_alternative<uint16_t>(op)) {
			return std::get<uint16_t>(op);
		}
		__assume(0);
	}

	struct component
	{
		virtual void execute(circuit& c) = 0;

		virtual ~component() = default;
		component() = default;
		component(const component&) = default;
		component(component&&) = default;
		component& operator=(const component&) = default;
		component& operator=(component&&) = default;
	};

	struct assign : component
	{
		assign(operand value_, wire target_) : value(value_), target(target_) {
		}

		virtual void execute(circuit& c) override {
			c[target] = resolve_operand(value, c);;
		}

		operand value;
		wire target;
	};

	struct not_gate : component
	{
		not_gate(wire source_, wire target_) : source(source_), target(target_) {
		}

		virtual void execute(circuit& c) override {
			c[target] = gsl::narrow<uint16_t>(~c[source]);
		}

		wire source;
		wire target;
	};

	struct binary_component : component
	{
		binary_component(operand op1_, operand op2_, wire target_) : op1(op1_), op2(op2_), target(target_) {
		}

		operand op1;
		operand op2;
		wire target;
	};

	struct and_gate : binary_component
	{
		and_gate(operand op1_, operand op2_, wire target_) : binary_component(op1_, op2_, target_) {
		}

		virtual void execute(circuit& c) override {
			c[target] = gsl::narrow<uint16_t>(resolve_operand(op1, c) & resolve_operand(op2, c));
		}
	};

	struct or_gate : binary_component
	{
		or_gate(operand op1_, operand op2_, wire target_) : binary_component(op1_, op2_, target_) {
		}

		virtual void execute(circuit& c) override {
			c[target] = gsl::narrow<uint16_t>(resolve_operand(op1, c) | resolve_operand(op2, c));
		}
	};

	struct lshift : binary_component
	{
		lshift(operand op1_, operand op2_, wire target_) : binary_component(op1_, op2_, target_) {
		}

		virtual void execute(circuit& c) override {
			c[target] = gsl::narrow<uint16_t>(resolve_operand(op1, c) << resolve_operand(op2, c));
		}
	};

	struct rshift : binary_component
	{
		rshift(operand op1_, operand op2_, wire target_) : binary_component(op1_, op2_, target_) {
		}

		virtual void execute(circuit& c) override {
			c[target] = gsl::narrow<uint16_t>(resolve_operand(op1, c) >> resolve_operand(op2, c));
		}
	};

	using component_ptr = std::unique_ptr<component>;

	operand parse_operand(const std::string& s) {
		std::regex pattern(R"(([[:lower:]]+)|([[:digit:]]+))");
		std::smatch m;
		std::regex_search(s, m, pattern);
		if(m[1].matched) {
			return wire(m[1].str());
		} else if(m[2].matched) {
			return gsl::narrow<uint16_t>(std::stoul(m[2].str()));
		}
		__assume(0);
	}

	component_ptr parse_component(const std::string& s) {
		std::regex assign_pattern(R"(([[:alnum:]]+) -> ([[:lower:]]+))");
		std::regex not_pattern   (R"(NOT ([[:lower:]]+) -> ([[:lower:]]+))");
		std::regex binary_pattern(R"(([[:alnum:]]+) (AND|OR|LSHIFT|RSHIFT) ([[:alnum:]]+) -> ([[:lower:]]+))");
		std::smatch m;
		if(std::regex_match(s, m, assign_pattern)) {
			//return std::make_unique<assign>();
		}
	}

	void precompute() override {

	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_7>() {
	advent_2015_7 a;
	a.solve();
}
