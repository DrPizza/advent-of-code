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
		int r;
		int c;
		int id;
	};

	std::vector<point> locations;

	struct area
	{
		int r;
		int c;
		int closest_id;
		int all_points_distance;
	};

	std::vector<area> grid;

	int rmin = std::numeric_limits<int>::max();
	int rmax = std::numeric_limits<int>::min();
	int cmin = std::numeric_limits<int>::max();
	int cmax = std::numeric_limits<int>::min();

	void prepare_input(std::ifstream& fin) override {
		std::regex pattern(R"(([[:digit:]]+), ([[:digit:]]+))");
		for(std::string line; std::getline(fin, line);) {
			std::smatch m;
			if(std::regex_match(line, m, pattern)) {
				int c = std::stoi(m[1]);
				int r = std::stoi(m[2]);

				rmin = std::min(rmin, r);
				rmax = std::max(rmax, r);
				cmin = std::min(cmin, c);
				cmax = std::max(cmax, c);

				locations.push_back({ r, c, gsl::narrow_cast<int>(locations.size() + 1) });
			}
		}
		for(int c = cmin; c < cmax + 1; ++c) {
			for(int r = rmin; r < rmax + 1; ++r) {
				grid.push_back({ r, c, std::numeric_limits<int>::max(), 0 });
			}
		}
		for(area& a : grid) {
			int shortest_distance = std::numeric_limits<int>::max();
			for(const point& p : locations) {
				const int distance = std::abs(p.r - a.r) + std::abs(p.c - a.c);
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
		std::vector<int> sizes(locations.size() + 1, 0);
		for(const area& a : grid) {
			if(a.r == rmin || a.r == rmax || a.c == cmin || a.c == cmax) {
				sizes[a.closest_id] = std::numeric_limits<int>::min();
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

template<>
void solve<advent_year::year_2018, advent_day::day_6>() {
	advent_2018_6 a;
	a.solve();
}
