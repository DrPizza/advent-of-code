#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <utility>
#include <vector>

#include <boost/algorithm/string.hpp>

struct advent_2017_6 : problem
{
	advent_2017_6() noexcept : problem(2017, 6) {
	}

	using memory_type = std::vector<std::size_t>;

protected:
	memory_type memory;

	void prepare_input() override {
		std::ifstream fin("input/2017/day-6.txt");
		std::string line;
		std::getline(fin, line);

		std::vector<std::string> elements;
		boost::split(elements, line, [](char c) { return c == '\t'; });
		std::transform(std::begin(elements), std::end(elements), std::back_inserter(memory), [](const std::string& element) {
			return std::stoull(element);
		});
	}

	std::size_t rebalance_iterations = 0;
	std::size_t cycle_length         = 0;

	void precompute() override {
		memory_type my_memory(memory);
		using cyc_it = cyclic_iterator<memory_type::iterator>;
		std::vector<memory_type> previous_configurations;
		for(;;) {
			++rebalance_iterations;
			auto highest       = std::max_element(std::begin(my_memory), std::end(my_memory));
			std::size_t blocks = std::exchange(*highest, 0);
			for(cyc_it mem_cyc(++highest, std::begin(my_memory), std::end(my_memory)); blocks != 0; ++mem_cyc) {
				*mem_cyc += 1;
				--blocks;
			}
			if(auto it = std::find(std::begin(previous_configurations), std::end(previous_configurations), my_memory);
			   it != previous_configurations.end()) {
				cycle_length = gsl::narrow<std::size_t>(std::distance(it, std::end(previous_configurations)));
				break;
			} else {
				previous_configurations.push_back(my_memory);
			}
		}
	}

	std::string part_1() noexcept override {
		return std::to_string(rebalance_iterations);
	}

	std::string part_2() noexcept override {
		return std::to_string(cycle_length);
	}
};

template<>
void solve<advent_year::year_2017, advent_day::day_6>() {
	advent_2017_6 a;
	a.solve();
}
