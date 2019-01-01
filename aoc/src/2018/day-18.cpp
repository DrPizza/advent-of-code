#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <intrin.h>

struct advent_2018_18 : problem
{
	advent_2018_18() noexcept : problem(2018, 18) {
	}

protected:
	using grid_t = std::vector<std::vector<char>>;
	grid_t original_grid;

	void prepare_input(std::ifstream& fin) override {
		original_grid = ranges::getlines(fin)
		              | ranges::view::transform([](std::string str) {
		                	str = " " + str + " ";
		                	return std::vector(std::begin(str), std::end(str));
                        })
		              | ranges::to_vector;
		original_grid.insert(std::begin(original_grid), std::vector<char>{});
		original_grid[0].resize(original_grid[1].size(), ' ');
		original_grid.push_back(original_grid[0]);
	}

	const std::uint64_t tree_mask  = 0b0000'0000'1111'1111ui64;
	const std::uint64_t tree_shift = 0ui64;
	const std::uint64_t yard_mask  = 0b1111'1111'0000'0000ui64;
	const std::uint64_t yard_shift = 8ui64;

	std::uint64_t get_neighbours(const grid_t& g, std::size_t x, std::size_t y) const {
		const std::vector<std::pair<std::size_t, std::size_t>> neighbours = {
			{y - 1, x - 1}, { y - 1, x    }, { y - 1, x + 1},
			{y    , x - 1},                  { y    , x + 1},
			{y + 1, x - 1}, { y + 1, x    }, { y + 1, x + 1},
		};
		std::uint64_t value = 0ui64;
		for(std::size_t i = 0ui64; i < 8ui64; ++i) {
			auto p = neighbours[i];
			value |= (g[p.first][p.second] == '|' ? 1ui64 : 0ui64) << (i + tree_shift);
			value |= (g[p.first][p.second] == '#' ? 1ui64 : 0ui64) << (i + yard_shift);
		}
		return value;
	}

	char iterate(char current, std::uint64_t neighbours) const noexcept {
		switch(current) {
		case '.':
			if(__popcnt64((neighbours & tree_mask) >> tree_shift) >= 3ui64) {
				return '|';
			} else {
				return '.';
			}
		case '|':
			if(__popcnt64((neighbours & yard_mask) >> yard_shift) >= 3ui64) {
				return '#';
			} else {
				return '|';
			}
			break;
		case '#':
			if(__popcnt64((neighbours & yard_mask) >> yard_shift) >= 1ui64
			&& __popcnt64((neighbours & tree_mask) >> tree_shift) >= 1ui64) {
				return '#';
			} else {
				return '.';
			}
			break;
		}
		__assume(0);
	}

	void perform_round(grid_t& g) {
		grid_t next_grid(g.size(), std::vector<char>(g[0].size(), ' '));
		for(std::size_t y = 1; y < g.size() - 1; ++y) {
			for(std::size_t x = 1; x < g[y].size() - 1; ++x) {
				const std::uint64_t neighbours = get_neighbours(g, x, y);
				const char next_symbol = iterate(g[y][x], neighbours);
				next_grid[y][x] = next_symbol;
			}
		}
		g.swap(next_grid);
	}

	void draw(grid_t& g) const {
		COORD top_left = { 0, 0 };
		::SetConsoleCursorPosition(::GetStdHandle(STD_OUTPUT_HANDLE), top_left);

		for(std::size_t y = 1; y < g.size() - 1; ++y) {
			for(std::size_t x = 1; x < g[y].size() - 1; ++x) {
				std::cout << g[y][x];
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	std::size_t get_resource_value(const grid_t& g) {
		const std::size_t trees = ranges::accumulate(g, 0ui64, std::plus<void>{}, [] (const std::vector<char> v) { return ranges::count(v, '|'); });
		const std::size_t yards = ranges::accumulate(g, 0ui64, std::plus<void>{}, [] (const std::vector<char> v) { return ranges::count(v, '#'); });
		const std::size_t resource_value = trees * yards;
		return resource_value;
	}

	std::string part_1() override {
		grid_t my_grid = original_grid;
		for(std::size_t i = 0ui64; i < 10ui64; ++i) {
			perform_round(my_grid);
		}
		return std::to_string(get_resource_value(my_grid));
	}

	std::string part_2() override {
		grid_t my_grid = original_grid;
		std::vector<grid_t> historic_grids;
		historic_grids.push_back(my_grid);
		for(;;) { 
			perform_round(my_grid);
			auto it = std::find(std::begin(historic_grids), std::end(historic_grids), my_grid);
			if(it != historic_grids.end()) {
				const std::ptrdiff_t dist      = std::distance(it, std::end(historic_grids));
				const std::size_t    remainder = (1'000'000'000ui64 - historic_grids.size()) % dist;
				const grid_t& g = *(std::end(historic_grids) - dist + remainder);
				return std::to_string(get_resource_value(g));
			}
			historic_grids.push_back(my_grid);
		}
	}
};

template<>
void solve<advent_year::year_2018, advent_day::day_18>() {
	advent_2018_18 a;
	a.solve();
}
