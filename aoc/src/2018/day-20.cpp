#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <utility>

namespace
{
	struct point
	{
		int c, r;

		point& operator+=(const point& rhs) noexcept {
			c += rhs.c;
			r += rhs.r;
			return *this;
		}
		point operator+(const point& rhs) const noexcept {
			return { c + rhs.c, r + rhs.r };
		}
		bool operator==(const point& rhs) const noexcept {
			return std::tie(r, c) == std::tie(rhs.r, rhs.c);
		}
		bool operator<(const point& rhs) const noexcept {
			return std::tie(r, c) < std::tie(rhs.r, rhs.c);
		}
	};
}

struct advent_2018_20 : problem
{
	advent_2018_20() noexcept : problem(2018, 20) {
	}

protected:
	std::string expression;
	std::map<std::ptrdiff_t, std::map<std::ptrdiff_t, char>> maze;
	std::size_t longest = 0ui64;
	std::size_t at_least_1000 = 0ui64;

	void prepare_input(std::ifstream& fin) override {
		std::getline(fin, expression);
	}

	void precompute() override {
		maze[0][0] = 'X';
		generate_map(expression.begin(), expression.end(), { 0, 0 });
		const auto tup = breadth_first_search({ 0, 0 });
		longest        = std::get<0>(tup);
		at_least_1000  = std::get<1>(tup);
	}

	void generate_map(std::string::const_iterator it, std::string::const_iterator end, point pos) {
		const std::map<char, point> directions = {
			{ 'N', {  0, -1 } },
			{ 'E', {  1,  0 } },
			{ 'S', {  0,  1 } },
			{ 'W', { -1,  0 } }
		};
		const std::map<char, char> door_type = {
			{ 'N', '-' },
			{ 'E', '|' },
			{ 'S', '-' },
			{ 'W', '|' }
		};

		for(; it != end; ++it) {
			switch(*it) {
			case '^':
			case '$':
				break;
			case 'N':
			case 'E':
			case 'S':
			case 'W':
				{
					const point& dir = directions.at(*it);
					const point door = pos + dir;
					maze[door.r][door.c] = door_type.at(*it);
					const point room = door + dir;
					maze[room.r][room.c] = '.';
					pos = room;
				}
				break;
			case '(':
				{
					std::size_t opens = 0ui64;
					std::string::const_iterator close = it;
					std::vector<std::string::const_iterator> choices;
					for(; close != end; ++close) {
						if(*close == '(') {
							++opens;
						}
						if(*close == '|' && opens == 1ui64) {
							choices.push_back(close);
						}
						if(*close == ')') {
							--opens;
						}
						if(0ui64 == opens) {
							break;
						}
					}
					std::string::const_iterator b = std::next(it);
					std::string::const_iterator e = close;
					for(auto i : choices) {
						generate_map(b, i, pos);
						b = std::next(i);
					}
					generate_map(b, close, pos);
					it = close;
				}
				break;
			}
		}
	}

	void draw() const {
		COORD top_left = { 0, 0 };
		::SetConsoleCursorPosition(::GetStdHandle(STD_OUTPUT_HANDLE), top_left);
		std::ptrdiff_t x_min = std::numeric_limits<std::ptrdiff_t>::max();
		std::ptrdiff_t x_max = std::numeric_limits<std::ptrdiff_t>::min();
		std::ptrdiff_t y_min = std::numeric_limits<std::ptrdiff_t>::max();
		std::ptrdiff_t y_max = std::numeric_limits<std::ptrdiff_t>::min();
		for(const auto& p : maze) {
			y_min = std::min(y_min, p.first);
			y_max = std::max(y_max, p.first);
			for(const auto& q : p.second) {
				x_min = std::min(x_min, q.first);
				x_max = std::max(x_max, q.first);
			}
		}

		for(std::ptrdiff_t x = x_min; x <= x_max + 2; ++x) {
			std::cout << '#';
		}
		std::cout << std::endl;
		for(std::ptrdiff_t y = y_min; y <= y_max; ++y) {
			std::cout << '#';
			for(std::ptrdiff_t x = x_min; x <= x_max; ++x) {
				if(maze.count(y) && maze.at(y).count(x)) {
					std::cout << maze.at(y).at(x);
				} else {
					std::cout << "#";
				}
			}
			std::cout << '#';
			std::cout << std::endl;
		}
		for(std::ptrdiff_t x = x_min; x <= x_max + 2; ++x) {
			std::cout << '#';
		}
		std::cout << std::endl;
	}

	std::tuple<std::size_t, std::size_t> breadth_first_search(const point& initial) const {
		const std::vector<point> dirs{ { 0, -1 }, { -1, 0 }, { 1, 0 }, { 0, 1 } };
		struct path
		{
			point pos;
			std::size_t length;
		};
		std::set<point> visited;
		std::queue<path> candidates;
		std::size_t lngst = 0ui64;
		std::size_t at_least_1k = 0ui64;
		candidates.push({ { initial.c, initial.r}, lngst });
		while(!candidates.empty()) {
			const path p = candidates.front();
			candidates.pop();
			if(visited.count(p.pos)) {
				continue;
			}
			visited.insert(p.pos);
			lngst = std::max(p.length, lngst);
			if(p.length >= 1'000ui64) {
				++at_least_1k;
			}
			for(const point dir : dirs) {
				const point door = p.pos + dir;
				const point next = door + dir;
				if(maze.count(door.r) && maze.at(door.r).count(door.c) && (maze.at(door.r).at(door.c) == '-' || maze.at(door.r).at(door.c) == '|')) {
					candidates.push({ next, p.length + 1 });
				}
			}
		}
		return { lngst, at_least_1k };
	}

	std::string part_1() override {
		return std::to_string(longest);
	}

	std::string part_2() override {
		return std::to_string(at_least_1000);
	}
};

REGISTER_SOLVER(2018, 20);
