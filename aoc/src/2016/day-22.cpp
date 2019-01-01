#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <regex>
#include <map>

namespace
{
	struct coord
	{
		std::size_t x;
		std::size_t y;
	};

	struct node
	{
		coord position;
		std::size_t size;
		std::size_t used;
		std::size_t avail;
	};

	constexpr bool operator==(const coord& lhs, const coord& rhs) noexcept {
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}
}

namespace std {
	template<>
	struct hash<coord>
	{
		std::size_t operator()(const coord& c) const noexcept {
			return std::hash<std::size_t>{}(c.x) ^ std::hash<std::size_t>{}(c.y);
		}
	};
}

namespace
{
	std::string print(const node& n) {
		if(n.used == 0) {
			return "_";
		}
		if(n.used >= 90) {
			return "#";
		}
		return ".";
	}

	using grid_type = std::map<std::size_t, std::map<std::size_t, node>>;

	void print(grid_type& grid) {
		for(std::size_t y = 0; y < grid.size(); ++y) {
			for(std::size_t x = 0; x < grid[y].size(); ++x) {
				if(y == 0 && x == 0) {
					std::cout << "(" << print(grid[y][x]) << ") ";
				} else if(y == 0 && x == grid[y].size() - 1) {
					std::cout << " G  ";
				} else {
					std::cout << " " << print(grid[y][x]) << "  ";
				}
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

struct advent_2016_22 : problem
{
	advent_2016_22() noexcept : problem(2016, 22) {
	}

protected:
	std::vector<node> nodes;
	grid_type grid;

	void prepare_input(std::ifstream& fin) override {
		std::regex pattern(R"(/dev/grid/node-x([[:digit:]]+)-y([[:digit:]]+) +([[:digit:]]+)T +([[:digit:]]+)T +([[:digit:]]+)T +..%)");
		std::string line;
		std::getline(fin, line);
		std::getline(fin, line);
		for(; std::getline(fin, line); ) {
			std::smatch m;
			std::regex_search(line, m, pattern);
			node n{ { std::stoull(m[1].str()), std::stoull(m[2].str()) },
			          std::stoull(m[3].str()), std::stoull(m[4].str()), std::stoull(m[5].str())
			};
			nodes.push_back(n);
			grid[n.position.y][n.position.x] = n;
		}
	}

	std::string part_1() override {
		std::size_t viable_pairs = 0;
		for(std::size_t a = 0; a < nodes.size(); ++a) {
			for(std::size_t b = 0; b < nodes.size(); ++b) {
				if(a == b) {
					continue;
				}
				if(nodes[a].used == 0ui64) {
					continue;
				}
				if(nodes[a].used <= nodes[b].avail) {
					++viable_pairs;
				}
			}
		}
		return std::to_string(viable_pairs);
	}

	std::string part_2() override {
		coord empty_node = { 0, 0 };
		coord wall_start = { 0, 0 };
		bool wall_found = false;
		for(std::size_t y = 0; y < grid.size(); ++y) {
			for(std::size_t x = 0; x < grid[y].size(); ++x) {
				if(grid[y][x].used == 0) {
					empty_node.x = x;
					empty_node.y = y;
				}
				if(!wall_found && grid[y][x].used > 90) {
					wall_found = true;
					wall_start.x = x;
					wall_start.y = y;
				}
			}
		}
		// step 1: move the hole to (x - 2, 0)
		const std::size_t distance = empty_node.y + (grid[0].size() - 1 - empty_node.x) + 2 * (1 + empty_node.x - wall_start.x)
		// step 2: leapfrog hole and data to (0, 0); 4 hole movements to 1 data movement
		                           + (grid[0].size() - 2) * 5;
		return std::to_string(distance);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_22>() {
	advent_2016_22 a;
	a.solve();
}
