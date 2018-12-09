#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <optional>
#include <queue>

#include "utility.hpp"

#pragma warning(disable: 26434) // warning C26434: Function '%s' hides a non-virtual function '%s' (c.128).
#pragma warning(disable: 26487) // warning C26487: Don't return a pointer that may be invalid (lifetime.1).

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

constexpr bool is_open(char c) noexcept {
	return c > 'a';
}

struct direction
{
	enum
	{
		up,
		down,
		left,
		right
	};
};

struct state_t
{
	std::size_t moves;
	std::string path;
	coord position;
};

using path = std::optional<std::pair<std::string, std::string>>;

path shortest_path(const coord& position, const coord& destination, const std::string& passcode, const std::string& path_so_far) {
	
	if(position == destination) {
		return std::make_pair(path_so_far, path_so_far);
	}
	
	const std::string code = passcode + path_so_far;
	utility::hex_digest_type dig = utility::md5str(gsl::make_span(code.data(), gsl::narrow_cast<std::ptrdiff_t>(code.size())));

	const bool viable[4] = {
		is_open(dig[direction::up])    && position.y != 0,
		is_open(dig[direction::down])  && position.y != 3,
		is_open(dig[direction::left])  && position.x != 0,
		is_open(dig[direction::right]) && position.x != 3
	};

	path trials[] = {
		viable[direction::up   ] ? shortest_path(coord { position.x, position.y - 1}, destination, passcode, path_so_far + "U") : std::nullopt,
		viable[direction::down ] ? shortest_path(coord { position.x, position.y + 1}, destination, passcode, path_so_far + "D") : std::nullopt,
		viable[direction::left ] ? shortest_path(coord { position.x - 1, position.y}, destination, passcode, path_so_far + "L") : std::nullopt,
		viable[direction::right] ? shortest_path(coord { position.x + 1, position.y}, destination, passcode, path_so_far + "R") : std::nullopt
	};

	if(std::all_of(std::begin(trials), std::end(trials), [](const path& p) {
		return !p.has_value();
	})) {
		return std::nullopt;
	}

	std::string shrt;
	std::string lng;

	for(std::size_t i = 0; i < 4; ++i) {
		if(trials[i].has_value()) {
			if(shrt.size() == 0 || trials[i]->first.size() < shrt.size()) {
				shrt = trials[i]->first;
			}
			if(trials[i]->second.size() > lng.size()) {
				lng = trials[i]->second;
			}
		}
	}

	return std::make_pair(shrt, lng);
}

struct advent_2016_17 : problem
{
	advent_2016_17() noexcept : problem(2016, 17) {
	}

protected:
	std::string passcode;

	void prepare_input(std::ifstream& fin) override {
		std::getline(fin, passcode);
	}

	std::string shortest;
	std::string longest;

	void precompute() override {
		coord initial = { 0, 0 };
		coord target  = { 3, 3 };
		const path p = shortest_path(initial, target, passcode, "");
		if(p.has_value()) {
			shortest = p->first;
			longest = p->second;
		}
	}

	std::string part_1() override {
		return shortest;
	}

	std::string part_2() override {
		return std::to_string(longest.size());
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_17>() {
	advent_2016_17 a;
	a.solve();
}
