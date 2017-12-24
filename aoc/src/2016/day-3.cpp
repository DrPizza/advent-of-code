#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <regex>

struct advent_2016_3 : problem
{
	advent_2016_3() noexcept : problem(2016, 3) {
	}

protected:
	struct triangle
	{
		std::array<std::size_t, 3> sides;
	};

	std::vector<triangle> triangles;

	void prepare_input(std::ifstream& fin) override {
		std::regex pattern(R"([[:space:]]+([[:digit:]]+)[[:space:]]+([[:digit:]]+)[[:space:]]+([[:digit:]]+))");
		for(std::string line; std::getline(fin, line);) {
			std::smatch m;
			std::regex_search(line, m, pattern);
			triangles.push_back(triangle{
				{ std::stoull(m[1].str()), std::stoull(m[2].str()), std::stoull(m[3].str()) }
			});
		}
	}

	std::ptrdiff_t count_valid_triangles(std::vector<triangle> ts) {
		for(triangle& t : ts) {
			std::sort(std::begin(t.sides), std::end(t.sides));
		}
		return std::count_if(std::begin(ts), std::end(ts), [](const triangle& t) {
			return t.sides[0] + t.sides[1] > t.sides[2];
		});
	}

	std::string part_1() override {
		const std::ptrdiff_t valid_triangles = count_valid_triangles(triangles);
		return std::to_string(valid_triangles);
	}
	
	std::string part_2() override {
		std::vector<triangle> vertical_triangles;
		
		for(std::size_t i = 0; i < triangles.size(); i += 3) {
			vertical_triangles.push_back(triangle{
				{ triangles[i].sides[0], triangles[i + 1].sides[0], triangles[i + 2].sides[0] }
			});
			vertical_triangles.push_back(triangle{
				{ triangles[i].sides[1], triangles[i + 1].sides[1], triangles[i + 2].sides[1] }
			});
			vertical_triangles.push_back(triangle{
				{ triangles[i].sides[2], triangles[i + 1].sides[2], triangles[i + 2].sides[2] }
			});
		}
		const std::ptrdiff_t valid_triangles = count_valid_triangles(vertical_triangles);
		return std::to_string(valid_triangles);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_3>() {
	advent_2016_3 a;
	a.solve();
}
