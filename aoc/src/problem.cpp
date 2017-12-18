#include "stdafx.h"

#include "problem.hpp"

#include <iostream>
#include <chrono>
#include <tuple>

template<class...Durations, class DurationIn>
std::tuple<Durations...> break_down_durations(DurationIn d) noexcept {
	std::tuple<Durations...> result;
	(
		(
			std::get<Durations>(result) = std::chrono::duration_cast<Durations>(d),
			d -= std::chrono::duration_cast<DurationIn>(std::get<Durations>(result))
		)
	, ...);
	return result;
}

void problem::solve() {
	std::cout << "advent of code 2017 day " << day << std::endl;
	prepare_input();
	precompute();
	const auto run_part = [&, this](std::size_t part, auto fn) {
		const auto start = std::chrono::high_resolution_clock::now();
		std::string result = (this->*fn)();
		const auto end = std::chrono::high_resolution_clock::now();
		const auto delta = end - start;
		const auto split_times = break_down_durations<std::chrono::seconds, std::chrono::milliseconds, std::chrono::microseconds>(delta);
		std::cout << "\tpart " << part << ": " << result << " in ";
		std::cout << std::setfill('0') << std::get<0>(split_times).count() << "." << std::setw(3) << std::get<1>(split_times).count() << "'" << std::setw(3) << std::get<2>(split_times).count() << " seconds." << std::endl;
	};
	run_part(1, &problem::part_1);
	tidy_up();
	run_part(2, &problem::part_2);
}
