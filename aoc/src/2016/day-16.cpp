#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

struct advent_2016_16 : problem
{
	advent_2016_16() noexcept : problem(2016, 16) {
	}

protected:
	std::string initial_state;

	void prepare_input(std::ifstream& fin) override {
		std::getline(fin, initial_state);
	}

	std::string dragon_curve(const std::string& a) {
		std::string b;
		std::transform(std::crbegin(a), std::crend(a), std::back_inserter(b), [](char c) {
			return (c == '1' ? '0' : '1');
		});
		return a + "0" + b;
	}

	std::string dragon_curve_length(const std::string& seed, std::size_t length) {
		std::string dragon = seed;
		while(dragon.size() < length) {
			dragon = dragon_curve(dragon);
		}
		return dragon.substr(0, length);
	}

	std::string checksum(const std::string& s) {
		if(s.size() % 2 == 1) {
			return s;
		}
		std::string cksum;
		for(std::size_t i = 0; i < s.size(); i += 2) {
			cksum.push_back(s[i] == s[i + 1] ? '1' : '0');
		}
		return checksum(cksum);
	}

	std::string part_1() override {
		return checksum(dragon_curve_length(initial_state, 272));
	}

	std::string part_2() override {
		return checksum(dragon_curve_length(initial_state, 35'651'584));
	}
};

REGISTER_SOLVER(2016, 16);
