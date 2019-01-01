#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <map>

namespace
{
	struct point
	{
		std::size_t c;
		std::size_t r;
	};

	enum struct direction
	{
		left,
		down,
		right
	};
}

struct advent_2018_17 : problem
{
	advent_2018_17() noexcept : problem(2018, 17) {
	}

protected:
	std::map<std::size_t, std::map<std::size_t, char>> survey;
	std::size_t y_min;
	std::size_t y_max;

	void fill(std::size_t x, std::size_t y, direction d) {
		if(survey[y][x] == '|') {
			switch(d) {
			case direction::left:
				fill(x - 1, y, d);
				break;
			case direction::right:
				fill(x + 1, y, d);
				break;
			case direction::down:
				return;
			}
			survey[y][x] = '~';
		}
	}

	bool flow(std::size_t x, std::size_t y, direction d) {
		if(y > y_max) {
			return true;
		}
		
		char& sq = survey[y][x];
		if(sq != '\0' && sq != '.') {
			return (sq != '#') && (sq != '~');
		}
		sq = '|';

		bool flowing = flow(x, y + 1, direction::down);
		if(flowing) {
			return true;
		}
		
		flowing  = d != direction::right && flow(x - 1, y, direction::left );
		flowing |= d != direction::left  && flow(x + 1, y, direction::right);

		if(flowing) {
			return true;
		}

		if(d == direction::down) {
			fill(x    , y, direction::left);
			fill(x + 1, y, direction::right);
		}

		return false;
	}

	void prepare_input(std::ifstream& fin) override {
		std::regex pattern(R"((.)=([[:digit:]]+), (.)=([[:digit:]]+)\.\.([[:digit:]]+))");
		std::smatch m;
		for(std::string line; std::getline(fin, line); ) {
			std::regex_match(line, m, pattern);
			const std::size_t row_or_column = std::stoull(m[2]);
			const std::size_t start = std::stoull(m[4]);
			const std::size_t end = std::stoull(m[5]);
			if(m[1].str() == "x") {
				const std::size_t col = row_or_column;
				for(std::size_t row = start; row <= end; ++row) {
					survey[row][col] = '#';
				}
			} else {
				const std::size_t row = row_or_column;
				for(std::size_t col = start; col <= end; ++col) {
					survey[row][col] = '#';
				}
			}
		}
		y_min = survey.begin()->first;
		y_max = std::prev(survey.end())->first;

		flow(500, 0, direction::down);
	}

	std::string part_1() override {
		std::size_t water_reached = 0ui64;
		auto start = survey.lower_bound(y_min);
		const auto end = survey.upper_bound(y_max);
		for(; start != end; ++start) {
			for(const auto& p : start->second) {
				if(p.second == '|'
				|| p.second == '~') {
					++water_reached;
				}
			}
		}
		return std::to_string(water_reached);
	}

	std::string part_2() override {
		std::size_t still_water = 0ui64;
		auto start = survey.lower_bound(y_min);
		const auto end = survey.upper_bound(y_max);
		for(; start != end; ++start) {
			for(const auto& p : start->second) {
				if(p.second == '~') {
					++still_water;
				}
			}
		}
		return std::to_string(still_water);
	}
};

template<>
void solve<advent_year::year_2018, advent_day::day_17>() {
	advent_2018_17 a;
	a.solve();
}
