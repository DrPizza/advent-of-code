#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <regex>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <boost/algorithm/string.hpp>

struct advent_15 : problem
{
	advent_15() noexcept : problem(15) {
	}

	struct generator
	{
		std::string name;
		uint64_t seed;
		uint64_t factor;
		uint64_t current;

		generator(const std::string& name_, uint64_t seed_, uint64_t factor_) : name(name_), seed(seed_), factor(factor_), current(seed_) {
		}

		uint16_t generate(uint64_t divisor) noexcept {
			for(;;) {
				const uint64_t product = current * factor;
				current = product % 2'147'483'647ui64;
				if((current % divisor) == 0ui64) {
					return current & 0xffff;
				}
			}
		}

		void reset() noexcept {
			current = seed;
		}
	};

	std::unique_ptr<generator> a;
	std::unique_ptr<generator> b;

	void prepare_input() override {
		std::ifstream fin("day-15.txt");
		std::regex pattern(R"(Generator ([[:upper:]]) starts with ([[:digit:]]*))");
		std::string line;
		std::getline(fin, line);
		std::smatch m;
		std::regex_search(line, m, pattern);
		a = std::make_unique<generator>(m[1], std::stoul(m[2]), 16'807);
		std::getline(fin, line);
		std::regex_search(line, m, pattern);
		b = std::make_unique<generator>(m[1], std::stoul(m[2]), 48'271);
	}

	std::string part_1() override {
		std::size_t pairs = 0;
		for(std::size_t i = 0; i < 40'000'000; ++i) {
			pairs += a->generate(1) == b->generate(1);
		}
		return std::to_string(pairs);
	}

	virtual void tidy_up() noexcept {
		a->reset();
		b->reset();
	}

	std::string part_2() override {
		std::size_t pairs = 0;
		for(std::size_t i = 0; i < 5'000'000; ++i) {
			pairs += a->generate(4) == b->generate(8);
		}
		return std::to_string(pairs);
	}
};

template<>
void solve<advent_day::day_15>() {
	advent_15 a;
	a.solve();
}
