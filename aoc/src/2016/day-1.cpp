#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <set>

#include <boost/algorithm/string.hpp>

struct advent_2016_1 : problem
{
	advent_2016_1() noexcept : problem(2016, 1) {
	}

protected:
	enum struct direction
	{
		north, east, south, west
	};

	direction turn_left(direction d) noexcept {
		switch(d) {
		case direction::north:
			return direction::west;
		case direction::east:
			return direction::north;
		case direction::south:
			return direction::east;
		case direction::west:
			return direction::south;
		}
		__assume(0);
	}

	direction turn_right(direction d) noexcept {
		switch(d) {
		case direction::north:
			return direction::east;
		case direction::east:
			return direction::south;
		case direction::south:
			return direction::west;
		case direction::west:
			return direction::north;
		}
		__assume(0);
	}

	enum struct turn
	{
		left,
		right
	};

	direction make_turn(direction d, turn t) noexcept {
		switch(t) {
		case turn::left:
			return turn_left(d);
			break;
		case turn::right:
			return turn_right(d);
			break;
		}
		__assume(0);
	}

	struct instruction
	{
		turn trn;
		std::ptrdiff_t distance;
	};

	std::vector<instruction> instructions;

	void prepare_input(std::ifstream& fin) override {
		std::string line;
		std::getline(fin, line);
		std::vector<std::string> raw_instructions;
		boost::split(raw_instructions, line, [](char c) {
			return c == ' ';
		});
		for(const std::string& s : raw_instructions) {
			instructions.push_back(instruction{ s[0] == 'L' ? turn::left : turn::right, std::stoll(s.substr(1, s.find(','))) });
		}
	}

	using coordinate = std::pair<std::ptrdiff_t, std::ptrdiff_t>;

	coordinate forward(coordinate pos, direction dir, std::ptrdiff_t dist) noexcept {
		switch(dir) {
		case direction::north:
			pos.second += dist;
			break;
		case direction::east:
			pos.first += dist;
			break;
		case direction::south:
			pos.second -= dist;
			break;
		case direction::west:
			pos.first -= dist;
			break;
		}
		return pos;
	}

	std::string part_1() override {
		coordinate position = { 0, 0 };
		direction dir = direction::north;
		for(const instruction& i : instructions) {
			dir = make_turn(dir, i.trn);
			position = forward(position, dir, i.distance);
		}
		const std::ptrdiff_t manhattan_distance = std::abs(position.first) + std::abs(position.second);
		return std::to_string(manhattan_distance);
	}

	std::string part_2() override {
		std::set<coordinate> past_locations;
		coordinate position = { 0, 0 };
		past_locations.insert(position);
		direction dir = direction::north;
		for(const instruction& i : instructions) {
			dir = make_turn(dir, i.trn);
			for(std::ptrdiff_t j = 0; j < i.distance; ++j) {
				switch(dir) {
				case direction::north:
					position.second += 1;
					break;
				case direction::east:
					position.first += 1;
					break;
				case direction::south:
					position.second -= 1;
					break;
				case direction::west:
					position.first -= 1;
					break;
				}

				if(past_locations.insert(position).second == false) {
					const std::ptrdiff_t manhattan_distance = std::abs(position.first) + std::abs(position.second);
					return std::to_string(manhattan_distance);
				}
			}
		}
		__assume(0);
	}
};

REGISTER_SOLVER(2016, 1);
