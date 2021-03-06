#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

#include <boost/algorithm/string.hpp>

struct advent_2017_2 : problem
{
	advent_2017_2() noexcept : problem(2017, 2) {
	}

	using row         = std::vector<std::size_t>;
	using spreadsheet = std::vector<row>;

protected:
	spreadsheet ss;

	void prepare_input(std::ifstream& fin) override {
		//auto const ss = spreadsheet(
		//        input | ranges::view::split(is_char('\n'))
		//              | ranges::view::transform([](const std::string& line) -> row {
		//                return line | ranges::view::split(is_char('\t'))
		//                            | ranges::view::transform(ranges::convert_to<std::size_t>{});
		//                })
		//);

		for(std::string line; std::getline(fin, line);) {
			std::vector<std::string> cells;
			boost::split(cells, line, [](char c) { return c == '\t'; });
			row r;
			std::transform(begin(cells), end(cells), std::back_inserter(r), [](const std::string& s) { return std::stoull(s); });
			ss.push_back(r);
		}
	}

	std::string part_1() override {
		std::size_t running_checksum = 0;
		for(const row& r : ss) {
			const auto mm = std::minmax_element(begin(r), end(r));
			running_checksum += *mm.second - *mm.first;
		}
		return std::to_string(running_checksum);
	}

	std::string part_2() override {
		std::size_t running_sum = 0;
		for(const row& r : ss) {
			const std::size_t end = r.size();
			for(size_t i = 0; i != end; ++i) {
				for(size_t j = 0; j != end; ++j) {
					if(i == j) {
						continue;
					}
					if(r[i] % r[j] == 0) {
						running_sum += r[i] / r[j];
					}
				}
			}
		}
		return std::to_string(running_sum);
	}
};

REGISTER_SOLVER(2017, 2);
