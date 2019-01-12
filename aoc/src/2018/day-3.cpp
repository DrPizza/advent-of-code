#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <regex>
#include <map>

struct advent_2018_3 : problem
{
	[[gsl::suppress(f.6)]]
	advent_2018_3() : problem(2018, 3) {
	}

protected:
	struct claim
	{
		std::uintmax_t id;
		std::uintmax_t left;
		std::uintmax_t top;
		std::uintmax_t width;
		std::uintmax_t height;
	};

	std::vector<claim> claims;

	void prepare_input(std::ifstream& fin) override {
		std::regex pattern(R"(#([[:digit:]]+) @ ([[:digit:]]+),([[:digit:]]+): ([[:digit:]]+)x([[:digit:]]+))");
		for(std::string line; std::getline(fin, line);) {
			std::smatch m;
			std::regex_search(line, m, pattern);
			const std::uintmax_t id = std::stoull(m[1]);
			const std::uintmax_t left = std::stoull(m[2]);
			const std::uintmax_t top = std::stoull(m[3]);
			const std::uintmax_t width = std::stoull(m[4]);
			const std::uintmax_t height = std::stoull(m[5]);

			claim c = { id, left, top, width, height };
			claims.push_back(c);
		}
	}

	std::map<std::uintmax_t, std::map<std::uintmax_t, std::uintmax_t>> quilt;

	void precompute() override {
		for(const claim& c : claims) {
			for(std::uintmax_t w = c.left; w != c.left + c.width; ++w) {
				for(std::uintmax_t h = c.top; h != c.top + c.height; ++h) {
					++quilt[w][h];
				}
			}
		}
	}

	std::string part_1() override {
		std::uintmax_t overlaps = 0;
		for(const auto& r : quilt) {
			for(const auto& c : r.second) {
				if(c.second > 1) {
					++overlaps;
				}
			}
		}
		return std::to_string(overlaps);
	}

	std::string part_2() override {
		for(const claim& c : claims) {
			bool intact = true;
			for(std::uintmax_t w = c.left; w != c.left + c.width; ++w) {
				for(std::uintmax_t h = c.top; h != c.top + c.height; ++h) {
					if(quilt[w][h] != 1) {
						intact = false;
					}
				}
			}
			if(intact) {
				return std::to_string(c.id);
			}
		}
		__assume(0);
	}
};

REGISTER_SOLVER(2018, 3);
