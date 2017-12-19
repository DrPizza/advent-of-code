#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"
#include "mdspan.hpp"

#include <fstream>
#include <utility>
#include <numeric>

struct advent_2017_19 : problem
{
	advent_2017_19() noexcept : problem(2017, 19) {
	}

	std::vector<std::string> raw_map;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line); ) {
			raw_map.push_back(line);
		}
	}

	using grid_shape = md::extents<-1, -1>;
	using grid_type = md::mdspan<char, grid_shape>;

	enum struct direction
	{
		up,
		right,
		down,
		left,
	};

	struct position
	{
		direction dir;
		std::ptrdiff_t r, c;
	};

	bool get_next_position(position& pos, const grid_type& network) noexcept {
		if(network(pos.r, pos.c) != '+') {
			switch(pos.dir) {
			case direction::up:
				if(pos.r == 0) {
					return false;
				}
				--pos.r;
				break;
			case direction::right:
				if(pos.c == network.extent(1) - 1) {
					return false;
				}
				++pos.c;
				break;
			case direction::down:
				if(pos.r == network.extent(0) - 1) {
					return false;
				}
				++pos.r;
				break;
			case direction::left:
				if(pos.c == 0) {
					return false;
				}
				--pos.c;
				break;
			}
			return network(pos.r, pos.c) != ' ';
		} else {
			switch(pos.dir) {
			case direction::up:
			case direction::down:
				if(network(pos.r, pos.c - 1) != ' ') {
					pos.dir = direction::left;
					--pos.c;
					return true;
				} else if(network(pos.r, pos.c + 1) != ' ') {
					pos.dir = direction::right;
					++pos.c;
					return true;
				}
				break;
			case direction::left:
			case direction::right:
				if(network(pos.r - 1, pos.c) != ' ') {
					pos.dir = direction::up;
					--pos.r;
					return true;
				} else if(network(pos.r + 1, pos.c) != ' ') {
					pos.dir = direction::down;
					++pos.r;
					return true;
				}
				break;
			}
		}
		return true;
	}

	std::string letters;
	std::size_t steps = 0;

	[[gsl::suppress(r.5)]]
	void precompute() override {
		const std::size_t width = raw_map[0].size();
		const std::size_t height = raw_map.size();
		const std::size_t required_size = grid_type::required_span_size(width, height);
		std::unique_ptr<char[]> data(std::make_unique<char[]>(required_size));
		const grid_type network(data.get(), width, height);
		
		for(std::size_t i = 0; i < raw_map.size(); ++i) {
			for(std::size_t j = 0; j < raw_map[i].size(); ++j) {
				network(i, j) = raw_map[i][j];
			}
		}

		std::ptrdiff_t start = 0;
		for(std::ptrdiff_t i = 0; i < network.extent(0); ++i) {
			if(network(0, i) == '|') {
				start = i;
				break;
			}
		}

		position pos = { direction::down, 0, start };
		do {
			++steps;
			const char ch = network(pos.r, pos.c);
			if(ch != '|'
			&& ch != '+'
			&& ch != '-') {
				letters += ch;
			}

		} while(get_next_position(pos, network));
	}

	std::string part_1() override {
		return letters;
	}

	std::string part_2() override {
		return std::to_string(steps);
	}
};

template<>
void solve<advent_year::year_2017, advent_day::day_19>() {
	advent_2017_19 a;
	a.solve();
}
