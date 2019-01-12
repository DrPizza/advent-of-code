#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <tuple>

struct advent_2018_6 : problem
{
	[[gsl::suppress(f.6)]]
	advent_2018_6() : problem(2018, 6) {
	}

protected:
	struct point
	{
		std::intmax_t r;
		std::intmax_t c;
		std::intmax_t id;
	};

	std::vector<point> locations;

	struct area
	{
		std::intmax_t r;
		std::intmax_t c;
		std::intmax_t closest_id;
		std::intmax_t all_points_distance;
	};

	std::vector<area> grid;

	std::intmax_t rmin = std::numeric_limits<std::intmax_t>::max();
	std::intmax_t rmax = std::numeric_limits<std::intmax_t>::min();
	std::intmax_t cmin = std::numeric_limits<std::intmax_t>::max();
	std::intmax_t cmax = std::numeric_limits<std::intmax_t>::min();

	void prepare_input(std::ifstream& fin) override {
		std::regex pattern(R"(([[:digit:]]+), ([[:digit:]]+))");
		for(std::string line; std::getline(fin, line);) {
			std::smatch m;
			if(std::regex_match(line, m, pattern)) {
				std::intmax_t c = std::stoll(m[1]);
				std::intmax_t r = std::stoll(m[2]);

				rmin = std::min(rmin, r);
				rmax = std::max(rmax, r);
				cmin = std::min(cmin, c);
				cmax = std::max(cmax, c);

				locations.push_back({ r, c, gsl::narrow_cast<std::intmax_t>(locations.size() + 1) });
			}
		}
	}

	void precompute() override {
		for(std::intmax_t c = cmin; c < cmax + 1; ++c) {
			for(std::intmax_t r = rmin; r < rmax + 1; ++r) {
				grid.push_back({ r, c, std::numeric_limits<std::intmax_t>::max(), 0 });
			}
		}
		for(area& a : grid) {
			std::intmax_t shortest_distance = std::numeric_limits<std::intmax_t>::max();
			for(const point& p : locations) {
				const std::intmax_t distance = std::abs(p.r - a.r) + std::abs(p.c - a.c);
				if(distance == shortest_distance) {
					a.closest_id = 0;
				} else if(distance < shortest_distance) {
					a.closest_id = p.id;
					shortest_distance = distance;
				}
				a.all_points_distance += distance;
			}
		}
	}

	std::string part_1() override {
		std::vector<std::intmax_t> sizes(locations.size() + 1, 0);
		for(const area& a : grid) {
			if(a.r == rmin || a.r == rmax || a.c == cmin || a.c == cmax) {
				sizes[a.closest_id] = std::numeric_limits<std::intmax_t>::min();
			} else {
				++sizes[a.closest_id];
			}
		}
		return std::to_string(*std::max_element(std::next(std::begin(sizes)), std::end(sizes)));
	}

	std::string part_2() override {
		const auto below_threshold = ranges::distance(ranges::view::filter(grid, [] (const area& a) { return a.all_points_distance < 10000; }));
		return std::to_string(below_threshold);
	}
};

REGISTER_SOLVER(2018, 6);
