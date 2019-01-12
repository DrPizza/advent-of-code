#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <string>
#include <unordered_map>

#include <boost/functional/hash.hpp>

struct advent_2017_22 : problem
{
	advent_2017_22() noexcept : problem(2017, 22) {
	}

	using coordinate = std::pair<int32_t, int32_t>;

	enum struct state : uint8_t
	{
		clean,
		infected,
		weakened,
		flagged
	};

	struct hasher
	{
		std::size_t operator()(std::pair<int32_t, int32_t> p) const {
			return boost::hash_value(p);
		}
	};

	using infection_map = std::unordered_map<coordinate, state, hasher>;

	infection_map original_infections;

	void prepare_input(std::ifstream& fin) override {
		std::vector<std::string> lines;
		for(std::string line; std::getline(fin, line);) {
			lines.push_back(line);
		}

		const std::ptrdiff_t height = gsl::narrow<std::ptrdiff_t>(lines.size());
		const std::ptrdiff_t width = gsl::narrow<std::ptrdiff_t>(lines[0].size());
		const std::ptrdiff_t h_offset = height / 2;
		const std::ptrdiff_t w_offset = width / 2;

		for(std::size_t i = 0; i < lines.size(); ++i) {
			for(std::size_t j = 0; j < lines[0].size(); ++j) {
				original_infections[{-gsl::narrow_cast<int32_t>(w_offset - j), -gsl::narrow_cast<int32_t>(i - h_offset)}] = lines[i][j] == '#' ? state::infected : state::clean;
			}
		}
	}

	enum struct direction
	{
		up, right, down, left
	};

	struct carrier
	{
		coordinate location = { 0, 0 };
		direction dir = direction::up;
		std::size_t infecting_bursts = 0;
	};


	constexpr direction turn_right(direction d) const noexcept {
		switch(d) {
		case direction::up:
			return direction::right;
		case direction::right:
			return direction::down;
		case direction::down:
			return direction::left;
		case direction::left:
			return direction::up;
		}
		__assume(0);
	}

	constexpr direction turn_left(direction d) const noexcept {
		switch(d) {
		case direction::up:
			return direction::left;
		case direction::right:
			return direction::up;
		case direction::down:
			return direction::right;
		case direction::left:
			return direction::down;
		}
		__assume(0);
	}

	constexpr direction reverse(direction d) const noexcept {
		switch(d) {
		case direction::up:
			return direction::down;
		case direction::right:
			return direction::left;
		case direction::down:
			return direction::up;
		case direction::left:
			return direction::right;
		}
		__assume(0);
	}

	constexpr coordinate move_forward(coordinate c, direction d) {
		switch(d) {
		case direction::up:
			return { c.first, c.second + 1};
		case direction::right:
			return { c.first + 1, c.second };
		case direction::down:
			return { c.first, c.second - 1 };
		case direction::left:
			return { c.first - 1, c.second };
		}
		__assume(0);
	}

	void burst(carrier& car, infection_map& infections) {
		state& s = infections[car.location];
		switch(s) {
		case state::clean:
			s = state::infected;
			car.dir = turn_left(car.dir);
			++car.infecting_bursts;
			break;
		case state::infected:
			s = state::clean;
			car.dir = turn_right(car.dir);
			break;
		case state::weakened:
		case state::flagged:
			__assume(0);
		}
		car.location = move_forward(car.location, car.dir);
	}

	std::string part_1() override {
		infection_map infections(original_infections);
		carrier car;
		for(std::size_t i = 0; i < 10'000; ++i) {
			burst(car, infections);
		}
		return std::to_string(car.infecting_bursts);
	}

	void modified_burst(carrier& car, infection_map& infections) {
		state& s = infections[car.location];
		switch(s) {
		case state::clean:
			s = state::weakened;
			car.dir = turn_left(car.dir);
			break;
		case state::weakened:
			s = state::infected;
			++car.infecting_bursts;
			break;
		case state::infected:
			s = state::flagged;
			car.dir = turn_right(car.dir);
			break;
		case state::flagged:
			s = state::clean;
			car.dir = reverse(car.dir);
			break;
		}
		car.location = move_forward(car.location, car.dir);
	}

	std::string part_2() override {
		infection_map infections(original_infections);
		carrier car;
		for(std::size_t i = 0; i < 10'000'000; ++i) {
			modified_burst(car, infections);
		}
		return std::to_string(car.infecting_bursts);
	}
};

REGISTER_SOLVER(2017, 22);
