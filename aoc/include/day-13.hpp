#pragma once

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <boost/algorithm/string.hpp>

struct advent_13 : problem
{
	advent_13() noexcept : problem(13) {
	}

	std::unordered_map<std::size_t, std::size_t> firewall;

	void prepare_input() override {
		std::ifstream fin("day-13.txt");
		for(std::string line; std::getline(fin, line); ) {
			std::size_t level = std::stoull(line.substr(0, line.find(':')));
			std::size_t depth = std::stoull(line.substr(line.find(' ')));
			firewall[level] = (depth - 1) * 2;
		}
	}

	auto traverse_firewall(std::size_t delay, bool exact) {
		bool caught = false;
		std::size_t severity = 0;
		for(const auto& layer : firewall) {
			if((layer.first + delay) % layer.second == 0) {
				caught = true;
				if(!exact) {
					return std::make_pair(true, 1ull);
				}
				severity += layer.first * ((layer.second / 2) + 1);
			}
		}
		return std::make_pair(caught, severity);
	}

	std::string part_1() override {
		auto result = traverse_firewall(0, true);
		return std::to_string(result.second);
	}

	std::string part_2() override {
		for(std::size_t delay = 0; ; ++delay) {
			auto result = traverse_firewall(delay, false);
			if(!result.first) {
				return std::to_string(delay);
			}
		}
	}
};
