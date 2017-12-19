#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <regex>
#include <variant>
#include <utility>

struct component;
using component_ptr = std::unique_ptr<component>;

using wire = std::string;
using operand = std::variant<wire, uint16_t>;

struct circuit
{
	std::unordered_map<wire, uint16_t> wires;
	std::unordered_map<wire, component_ptr> components;

	uint16_t resolve_wire(wire w);

	uint16_t resolve_operand(operand op) {
		if(std::holds_alternative<wire>(op)) {
			return resolve_wire(std::get<std::string>(op));
		} else if(std::holds_alternative<uint16_t>(op)) {
			return std::get<uint16_t>(op);
		}
		__assume(0);
	}
};

struct component
{
	wire target;

	void execute(circuit& c) {
		c.wires[target] = do_execute(c);
	}
	virtual uint16_t do_execute(circuit& c) = 0;

	component(wire target_) : target(target_) {
	}

	virtual ~component() = default;
	component() = default;
	component(const component&) = default;
	component(component&&) = default;
	component& operator=(const component&) = default;
	component& operator=(component&&) = default;
};

uint16_t circuit::resolve_wire(wire w) {
	if(wires.find(w) == wires.end()) {
		components[w]->execute(*this);
	}
	return wires[w];
}

struct unary_component : component 
{
	unary_component(operand source_, wire target_) : component(target_), source(source_) {
	}

	operand source;
};

struct assign : unary_component
{
	assign(operand source_, wire target_) : unary_component(source_, target_) {
	}

	virtual uint16_t do_execute(circuit& c) override {
		return c.resolve_operand(source);
	}
};

struct not_gate : unary_component
{
	not_gate(operand source_, wire target_) : unary_component(source_, target_) {
	}

	virtual uint16_t do_execute(circuit& c) override {
		return gsl::narrow_cast<uint16_t>(~c.resolve_operand(source));
	}
};

struct binary_component : component
{
	binary_component(operand op1_, operand op2_, wire target_) : component(target_), op1(op1_), op2(op2_) {
	}

	operand op1;
	operand op2;
};

struct and_gate : binary_component
{
	and_gate(operand op1_, operand op2_, wire target_) : binary_component(op1_, op2_, target_) {
	}

	virtual uint16_t do_execute(circuit& c) override {
		return gsl::narrow_cast<uint16_t>(c.resolve_operand(op1) & c.resolve_operand(op2));
	}
};

struct or_gate : binary_component
{
	or_gate(operand op1_, operand op2_, wire target_) : binary_component(op1_, op2_, target_) {
	}

	virtual uint16_t do_execute(circuit& c) override {
		return gsl::narrow_cast<uint16_t>(c.resolve_operand(op1) | c.resolve_operand(op2));
	}
};

struct lshift : binary_component
{
	lshift(operand op1_, operand op2_, wire target_) : binary_component(op1_, op2_, target_) {
	}

	virtual uint16_t do_execute(circuit& c) override {
		return gsl::narrow_cast<uint16_t>(c.resolve_operand(op1) << c.resolve_operand(op2));
	}
};

struct rshift : binary_component
{
	rshift(operand op1_, operand op2_, wire target_) : binary_component(op1_, op2_, target_) {
	}

	virtual uint16_t do_execute(circuit& c) override {
		return gsl::narrow_cast<uint16_t>(c.resolve_operand(op1) >> c.resolve_operand(op2));
	}
};

struct advent_2015_7 : problem
{
	advent_2015_7() noexcept : problem(2015, 7) {
	}

protected:
	std::vector<std::string> raw_components;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line); ) {
			raw_components.push_back(line);
		}
	}

	operand parse_operand(const std::string& s) {
		std::regex pattern(R"(([[:lower:]]+)|([[:digit:]]+))");
		std::smatch m;
		std::regex_search(s, m, pattern);
		if(m[1].matched) {
			return std::string(m[1].str());
		} else if(m[2].matched) {
			return gsl::narrow<uint16_t>(std::stoul(m[2].str()));
		}
		__assume(0);
	}

	wire parse_wire(const std::string& s) {
		return wire(s);
	}

	component_ptr parse_component(const std::string& s) {
		std::regex assign_pattern(R"(([[:alnum:]]+) -> ([[:lower:]]+))");
		std::regex not_pattern   (R"(NOT ([[:lower:]]+) -> ([[:lower:]]+))");
		std::regex binary_pattern(R"(([[:alnum:]]+) (AND|OR|LSHIFT|RSHIFT) ([[:alnum:]]+) -> ([[:lower:]]+))");
		std::smatch m;
		if(std::regex_match(s, m, assign_pattern)) {
			return std::make_unique<assign>(parse_operand(m[1].str()), parse_wire(m[2].str()));
		} else if(std::regex_match(s, m, not_pattern)) {
			return std::make_unique<not_gate>(parse_wire(m[1].str()), parse_wire(m[2].str()));
		} else if(std::regex_match(s, m, binary_pattern)) {
			const std::string gate = m[2].str();
			       if(gate == "AND") {
				return std::make_unique<and_gate>(parse_operand(m[1].str()), parse_operand(m[3].str()), parse_wire(m[4].str()));
			} else if(gate == "OR") {
				return std::make_unique<or_gate>(parse_operand(m[1].str()), parse_operand(m[3].str()), parse_wire(m[4].str()));
			} else if(gate == "LSHIFT") {
				return std::make_unique<lshift>(parse_operand(m[1].str()), parse_operand(m[3].str()), parse_wire(m[4].str()));
			} else if(gate == "RSHIFT") {
				return std::make_unique<rshift>(parse_operand(m[1].str()), parse_operand(m[3].str()), parse_wire(m[4].str()));
			}
		}
		__assume(0);
	}

	std::string part_1() override {
		circuit c;
		for(const std::string& s : raw_components) {
			component_ptr com = parse_component(s);
			c.components[com->target] = std::move(com);
		}
		return std::to_string(c.resolve_wire("a"));
	}

	std::string part_2() override {
		std::string b_value = part_1();
		circuit c;
		for(const std::string& s : raw_components) {
			component_ptr com = parse_component(s);
			c.components[com->target] = std::move(com);
		}
		c.wires["b"] = gsl::narrow<uint16_t>(std::stoull(b_value));
		return std::to_string(c.resolve_wire("a"));
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_7>() {
	advent_2015_7 a;
	a.solve();
}
