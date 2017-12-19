#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <unordered_map>
#include <boost/algorithm/string.hpp>

struct advent_2017_11 : problem
{
	advent_2017_11() noexcept : problem(2017, 11) {
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

	void prepare_input(std::ifstream& fin) override {
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

	struct hex_coord
	{
		std::ptrdiff_t x, y, z;

		hex_coord& operator+=(direction d) noexcept {
			switch(d) {
			case n:      ++y; --z; break;
			case ne: ++x;      --z; break;
			case se: ++x; --y;      break;
			case s:      --y; ++z; break;
			case sw: --x;      ++z; break;
			case nw: --x; ++y;      break;
			}
			return *this;
		}
	};

	std::size_t distance_from_origin(hex_coord c) {
		return gsl::narrow<std::size_t>((std::abs(c.x) + std::abs(c.y) + std::abs(c.z)) / 2);
	}

	void precompute() noexcept override {
		hex_coord position{ 0, 0, 0 };
		std::for_each(std::begin(directions), std::end(directions), [&](direction d) {
			position += d;
			current_distance = distance_from_origin(position);
			greatest_distance = std::max(greatest_distance, current_distance);
		});
	}

	std::string part_1() noexcept override {
		return std::to_string(current_distance);
	}

	std::string part_2() noexcept override {
		return std::to_string(greatest_distance);
	}
};

template<>
void solve<advent_year::year_2017, advent_day::day_11>() {
	advent_2017_11 a;
	a.solve();
}
