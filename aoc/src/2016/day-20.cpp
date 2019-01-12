#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <regex>
#include <vector>
#include <boost/icl/interval_set.hpp>

struct advent_2016_20 : problem
{
	advent_2016_20() noexcept : problem(2016, 20) {
	}

protected:
	using interval_type = boost::icl::closed_interval<size_t>::type;
	std::vector<interval_type> intervals;

	void prepare_input(std::ifstream& fin) override {
		std::regex pattern(R"(([[:digit:]]+)-([[:digit:]]+))");
		for(std::string line; std::getline(fin, line);) {
			std::smatch m;
			std::regex_search(line, m, pattern);
			intervals.push_back({ std::stoull(m[1].str()), std::stoull(m[2].str()) });
		}
	}

	using set_type = boost::icl::interval_set<std::size_t, std::less, interval_type>;
	set_type firewall;

	void precompute() override {
		for(const interval_type& i : intervals) {
			firewall.add(i);
		}
	}

	std::string part_1() override {
		return std::to_string(firewall.begin()->upper() + 1);
	}

	std::string part_2() override {
		std::size_t available = 0x1'0000'0000;
		for(const interval_type& i : firewall) {
			available -= i.upper() - i.lower() + 1;
		}
		return std::to_string(available);
	}
};

REGISTER_SOLVER(2016, 20);
