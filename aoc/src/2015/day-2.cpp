#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <functional>
#include <vector>

#include <boost/algorithm/string.hpp>

struct advent_2015_2 : problem
{
	advent_2015_2() noexcept : problem(2015, 2) {
	}

	struct box
	{
		std::size_t l, w, h;
	};

protected:
	std::vector<box> boxes;
	
	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line); ) {
			std::vector<std::string> fragments;
			boost::split(fragments, line, [](char ch) { return ch == 'x'; });
			boxes.push_back(box{ std::stoull(fragments[0]), std::stoull(fragments[1]), std::stoull(fragments[2]) });
		}
	}

	std::size_t get_paper_area(box b) {
		const std::size_t areas[] = { b.l * b.w, b.w * b.h, b.h * b.l };
		const std::size_t smallest = *std::min_element(std::begin(areas), std::end(areas));
		return 2 * std::accumulate(std::begin(areas), std::end(areas), 0ui64) + smallest;
	}

	std::size_t get_ribbon_length(box b) noexcept {
		const std::size_t semiperimeters[] = { b.l + b.w, b.w + b.h, b.h + b.l };
		const std::size_t smallest = *std::min_element(std::begin(semiperimeters), std::end(semiperimeters));
		return (2 * smallest) + (b.l * b.w * b.h);
	}

	std::string part_1() override {
		std::size_t paper_required = 0;
		for(const box b : boxes) {
			paper_required += get_paper_area(b);
		}
		return std::to_string(paper_required);
	}

	std::string part_2() override {
		std::size_t ribbon_required = 0;
		for(const box b : boxes) {
			ribbon_required += get_ribbon_length(b);
		}
		return std::to_string(ribbon_required);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_2>() {
	advent_2015_2 a;
	a.solve();
}
