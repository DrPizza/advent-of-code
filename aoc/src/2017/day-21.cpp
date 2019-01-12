#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <string>
#include <regex>
#include <unordered_map>

#include <boost/algorithm/string.hpp>

struct advent_2017_21 : problem
{
	[[gsl::suppress(f.6)]]
	advent_2017_21() : problem(2017, 21) {
	}

protected:
	std::unordered_map<std::string, std::string> rewrite_rules;

	using grid = std::vector<std::vector<char>>;
	using semi_grid = std::vector<std::vector<std::string>>;

	grid linear_to_grid(const std::string& linear) {
		std::vector<std::string> partial_grid;
		boost::split(partial_grid, linear, [](char ch) {
			return ch == '/';
		});
		grid result;
		for(const std::string& s : partial_grid) {
			std::vector<char> cs(std::begin(s), std::end(s));
			result.push_back(cs);
		}
		return result;
	}

	std::string grid_to_linear(const grid& g) {
		std::string linear;
		for(const auto& v : g) {
			for(const auto& c : v) {
				linear += c;
			}
			linear += '/';
		}
		return linear.substr(0, linear.size() - 1);
	}

	semi_grid split_grid(const std::string& in) {
		const std::size_t size = in.find('/');
		const std::size_t subgrid_size = (size % 2 == 0) ? 2u : 3u;
		const std::size_t fragment_count = (size % 2 == 0) ? size / 2 : size / 3;
		semi_grid fragments;
		fragments.resize(fragment_count);
		for(std::vector<std::string>& v : fragments) {
			v.resize(fragment_count);
		}
		grid source = linear_to_grid(in);
		for(std::size_t i = 0; i < fragment_count; ++i) {
			for(std::size_t j = 0; j < fragment_count; ++j) {
				for(std::size_t k = 0; k < subgrid_size; ++k) {
					for(std::size_t l = 0; l < subgrid_size; ++l) {
						fragments[i][j] += source[k + (i * subgrid_size)][l + (j * subgrid_size)];
					}
					if(k != subgrid_size - 1) {
						fragments[i][j] += '/';
					}
				}
			}
		}
		return fragments;
	}

	std::string unsplit_grid(const semi_grid& g) {
		std::string result;
		for(std::size_t major_row = 0; major_row < g.size(); ++major_row) {
			std::string row;
			std::vector<grid> subgrids;
			for(std::size_t major_col = 0; major_col < g.size(); ++major_col) {
				subgrids.push_back(linear_to_grid(g[major_row][major_col]));
			}

			for(std::size_t sub_row = 0; sub_row < subgrids[0].size(); ++sub_row) {
				for(std::size_t major_col = 0; major_col < g.size(); ++major_col) {
					for(std::size_t sub_col = 0; sub_col < subgrids[0].size(); ++sub_col) {
						row += subgrids[major_col][sub_row][sub_col];
					}
				}
				if(sub_row != subgrids[0].size() && major_row != g.size()) {
					row += '/';
				}
			}
			result += row;
		}
		return result;
	}

	grid flip_up_down(const grid& g) {
		grid result = g;
		if(g.size() == 2) {
			result[0][0] = g[1][0];
			result[0][1] = g[1][1];
			result[1][0] = g[0][0];
			result[1][1] = g[0][1];
		} else {
			result[0][0] = g[2][0];
			result[0][1] = g[2][1];
			result[0][2] = g[2][2];
			result[1][0] = g[1][0];
			result[1][1] = g[1][1];
			result[1][2] = g[1][2];
			result[2][0] = g[0][0];
			result[2][1] = g[0][1];
			result[2][2] = g[0][2];
		}
		return result;
	}

	grid flip_left_right(const grid& g) {
		grid result = g;
		if(g.size() == 2) {
			result[0][0] = g[0][1];
			result[0][1] = g[0][0];
			result[1][0] = g[1][1];
			result[1][1] = g[1][0];
		} else {
			result[0][0] = g[0][2];
			result[0][1] = g[0][1];
			result[0][2] = g[0][0];
			result[1][0] = g[1][2];
			result[1][1] = g[1][1];
			result[1][2] = g[1][0];
			result[2][0] = g[2][2];
			result[2][1] = g[2][1];
			result[2][2] = g[2][0];
		}
		return result;
	}

	grid rotate(const grid& g) {
		grid result = g;
		if(g.size() == 2) {
			result[0][0] = g[1][0];
			result[0][1] = g[0][0];
			result[1][0] = g[1][1];
			result[1][1] = g[0][1];
		} else {
			result[0][0] = g[2][0];
			result[0][1] = g[1][0];
			result[0][2] = g[0][0];
			result[1][0] = g[2][1];
			result[1][1] = g[1][1];
			result[1][2] = g[0][1];
			result[2][0] = g[2][2];
			result[2][1] = g[1][2];
			result[2][2] = g[0][2];
		}
		return result;
	}

	std::vector<std::string> generate_variations(const std::string& s) {
		std::vector<std::string> results;
		auto g = linear_to_grid(s);
		// blindly generate all the rotations and flips; duplicates will get removed by the map.
		for(std::size_t i = 0; i < 4; ++i) {
			results.push_back(grid_to_linear(g));
			results.push_back(grid_to_linear(flip_up_down(g)));
			results.push_back(grid_to_linear(flip_left_right(g)));
			results.push_back(grid_to_linear(flip_left_right(flip_up_down(g))));
			g = rotate(g);
		}
		return results;
	}

	void prepare_input(std::ifstream& fin) override {
		std::regex short_pattern(R"((../..) => (.../.../...))");
		std::regex long_pattern (R"((.../.../...) => (..../..../..../....))");
		for(std::string line; std::getline(fin, line); ) {
			std::smatch m;
			std::regex_search(line, m, short_pattern) || std::regex_search(line, m, long_pattern);
			const std::vector<std::string> variations = generate_variations(m[1].str());
			const std::string target = m[2].str();
			for(const std::string& var : variations) {
				rewrite_rules[var] = target;
			}
		}
	}

	const std::string initial = ".#./..#/###";

	std::string do_art(std::size_t iterations) {
		std::string picture = initial;
		for(std::size_t i = 0; i < iterations; ++i) {
			semi_grid split = split_grid(picture);
			for(auto& r : split) {
				for(auto& c : r) {
					c = rewrite_rules[c];
				}
			}
			picture = unsplit_grid(split);
		}
		return picture;
	}

	std::string part_1() override {
		const std::string art = do_art(5);
		const std::ptrdiff_t lit = std::count(std::begin(art), std::end(art), '#');
		return std::to_string(lit);
	}

	std::string part_2() override {
		const std::string art = do_art(18);
		const std::ptrdiff_t lit = std::count(std::begin(art), std::end(art), '#');
		return std::to_string(lit);
	}
};

REGISTER_SOLVER(2017, 21);
