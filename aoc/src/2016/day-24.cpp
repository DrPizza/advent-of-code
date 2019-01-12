#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <unordered_map>
#include <queue>

namespace
{

	struct coord
	{
		std::size_t r;
		std::size_t c;
	};

	constexpr bool operator==(const coord& lhs, const coord& rhs) noexcept {
		return lhs.r == rhs.r && lhs.c == rhs.c;
	}
}

namespace std {
	template<>
	struct hash<coord>
	{
		std::size_t operator()(const coord& c) const noexcept {
			return std::hash<std::size_t>{}(c.r) ^ std::hash<std::size_t>{}(c.c);
		}
	};
}

namespace
{

	struct block
	{
		bool passable;
		bool has_number;
		std::size_t number;
	};

	using grid_type = std::vector<std::vector<block>>;
	using memo_type = std::vector<std::vector<bool>>;
	struct state_t
	{
		coord pos;
		std::size_t distance;
	};

	std::size_t shortest_path(coord position, coord target, grid_type& grid) {
		std::queue<state_t> q;
		q.push(state_t{ position, 0 });
		memo_type visited;
		visited.resize(grid.size());
		for(std::vector<bool>& v : visited) {
			v.resize(grid[0].size(), false);
		}

		while(!q.empty()) {
			const state_t s = q.front();
			q.pop();

			if(s.pos == target) {
				return s.distance;
			}

			if(grid[s.pos.r][s.pos.c - 1].passable && !visited[s.pos.r][s.pos.c - 1]) {
				q.push({ { s.pos.r, s.pos.c - 1 }, s.distance + 1 });
				visited[s.pos.r][s.pos.c - 1] = true;
			}
			if(grid[s.pos.r][s.pos.c + 1].passable && !visited[s.pos.r][s.pos.c + 1]) {
				q.push({ { s.pos.r, s.pos.c + 1 }, s.distance + 1 });
				visited[s.pos.r][s.pos.c + 1] = true;
			}
			if(grid[s.pos.r - 1][s.pos.c].passable && !visited[s.pos.r - 1][s.pos.c]) {
				q.push({ { s.pos.r - 1, s.pos.c }, s.distance + 1 });
				visited[s.pos.r - 1][s.pos.c] = true;
			}
			if(grid[s.pos.r + 1][s.pos.c].passable && !visited[s.pos.r + 1][s.pos.c]) {
				q.push({ { s.pos.r + 1, s.pos.c }, s.distance + 1 });
				visited[s.pos.r + 1][s.pos.c] = true;
			}
		}
		return std::numeric_limits<std::size_t>::max();
	}
}

#pragma warning(push)
#pragma warning(disable: 4709) // warning C4709: comma operator within array index expression

struct advent_2016_24 : problem
{
	advent_2016_24() noexcept : problem(2016, 24) {
	}

protected:
	grid_type grid;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line); ) {
			std::vector<block> row;
			for(const char c : line) {
				block b{ c == '#' ? false : true, (c != '#' && c != '.') ? true : false, (c != '#' && c != '.') ? gsl::narrow<std::size_t>(c - '0') : 0ui64 };
				row.push_back(b);
			}
			grid.push_back(row);
		}
	}

	std::unordered_map<coord, std::size_t> pairwise_distances;

	void precompute() override {
		std::unordered_map<std::size_t, coord> number_positions;
		for(std::size_t r = 0; r < grid.size(); ++r) {
			for(std::size_t c = 0; c < grid[r].size(); ++c) {
				if(grid[r][c].has_number) {
					number_positions[grid[r][c].number] = coord{ r, c };
				}
			}
		}
		for(std::size_t i = 0; i < number_positions.size(); ++i) {
			for(std::size_t j = i + 1; j < number_positions.size(); ++j) {
				if(i == j) {
					continue;
				}
				const std::size_t distance = shortest_path(number_positions[i], number_positions[j], grid);
				pairwise_distances[coord{ i, j }] = distance;
				pairwise_distances[coord{ j, i }] = distance;
			}
		}
	}

	std::string part_1() override {
		std::vector<std::size_t> order(7);
		std::iota(std::begin(order), std::end(order), 1ui64);
		std::size_t shortest = std::numeric_limits<std::size_t>::max();
		do {
			std::size_t distance = 0;
			std::size_t first = 0;
			for(const std::size_t last : order) {
				distance += pairwise_distances[coord{ first, last }];
				first = last;
			}
			shortest = std::min(shortest, distance);
		} while(std::next_permutation(std::begin(order), std::end(order)));
		return std::to_string(shortest);
	}

	std::string part_2() override {
		std::vector<std::size_t> order(7);
		std::iota(std::begin(order), std::end(order), 1ui64);
		std::size_t shortest = std::numeric_limits<std::size_t>::max();
		do {
			std::size_t distance = 0;
			std::size_t first = 0;
			for(const std::size_t last : order) {
				distance += pairwise_distances[coord{ first, last }];
				first = last;
			}
			distance += pairwise_distances[coord{ first, 0 }];
			shortest = std::min(shortest, distance);
		} while(std::next_permutation(std::begin(order), std::end(order)));
		return std::to_string(shortest);
	}
};

#pragma warning(pop)

REGISTER_SOLVER(2016, 24);
