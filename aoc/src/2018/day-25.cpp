#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <vector>

namespace
{
	struct point
	{
		std::intmax_t x;
		std::intmax_t y;
		std::intmax_t z;
		std::intmax_t t;

		friend bool operator==(const point& lhs, const point& rhs) noexcept {
			return std::tie(lhs.x, lhs.y, lhs.z, lhs.t) == std::tie(rhs.x, rhs.y, rhs.z, rhs.t);
		}
	};

	std::intmax_t manhattan(const point& lhs, const point& rhs) noexcept {
		return std::abs(lhs.x - rhs.x) + std::abs(lhs.y - rhs.y) + std::abs(lhs.z - rhs.z) + std::abs(lhs.t - rhs.t);
	}
}

namespace std {
	template<>
	struct hash<point>
	{
		std::size_t operator()(const point& c) const noexcept {
			return std::hash<std::size_t>{}(gsl::narrow_cast<std::size_t>(c.x))
			     ^ std::hash<std::size_t>{}(gsl::narrow_cast<std::size_t>(c.y))
			     ^ std::hash<std::size_t>{}(gsl::narrow_cast<std::size_t>(c.z))
			     ^ std::hash<std::size_t>{}(gsl::narrow_cast<std::size_t>(c.t));
		}
	};
}

struct advent_2018_25 : problem
{
	advent_2018_25() noexcept : problem(2018, 25) {
	}

protected:
	std::vector<point> points;

	utility::disjoint_set<point> constellations;

	void prepare_input(std::ifstream& fin) override {
		const std::regex pattern(R"((-?[[:digit:]]+),(-?[[:digit:]]+),(-?[[:digit:]]+),(-?[[:digit:]]+))");
		for(std::string line; std::getline(fin, line); ) {
			std::smatch m;
			std::regex_match(line, m, pattern);
			points.push_back({ std::stoll(m[1]), std::stoll(m[2]), std::stoll(m[3]), std::stoll(m[4]) });
			constellations.insert_set(points.back());
		}
	}

	std::string part_1() override {
		for(std::size_t i = 0; i < points.size(); ++i) {
			for(std::size_t j = i + 1; j < points.size(); ++j) {
				if(manhattan(points[i], points[j]) <= 3) {
					constellations.merge_sets(points[i], points[j]);
				}
			}
		}
		return std::to_string(constellations.count_sets());
	}

	std::string part_2() override {
		return "*";
	}
};

REGISTER_SOLVER(2018, 25);
