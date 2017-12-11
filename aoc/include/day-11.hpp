#pragma once

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <unordered_map>
#include <boost/algorithm/string.hpp>

struct advent_11 : problem
{
	advent_11() noexcept : problem(11) {
	}

	enum direction
	{
		n,
		ne,
		se,
		s,
		sw,
		nw,
	};

	direction str_to_direction(const std::string& str) {
		if(str == "n") {
			return n;
		} else if(str == "ne") {
			return ne;
		} else if(str == "se") {
			return se;
		} else if(str == "s") {
			return s;
		} else if(str == "sw") {
			return sw;
		} else if(str == "nw") {
			return nw;
		} else {
			__assume(0);
		}
	}

	std::vector<direction> directions;

	void prepare_input() override {
		std::ifstream fin("day-11.txt");
		std::string line;
		std::getline(fin, line);
		std::vector<std::string> raw_directions;
		boost::split(raw_directions, line, [](char c) { return c == ','; });
		std::transform(std::begin(raw_directions), std::end(raw_directions), std::back_inserter(directions), [&](const std::string& str) {
			return str_to_direction(str);
		});
	}

	std::size_t greatest_distance = 0;
	std::size_t current_distance = 0;

	std::size_t distance(std::ptrdiff_t x, std::ptrdiff_t y, std::ptrdiff_t z) {
		return gsl::narrow<std::size_t>((std::abs(x) + std::abs(y) + std::abs(z)) / 2);
	}

	void precompute() noexcept override {
		std::ptrdiff_t x(0), y(0), z(0);
		for(const direction d : directions) {
			switch(d) {
			case n :      ++y; --z; break;
			case ne: ++x;      --z; break;
			case se: ++x; --y;      break;
			case s :      --y; ++z; break;
			case sw: --x;      ++z; break;
			case nw: --x; ++y;      break;
			}
			current_distance = distance(x, y, z);
			greatest_distance = std::max(greatest_distance, current_distance);
		}
	}

	std::string part_1() noexcept override {
		return std::to_string(current_distance);
	}

	std::string part_2() noexcept override {
		return std::to_string(greatest_distance);
	}
};
