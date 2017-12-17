#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>

#include <boost/algorithm/string.hpp>

struct advent_4 : problem
{
	advent_4() noexcept : problem(4) {
	}

protected:
	std::vector<std::string> lines;

	void prepare_input() override {
		std::ifstream fin("day-4.txt");
		for(std::string line; std::getline(fin, line);) {
			lines.push_back(line);
		}
	}

	template<typename Fn>
	std::size_t core(Fn&& validate) {
		return gsl::narrow<std::size_t>(std::count_if(begin(lines), end(lines), [&](const std::string& line) {
			std::vector<std::string> words;
			boost::split(words, line, [](char c) { return c == ' '; });
			return validate(words);
		}));
	}

	std::string part_1() override {
		return std::to_string(core([](std::vector<std::string>& words) {
			std::sort(begin(words), end(words));
			return end(words) == std::unique(begin(words), end(words));
		}));
	}

	std::string part_2() override {
		return std::to_string(core([](std::vector<std::string>& words) {
			for(std::string& word : words) {
				std::sort(begin(word), end(word));
			}
			std::sort(begin(words), end(words));
			return end(words) == std::unique(begin(words), end(words));
		}));
	}
};

template<>
void solve<advent_day::day_4>() {
	advent_4 a;
	a.solve();
}
