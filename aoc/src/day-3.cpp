#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <unordered_map>
#include <utility>

struct advent_3 : problem
{
	advent_3() noexcept : problem(3) {
	}

protected:
	std::ptrdiff_t target = 0;

	void prepare_input() override {
		std::ifstream fin("day-3.txt");
		std::string line;
		std::getline(fin, line);
		target = std::stoll(line);
	}

	template<typename F>
	void spiral_iterate(F&& fn) noexcept {
		std::ptrdiff_t dx = 1;
		std::ptrdiff_t dy = 0;
		for(std::ptrdiff_t size = 1; true; ++size) {
			for(std::ptrdiff_t y = dy; y < dx; ++y) {
				if(!fn(dx, y)) {
					return;
				}
			}
			dy = dx;
			for(std::ptrdiff_t x = dx; x > -dx; --x) {
				if(!fn(x, dy)) {
					return;
				}
			}
			dx = -dx;
			for(std::ptrdiff_t y = dy; y > -dy; --y) {
				if(!fn(dx, y)) {
					return;
				}
			}
			dy = -dy;
			for(std::ptrdiff_t x = dx; x < -dx + 1; ++x) {
				if(!fn(x, dy)) {
					return;
				}
			}
			dx = -dx + 1;
		}
	}

	std::string part_1() override {
		// this is, of course, a silly way to solve this part, but it's the only way I can see to meaningfully share
		// the solution between parts 1 and parts 2. Badly designed question, IMO; the natural solution for the first
		// part is algebraic, rather than the brute force of the second part.

		std::ptrdiff_t current  = 1;
		std::ptrdiff_t distance = 0;
		spiral_iterate([&](std::ptrdiff_t x, std::ptrdiff_t y) {
			if(++current == target) {
				distance = std::abs(x) + std::abs(y);
				return false;
			}
			return true;
		});
		return std::to_string(gsl::narrow<std::size_t>(distance));
	}

	using coordinate = std::pair<std::ptrdiff_t, std::ptrdiff_t>;

	struct coordinate_hash
	{
		std::size_t operator()(const coordinate& c) const noexcept {
			return std::hash<std::ptrdiff_t>{}(c.first) | std::hash<std::ptrdiff_t>{}(c.second);
		}
	};

	using visited_map = std::unordered_map<coordinate, std::ptrdiff_t, coordinate_hash>;

	std::ptrdiff_t sum_neighbours(const visited_map& visited, std::ptrdiff_t x, std::ptrdiff_t y) {
		const auto end = visited.end();
		auto it        = visited.end();

		// clang-format off
		const std::ptrdiff_t sum = ((it = visited.find({x - 1, y - 1})) != end ? it->second : 0)
		                         + ((it = visited.find({x - 1, y    })) != end ? it->second : 0)
		                         + ((it = visited.find({x - 1, y + 1})) != end ? it->second : 0)
		                         + ((it = visited.find({x    , y - 1})) != end ? it->second : 0)
		                         + ((it = visited.find({x    , y + 1})) != end ? it->second : 0)
		                         + ((it = visited.find({x + 1, y - 1})) != end ? it->second : 0)
		                         + ((it = visited.find({x + 1, y    })) != end ? it->second : 0)
		                         + ((it = visited.find({x + 1, y + 1})) != end ? it->second : 0);
		// clang-format on
		return sum;
	}

	std::string part_2() override {
		visited_map visited;
		visited[coordinate{ 0, 0 }] = 1;
		std::ptrdiff_t next_highest = 0;
		spiral_iterate([&](std::ptrdiff_t x, std::ptrdiff_t y) {
			std::ptrdiff_t value = sum_neighbours(visited, x, y);
			if(value > target) {
				next_highest = value;
				return false;
			}
			visited[coordinate{ x, y }] = value;
			return true;
		});
		return std::to_string(gsl::narrow<std::size_t>(next_highest));
	}
};

template<>
void solve<advent_day::day_3>() {
	advent_3 a;
	a.solve();
}
