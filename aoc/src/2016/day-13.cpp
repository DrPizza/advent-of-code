#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <unordered_map>
#include <unordered_set>

struct coord
{
	std::size_t x;
	std::size_t y;
};

constexpr bool operator==(const coord& lhs, const coord& rhs) noexcept {
	return lhs.x == rhs.x && lhs.y == rhs.y;
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

enum struct block_type
{
	open,
	wall
};

block_type get_block_type(const coord& c, std::size_t favourite) noexcept {
	std::size_t eqn = (c.x * c.x) + (3 * c.x) + (2 * c.x * c.y) + c.y + (c.y * c.y);
	eqn += favourite;

	return gsl::narrow_cast<block_type>(__popcnt64(eqn) % 2);
}

using office_type = std::unordered_map<coord, block_type>;
using memo_type   = std::unordered_set<coord>;

std::size_t shortest_path(const coord& position, const coord& destination, std::size_t dist, std::size_t favourite, office_type& office, memo_type& memo, memo_type& all_visited, std::size_t recursion_limit) {
	if(position == destination) {
		return dist;
	}
	if(memo.end() !=  memo.find(position)) {
		return std::numeric_limits<std::size_t>::max();
	}
	if(position.x == std::numeric_limits<std::size_t>::max()) {
		return std::numeric_limits<std::size_t>::max();
	}
	if(position.y == std::numeric_limits<std::size_t>::max()) {
		return std::numeric_limits<std::size_t>::max();
	}
	auto it = office.find(position);
	if(it == office.end()) {
		it = office.insert(std::make_pair(position, get_block_type(position, favourite))).first;
	}
	if(it->second == block_type::wall) {
		return std::numeric_limits<std::size_t>::max();
	}

	all_visited.insert(position);
	if(0 == recursion_limit) {
		return std::numeric_limits<std::size_t>::max();
	}

	const coord next_locations[] = {
		{ position.x + 1ui64, position.y         },
		{ position.x        , position.y + 1ui64 },
		{ position.x - 1ui64, position.y         },
		{ position.x        , position.y - 1ui64 }
	};

	memo.insert(position);
	std::size_t trials[] = {
		shortest_path(next_locations[0], destination, dist + 1, favourite, office, memo, all_visited, recursion_limit - 1),
		shortest_path(next_locations[1], destination, dist + 1, favourite, office, memo, all_visited, recursion_limit - 1),
		shortest_path(next_locations[2], destination, dist + 1, favourite, office, memo, all_visited, recursion_limit - 1),
		shortest_path(next_locations[3], destination, dist + 1, favourite, office, memo, all_visited, recursion_limit - 1)
	};
	memo.erase(position);

	return *std::min_element(std::begin(trials), std::end(trials));
}

struct advent_2016_13 : problem
{
	advent_2016_13() noexcept : problem(2016, 13) {
	}

protected:
	std::size_t favourite;

	void prepare_input(std::ifstream& fin) override {
		std::string line;
		std::getline(fin, line);
		favourite = std::stoull(line);
	}

	std::string part_1() override {
		coord initial = { 1, 1 };
		coord target = { 31, 39 };
		office_type office;
		memo_type memo;
		memo_type all_visited;
		office[initial] = block_type::open;

		const std::size_t shortest = shortest_path(initial, target, 0ui64, favourite, office, memo, all_visited, std::numeric_limits<std::size_t>::max());
		return std::to_string(shortest);
	}

	std::string part_2() override {
		coord initial = { 1, 1 };
		coord target = { 31, 39 };
		office_type office;
		memo_type memo;
		memo_type all_visited;
		office[initial] = block_type::open;

		shortest_path(initial, target, 0ui64, favourite, office, memo, all_visited, 50);
		return std::to_string(all_visited.size());
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_13>() {
	advent_2016_13 a;
	a.solve();
}
