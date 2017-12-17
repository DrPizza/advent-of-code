#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include <gsl/gsl>
#include <iostream>
#include <string>

struct problem
{
	void solve() {
		std::cout << "advent of code 2017 day " << day << std::endl;
		prepare_input();
		precompute();
		std::cout << "\tpart 1: " << part_1() << std::endl;
		tidy_up();
		std::cout << "\tpart 2: " << part_2() << std::endl;
	}

	problem(std::size_t day_) noexcept : day(day_) {
	}

	problem(const problem&) = default;
	problem(problem&&)      = default;
	problem& operator=(const problem&) = default;
	problem& operator=(problem&&) = default;
	virtual ~problem()            = default;

protected:
	[[gsl::suppress(f .6)]]
	virtual void prepare_input() {
	}

	[[gsl::suppress(f .6)]]
	virtual void precompute() {
	}

	[[gsl::suppress(f .6)]]
	virtual void tidy_up() {
	}


	[[gsl::suppress(f .6)]]
	virtual std::string part_1() {
		return "";
	}

	[[gsl::suppress(f .6)]] virtual std::string part_2() {
		return "";
	}

private:
	std::size_t day;
};

enum struct advent_day
{
	day_1,
	day_2,
	day_3,
	day_4,
	day_5,
	day_6,
	day_7,
	day_8,
	day_9,
	day_10,
	day_11,
	day_12,
	day_13,
	day_14,
	day_15,
	day_16,
	day_17,
	day_18,
	day_19,
	day_20,
	day_21,
	day_22,
	day_23,
	day_24,
	day_25
};

template<advent_day day>
inline void solve();

#endif
