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

	struct cancellation
	{
		direction full;
		std::array<direction, 2> semi;
		std::array<direction, 2> net;
	};

	std::unordered_map<direction, cancellation> cancellations = {
		{ n,  { s,  { se, sw }, { ne, nw } } },
		{ ne, { sw, { nw, s  }, { n,  se } } },
		{ se, { nw, { sw, n  }, { s,  ne } } },
		{ s,  { n,  { nw, ne }, { sw, se } } },
		{ sw, { ne, { n , se }, { nw, s  } } },
		{ nw, { se, { s,  ne }, { sw, n  } } }
	};

	std::size_t greatest_distance = 0;
	std::size_t current_distance = 0;

	void precompute() noexcept override {
		std::unordered_map<direction, std::size_t> counts;
		for(const direction d : directions) {
			cancellation c = cancellations[d];
			if(counts[c.full]) {
				--counts[c.full];
				--current_distance;
			} else if(counts[c.semi[0]]) {
				--counts[c.semi[0]];
				++counts[c.net[0]];
			} else if(counts[c.semi[1]]) {
				--counts[c.semi[1]];
				++counts[c.net[1]];
			} else {
				++counts[d];
				++current_distance;
				if(current_distance > greatest_distance) {
					greatest_distance = current_distance;
				}
			}
		}
	}

	std::string part_1() noexcept override {
		return std::to_string(current_distance);
	}

	std::string part_2() noexcept override {
		return std::to_string(greatest_distance);
	}
};
